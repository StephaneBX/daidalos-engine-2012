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

#include "../Core/Logger/ILogger.hpp"
#include "../Core/Memory/CExceptionLoad.hpp"
#include "../Loaders/CMS3DLoader.hpp"

namespace DE {
	CMS3DLoader::CMS3DLoader() {
		LOG_TRACE();
	}

	CMS3DLoader::~CMS3DLoader() {
		LOG_TRACE();
	}

	IAnimatedMeshBase * CMS3DLoader::LoadFromFile(const std::string & filename) {
		LOG_TRACE();

		this->m_data = new SMS3DData;

		FILE * file;
		fopen_s(&file, filename.c_str(), "rb");

		// Get the file size
		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		fread(&this->m_data->header.id, sizeof(char), 10, file);
		fread(&this->m_data->header.version, sizeof(int), 1, file);

		if(strncmp(this->m_data->header.id, "MS3D000000", 10) != 0) {
			LOG_ERROR("Invalid header id");
			ILogger::Kill();

			throw CExceptionLoad(filename);
		}

		if(this->m_data->header.version != 4) {
			LOG_ERROR("Invalid header version");
			ILogger::Kill();

			throw CExceptionLoad(filename);
		}


		// Load the vertices
		fread(&this->m_data->numberOfVertices, sizeof(unsigned short), 1, file);
		this->m_data->transformedVertex = new CVector3F[this->m_data->numberOfVertices];
		this->m_data->vertex = new SMS3DVertex[this->m_data->numberOfVertices];
		for(int i = 0; i < this->m_data->numberOfVertices; i++) {
			fread(&this->m_data->vertex[i].flag, sizeof(unsigned char), 1, file);
			fread(&this->m_data->vertex[i].position, sizeof(float), 3, file);
			fread(&this->m_data->vertex[i].boneId, sizeof(char), 1, file);
			fread(&this->m_data->vertex[i].unused, sizeof(unsigned char), 1, file);
		}

		// Load the faces
		fread(&this->m_data->numberOfFaces, sizeof(unsigned short), 1, file);
		this->m_data->transformedNormal = new CVector3F[this->m_data->numberOfFaces * 3];
		this->m_data->face = new SMS3DFace[this->m_data->numberOfFaces];
		for(int i = 0; i < this->m_data->numberOfFaces; i++) {
			fread(&this->m_data->face[i].flags, sizeof(unsigned short), 1, file);
			fread(&this->m_data->face[i].vertexIndices, sizeof(unsigned short), 3, file);
			fread(&this->m_data->face[i].normals, sizeof(float), 3 * 3, file);
			fread(&this->m_data->face[i].texCoords[0], sizeof(float), 3, file);
			fread(&this->m_data->face[i].texCoords[1], sizeof(float), 3, file);
			fread(&this->m_data->face[i].smoothing, sizeof(unsigned char), 1, file);
			fread(&this->m_data->face[i].groupId, sizeof(unsigned char), 1, file);
		}

		// Load the meshes
		fread(&this->m_data->numberOfMeshes, sizeof(unsigned short), 1, file);
		this->m_data->mesh = new SMS3DMesh[this->m_data->numberOfMeshes];
		for(int i = 0; i < this->m_data->numberOfMeshes; i++) {
			fread(&this->m_data->mesh[i].flags, sizeof(unsigned char), 1, file);
			fread(&this->m_data->mesh[i].name, sizeof(char), 32, file);
			fread(&this->m_data->mesh[i].numFaces, sizeof(unsigned short), 1, file);

			this->m_data->mesh[i].facesIndices = new unsigned short[this->m_data->mesh[i].numFaces];
			fread(this->m_data->mesh[i].facesIndices, sizeof(unsigned short), this->m_data->mesh[i].numFaces, file);
			fread(&this->m_data->mesh[i].materialId, sizeof(char), 1, file);
		}

		// Load the materials
		fread(&this->m_data->numberOfMaterials, sizeof(unsigned short), 1, file);
		this->m_data->material = nullptr;
		if(this->m_data->numberOfMaterials > 0) {
			this->m_data->material = new SMS3DMaterial[this->m_data->numberOfMaterials];
			for(int i = 0; i < this->m_data->numberOfMaterials; i++) {
				fread(&this->m_data->material[i].name, sizeof(char), 32, file);
				fread(&this->m_data->material[i].ambient, sizeof(float), 4, file);
				fread(&this->m_data->material[i].diffuse, sizeof(float), 4, file);
				fread(&this->m_data->material[i].specular, sizeof(float), 4, file);
				fread(&this->m_data->material[i].emission, sizeof(float), 4, file);
				fread(&this->m_data->material[i].shininess, sizeof(float), 1, file);
				fread(&this->m_data->material[i].transparency, sizeof(float), 1, file);
				fread(&this->m_data->material[i].mode, sizeof(unsigned char), 1, file);
				fread(&this->m_data->material[i].texture, sizeof(char), 128, file);
				fread(&this->m_data->material[i].alpha, sizeof(char), 128, file);
			}
		}

		// Animation information
		float animationFPS;
		float currentTime;
		fread(&animationFPS, sizeof(float), 1, file);
		if(animationFPS < 1.0F)
			animationFPS = 1.0F;
		fread(&currentTime, sizeof(float), 1, file);
		fread(&this->m_data->numberOfFrames, sizeof(int), 1, file);

		// Load the joints
		fread(&this->m_data->numberOfJoints, sizeof(unsigned short), 1, file);
		if(this->m_data->numberOfJoints > 0) {
			this->m_data->joint = new SMS3DJoint[this->m_data->numberOfJoints];
			for(int i = 0; i < this->m_data->numberOfJoints; i++) {
				fread(&this->m_data->joint[i].flags, sizeof(unsigned char), 1, file);
				fread(&this->m_data->joint[i].name, sizeof(char), 32, file);
				fread(&this->m_data->joint[i].parentName, sizeof(char), 32, file);
				fread(&this->m_data->joint[i].rotation, sizeof(float), 3, file);
				fread(&this->m_data->joint[i].position, sizeof(float), 3, file);
				fread(&this->m_data->joint[i].numRotationFrames, sizeof(unsigned short), 1, file);
				fread(&this->m_data->joint[i].numTranslationFrames, sizeof(unsigned short), 1, file);

				// Rotation key frames
				this->m_data->joint[i].rotationKeyFrames = new SMS3DKeyFrame[this->m_data->joint[i].numRotationFrames];
				for(int j = 0; j < this->m_data->joint[i].numRotationFrames; j++) {
					fread(&this->m_data->joint[i].rotationKeyFrames[j].time, sizeof(float), 1, file);
					fread(&this->m_data->joint[i].rotationKeyFrames[j].param, sizeof(float), 3, file);

					// Time in second, we need to multiply by the animation fps to get the frame
					this->m_data->joint[i].rotationKeyFrames[j].time *= animationFPS;
				}

				// Translation key frames
				this->m_data->joint[i].translationKeyFrames = new SMS3DKeyFrame[this->m_data->joint[i].numTranslationFrames];
				for(int j = 0; j < this->m_data->joint[i].numTranslationFrames; j++) {
					fread(&this->m_data->joint[i].translationKeyFrames[j].time, sizeof(float), 1, file);
					fread(&this->m_data->joint[i].translationKeyFrames[j].param, sizeof(float), 3, file);

					// Time in second, we need to multiply by the animation fps to get the frame
					this->m_data->joint[i].translationKeyFrames[j].time *= animationFPS;
				}
			}
			InitializeParents();
			InitializeSkeletonsMatrixes();
		}

		// Read comments
		char * tmpBuffer;
		long filePos = ftell(file);
		if(filePos < fileSize) {
			int subVersion = 0;
			fread(&subVersion, sizeof(int), 1, file);
			if(subVersion == 1) {
				int numComments = 0;
				unsigned int commentSize = 0;

				// Meshes
				fread(&numComments, sizeof(int), 1, file);
				for(int i = 0; i < numComments; i++) {
					int index;
					fread(&index, sizeof(int), 1, file);
					fread(&commentSize, sizeof(unsigned int), 1, file);

					if(commentSize > 0) {
						tmpBuffer = new char[commentSize];
						fread(tmpBuffer, sizeof(char), commentSize, file);
						if(index >= 0 && index < (int)this->m_data->numberOfMeshes)
							this->m_data->mesh[index].comment.append(tmpBuffer);
						delete[] tmpBuffer;
					}
				}

				// Materials
				fread(&numComments, sizeof(int), 1, file);
				for(int i = 0; i < numComments; i++) {
					int index;
					fread(&index, sizeof(int), 1, file);
					fread(&commentSize, sizeof(unsigned int), 1, file);

					if(commentSize > 0) {
						tmpBuffer = new char[commentSize];
						fread(tmpBuffer, sizeof(char), commentSize, file);
						if(index >= 0 && index < (int)this->m_data->numberOfMeshes)
							this->m_data->material[index].comment.append(tmpBuffer);
						delete[] tmpBuffer;
					}
				}

				// Joints
				fread(&numComments, sizeof(int), 1, file);
				for(int i = 0; i < numComments; i++) {
					int index;
					fread(&index, sizeof(int), 1, file);
					fread(&commentSize, sizeof(unsigned int), 1, file);

					if(commentSize > 0) {
						tmpBuffer = new char[commentSize];
						fread(tmpBuffer, sizeof(char), commentSize, file);
						if(index >= 0 && index < (int)this->m_data->numberOfMeshes)
							this->m_data->joint[index].comment.append(tmpBuffer);
						delete[] tmpBuffer;
					}
				}

				// Model
				fread(&numComments, sizeof(int), 1, file);
				for(int i = 0; i < numComments; i++) {
					fread(&commentSize, sizeof(unsigned int), 1, file);

					if(commentSize > 0) {
						tmpBuffer = new char[commentSize];
						fread(tmpBuffer, sizeof(char), commentSize, file);
						this->m_data->comment.append(tmpBuffer);
						delete[] tmpBuffer;
					}
				}
			}
		}

		fclose(file);


		InitializeInternalData();

		// Delete the temporary data
		SAFE_DELETE_ARRAY(this->m_data->vertex);
		SAFE_DELETE_ARRAY(this->m_data->face);

		for(int i = 0; i < this->m_data->numberOfMeshes; ++i)
			SAFE_DELETE_ARRAY(this->m_data->mesh[i].facesIndices)
			SAFE_DELETE_ARRAY(this->m_data->mesh);

		SAFE_DELETE_ARRAY(this->m_data->material);

		for(int i = 0; i < this->m_data->numberOfJoints; ++i) {
			SAFE_DELETE_ARRAY(this->m_data->joint[i].rotationKeyFrames);
			SAFE_DELETE_ARRAY(this->m_data->joint[i].translationKeyFrames);
		}
		SAFE_DELETE_ARRAY(this->m_data->joint);
		SAFE_DELETE_ARRAY(this->m_data->transformedVertex);
		SAFE_DELETE_ARRAY(this->m_data->transformedNormal);

		SAFE_DELETE(this->m_data);

		return new CMS3DMesh(filename, this->m_internalData);
	}

	void CMS3DLoader::InitializeParents() {
		LOG_TRACE();

		bool parentFound;

		// Search for the parent
		for(int i = 0; i < this->m_data->numberOfJoints; ++i) {
			parentFound = false;
			for(int j = 0; j < this->m_data->numberOfJoints && !parentFound; ++j) {
				if(strncmp(this->m_data->joint[i].parentName, this->m_data->joint[j].name, 32) == 0) {
					this->m_data->joint[i].parent = j;
					parentFound = true;
				}
			}

			// The joint is the root
			if(!parentFound)
				this->m_data->joint[i].parent = MS3D_ROOT_JOINT;
		}
	}

	void CMS3DLoader::InitializeSkeletonsMatrixes() {
		LOG_TRACE();

		// Local Skeleton Matrix initialization
		SMS3DJoint * currentJoint;
		SMS3DJoint * parentJoint;

		// Initialize the local skeleton matrixes that contains translation and rotation information
		for(int i = 0; i < this->m_data->numberOfJoints; ++i) {
			currentJoint = &this->m_data->joint[i];
			currentJoint->matrixLocalSkeleton = CMatrix(CVector3F(1, 1, 1), CVector3F(currentJoint->rotation.x, currentJoint->rotation.y, currentJoint->rotation.z), CVector3F(currentJoint->position.x, currentJoint->position.y, currentJoint->position.z));
		}

		// Global Skeleton Matrix initialization
		// Multiply the local skeleton matrix with the global skeleton matrix of the parent
		for(int i = 0; i < this->m_data->numberOfJoints; ++i) {
			currentJoint = &this->m_data->joint[i];

			if(currentJoint->parent == MS3D_ROOT_JOINT) {
				currentJoint->matrixGlobalSkeleton = currentJoint->matrixLocalSkeleton;
			} else {
				parentJoint = &this->m_data->joint[currentJoint->parent];
				currentJoint->matrixGlobalSkeleton = currentJoint->matrixLocalSkeleton * parentJoint->matrixGlobalSkeleton;
			}
		}
	}

	void CMS3DLoader::InitializeInternalData() {
		LOG_TRACE();

		// Data initialization
		this->m_internalData = new SMS3DInternalData;
		this->m_internalData->numberOfFrames = this->m_data->numberOfFrames;
		this->m_internalData->numberOfMeshes = this->m_data->numberOfMeshes;
		this->m_internalData->mesh = new SMS3DInternalDataMesh[this->m_internalData->numberOfMeshes];

		for(int i = 0; i < this->m_internalData->numberOfMeshes; ++i) {
			this->m_internalData->mesh[i].numberOfFaces = this->m_data->mesh[i].numFaces;
			this->m_internalData->mesh[i].vertex = new CVector3F*[this->m_internalData->numberOfFrames];
			this->m_internalData->mesh[i].normal = new CVector3F*[this->m_internalData->numberOfFrames];
			this->m_internalData->mesh[i].textureCoordinates = new CVector2F[this->m_internalData->mesh[i].numberOfFaces * 3];

			for(int j = 0; j < this->m_internalData->numberOfFrames; ++j) {
				this->m_internalData->mesh[i].vertex[j] = new CVector3F[this->m_internalData->mesh[i].numberOfFaces * 3];
				this->m_internalData->mesh[i].normal[j] = new CVector3F[this->m_internalData->mesh[i].numberOfFaces * 3];
			}
		}

		// Copy texture coordinates
		for(int i = 0; i < this->m_internalData->numberOfMeshes; ++i) {
			for(int j = 0; j < this->m_internalData->mesh[i].numberOfFaces; ++j) {
				for(int k = 0; k < 3; ++k) {
					this->m_internalData->mesh[i].textureCoordinates[j * 3 + k].x = this->m_data->face[this->m_data->mesh[i].facesIndices[j]].texCoords[0][k];
					this->m_internalData->mesh[i].textureCoordinates[j * 3 + k].y = this->m_data->face[this->m_data->mesh[i].facesIndices[j]].texCoords[1][k];
				}
			}
		}

		// Copy vertices and normals
		for(int i = 0; i < this->m_data->numberOfFrames; ++i) {
			Animate((float)i);

			for(int j = 0; j < this->m_internalData->numberOfMeshes; ++j) {
				for(int k = 0; k < this->m_internalData->mesh[j].numberOfFaces; ++k) {
					for(int l = 0; l < 3; ++l) {
						this->m_internalData->mesh[j].vertex[i][k * 3 + l] = this->m_data->transformedVertex[this->m_data->face[this->m_data->mesh[j].facesIndices[k]].vertexIndices[l]];
						this->m_internalData->mesh[j].normal[i][k * 3 + l] = this->m_data->transformedNormal[this->m_data->mesh[j].facesIndices[k] * 3 + l];
					}
				}
			}
		}
	}

	void CMS3DLoader::Animate(const float frame) {
		LOG_TRACE();

		if(frame < 0.0f) {
			for(int i = 0; i < this->m_data->numberOfJoints; i++) {
				this->m_data->joint[i].matrixLocal = this->m_data->joint[i].matrixLocalSkeleton;
				this->m_data->joint[i].matrixGlobal = this->m_data->joint[i].matrixGlobalSkeleton;
			}

			for(int i = 0; i < this->m_data->numberOfVertices; i++)
				this->m_data->transformedVertex[i] = this->m_data->vertex[i].position;

			for(int i = 0; i < this->m_data->numberOfFaces; i++) {
				for(int j = 0; j < 3; j++) {
					this->m_data->transformedNormal[i * 3 + j] = this->m_data->face[i].normals[j];
				}
			}
		} else {
			SMS3DJoint * currentJoint;
			CVector3F interpolatedVector;

			CMatrix translation, rotationMatrix;
			CQuaternion rotation;

			float interpolationFactor;
			bool keyFrameFound;

			// Update the joints animation matrixes
			for(int i = 0; i < this->m_data->numberOfJoints; i++) {
				keyFrameFound = false;
				currentJoint = &this->m_data->joint[i];
				currentJoint->currentTranslationFrame = currentJoint->numTranslationFrames;
				currentJoint->currentRotationFrame = currentJoint->numRotationFrames;
				currentJoint->matrixLocal = currentJoint->matrixLocal.Identity();


				// We need to find the current keyframe

				// Translation
				for(int j = 0; j < currentJoint->numTranslationFrames - 1 && !keyFrameFound; ++j) {
					if(frame >= currentJoint->translationKeyFrames[j].time && frame < currentJoint->translationKeyFrames[j + 1].time) {
						currentJoint->currentTranslationFrame = j;
						keyFrameFound = true;
					}
				}

				keyFrameFound = false;

				// Rotation
				for(int j = 0; j < currentJoint->numRotationFrames - 1 && !keyFrameFound; ++j) {
					if(frame >= currentJoint->rotationKeyFrames[j].time && frame < currentJoint->rotationKeyFrames[j + 1].time) {
						currentJoint->currentRotationFrame = j;
						keyFrameFound = true;
					}
				}

				// Translation calculation
				if(currentJoint->currentTranslationFrame == currentJoint->numTranslationFrames && frame < currentJoint->translationKeyFrames[0].time) {
					// The first keyframe, no interpolation needed
					interpolatedVector = currentJoint->translationKeyFrames[0].param;
				} else {
					if(currentJoint->currentTranslationFrame == currentJoint->numTranslationFrames && frame >= currentJoint->translationKeyFrames[currentJoint->numTranslationFrames - 1].time) {
						// The last keyframe, no interpolation needed
						interpolatedVector = currentJoint->translationKeyFrames[currentJoint->numTranslationFrames - 1].param;
					} else {
						// Interpolation needed as we are in the middle of the keyframes
						interpolationFactor = (frame - currentJoint->translationKeyFrames[currentJoint->currentTranslationFrame].time) / (currentJoint->translationKeyFrames[currentJoint->currentTranslationFrame + 1].time - currentJoint->translationKeyFrames[currentJoint->currentTranslationFrame].time);
						interpolatedVector = currentJoint->translationKeyFrames[currentJoint->currentTranslationFrame].param + (currentJoint->translationKeyFrames[currentJoint->currentTranslationFrame + 1].param - currentJoint->translationKeyFrames[currentJoint->currentTranslationFrame].param) * interpolationFactor;
					}
				}
				translation = translation.SetTranslation(interpolatedVector.x, interpolatedVector.y, interpolatedVector.z);

				// Rotation calculation
				// We use quaternion to interpolate rotations, as it will produce better results
				if(currentJoint->currentRotationFrame == currentJoint->numRotationFrames && frame < currentJoint->rotationKeyFrames[0].time) {
					// The first keyframe, no interpolation needed
					interpolatedVector = currentJoint->rotationKeyFrames[0].param;
					rotation = rotation.YawPitchRoll(interpolatedVector.x, interpolatedVector.y, interpolatedVector.z);
				} else {
					if(currentJoint->currentRotationFrame == currentJoint->numRotationFrames && frame >= currentJoint->rotationKeyFrames[currentJoint->numRotationFrames - 1].time) {
						// The last keyframe, no interpolation needed
						interpolatedVector = currentJoint->rotationKeyFrames[currentJoint->numRotationFrames - 1].param;
						rotation = rotation.YawPitchRoll(interpolatedVector.x, interpolatedVector.y, interpolatedVector.z);
					} else {
						// Interpolation needed as we are in the middle of the keyframes
						interpolationFactor = (frame - currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame].time) / (currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame + 1].time - currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame].time);

						// Create a quaternion with the current keyframe values
						CQuaternion rot1, rot2;
						CQuaternion::YawPitchRoll(rot1, currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame].param.x, currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame].param.y, currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame].param.z);

						// Create a quaternion with the next keyframe values
						CQuaternion::YawPitchRoll(rot2, currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame + 1].param.x, currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame + 1].param.y, currentJoint->rotationKeyFrames[currentJoint->currentRotationFrame + 1].param.z);

						// Interpolate between the two quaternions
						rotation = rot1.Slerp(rot2, interpolationFactor);
					}
				}

				// Get the final local matrix
				rotationMatrix = rotationMatrix.RotationQuaternion(rotation);
				currentJoint->matrixLocal = rotationMatrix * translation * currentJoint->matrixLocalSkeleton;
			}

			BuildGlobalMatrix();
			TransformedVerticesCalculation();
			TransformedNormalsCalculation();
		}
	}

	void CMS3DLoader::BuildGlobalMatrix() {
		LOG_TRACE();

		SMS3DJoint * currentJoint;
		SMS3DJoint * parentJoint;

		// Build the hierarchy for the global matrix
		for(int i = 0; i < this->m_data->numberOfJoints; ++i) {
			currentJoint = &this->m_data->joint[i];

			if(currentJoint->parent == MS3D_ROOT_JOINT) {
				currentJoint->matrixGlobal = currentJoint->matrixLocal;
			} else {
				parentJoint = &this->m_data->joint[currentJoint->parent];
				currentJoint->matrixGlobal = currentJoint->matrixLocal * parentJoint->matrixGlobal;
			}
		}
	}

	void CMS3DLoader::TransformedVerticesCalculation() {
		LOG_TRACE();

		int currentBone;

		// Apply modifications to vertices
		for(int i = 0; i < this->m_data->numberOfVertices; ++i) {
			// Only vertices that are attached to a bone will be updated
			currentBone = (int)this->m_data->vertex[i].boneId;
			if(currentBone != MS3D_VERTEX_NO_BONES) {
				this->m_data->transformedVertex[i] = this->m_data->joint[currentBone].matrixGlobal.Transform(this->m_data->joint[currentBone].matrixGlobalSkeleton.Inverse().Transform(this->m_data->vertex[i].position));
			} else
				this->m_data->transformedVertex[i] = this->m_data->vertex[i].position;
		}
	}

	void CMS3DLoader::TransformedNormalsCalculation() {
		LOG_TRACE();

		int currentBone;
		SMS3DFace * currentFace;

		// Copy local static normals
		for(int i = 0; i < this->m_data->numberOfFaces; ++i) {
			currentFace = &this->m_data->face[i];
			for(int j = 0; j < 3; ++j) {
				this->m_data->transformedNormal[i * 3 + j] = currentFace->normals[j];
			}
		}

		// Apply modifications to normals
		for(int i = 0; i < this->m_data->numberOfFaces; ++i) {
			currentFace = &this->m_data->face[i];
			for(int j = 0; j < 3; ++j) {
				currentBone = this->m_data->vertex[currentFace->vertexIndices[j]].boneId;

				if(currentBone != MS3D_VERTEX_NO_BONES) {
					CMatrix rot;
					rot = rot.RotationQuaternion(this->m_data->joint[currentBone].matrixGlobalSkeleton.TransformRotation()).Inverse();
					this->m_data->transformedNormal[i * 3 + j] = rot.Transform(this->m_data->transformedNormal[i * 3 + j]);

					rot = rot.RotationQuaternion(this->m_data->joint[currentBone].matrixGlobal.TransformRotation());
					this->m_data->transformedNormal[i * 3 + j] = rot.Transform(this->m_data->transformedNormal[i * 3 + j]);
				}
			}
		}
	}
}
