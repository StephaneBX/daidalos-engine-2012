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

#include <string>
#include "../Core/Logger/ILogger.hpp"
#include "../Core/Memory/CExceptionLoad.hpp"
#include "../Loaders/CParticleEmittersTemplateLoader.hpp"

namespace DE {
	/**
	* Constructor
	*/
	CParticleEmittersTemplateLoader::CParticleEmittersTemplateLoader() {
		LOG_TRACE();
	}

	/**
	* Destructor
	*/
	CParticleEmittersTemplateLoader::~CParticleEmittersTemplateLoader() {
		LOG_TRACE();
	}

	/**
	* Load an obj mesh from a file
	* @param filename The obj file path
	* @return The mesh
	*/
	IParticleEmittersTemplate * CParticleEmittersTemplateLoader::LoadFromFile(const std::string & filename) {
		LOG_TRACE();

		this->m_filename = filename;
		tinyxml2::XMLDocument doc;
		int loadOk = doc.LoadFile(filename.c_str());
		if(loadOk == tinyxml2::XML_NO_ERROR) {
			tinyxml2::XMLHandle hDoc(&doc);

			// Read the structure nodes
			tinyxml2::XMLElement * element = hDoc.FirstChildElement("emitters").FirstChildElement("emitter").ToElement();
			for(tinyxml2::XMLElement * elementIt = element; elementIt != nullptr; elementIt = elementIt->NextSiblingElement()) {
				LoadParticleEmitterNode(tinyxml2::XMLHandle(elementIt));
			}

			return new IParticleEmittersTemplate(filename, this->m_emittersTemplate);
		} else {
			LOG_ERROR("Unable to load the scene file '" << this->m_filename << "' (" << doc.GetErrorStr1() << ")");
			ILogger::Kill();

			throw CExceptionLoad(this->m_filename);
		}
	}

	/**
	* Load a particle emitter node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CParticleEmittersTemplateLoader::LoadParticleEmitterNode(tinyxml2::XMLHandle & handle) {
		LOG_TRACE();

		bool ignoreGravity;
		std::string emitterName = handle.ToElement()->Attribute("name");
		int emitterBlendMode;
		float emitterEmissionRate;
		float emitterLife;
		float emitterLifeMax;
		CVector3F emitterPosition;

		// Basic values
		GetNodeData(handle, "blendMode", emitterBlendMode, (int)RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
		GetNodeData(handle, "life", emitterLife, -1);
		GetNodeData(handle, "lifeMax", emitterLifeMax, -1);
		GetNodeData(handle, "emissionRate", emitterEmissionRate, 10);
		GetNodeData(handle, "ignoreGravity", ignoreGravity, false);

		// Create the emitter
		CParticleEmitter * emitter = new CParticleEmitter();
		emitter->SetActive(true);
		emitter->SetTextureId(0);
		emitter->SetBlendMode(emitterBlendMode);
		emitter->SetLife(emitterLife);
		emitter->SetLifeMax(emitterLifeMax);
		emitter->SetEmissionRate(emitterEmissionRate);
		emitter->SetIgnoreGravity(ignoreGravity);

		// Look for emission rate changes
		tinyxml2::XMLElement * emissionRateChangeElement = handle.FirstChildElement("emissionRateChange").ToElement();
		LoadParticleEmitterEmissionRateChange(emitter, emissionRateChangeElement);

		// Look for initializers
		tinyxml2::XMLElement * initializersElement = handle.FirstChildElement("initializers").ToElement();
		LoadParticleEmitterInitializers(emitter, initializersElement);

		// Look for updaters
		tinyxml2::XMLElement * updatersElement = handle.FirstChildElement("updaters").ToElement();
		LoadParticleEmitterUpdaters(emitter, updatersElement);

		this->m_emittersTemplate[emitterName] = emitter;
	}

	/**
	* Load a particle emitter emission rate change
	* @param emitter The emitter
	* @param initializersElement The initializers
	*/
	void CParticleEmittersTemplateLoader::LoadParticleEmitterEmissionRateChange(CParticleEmitter * emitter, tinyxml2::XMLElement * emissionRateChangeElement) {
		if (emissionRateChangeElement != nullptr) {
			float emitterEmissionRateTime;
			float emitterEmissionRateValue;

			for (tinyxml2::XMLElement* child = emissionRateChangeElement->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
				std::string childName = std::string(child->Name());
				tinyxml2::XMLHandle childHandle = tinyxml2::XMLHandle(child);

				// Read an emission rate
				if (childName.compare("emissionRate") == 0) {
					GetNodeData(childHandle, "time", emitterEmissionRateTime, 0);
					GetNodeData(childHandle, "value", emitterEmissionRateValue, 0);

					SParticleEmitterEmissionRateChange emissionRateCHange;
					emissionRateCHange.time = emitterEmissionRateTime;
					emissionRateCHange.value = emitterEmissionRateValue;

					emitter->AddEmissionRateChange(emissionRateCHange);
				}
			}
		}
	}

	/**
	* Load a particle emitter initializers
	* @param emitter The emitter
	* @param initializersElement The initializers
	*/
	void CParticleEmittersTemplateLoader::LoadParticleEmitterInitializers(CParticleEmitter * emitter, tinyxml2::XMLElement * initializersElement) {
		if (initializersElement != nullptr) {
			tinyxml2::XMLHandle initializersHandle = tinyxml2::XMLHandle(initializersElement);
			float emitterParticleSize;
			float emitterParticleLifeMin;
			float emitterParticleLifeMax;
			float emitterParticleRotationMin;
			float emitterParticleRotationMax;
			float emitterParticleVelocityMin;
			float emitterParticleVelocityMax;
			float emitterParticleLinearVelocityVariation;
			float emitterParticleMass;
			CVector3F emitterParticlePosition;
			CVector3F emitterParticleVelocityDirection;
			CVector3F emitterParticlePositionMin;
			CVector3F emitterParticlePositionMax;
			CColor emitterParticleColor;

			for (tinyxml2::XMLElement* child = initializersElement->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
				std::string childName = std::string(child->Name());
				tinyxml2::XMLHandle childHandle = tinyxml2::XMLHandle(child);

				if (childName.compare("color") == 0) {
					GetNodeData(initializersHandle, "color", emitterParticleColor, CColor(1.0F, 1.0F, 1.0F, 1.0F));
					CColorParticleInitializer * initializer = new CColorParticleInitializer(emitterParticleColor);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("size") == 0) {
					GetNodeData(initializersHandle, "size", emitterParticleSize, 1);
					CSizeParticleInitializer * initializer = new CSizeParticleInitializer(emitterParticleSize);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("minMaxLife") == 0) {
					GetNodeData(childHandle, "min", emitterParticleLifeMin, 0);
					GetNodeData(childHandle, "max", emitterParticleLifeMax, 1000);
					CLifeParticleInitializer * initializer = new CLifeParticleInitializer(emitterParticleLifeMin, emitterParticleLifeMax);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("minMaxRotation") == 0) {
					GetNodeData(childHandle, "min", emitterParticleRotationMin, 0);
					GetNodeData(childHandle, "max", emitterParticleRotationMax, 0);
					CRotationParticleInitializer * initializer = new CRotationParticleInitializer(emitterParticleRotationMin, emitterParticleRotationMax);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("sphereVelocity") == 0) {
					GetNodeData(childHandle, "min", emitterParticleVelocityMin, 0);
					GetNodeData(childHandle, "max", emitterParticleVelocityMax, 1);
					CSphereVelocityParticleInitializer * initializer = new CSphereVelocityParticleInitializer(emitterParticleVelocityMin, emitterParticleVelocityMax);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("linearVelocity") == 0) {
					GetNodeData(childHandle, "direction", emitterParticleVelocityDirection, CVector3F());
					GetNodeData(childHandle, "variationFactor", emitterParticleLinearVelocityVariation, 0);
					GetNodeData(childHandle, "min", emitterParticleVelocityMin, 0);
					GetNodeData(childHandle, "max", emitterParticleVelocityMax, 1);
					CLinearVelocityParticleInitializer * initializer = new CLinearVelocityParticleInitializer(emitterParticleVelocityDirection, emitterParticleLinearVelocityVariation, emitterParticleVelocityMin, emitterParticleVelocityMax);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("minMaxBoxPosition") == 0) {
					GetNodeData(childHandle, "min", emitterParticlePositionMin, CVector3F());
					GetNodeData(childHandle, "max", emitterParticlePositionMax, CVector3F());
					CBoxPositionParticleInitializer * initializer = new CBoxPositionParticleInitializer(emitterParticlePositionMin, emitterParticlePositionMax);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("radiusPosition") == 0) {
					float emitterParticleInnerRadiusPosition;
					float emitterParticleOuterRadiusPosition;
					CVector3F emitterParticleAxis;
					GetNodeData(childHandle, "innerRadius", emitterParticleInnerRadiusPosition, 0);
					GetNodeData(childHandle, "outerRadius", emitterParticleOuterRadiusPosition, 0);
					GetNodeData(childHandle, "axis", emitterParticleAxis, CVector3F());
					CRadiusPositionParticleInitializer * initializer = new CRadiusPositionParticleInitializer(emitterParticleInnerRadiusPosition, emitterParticleOuterRadiusPosition, emitterParticleAxis);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("position") == 0) {
					GetNodeData(initializersHandle, "position", emitterParticlePosition, CVector3F());
					CPositionParticleInitializer * initializer = new CPositionParticleInitializer(emitterParticlePosition);
					emitter->AddInitializer(initializer);

				} else if (childName.compare("mass") == 0) {
					GetNodeData(initializersHandle, "mass", emitterParticleMass, 1);
					CMassParticleInitializer * initializer = new CMassParticleInitializer(emitterParticleMass);
					emitter->AddInitializer(initializer);
				}
			}
		}
	}

	/**
	* Load a particle emitter initializers
	* @param emitter The emitter
	* @param updatersElement The initializers
	*/
	void CParticleEmittersTemplateLoader::LoadParticleEmitterUpdaters(CParticleEmitter * emitter, tinyxml2::XMLElement * updatersElement) {
		if (updatersElement != nullptr) {
			tinyxml2::XMLHandle updatersHandle = tinyxml2::XMLHandle(updatersElement);

			for (tinyxml2::XMLElement* child = updatersElement->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
				std::string childName = std::string(child->Name());
				tinyxml2::XMLHandle childHandle = tinyxml2::XMLHandle(child);	

				if (childName.compare("colorChange") == 0) {
					float updaterTime;
					CVector4F updaterColor;

					// Read each step
					std::vector<SColorParticleChangeUpdater> colorChangeVector;
					tinyxml2::XMLElement * colorElement = childHandle.FirstChildElement("color").ToElement();
					for (tinyxml2::XMLElement * colorElementIt = colorElement; colorElementIt != nullptr; colorElementIt = colorElementIt->NextSiblingElement()) {
						tinyxml2::XMLHandle colorChangeHandle = tinyxml2::XMLHandle(colorElementIt);
						GetNodeData(colorChangeHandle, "time", updaterTime, 0);
						GetNodeData(colorChangeHandle, "value", updaterColor, CVector4F(1.0F, 1.0F, 1.0F, 1.0F));

						SColorParticleChangeUpdater colorChange;
						colorChange.time = updaterTime;
						colorChange.color = updaterColor;
						colorChangeVector.push_back(colorChange);
					}

					// Add the updater if the definition is not empty
					if (colorChangeVector.size() > 0) {
						CColorParticleUpdater * updater = new CColorParticleUpdater(colorChangeVector);
						emitter->AddUpdater(updater);
					}

				} else if (childName.compare("life") == 0) {
					CLifeParticleUpdater * updater = new CLifeParticleUpdater();
					emitter->AddUpdater(updater);

				} else if (childName.compare("rotation") == 0) {
					CRotationParticleUpdater * updater = new CRotationParticleUpdater();
					emitter->AddUpdater(updater);

				} else if (childName.compare("sizeChange") == 0) {
					float updaterTime;
					float updaterSize;

					// Read each step
					std::vector<SSizeParticleChangeUpdater> sizeChangeVector;
					tinyxml2::XMLElement * sizeElement = childHandle.FirstChildElement("size").ToElement();
					for (tinyxml2::XMLElement * sizeElementIt = sizeElement; sizeElementIt != nullptr; sizeElementIt = sizeElementIt->NextSiblingElement()) {
						tinyxml2::XMLHandle sizeChangeHandle = tinyxml2::XMLHandle(sizeElementIt);
						GetNodeData(sizeChangeHandle, "time", updaterTime, 0);
						GetNodeData(sizeChangeHandle, "value", updaterSize, 1);

						SSizeParticleChangeUpdater sizeChange;
						sizeChange.time = updaterTime;
						sizeChange.size = updaterSize;
						sizeChangeVector.push_back(sizeChange);
					}

					// Add the updater if the definition is not empty
					if (sizeChangeVector.size() > 0) {
						CSizeParticleUpdater * updater = new CSizeParticleUpdater(sizeChangeVector);
						emitter->AddUpdater(updater);
					}

				} else if (childName.compare("linearVelocityChange") == 0) {
					float updaterDamping;
					float updaterTime;
					float updaterVariationFactor;
					float updaterVelocityMin;
					float updaterVelocityMax;
					CVector3F updaterDirection;

					// Read the damping
					GetNodeData(childHandle, "damping", updaterDamping, 0);

					// Read each step
					std::vector<SLinearVelocityParticleChangeUpdater> velocityChangeVector;
					tinyxml2::XMLElement * velocityElement = childHandle.FirstChildElement("velocity").ToElement();
					for (tinyxml2::XMLElement * velocityElementIt = velocityElement; velocityElementIt != nullptr; velocityElementIt = velocityElementIt->NextSiblingElement()) {
						tinyxml2::XMLHandle velocityChangeHandle = tinyxml2::XMLHandle(velocityElementIt);
						GetNodeData(velocityChangeHandle, "time", updaterTime, 0);
						GetNodeData(velocityChangeHandle, "direction", updaterDirection, CVector3F(0.0F, 0.0F, 0.0F));
						GetNodeData(velocityChangeHandle, "variationFactor", updaterVariationFactor, 0);
						GetNodeData(velocityChangeHandle, "velocityMin", updaterVelocityMin, 1);
						GetNodeData(velocityChangeHandle, "velocityMax", updaterVelocityMax, 1);

						SLinearVelocityParticleChangeUpdater velocityChange;
						velocityChange.time = updaterTime;
						velocityChange.direction = updaterDirection;
						velocityChange.variationFactor = updaterVariationFactor;
						velocityChange.velocityMin = updaterVelocityMin;
						velocityChange.velocityMax = updaterVelocityMax;
						velocityChangeVector.push_back(velocityChange);
					}

					// Add the updater if the definition is not empty
					CLinearVelocityParticleUpdater * updater = new CLinearVelocityParticleUpdater(velocityChangeVector, updaterDamping);
					emitter->AddUpdater(updater);

				} else if (childName.compare("sphereVelocityChange") == 0) {
					float updaterDamping;
					float updaterTime;
					float updaterVelocityMin;
					float updaterVelocityMax;

					// Read the damping
					GetNodeData(childHandle, "damping", updaterDamping, 0);

					// Read each step
					std::vector<SSphereVelocityParticleChangeUpdater> velocityChangeVector;
					tinyxml2::XMLElement * velocityElement = childHandle.FirstChildElement("velocity").ToElement();
					for (tinyxml2::XMLElement * velocityElementIt = velocityElement; velocityElementIt != nullptr; velocityElementIt = velocityElementIt->NextSiblingElement()) {
						tinyxml2::XMLHandle velocityChangeHandle = tinyxml2::XMLHandle(velocityElementIt);
						GetNodeData(velocityChangeHandle, "time", updaterTime, 0);
						GetNodeData(velocityChangeHandle, "velocityMin", updaterVelocityMin, 1);
						GetNodeData(velocityChangeHandle, "velocityMax", updaterVelocityMax, 1);

						SSphereVelocityParticleChangeUpdater velocityChange;
						velocityChange.time = updaterTime;
						velocityChange.velocityMin = updaterVelocityMin;
						velocityChange.velocityMax = updaterVelocityMax;
						velocityChangeVector.push_back(velocityChange);
					}

					// Add the updater if the definition is not empty
					CSphereVelocityParticleUpdater * updater = new CSphereVelocityParticleUpdater(velocityChangeVector, updaterDamping);
					emitter->AddUpdater(updater);

				} else if (childName.compare("circularVelocityChange") == 0) {
					float updaterDamping;
					float updaterTime;
					float updaterVelocityMin;
					float updaterVelocityMax;
					CVector3F updaterAxis;

					// Read the damping
					GetNodeData(childHandle, "damping", updaterDamping, 0);

					// Read the rotation axis
					GetNodeData(childHandle, "axis", updaterAxis, CVector3F(0,1,0));

					// Read each step
					std::vector<SCircularVelocityParticleChangeUpdater> velocityChangeVector;
					tinyxml2::XMLElement * velocityElement = childHandle.FirstChildElement("velocity").ToElement();
					for (tinyxml2::XMLElement * velocityElementIt = velocityElement; velocityElementIt != nullptr; velocityElementIt = velocityElementIt->NextSiblingElement()) {
						tinyxml2::XMLHandle velocityChangeHandle = tinyxml2::XMLHandle(velocityElementIt);
						GetNodeData(velocityChangeHandle, "time", updaterTime, 0);
						GetNodeData(velocityChangeHandle, "velocityMin", updaterVelocityMin, 1);
						GetNodeData(velocityChangeHandle, "velocityMax", updaterVelocityMax, 1);

						SCircularVelocityParticleChangeUpdater velocityChange;
						velocityChange.time = updaterTime;
						velocityChange.velocityMin = updaterVelocityMin;
						velocityChange.velocityMax = updaterVelocityMax;
						velocityChangeVector.push_back(velocityChange);
					}

					// Add the updater if the definition is not empty
					CCircularVelocityParticleUpdater * updater = new CCircularVelocityParticleUpdater(velocityChangeVector, updaterAxis, updaterDamping);
					emitter->AddUpdater(updater);
				}
			}
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, bool & data, const bool defaultValue) {
		LOG_TRACE();

		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			std::string tmpText = TrimString(text->Value());
			std::transform(tmpText.begin(), tmpText.end(), tmpText.begin(), tolower);

			if(tmpText == "true" || tmpText == "1")
				data = true;
			if(tmpText == "false" || tmpText == "0")
				data = false;
		} else {
			data = defaultValue;
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, int & data, const int defaultValue) {
		LOG_TRACE();

		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			std::istringstream ss(text->Value());
			ss >> data;
		} else {
			data = defaultValue;
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector2I & data, const CVector2I & defaultValue) {
		LOG_TRACE();

		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			data = CVector2I::GetFrom(std::string(text->Value()), defaultValue);
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector2F & data, const CVector2F & defaultValue) {
		LOG_TRACE();

		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if (text != nullptr) {
			data = CVector2F::GetFrom(std::string(text->Value()), defaultValue);
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector3F & data, const CVector3F & defaultValue) {
		LOG_TRACE();

		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			data = CVector3F::GetFrom(std::string(text->Value()), defaultValue);
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector4F & data, const CVector4F & defaultValue) {
		LOG_TRACE();

		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			data = CVector4F::GetFrom(std::string(text->Value()), defaultValue);
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CColor & data, const CColor & defaultValue) {
		LOG_TRACE();

		CTokenizer tokenizer;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
			tokenizer.AddLine(text->Value());

		if(tokenizer.Tokenize(":", 4) && tokenizer.GetTokens().size() == 4) {
			std::istringstream ss;
			float r;
			float g;
			float b;
			float a;

			ss.str(tokenizer.GetTokens()[0]);
			ss >> r;
			ss.clear();
			ss.str(tokenizer.GetTokens()[1]);
			ss >> g;
			ss.clear();
			ss.str(tokenizer.GetTokens()[2]);
			ss >> b;
			ss.clear();
			ss.str(tokenizer.GetTokens()[3]);
			ss >> a;

			data = CColor(r, g, b, a);
		} else {
			data = defaultValue;
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, float & data, const float defaultValue) {
		LOG_TRACE();

		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			std::istringstream ss(text->Value());
			ss >> data;
		} else {
			data = defaultValue;
		}
	}

	void CParticleEmittersTemplateLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, std::string & data, const std::string & defaultValue) {
		LOG_TRACE();

		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			data = text->Value();
		} else {
			data = defaultValue;
		}
	}
}

