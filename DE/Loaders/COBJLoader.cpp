/*
==============================================================================================================================================================

Author: Stephane Baudoux
Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#include <sstream>
#include "../Core/Logger/ILogger.h"
#include "../Core/Memory/CExceptionLoad.h"
#include "../Core/Utility/CTokenizer.h"
#include "../Loaders/COBJLoader.h"

namespace DE {
#define OBJ_LOADER_ELEMENT_PER_LINE 4

	COBJLoader::COBJLoader() {
		LOG_TRACE();
	}

	COBJLoader:: ~COBJLoader() {
		LOG_TRACE();
	}

	IMeshBase * COBJLoader::LoadFromFile(const std::string & filename) {
		LOG_TRACE();

		// Temporary data
		this->m_filename = filename;
		CVector3F tmpVec3;
		CVector2F tmpVec2;
		std::ostringstream stream;
		std::istringstream ss;
		std::vector<CVector3F> tmpVertex;
		std::vector<CVector3F> tmpNormal;
		std::vector<CVector2F> tmpTexture;
		std::vector<int> vertexOrder;
		std::vector<int> normalOrder;
		std::vector<int> textureOrder;
		std::string tmpLine;
		std::string tmpString;
		int groupCounter = -1;

		// Data
		this->m_internalData = new SOBJInternalData;
		this->m_internalData->vertex = nullptr;
		this->m_internalData->normal = nullptr;
		this->m_internalData->texcoord = nullptr;

		// File filter
		std::vector<std::string> filterList;
		filterList.push_back("vt ");
		filterList.push_back("vn ");
		filterList.push_back("v ");
		filterList.push_back("f ");
		filterList.push_back("o ");
		filterList.push_back("g ");

		CTokenizer token(filename, filterList);
		token.Tokenize(" ");
		token.Tokenize("/", 3);

		int numberOfGroups = token.Count("g");

		// Data initialization
		this->m_internalData->groupSize.resize(numberOfGroups);
		this->m_internalData->vertex = new CVector3F*[numberOfGroups];
		this->m_internalData->normal = new CVector3F*[numberOfGroups];
		this->m_internalData->texcoord = new CVector2F*[numberOfGroups];

		std::fill(this->m_internalData->vertex, this->m_internalData->vertex + numberOfGroups, nullptr);
		std::fill(this->m_internalData->normal, this->m_internalData->normal + numberOfGroups, nullptr);
		std::fill(this->m_internalData->texcoord, this->m_internalData->texcoord + numberOfGroups, nullptr);

		// Read the data
		for(unsigned int i = 0; i < token.GetTokens().size();) {
			if(token.GetTokens().at(i) == "v") {
				// Vertex declaration
				ss.clear();
				ss.str(token.GetTokens().at(i + 1));
				ss >> tmpVec3.x;
				ss.clear();
				ss.str(token.GetTokens().at(i + 2));
				ss >> tmpVec3.y;
				ss.clear();
				ss.str(token.GetTokens().at(i + 3));
				ss >> tmpVec3.z;

				tmpVertex.push_back(tmpVec3);
				i += OBJ_LOADER_ELEMENT_PER_LINE;
			} else {
				if(token.GetTokens().at(i) == "vt") {
					// Texture declaration
					ss.clear();
					ss.str(token.GetTokens().at(i + 1));
					ss >> tmpVec2.x;

					ss.clear();
					ss.str(token.GetTokens().at(i + 2));
					ss >> tmpVec2.y;

					// Temporary fix?
					tmpVec2.y = 1 - tmpVec2.y;

					tmpTexture.push_back(tmpVec2);
					i += OBJ_LOADER_ELEMENT_PER_LINE - 1;
				} else {
					if(token.GetTokens().at(i) == "vn") {
						// Normal declaration
						ss.clear();
						ss.str(token.GetTokens().at(i + 1));
						ss >> tmpVec3.x;
						ss.clear();
						ss.str(token.GetTokens().at(i + 2));
						ss >> tmpVec3.y;
						ss.clear();
						ss.str(token.GetTokens().at(i + 3));
						ss >> tmpVec3.z;

						tmpNormal.push_back(tmpVec3);
						i += OBJ_LOADER_ELEMENT_PER_LINE;
					} else {
						if(token.GetTokens().at(i) == "f") {
							// Face declaration
							// Count the number of vertex for this face

							int vertexNumber = 0;
							int vertexCountIndex = 1;

							while((i + vertexCountIndex) < (int)token.GetTokens().size() && token.GetTokens().at(i + vertexCountIndex) != "f" && token.GetTokens().at(i + vertexCountIndex) != "g") {
								vertexNumber++;
								vertexCountIndex += 3;
							}

							// Accept only triangles and quads
							if(vertexNumber == 3 || vertexNumber == 4) {
								for(int k = 0; k < 3; k++) {
									if(token.GetTokens().at(i + k * 3 + 1) != "")
										vertexOrder.push_back(atoi(token.GetTokens().at(i + k * 3 + 1).c_str()));

									if(token.GetTokens().at(i + k * 3 + 2) != "")
										textureOrder.push_back(atoi(token.GetTokens().at(i + k * 3 + 2).c_str()));

									if(token.GetTokens().at(i + k * 3 + 3) != "")
										normalOrder.push_back(atoi(token.GetTokens().at(i + k * 3 + 3).c_str()));
								}

								// Quad detected, need to add a second triangle
								if(vertexNumber == 4) {
									for(int k = 2; k < 5; k++) {
										if(token.GetTokens().at(i + (k % 4) * 3 + 1) != "")
											vertexOrder.push_back(atoi(token.GetTokens().at(i + (k % 4) * 3 + 1).c_str()));

										if(token.GetTokens().at(i + (k % 4) * 3 + 2) != "")
											textureOrder.push_back(atoi(token.GetTokens().at(i + (k % 4) * 3 + 2).c_str()));

										if(token.GetTokens().at(i + (k % 4) * 3 + 3) != "")
											normalOrder.push_back(atoi(token.GetTokens().at(i + (k % 4) * 3 + 3).c_str()));
									}
									i += 13;
								} else
									i += 10;
							}
						} else {
							if(token.GetTokens().at(i) == "o") {
								// New object detected, we add the previus and clear all the temporary buffers
								if(groupCounter >= 0)
									FillBuffers(tmpVertex, tmpNormal, tmpTexture, vertexOrder, normalOrder, textureOrder, groupCounter);

								i += 2;
							} else {
								if(token.GetTokens().at(i) == "g") {
									// Clear the face buffer
									if(groupCounter >= 0)
										FillBuffers(tmpVertex, tmpNormal, tmpTexture, vertexOrder, normalOrder, textureOrder, groupCounter);
									vertexOrder.clear();
									normalOrder.clear();
									textureOrder.clear();
									groupCounter++;

									i += 2;
								} else
									i++;
							}
						}
					}
				}
			}
		}
		FillBuffers(tmpVertex, tmpNormal, tmpTexture, vertexOrder, normalOrder, textureOrder, groupCounter);

		return new COBJMesh(filename, this->m_internalData);
	}

	void COBJLoader::FillBuffers(const std::vector<CVector3F> & tmpVertex, const std::vector<CVector3F> & tmpNormal, const std::vector<CVector2F> & tmpTexture, const std::vector<int> vertexOrder, const std::vector<int> normalOrder, const std::vector<int> & textureOrder, const int groupCounter) {
		LOG_TRACE();

		// The data is read, time to fill the right buffers
		if(vertexOrder.size() > 3) {
			this->m_internalData->groupSize[groupCounter] = vertexOrder.size();
			this->m_internalData->vertex[groupCounter] = new CVector3F[this->m_internalData->groupSize[groupCounter]];

			// Fill the vertex buffer
			for(int i = 0; i < this->m_internalData->groupSize[groupCounter]; i++) {
				if(vertexOrder[i] >= 1 && vertexOrder[i] <= (int)tmpVertex.size()) {
					this->m_internalData->vertex[groupCounter][i] = tmpVertex[vertexOrder[i] - 1];
				} else {
					LOG_ERROR("Unable to load the mesh '" << this->m_filename << "' (Invalid vertex id)");
					ILogger::Kill();

					throw CExceptionLoad(this->m_filename);
				}
			}

			// If normal information exists, fill the normal buffer
			if(normalOrder.size() > 0) {
				if(normalOrder.size() == this->m_internalData->groupSize[groupCounter]) {
					// Verify the validity of the normal numbers
					bool hasNormals = true;
					for(int i = 0; i < this->m_internalData->groupSize[groupCounter] && hasNormals; i++) {
						if(normalOrder[i] < 1 || normalOrder[i] > (int)tmpNormal.size())
							hasNormals = false;
					}

					if(hasNormals) {
						this->m_internalData->normal[groupCounter] = new CVector3F[this->m_internalData->groupSize[groupCounter]];
						for(int i = 0; i < this->m_internalData->groupSize[groupCounter]; i++) {
							if(normalOrder[i] >= 1 && normalOrder[i] <= (int)tmpNormal.size()) {
								this->m_internalData->normal[groupCounter][i] = tmpNormal[normalOrder[i] - 1];
							} else {
								if(normalOrder[i] != 0) {
									LOG_ERROR("Unable to load the mesh '" << this->m_filename << "' (Invalid normal id)");
									ILogger::Kill();

									throw CExceptionLoad(this->m_filename);
								}
							}
						}
					}
				} else {
					LOG_ERROR("Unable to load the mesh '" << this->m_filename << "' (Invalid number of normals)");
					ILogger::Kill();

					throw CExceptionLoad(this->m_filename);
				}
			}

			// If texture information exists, fill the texture coordinate buffer
			if(textureOrder.size() > 0) {
				if(textureOrder.size() == this->m_internalData->groupSize[groupCounter]) {
					this->m_internalData->texcoord[groupCounter] = new CVector2F[this->m_internalData->groupSize[groupCounter]];
					for(int i = 0; i < this->m_internalData->groupSize[groupCounter]; i++) {
						if(textureOrder[i] >= 1 && textureOrder[i] <= (int)tmpTexture.size()) {
							this->m_internalData->texcoord[groupCounter][i] = tmpTexture[textureOrder[i] - 1];
						} else {
							LOG_ERROR("Unable to load the mesh '" << this->m_filename << "' (Invalid texcoord id)");
							ILogger::Kill();

							throw CExceptionLoad(this->m_filename);
						}
					}
				} else {
					LOG_ERROR("Unable to load the mesh '" << this->m_filename << "' (Invalid number of texcoords)");
					ILogger::Kill();

					throw CExceptionLoad(this->m_filename);
				}
			}
		} else {
			LOG_ERROR("Unable to load the mesh '" << this->m_filename << "' (Less than 3 vertices found)");
			ILogger::Kill();

			throw CExceptionLoad(this->m_filename);
		}
	}
}
