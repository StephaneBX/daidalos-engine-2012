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

#include "../Scene/3D/I3DScene.h"
#include "../Core/Core.h"
#include "../Core/Utility/CTokenizer.h"
#include "../Core/Utility/CVector4.h"
#include "../Core/Memory/CExceptionLoad.h"
#include "../Core/Logger/ILogger.h"
#include "../Scene/3D/CProjectionCameraSceneNode.h"
#include "../Scene/3D/CSpotLightSceneNode.h"
#include "../Scene/3D/CPointLightSceneNode.h"
#include "../Scene/3D/CDirectionalLightSceneNode.h"
#include "../Scene/3D/CStaticMeshSceneNode.h"
#include "../Scene/3D/CAnimatedMeshSceneNode.h"
#include "../Scene/3D/CSoundSceneNode.h"
#include "../Scene/3D/CEmptySceneNode.h"
#include "../Scene/3D/CParticleEmitterSceneNode.h"
#include "../Scene/3D/CPlane3D.h"
#include "../Scene/Particles/Initializers/CColorParticleInitializer.h"
#include "../Scene/Particles/Initializers/CLifeParticleInitializer.h"
#include "../Scene/Particles/Initializers/CLinearVelocityParticleInitializer.h"
#include "../Scene/Particles/Initializers/CRotationParticleInitializer.h"
#include "../Scene/Particles/Initializers/CSizeParticleInitializer.h"
#include "../Scene/Particles/Initializers/CSphereVelocityParticleInitializer.h"
#include "../Scene/Particles/Initializers/CBoxPositionParticleInitializer.h"
#include "../Scene/Particles/Initializers/CRadiusPositionParticleInitializer.h"
#include "../Scene/Particles/Updaters/CColorParticleUpdater.h"
#include "../Scene/Particles/Updaters/CGravityParticleUpdater.h"
#include "../Scene/Particles/Updaters/CLifeParticleUpdater.h"
#include "../Scene/Particles/Updaters/CRotationParticleUpdater.h"
#include "../Scene/Particles/Updaters/CSizeParticleUpdater.h"
#include "../Scene/Particles/Updaters/CVelocityParticleUpdater.h"
#include "../Medias/CTexture2D.h"
#include "../Medias/CMaterial.h"
#include "../Loaders/CSceneLoader.h"

namespace DE {
	CSceneLoader::CSceneLoader() {
		LOG_TRACE();

		// Nodes default configuration file
		this->m_nodesConf = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_NODES_CONFIGURATION);
		this->m_materialsConf = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_MATERIALS_CONFIGURATION);
		this->m_structure = SCENE_NODE_ROOT_STRUCTURE_TYPE_BSP;
		this->m_renderer = nullptr;
	}

	CSceneLoader::~CSceneLoader() {
		LOG_TRACE();
	}

	I3DScene * CSceneLoader::LoadFromFile(const std::string & filename) {
		LOG_TRACE();

		I3DScene * scene = new I3DScene();

		if(this->m_renderer == nullptr)
			this->m_renderer = IRenderer::GetRenderer();

		this->m_filename = filename;
		ISceneNode * root = nullptr;
		tinyxml2::XMLElement * element = nullptr;

		tinyxml2::XMLDocument doc;
		int loadOk = doc.LoadFile(filename.c_str());
		if(loadOk == tinyxml2::XML_NO_ERROR) {
			tinyxml2::XMLHandle hDoc(&doc);

			int tmpInt = 0;
			hDoc.FirstChildElement("scene").ToElement()->QueryIntAttribute("structure", &tmpInt);
			this->m_structure = (ESceneNodeRootStructureType)tmpInt;

			// Read the particle system declaration
			element = hDoc.FirstChildElement("scene").FirstChildElement("particleSystem").ToElement();
			if(element != nullptr) {
				tinyxml2::XMLHandle & handle = tinyxml2::XMLHandle(element);
				LoadParticleSystem(scene, handle);
			}

			// Read the structure nodes
			element = hDoc.FirstChildElement("scene").FirstChildElement("node").ToElement();
			for(element; element; element = element->NextSiblingElement()) {
				LoadNode(scene, root, tinyxml2::XMLHandle(element));
			}
		} else {
			LOG_ERROR("Unable to load the scene file '" << this->m_filename << "' (" << doc.GetErrorStr1() << ")");
			ILogger::Kill();

			throw CExceptionLoad(this->m_filename);
		}

		scene->SetRoot(root);

		return scene;
	}

	void CSceneLoader::SaveToFile(ISceneNode * root, const std::string & filename) {
		LOG_TRACE();
	}

	void CSceneLoader::LoadNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle) {
		LOG_TRACE();

		// Global data variables
		bool visible, flatShading, enabled, shadowCaster;
		float fov, ratio, near, far, range, radius, falloff, tightness;
		CVector3F position, rotation, scale;
		std::map<std::string, std::string> attributes;
		CColor color;
		tinyxml2::XMLElement * child = nullptr;
		ISceneNode * node = nullptr;
		int type = -1;
		handle.ToElement()->QueryIntAttribute("type", &type);
		std::string file, diffuseTexture, normalTexture, material;
		CTokenizer tokenizer;

		std::string name = "undefined";
		if(handle.ToElement()->Attribute("name")) {
			name = handle.ToElement()->Attribute("name");
		}

		int id = 0;
		handle.ToElement()->QueryIntAttribute("id", &id);

		CVector4F nodeDefaultColor;
		CVector3F nodeDefaultScale;

		switch(type) {
		case SCENE_NODE_ROOT:

			switch(this->m_structure) {
				// Simple structure
			case SCENE_NODE_ROOT_STRUCTURE_TYPE_SIMPLE:
				LoadSimpleStructure(scene, parent, handle, node);
				if(parent == nullptr)
					parent = node;

				break;

				// BSP structure
			case SCENE_NODE_ROOT_STRUCTURE_TYPE_BSP:
				LoadBSPStructure(scene, parent, handle, node);
				if(parent == nullptr)
					parent = node;

				break;
			}

			break;

		case SCENE_NODE_CAMERA:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
			GetNodeData(handle, "scale", scale, CVector3F(1, 1, 1));
			GetNodeData(handle, "fov", fov, DegreeToRadian(this->m_nodesConf.GetValueFloat("ICameraSceneNode", "fov", 55)));
			GetNodeData(handle, "ratio", ratio, this->m_renderer->GetResolution().x / (float)this->m_renderer->GetResolution().y);
			GetNodeData(handle, "near", near, this->m_nodesConf.GetValueFloat("ICameraSceneNode", "near", 0.1f));
			GetNodeData(handle, "far", far, this->m_nodesConf.GetValueFloat("ICameraSceneNode", "far", 100.0f));

			// Create the node
			node = new CProjectionCameraSceneNode(scene, parent, name, position, rotation, DegreeToRadian(fov), ratio, near, far);
			node->SetScale(scale);
			node->SetId(id);
			break;

		case SCENE_NODE_SPOTLIGHT:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "enabled", enabled, true);
			GetNodeData(handle, "shadow_caster", shadowCaster, false);
			GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
			nodeDefaultColor = CVector4F::GetFrom(this->m_nodesConf.GetValueString("ISpotLightSceneNode", "color", "1.0f:1.0f:1.0f:1.0f"));
			GetNodeData(handle, "color", color, CColor(nodeDefaultColor.x, nodeDefaultColor.y, nodeDefaultColor.z, nodeDefaultColor.w));
			GetNodeData(handle, "range", range, this->m_nodesConf.GetValueFloat("ISpotLightSceneNode", "range", 100.0f));
			GetNodeData(handle, "radius", radius, this->m_nodesConf.GetValueFloat("ISpotLightSceneNode", "radius", 30.0f));
			GetNodeData(handle, "falloff", falloff, this->m_nodesConf.GetValueFloat("ISpotLightSceneNode", "falloff", 0.8f));
			GetNodeData(handle, "tightness", tightness, this->m_nodesConf.GetValueFloat("ISpotLightSceneNode", "tightness", 0.6f));

			// Create the node
			node = new CSpotLightSceneNode(scene, parent, name, position, rotation, color, range, radius, falloff, tightness, shadowCaster, enabled);
			node->SetId(id);
			break;

		case SCENE_NODE_POINTLIGHT:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "enabled", enabled, true);
			GetNodeData(handle, "shadow_caster", shadowCaster, false);
			GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
			nodeDefaultColor = CVector4F::GetFrom(this->m_nodesConf.GetValueString("IPointLightSceneNode", "color", "1.0f:1.0f:1.0f:1.0f"));
			GetNodeData(handle, "color", color, CColor(nodeDefaultColor.x, nodeDefaultColor.y, nodeDefaultColor.z, nodeDefaultColor.w));
			GetNodeData(handle, "range", range, this->m_nodesConf.GetValueFloat("IPointLightSceneNode", "range", 100.0f));
			GetNodeData(handle, "falloff", falloff, this->m_nodesConf.GetValueFloat("IPointLightSceneNode", "falloff", 0.8f));

			// Create the node
			node = new CPointLightSceneNode(scene, parent, name, position, color, range, falloff, shadowCaster, enabled);
			node->SetId(id);
			break;

		case SCENE_NODE_DIRECTIONALLIGHT:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "enabled", enabled, true);
			GetNodeData(handle, "shadow_caster", shadowCaster, false);
			GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
			nodeDefaultColor = CVector4F::GetFrom(this->m_nodesConf.GetValueString("IPointLightSceneNode", "color", "1.0f:1.0f:1.0f:1.0f"));
			GetNodeData(handle, "color", color, CColor(nodeDefaultColor.x, nodeDefaultColor.y, nodeDefaultColor.z, nodeDefaultColor.w));

			// Create the node
			node = new CDirectionalLightSceneNode(scene, parent, name, rotation, color, shadowCaster, enabled);
			node->SetId(id);
			break;

		case SCENE_NODE_STATICMESH:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "visible", visible, this->m_nodesConf.GetValueBool("IStaticMeshSceneNode", "visible", true));
			GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
			nodeDefaultScale = CVector3F::GetFrom(this->m_nodesConf.GetValueString("IStaticMeshSceneNode", "scale", "1.0f:1.0f:1.0f"));
			GetNodeData(handle, "scale", scale, CVector3F(nodeDefaultScale.x, nodeDefaultScale.y, nodeDefaultScale.z));
			GetNodeData(handle, "diffuse_texture", diffuseTexture, "default.png");
			GetNodeData(handle, "normal_texture", normalTexture, "default.png");
			GetNodeData(handle, "flat_shading", flatShading, false);
			GetNodeData(handle, "material", material, "default.mtl");
			file = handle.ToElement()->Attribute("file");

			// Create the node
			node = new CStaticMeshSceneNode(scene, parent, name, file, position, rotation, scale);
			node->SetId(id);
			static_cast<CStaticMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_DIFFUSE, diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			static_cast<CStaticMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_NORMAL, normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			if(flatShading) {
				static_cast<CStaticMeshSceneNode*>(node)->SetMeshShadingType(DE::MESH_SHADING_FLAT);
			}
			static_cast<CStaticMeshSceneNode*>(node)->SetMeshMaterial(material);

			break;

		case SCENE_NODE_ANIMATEDMESH:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "visible", visible, this->m_nodesConf.GetValueBool("IAnimatedMeshSceneNode", "visible", true));
			GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
			nodeDefaultScale = CVector3F::GetFrom(this->m_nodesConf.GetValueString("IStaticMeshSceneNode", "scale", "1.0f:1.0f:1.0f"));
			GetNodeData(handle, "scale", scale, CVector3F(nodeDefaultScale.x, nodeDefaultScale.y, nodeDefaultScale.z));
			GetNodeData(handle, "diffuse_texture", diffuseTexture, "default.png");
			GetNodeData(handle, "normal_texture", normalTexture, "default.png");
			GetNodeData(handle, "flat_shading", flatShading, false);
			GetNodeData(handle, "material", material, "default.mtl");
			file = handle.ToElement()->Attribute("file");

			// Create the node
			node = new CAnimatedMeshSceneNode(scene, parent, name, file, position, rotation, scale);
			node->SetId(id);
			static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_DIFFUSE, diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_NORMAL, normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			if(flatShading) {
				static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshShadingType(DE::MESH_SHADING_FLAT);
			}
			static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshMaterial(material);

			break;

		case SCENE_NODE_SOUND:
			int soundType;
			float gain;
			float coneInnerAngle;
			float coneOuterAngle;
			float referenceDistance;
			float maxDistance;
			bool relativeToListener;
			bool looped;
			bool playOnStartup;

			GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
			GetNodeData(handle, "type", soundType, this->m_nodesConf.GetValueBool("ISoundSceneNode", "type", 0));
			GetNodeData(handle, "gain", gain, this->m_nodesConf.GetValueBool("ISoundSceneNode", "gain", 1));
			GetNodeData(handle, "cone_inner_angle", coneInnerAngle, this->m_nodesConf.GetValueFloat("ISoundSceneNode", "cone_inner_angle", 360.0f));
			GetNodeData(handle, "cone_outer_angle", coneOuterAngle, this->m_nodesConf.GetValueFloat("ISoundSceneNode", "cone_outer_angle", 360.0f));
			GetNodeData(handle, "reference_distance", referenceDistance, this->m_nodesConf.GetValueFloat("ISoundSceneNode", "reference_distance", 2));
			GetNodeData(handle, "max_distance", maxDistance, this->m_nodesConf.GetValueFloat("ISoundSceneNode", "max_distance", 20));
			GetNodeData(handle, "relative_to_listener", relativeToListener, this->m_nodesConf.GetValueBool("ISoundSceneNode", "relative_to_listener", false));
			GetNodeData(handle, "looped", looped, this->m_nodesConf.GetValueBool("ISoundSceneNode", "looped", false));
			GetNodeData(handle, "play_on_startup", playOnStartup, this->m_nodesConf.GetValueBool("ISoundSceneNode", "play_on_startup", false));
			GetNodeData(handle, "file", file, "");

			node = new CSoundSceneNode(scene, file, static_cast<ESoundType>(soundType), parent, name);
			node->SetId(id);
			static_cast<CSoundSceneNode*>(node)->SetPosition(position);
			static_cast<CSoundSceneNode*>(node)->SetRotation(rotation);
			static_cast<CSoundSceneNode*>(node)->SetSoundGain(gain);
			static_cast<CSoundSceneNode*>(node)->SetSoundConeInnerAngle(coneInnerAngle);
			static_cast<CSoundSceneNode*>(node)->SetSoundConeOuterAngle(coneInnerAngle);
			static_cast<CSoundSceneNode*>(node)->SetSoundReferenceDistance(referenceDistance);
			static_cast<CSoundSceneNode*>(node)->SetSoundMaxDistance(maxDistance);
			static_cast<CSoundSceneNode*>(node)->SetSoundRelativeToListener(relativeToListener);
			static_cast<CSoundSceneNode*>(node)->SetSoundLooped(looped);
			if(playOnStartup) {
				static_cast<CSoundSceneNode*>(node)->SetSoundStatus(SOUND_STATUS_PLAY);
			}

			break;

		case SCENE_NODE_EMPTY:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));

			// Create the node
			node = new CEmptySceneNode(scene, parent, name, position);
			node->SetId(id);
			static_cast<CEmptySceneNode*>(node)->SetRotation(rotation);
			break;

		case SCENE_NODE_PARTICLE_EMITTER: {
			bool emitterActive;
			int emitterTextureId, emitterBlendMode;
			float emitterEmissionRate, emitterLife, emitterLifeMax, emitterParticleSizeMin, emitterParticleSizeMax, emitterParticleLifeMin, emitterParticleLifeMax, emitterParticleRotationMin, emitterParticleRotationMax, emitterParticleVelocityMin, emitterParticleVelocityMax, emitterParticleVelocityDamping, emitterParticleLinearVelocityVariation, emitterParticleRadiusPosition;
			CColor emitterParticleColorMin, emitterParticleColorMax;
			CVector3F emitterPosition, emitterParticleGravity, emitterParticleVelocityDirection, emitterParticlePositionMin, emitterParticlePositionMax;

			// Basic values
			GetNodeData(handle, "active", emitterActive, true);
			GetNodeData(handle, "position", emitterPosition, CVector3F(0, 0, 0));
			GetNodeData(handle, "textureId", emitterTextureId, 0);
			GetNodeData(handle, "blendMode", emitterBlendMode, (int)RENDER_STATE_BLEND_FUNCTION_INVSRCALPHA);
			GetNodeData(handle, "life", emitterLife, -1);
			GetNodeData(handle, "lifeMax", emitterLifeMax, -1);
			GetNodeData(handle, "emissionRate", emitterEmissionRate, 10);

			// Create the emitter
			CParticleEmitter * emitter = scene->GetParticleSystem().InstanciateEmitter();
			emitter->SetActive(emitterActive);
			emitter->SetTextureId(emitterTextureId);
			emitter->SetBlendMode(emitterBlendMode);
			emitter->SetLife(emitterLife);
			emitter->SetLifeMax(emitterLifeMax);
			emitter->SetEmissionRate(emitterEmissionRate);

			// Look for initializers
			tinyxml2::XMLElement * initializersElement = handle.FirstChildElement("initializers").ToElement();
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

			// Look for updaters
			tinyxml2::XMLElement * updatersElement = handle.FirstChildElement("updaters").ToElement();
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
			node = new CParticleEmitterSceneNode(scene, parent, name, emitter);
			node->SetId(id);
			static_cast<CParticleEmitterSceneNode*>(node)->SetPosition(emitterPosition);
		}
		break;

		default:
			// Invalid or missing type
			LOG_WARNING("Invalid or missing node attribute 'type' in '" << this->m_filename << "'");
			break;
		}

		// If a node has been successfully created
		if(node != nullptr) {
			// Get the node attributes
			attributes = GetNodeAttributes(handle);

			// Add the attributes
			node->SetAttributes(attributes);

			// Add the node to the parent
			if(parent != node) {
				parent->AddChild(node);
			}

			// Check for attached nodes
			child = handle.FirstChildElement("node").ToElement();
			for(child; child; child = child->NextSiblingElement()) {
				// Read only nodes
				if(std::string(child->Name()) == "node") {
					LoadNode(scene, node, tinyxml2::XMLHandle(child));
				}
			}
		}
	}

	void CSceneLoader::LoadBSPStructure(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node) {
		LOG_TRACE();

		tinyxml2::XMLElement * child;
		std::vector<SBSPBrushDefinition> brushDefinition;
		node = new CBSPSceneNode(scene);
		std::string skyName;
		CVector3F ambientColor, skyAmbientColor;

		// Get the ambient color
		if(handle.ToElement()->Attribute("ambientColor") != nullptr)
			ambientColor = CVector3F::GetFrom(handle.ToElement()->Attribute("ambientColor"));
		static_cast<CBSPSceneNode*>(node)->SetAmbientColor(CVector4F(ambientColor.x, ambientColor.y, ambientColor.z, 1));

		// Get the sky name
		if(handle.ToElement()->Attribute("skyName") != nullptr)
			skyName = handle.ToElement()->Attribute("skyName");
		static_cast<CBSPSceneNode*>(node)->SetSkyName(skyName);

		// Get the sky ambient color
		if(handle.ToElement()->Attribute("skyAmbientColor") != nullptr)
			skyAmbientColor = CVector3F::GetFrom(handle.ToElement()->Attribute("skyAmbientColor"));
		static_cast<CBSPSceneNode*>(node)->SetSkyAmbientColor(CVector4F(skyAmbientColor.x, skyAmbientColor.y, skyAmbientColor.z, 1));

		// Read the bsp geometry
		child = handle.FirstChildElement("multi_brush").ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			// A brush created with a bunch of static models
			if(std::string(child->Name()) == "multi_brush") {
				tinyxml2::XMLHandle bspHandle(child);
				tinyxml2::XMLElement * child2 = bspHandle.FirstChildElement("brush").ToElement();
				SBSPBrushDefinition brush;

				// Default values
				brush.id = SCENE_DEFAULT_ID;
				brush.collision = true;

				// Get the brush options
				bspHandle.ToElement()->QueryBoolAttribute("collisions", &brush.collision);
				bspHandle.ToElement()->QueryIntAttribute("id", &brush.id);

				for(child2; child2; child2 = child2->NextSiblingElement()) {
					if(std::string(child2->Name()) == "brush")
						AddBSPBrushData(tinyxml2::XMLHandle(child2), brush);
				}

				if(brush.geometry.size() > 0) {
					brushDefinition.push_back(brush);
				}
			}
		}

		child = handle.FirstChildElement("brush").ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			// A simple brush
			if(std::string(child->Name()) == "brush") {
				tinyxml2::XMLHandle bspHandle(child);
				SBSPBrushDefinition brush;

				// Default values
				brush.id = SCENE_DEFAULT_ID;
				brush.collision = true;

				// Get the brush options
				bspHandle.ToElement()->QueryBoolAttribute("collisions", &brush.collision);
				bspHandle.ToElement()->QueryIntAttribute("id", &brush.id);

				AddBSPBrushData(bspHandle, brush);
				brushDefinition.push_back(brush);
			}
		}

		// Add all the geometry in the bsp scene node
		static_cast<CBSPSceneNode*>(node)->LoadGeometry(brushDefinition);
	}

	void CSceneLoader::LoadSimpleStructure(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node) {
		LOG_TRACE();

		tinyxml2::XMLElement * child;
		std::vector<SSimpleBrush> brushes;
		node = new CSimpleSceneNode(scene);
		std::string skyName;
		CVector3F ambientColor, skyAmbientColor;

		// Get the ambient color
		if(handle.ToElement()->Attribute("ambientColor") != nullptr)
			ambientColor = CVector3F::GetFrom(handle.ToElement()->Attribute("ambientColor"));
		static_cast<CBSPSceneNode*>(node)->SetAmbientColor(CVector4F(ambientColor.x, ambientColor.y, ambientColor.z, 1));

		// Get the sky name
		if(handle.ToElement()->Attribute("skyName") != nullptr)
			skyName = handle.ToElement()->Attribute("skyName");
		static_cast<CBSPSceneNode*>(node)->SetSkyName(skyName);

		// Get the sky ambient color
		if(handle.ToElement()->Attribute("skyAmbientColor") != nullptr)
			skyAmbientColor = CVector3F::GetFrom(handle.ToElement()->Attribute("skyAmbientColor"));
		static_cast<CBSPSceneNode*>(node)->SetSkyAmbientColor(CVector4F(skyAmbientColor.x, skyAmbientColor.y, skyAmbientColor.z, 1));

		// Read the level geometry
		child = handle.FirstChildElement("multi_brush").ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			// A brush created with a bunch of static models
			if(std::string(child->Name()) == "multi_brush") {
				tinyxml2::XMLHandle bspHandle(child);
				tinyxml2::XMLElement * child2 = bspHandle.FirstChildElement("brush").ToElement();
				SSimpleBrush brush;

				for(child2; child2; child2 = child2->NextSiblingElement()) {
					if(std::string(child2->Name()) == "brush") {
						AddSimpleBrushData(tinyxml2::XMLHandle(child2), brush);
						brush.isMultiBrush = true;
					}
				}

				if(brush.geometry.size() > 0) {
					brushes.push_back(brush);
				}
			}
		}

		child = handle.FirstChildElement("brush").ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			// A simple brush
			if(std::string(child->Name()) == "brush") {
				tinyxml2::XMLHandle bspHandle(child);
				SSimpleBrush brush;

				AddSimpleBrushData(bspHandle, brush);
				brush.isMultiBrush = false;
				brushes.push_back(brush);
			}
		}

		// Add all the geometry in the bsp scene node
		static_cast<CSimpleSceneNode*>(node)->LoadGeometry(brushes);
	}

	void CSceneLoader::LoadParticleSystem(I3DScene * scene, tinyxml2::XMLHandle & handle) {
		LOG_TRACE();

		int particleSystemSize;
		std::string atlasTexture;
		CVector2I atlasSize;

		// Read the system size
		GetNodeData(handle, "systemSize", particleSystemSize, 0);

		// Read the atlas texture
		GetNodeData(handle, "atlasTextureFile", atlasTexture, "particleAtlas.tga");
		GetNodeData(handle, "atlasTextureSize", atlasSize, CVector2I(1, 1));

		scene->GetParticleSystem().InitializeSystem(particleSystemSize);
		scene->GetParticleSystem().SetAtlasTexture(atlasTexture, atlasSize);
	}

	void CSceneLoader::AddBSPBrushData(tinyxml2::XMLHandle & handle, SBSPBrushDefinition & brushDefinition) {
		LOG_TRACE();

		bool visible, flatShading;
		CVector3F position, rotation, scale;
		std::string diffuseTexture, normalTexture, material;
		std::ostringstream ss;

		std::string name = handle.ToElement()->Attribute("file");
		int id = SCENE_DEFAULT_ID;
		handle.ToElement()->QueryIntAttribute("id", &id);

		GetNodeData(handle, "visible", visible, true);
		GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
		GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
		GetNodeData(handle, "scale", scale, CVector3F(1, 1, 1));
		GetNodeData(handle, "diffuse_texture", diffuseTexture, "");
		GetNodeData(handle, "normal_texture", normalTexture, "");
		GetNodeData(handle, "flat_shading", flatShading, false);
		GetNodeData(handle, "material", material, "default.mtl");

		// Modification matrixes
		CMatrix worldMatrix = CMatrix(scale, CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), position);
		CMatrix rotationMatrix = CMatrix(CVector3F(1, 1, 1), CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), CVector3F(0, 0, 0));

		CMaterial mat(material);

		// Create the geometry
		CStaticMesh mesh(name);
		for(int i = 0; i < mesh.GetMesh()->GetNumberOfGroups(); ++i) {
			CTexture2D diffuse;
			CTexture2D normal;

			if(diffuseTexture != "") {
				diffuse.CreateFromFile(diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			}

			if(normalTexture != "") {
				normal.CreateFromFile(normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			}

			for(int j = 0; j < mesh.GetMesh()->GetGroupSize(i); j += 3) {
				CBSPTriangle * triangle = new CBSPTriangle();
				triangle->SetId(id);

				ss.str("");
				ss << "group_" << (i + 1);

				for(int k = 0; k < 3; ++k) {
					triangle->SetTexcoord(mesh.GetMesh()->GetTexcoords(i)[j + k], k);
					triangle->SetVertex(worldMatrix.Transform(mesh.GetMesh()->GetVertices(i)[j + k]), k);
					triangle->SetNormal(rotationMatrix.Transform(mesh.GetMesh()->GetNormals(i)[j + k], true), k);
				}

				triangle->SetVisible(visible);
				triangle->SetTexture(diffuse, TEXTURE_TYPE_DIFFUSE);
				triangle->SetTexture(normal, TEXTURE_TYPE_NORMAL);
				triangle->SetMaterial(mat, this->m_materialsConf.GetValueString(name, ss.str(), "default"));
				triangle->CreatePlane();
				triangle->Update();

				// Add the triangle to the geometry
				brushDefinition.geometry.push_back(triangle);
			}
		}
	}

	void CSceneLoader::AddSimpleBrushData(tinyxml2::XMLHandle & handle, SSimpleBrush & brush) {
		LOG_TRACE();

		bool visible, flatShading;
		CVector3F position, rotation, scale;
		std::string diffuseTexture, normalTexture, material;
		std::ostringstream ss;

		std::string name = handle.ToElement()->Attribute("file");
		int id = SCENE_DEFAULT_ID;
		handle.ToElement()->QueryIntAttribute("id", &id);

		GetNodeData(handle, "visible", visible, true);
		GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
		GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
		GetNodeData(handle, "scale", scale, CVector3F(1, 1, 1));
		GetNodeData(handle, "diffuse_texture", diffuseTexture, "");
		GetNodeData(handle, "normal_texture", normalTexture, "");
		GetNodeData(handle, "flat_shading", flatShading, false);
		GetNodeData(handle, "material", material, "default.mtl");

		// Modification matrixes
		CMatrix worldMatrix = CMatrix(scale, CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), position);
		CMatrix rotationMatrix = CMatrix(CVector3F(1, 1, 1), CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), CVector3F(0, 0, 0));

		CMaterial mat(material);

		// Create the geometry
		CStaticMesh mesh(name);
		CVector3F minVector = CVector3F(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		CVector3F maxVector = CVector3F(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
		for(int i = 0; i < mesh.GetMesh()->GetNumberOfGroups(); ++i) {
			CTexture2D diffuse;
			CTexture2D normal;
			
			if(diffuseTexture != "") {
				diffuse.CreateFromFile(diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			} 

			if(normalTexture != "") {
				normal.CreateFromFile(normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			}

			for(int j = 0; j < mesh.GetMesh()->GetGroupSize(i); j += 3) {
				CTriangle3D * triangle = new CTriangle3D();
				triangle->SetId(id);

				ss.str("");
				ss << "group_" << (i + 1);

				for(int k = 0; k < 3; ++k) {
					triangle->SetTexcoord(mesh.GetMesh()->GetTexcoords(i)[j + k], k);
					triangle->SetVertex(worldMatrix.Transform(mesh.GetMesh()->GetVertices(i)[j + k]), k);
					triangle->SetNormal(rotationMatrix.Transform(mesh.GetMesh()->GetNormals(i)[j + k], true), k);

					// Update the min and max values
					minVector.x = std::min(minVector.x, triangle->GetVertex(k).x);
					minVector.y = std::min(minVector.y, triangle->GetVertex(k).y);
					minVector.z = std::min(minVector.z, triangle->GetVertex(k).z);

					maxVector.x = std::max(maxVector.x, triangle->GetVertex(k).x);
					maxVector.y = std::max(maxVector.y, triangle->GetVertex(k).y);
					maxVector.z = std::max(maxVector.z, triangle->GetVertex(k).z);
				}

				triangle->SetTexture(diffuse, TEXTURE_TYPE_DIFFUSE);
				triangle->SetTexture(normal, TEXTURE_TYPE_NORMAL);
				triangle->SetMaterial(mat, this->m_materialsConf.GetValueString(name, ss.str(), "default"));
				triangle->Update();

				// Find the triangle plane
				CVector3F tmp[] = { triangle->GetVertex(0), triangle->GetVertex(1), triangle->GetVertex(2) };
				CPlane3DF plane = CPlane3DF(tmp);
				triangle->SetFlatNormal(plane.GetNormal());

				// Add the triangle to the geometry
				brush.geometry.push_back(triangle);
			}
		}
		// Add the visibility shape to the geometry
		brush.visibilityShape = new CSphereF((maxVector + minVector) / 2.0f, (maxVector - minVector).Length() / 2.0f);

		// Add the original values
		brush.filename = name;
		brush.position = position;
		brush.rotation = rotation;
		brush.scale = scale;
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, bool & data, const bool defaultValue) {
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

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, int & data, const int defaultValue) {
		LOG_TRACE();

		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			std::istringstream ss(text->Value());
			ss >> data;
		} else {
			data = defaultValue;
		}
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector2I & data, const CVector2I & defaultValue) {
		LOG_TRACE();

		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			data = CVector2I::GetFrom(std::string(text->Value()), defaultValue);
		}
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector3F & data, const CVector3F & defaultValue) {
		LOG_TRACE();

		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			data = CVector3F::GetFrom(std::string(text->Value()), defaultValue);
		}
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector4F & data, const CVector4F & defaultValue) {
		LOG_TRACE();

		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			data = CVector4F::GetFrom(std::string(text->Value()), defaultValue);
		}
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CColor & data, const CColor & defaultValue) {
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

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, float & data, const float defaultValue) {
		LOG_TRACE();

		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			std::istringstream ss(text->Value());
			ss >> data;
		} else {
			data = defaultValue;
		}
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, std::string & data, const std::string & defaultValue) {
		LOG_TRACE();

		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr) {
			data = text->Value();
		} else {
			data = defaultValue;
		}
	}

	const std::map<std::string, std::string> CSceneLoader::GetNodeAttributes(tinyxml2::XMLHandle & handle) {
		LOG_TRACE();

		std::map<std::string, std::string> attributes;
		tinyxml2::XMLElement * child;
		std::string attributeName;

		child = handle.FirstChildElement("attributes").FirstChild().ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			tinyxml2::XMLHandle attributeHandle(child);

			// Get the attribute name
			attributeName = attributeHandle.ToElement()->Name();

			// Get the attribute value
			tinyxml2::XMLText * valueText = attributeHandle.FirstChild().ToText();

			// Add the attribue only if the two elements are valid
			if(valueText != nullptr) {
				attributes[attributeName] = valueText->Value();
			}
		}

		return attributes;
	}
}
