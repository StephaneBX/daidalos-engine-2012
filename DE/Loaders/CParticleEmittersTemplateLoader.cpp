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
#include "../Core/Logger/ILogger.h"
#include "../Core/Memory/CExceptionLoad.h"
#include "../Loaders/CParticleEmittersTemplateLoader.h"

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
			for(element; element; element = element->NextSiblingElement()) {
				LoadParticleEmitterNode(tinyxml2::XMLHandle(element));
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

		std::string emitterName = handle.ToElement()->Attribute("name");
		int emitterBlendMode;
		float emitterEmissionRate, emitterLife, emitterLifeMax, emitterParticleSizeMin, emitterParticleSizeMax, emitterParticleLifeMin, emitterParticleLifeMax, emitterParticleRotationMin, emitterParticleRotationMax, emitterParticleVelocityMin, emitterParticleVelocityMax, emitterParticleVelocityDamping, emitterParticleLinearVelocityVariation, emitterParticleRadiusPosition;
		CColor emitterParticleColorMin, emitterParticleColorMax;
		CVector3F emitterPosition, emitterParticleGravity, emitterParticleVelocityDirection, emitterParticlePositionMin, emitterParticlePositionMax;

		// Basic values
		GetNodeData(handle, "blendMode", emitterBlendMode, (int)RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
		GetNodeData(handle, "life", emitterLife, -1);
		GetNodeData(handle, "lifeMax", emitterLifeMax, -1);
		GetNodeData(handle, "emissionRate", emitterEmissionRate, 10);

		// Create the emitter
		CParticleEmitter * emitter = new CParticleEmitter();
		emitter->SetActive(true);
		emitter->SetTextureId(0);
		emitter->SetBlendMode(emitterBlendMode);
		emitter->SetLife(emitterLife);
		emitter->SetLifeMax(emitterLifeMax);
		emitter->SetEmissionRate(emitterEmissionRate);

		// Look for initializers
		tinyxml2::XMLElement * initializersElement = handle.FirstChildElement("initializers").ToElement();
		if(initializersElement != nullptr) {
			for(tinyxml2::XMLElement* child = initializersElement->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
				std::string childName = std::string(child->Name());
				tinyxml2::XMLHandle childHandle = tinyxml2::XMLHandle(child);

				if(childName.compare("minMaxColor") == 0) {
					GetNodeData(childHandle, "min", emitterParticleColorMin, CColor(1.0f, 1.0f, 1.0f, 1.0f));
					GetNodeData(childHandle, "max", emitterParticleColorMax, CColor(1.0f, 1.0f, 1.0f, 1.0f));
					CColorParticleInitializer * initializer = new CColorParticleInitializer(emitterParticleColorMin, emitterParticleColorMax);
					emitter->AddInitializer(initializer);

				} else if(childName.compare("minMaxSize") == 0) {
					GetNodeData(childHandle, "min", emitterParticleSizeMin, 1);
					GetNodeData(childHandle, "max", emitterParticleSizeMax, 1);
					CSizeParticleInitializer * initializer = new CSizeParticleInitializer(emitterParticleSizeMin, emitterParticleSizeMax);
					emitter->AddInitializer(initializer);

				} else if(childName.compare("minMaxLife") == 0) {
					GetNodeData(childHandle, "min", emitterParticleLifeMin, 0);
					GetNodeData(childHandle, "max", emitterParticleLifeMax, 1000);
					CLifeParticleInitializer * initializer = new CLifeParticleInitializer(emitterParticleLifeMin, emitterParticleLifeMax);
					emitter->AddInitializer(initializer);

				} else if(childName.compare("minMaxRotation") == 0) {
					GetNodeData(childHandle, "min", emitterParticleRotationMin, 0);
					GetNodeData(childHandle, "max", emitterParticleRotationMax, 0);
					CRotationParticleInitializer * initializer = new CRotationParticleInitializer(emitterParticleRotationMin, emitterParticleRotationMax);
					emitter->AddInitializer(initializer);

				} else if(childName.compare("minMaxSphereVelocity") == 0) {
					GetNodeData(childHandle, "min", emitterParticleVelocityMin, 0);
					GetNodeData(childHandle, "max", emitterParticleVelocityMax, 1);
					CSphereVelocityParticleInitializer * initializer = new CSphereVelocityParticleInitializer(emitterParticleVelocityMin, emitterParticleVelocityMax);
					emitter->AddInitializer(initializer);

				} else if(childName.compare("linearVelocity") == 0) {
					GetNodeData(childHandle, "direction", emitterParticleVelocityDirection, CVector3F());
					GetNodeData(childHandle, "variationFactor", emitterParticleLinearVelocityVariation, 0);
					GetNodeData(childHandle, "min", emitterParticleVelocityMin, 0);
					GetNodeData(childHandle, "max", emitterParticleVelocityMax, 1);
					CLinearVelocityParticleInitializer * initializer = new CLinearVelocityParticleInitializer(emitterParticleVelocityDirection, emitterParticleLinearVelocityVariation, emitterParticleVelocityMin, emitterParticleVelocityMax);
					emitter->AddInitializer(initializer);

				} else if(childName.compare("minMaxBoxPosition") == 0) {
					GetNodeData(childHandle, "min", emitterParticlePositionMin, CVector3F());
					GetNodeData(childHandle, "max", emitterParticlePositionMax, CVector3F());
					CBoxPositionParticleInitializer * initializer = new CBoxPositionParticleInitializer(emitterParticlePositionMin, emitterParticlePositionMax);
					emitter->AddInitializer(initializer);

				} else if(childName.compare("radiusPosition") == 0) {
					GetNodeData(childHandle, "radius", emitterParticleRadiusPosition, 0);
					CRadiusPositionParticleInitializer * initializer = new CRadiusPositionParticleInitializer(emitterParticleRadiusPosition);
					emitter->AddInitializer(initializer);
				}
			}
		}

		// Look for updaters
		tinyxml2::XMLElement * updatersElement = handle.FirstChildElement("updaters").ToElement();
		if(updatersElement != nullptr) {
			for(tinyxml2::XMLElement* child = updatersElement->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
				std::string childName = std::string(child->Name());
				tinyxml2::XMLHandle childHandle = tinyxml2::XMLHandle(child);

				if(childName.compare("color") == 0) {
					CColorParticleUpdater * updater = new CColorParticleUpdater();
					emitter->AddUpdater(updater);

				} else if(childName.compare("gravity") == 0) {
					GetNodeData(childHandle, "vector", emitterParticleGravity, CVector3F());
					CGravityParticleUpdater * updater = new CGravityParticleUpdater(emitterParticleGravity);
					emitter->AddUpdater(updater);

				} else if(childName.compare("life") == 0) {
					CLifeParticleUpdater * updater = new CLifeParticleUpdater();
					emitter->AddUpdater(updater);

				} else if(childName.compare("rotation") == 0) {
					CRotationParticleUpdater * updater = new CRotationParticleUpdater();
					emitter->AddUpdater(updater);

				} else if(childName.compare("size") == 0) {
					CSizeParticleUpdater * updater = new CSizeParticleUpdater();
					emitter->AddUpdater(updater);

				} else if(childName.compare("velocity") == 0) {
					GetNodeData(childHandle, "damping", emitterParticleVelocityDamping, 0);
					CVelocityParticleUpdater * updater = new CVelocityParticleUpdater(emitterParticleVelocityDamping);
					emitter->AddUpdater(updater);
				}
			}
		}

		this->m_emittersTemplate[emitterName] = emitter;
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
			float r, g, b, a;

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

