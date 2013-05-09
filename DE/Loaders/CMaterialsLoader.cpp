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
#include "../Core/Utility/CFile.h"
#include "../Core/Utility/CVector3.h"
#include "../Core/Utility/CTokenizer.h"
#include "../Loaders/CMaterialsLoader.h"

namespace DE
{
	CMaterialsLoader::CMaterialsLoader() {}

	CMaterialsLoader::~CMaterialsLoader() {}

	IMaterialBase * CMaterialsLoader::LoadFromFile(const std::string & filename)
	{
		IMaterialBase * material = new IMaterialBase(filename);
		SMaterialElement * element = nullptr;
		std::string elementName;
		std::istringstream iss;
		
		CVector3F tmpVector;
		std::vector<std::string> filterList;
		filterList.push_back("newmtl ");
		filterList.push_back("Kd ");
		filterList.push_back("Ka ");
		filterList.push_back("Ks ");
		filterList.push_back("Ke ");
		filterList.push_back("Ns ");

		CTokenizer tokenizer(filename, filterList);
		tokenizer.Tokenize(" ");

		if(tokenizer.GetTokens().size() > 0)
		{
			for(unsigned int i = 0 ; i < tokenizer.GetTokens().size() ;)
			{
				if(tokenizer.GetTokens().at(i) == "newmtl")
				{
					if(element != nullptr)
						material->AddElement(elementName, element);
					elementName = tokenizer.GetTokens().at(i + 1);
					element = new SMaterialElement();
					i += 2;
				}
				else
				{
					if(tokenizer.GetTokens().at(i) == "Kd")
					{
						// Diffuse modification
						iss.clear();
						iss.str(tokenizer.GetTokens().at(i + 1));
						iss >> element->m_kDiffuse.x;

						iss.clear();
						iss.str(tokenizer.GetTokens().at(i + 2));
						iss >> element->m_kDiffuse.y;

						iss.clear();
						iss.str(tokenizer.GetTokens().at(i + 3));
						iss >> element->m_kDiffuse.z;

						i += 4;
					}
					else
					{
						if(tokenizer.GetTokens().at(i) == "Ka")
						{
							// Ambient modification
							iss.clear();
							iss.str(tokenizer.GetTokens().at(i + 1));
							iss >> element->m_kAmbient.x;

							iss.clear();
							iss.str(tokenizer.GetTokens().at(i + 2));
							iss >> element->m_kAmbient.y;

							iss.clear();
							iss.str(tokenizer.GetTokens().at(i + 3));
							iss >> element->m_kAmbient.z;

							i += 4;
						}
						else
						{
							if(tokenizer.GetTokens().at(i) == "Ks")
							{
								// Specular modification
								iss.clear();
								iss.str(tokenizer.GetTokens().at(i + 1));
								iss >> element->m_kSpecular.x;

								iss.clear();
								iss.str(tokenizer.GetTokens().at(i + 2));
								iss >> element->m_kSpecular.y;

								iss.clear();
								iss.str(tokenizer.GetTokens().at(i + 3));
								iss >> element->m_kSpecular.z;

								i += 4;
							}
							else
							{
								if(tokenizer.GetTokens().at(i) == "Ke")
								{
									// Emission modification
									iss.clear();
									iss.str(tokenizer.GetTokens().at(i + 1));
									iss >> element->m_kEmission.x;

									iss.clear();
									iss.str(tokenizer.GetTokens().at(i + 2));
									iss >> element->m_kEmission.y;

									iss.clear();
									iss.str(tokenizer.GetTokens().at(i + 3));
									iss >> element->m_kEmission.z;

									i += 4;
								}
								else
								{
									if(tokenizer.GetTokens().at(i) == "Ns")
									{
										// Specular factor modification
										iss.clear();
										iss.str(tokenizer.GetTokens().at(i + 1));
										iss >> element->m_ns;

										i += 2;
									}
									else
										i++;
								}
							}
						}
					}
				}
			}
		}

		if(element != nullptr)
			material->AddElement(elementName, element);

		return material;
	}
}
