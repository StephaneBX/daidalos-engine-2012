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

		I3DScene * scene = new I3DScene(filename);

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
			} else {
				LoadDefaultParticleSystem(scene);
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

	void CSceneLoader::SaveToFile(I3DScene * object, const std::string & filename) {
		LOG_TRACE();

		this->m_structure = SCENE_NODE_ROOT_STRUCTURE_TYPE_BSP;

		tinyxml2::XMLDocument document;

		// Create the root element
		tinyxml2::XMLElement * sceneElement = document.NewElement("scene");
		sceneElement->SetAttribute("structure", SCENE_NODE_ROOT_STRUCTURE_TYPE_BSP);
		document.InsertEndChild(sceneElement);

		// Create the particle system element
		SaveParticleSystem(object, document, sceneElement);

		// Save the nodes
		SaveNode(object->GetRoot(), document, sceneElement);

		// Save the xml
		document.SaveFile(filename.c_str());

		// Update the scene filename
		object->SetFilename(filename);
	}

	void CSceneLoader::LoadNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle) {
		LOG_TRACE();

		// Global data variables
		std::map<std::string, std::string> attributes;
		tinyxml2::XMLElement * child = nullptr;
		ISceneNode * node = nullptr;
		int type = -1;
		handle.ToElement()->QueryIntAttribute("type", &type);
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
				if(parent == nullptr) {
					parent = node;
				}
				break;

				// BSP structure
			case SCENE_NODE_ROOT_STRUCTURE_TYPE_BSP:
				LoadBSPStructure(scene, parent, handle, node);
				if(parent == nullptr) {
					parent = node;
				}
				break;
			}

			break;

		case SCENE_NODE_CAMERA:			
			LoadCameraNode(scene, parent, handle, node, name);
			break;

		case SCENE_NODE_SPOTLIGHT:
			LoadSpotLightNode(scene, parent, handle, node, name);;
			break;

		case SCENE_NODE_POINTLIGHT:
			LoadPointLightNode(scene, parent, handle, node, name);
			break;

		case SCENE_NODE_DIRECTIONALLIGHT:
			LoadDirectionalLightNode(scene, parent, handle, node, name);
			break;

		case SCENE_NODE_STATICMESH:
			LoadStaticMeshNode(scene, parent, handle, node, name);
			break;

		case SCENE_NODE_ANIMATEDMESH:
			LoadAnimatedMeshNode(scene, parent, handle, node, name);
			break;

		case SCENE_NODE_SOUND:
			LoadSoundNode(scene, parent, handle, node, name);
			break;

		case SCENE_NODE_EMPTY:
			LoadEmptyNode(scene, parent, handle, node, name);
			break;

		case SCENE_NODE_PARTICLE_EMITTER:
			LoadParticleEmitterNode(scene, parent, handle, node, name);
			break;

		default:
			// Invalid or missing type
			LOG_WARNING("Invalid or missing node attribute 'type' in '" << this->m_filename << "'");
			break;
		}

		// If a node has been successfully created
		if(node != nullptr) {
			// Set the id
			node->SetId(id);

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

		// Check for bounding box declared in nodes
		child = handle.FirstChildElement("node").ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			// A simple brush
			if(std::string(child->Name()) == "node") {
				tinyxml2::XMLHandle bspHandle(child);
				std::string boundBoxFile;

				GetNodeData(bspHandle, "boundingBox", boundBoxFile, "");

				// Bounding box detected
				if(boundBoxFile != "") {
					CVector3F position, rotation, scale, nodeDefaultScale;

					// Get the node position, rotation etc
					GetNodeData(bspHandle, "position", position, CVector3F(0, 0, 0));
					GetNodeData(bspHandle, "rotation", rotation, CVector3F(0, 0, 0));
					GetNodeData(bspHandle, "scale", scale, CVector3F(1, 1, 1));

					SBSPBrushDefinition brush;

					// Default values
					brush.id = SCENE_DEFAULT_ID;
					brush.collision = true;

					// Get the brush options
					bspHandle.ToElement()->QueryIntAttribute("id", &brush.id);

					AddBSPBoundingBoxData(boundBoxFile, position, rotation, scale, brush);
					brushDefinition.push_back(brush);
				}
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
		std::string atlasTexture, templateName;
		CVector2I atlasSize;
		CVector3F gravity;

		// Read the system size
		GetNodeData(handle, "systemSize", particleSystemSize, 0);

		// Read the atlas texture
		GetNodeData(handle, "atlasTextureFile", atlasTexture, "particleAtlas.tga");
		GetNodeData(handle, "atlasTextureSize", atlasSize, CVector2I(1, 1));

		// Read the gravity
		GetNodeData(handle, "gravity", gravity, CVector3F());

		// Read the emitters templates
		tinyxml2::XMLElement * emittersTemplates = handle.FirstChildElement("particleEmittersTemplates").ToElement();
		if(emittersTemplates != nullptr) {
			// Read the templates
			tinyxml2::XMLHandle & templatesHandle = tinyxml2::XMLHandle(emittersTemplates);
			tinyxml2::XMLElement * templateElement = templatesHandle.FirstChildElement("template").ToElement();

			if(templateElement != nullptr) {
				for(templateElement; templateElement; templateElement = templateElement->NextSiblingElement()) {
					tinyxml2::XMLHandle & templateHandle = tinyxml2::XMLHandle(templateElement);
					tinyxml2::XMLText * text = templateHandle.FirstChild().ToText();
					if(text != nullptr) {
						scene->GetParticleSystem().LoadEmittersTemplate(text->Value());
					}
				}
			}
		}

		scene->GetParticleSystem().InitializeSystem(particleSystemSize);
		scene->GetParticleSystem().SetAtlasTexture(atlasTexture, atlasSize);
		scene->GetParticleSystem().SetGravity(gravity);
	}

	void CSceneLoader::LoadDefaultParticleSystem(I3DScene * scene) {
		LOG_TRACE();

		scene->GetParticleSystem().InitializeSystem(5000);
		scene->GetParticleSystem().SetAtlasTexture("particleAtlas.tga", CVector2I(1, 1));
	}

	/**
	* Load a camera node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadCameraNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		float fov, ratio, near, far;
		CVector3F position, rotation, scale;

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
	}

	/**
	* Load a spot light node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadSpotLightNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		bool enabled, shadowCaster;
		float range, radius, falloff, tightness;
		CVector3F position, rotation, scale;
		CVector4F nodeDefaultColor;
		CColor color;

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
	}

	/**
	* Load a camera node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadPointLightNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		bool enabled, shadowCaster;
		float range, falloff;
		CVector3F position;
		CVector4F nodeDefaultColor;
		CColor color;

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
	}

	/**
	* Load a directional light node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadDirectionalLightNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		bool enabled, shadowCaster;
		CVector4F nodeDefaultColor;
		CVector3F rotation;
		CColor color;

		// Search for defined values in the element or use default values
		GetNodeData(handle, "enabled", enabled, true);
		GetNodeData(handle, "shadow_caster", shadowCaster, false);
		GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
		nodeDefaultColor = CVector4F::GetFrom(this->m_nodesConf.GetValueString("IPointLightSceneNode", "color", "1.0f:1.0f:1.0f:1.0f"));
		GetNodeData(handle, "color", color, CColor(nodeDefaultColor.x, nodeDefaultColor.y, nodeDefaultColor.z, nodeDefaultColor.w));

		// Create the node
		node = new CDirectionalLightSceneNode(scene, parent, name, rotation, color, shadowCaster, enabled);
	}

	/**
	* Load a static mesh node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadStaticMeshNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		bool enabled, shadowCaster;
		int meshShading;
		std::string diffuseTexture, normalTexture, material, file, boundingBox;
		CVector3F position, rotation, scale, nodeDefaultScale;

		// Search for defined values in the element or use default values
		GetNodeData(handle, "enabled", enabled, true);
		GetNodeData(handle, "shadow_caster", shadowCaster, true);
		GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
		GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
		nodeDefaultScale = CVector3F::GetFrom(this->m_nodesConf.GetValueString("IStaticMeshSceneNode", "scale", "1.0f:1.0f:1.0f"));
		GetNodeData(handle, "scale", scale, CVector3F(nodeDefaultScale.x, nodeDefaultScale.y, nodeDefaultScale.z));
		GetNodeData(handle, "diffuse_texture", diffuseTexture, "default.png");
		GetNodeData(handle, "normal_texture", normalTexture, "default.png");
		GetNodeData(handle, "meshShading", meshShading, 0);
		GetNodeData(handle, "material", material, "default.mtl");
		GetNodeData(handle, "boundingBox", boundingBox, "");
		file = handle.ToElement()->Attribute("file");

		// Create the node
		node = new CStaticMeshSceneNode(scene, parent, name, file, position, rotation, scale);
		node->SetEnabled(enabled);
		node->SetShadowCaster(shadowCaster);
		static_cast<CStaticMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_DIFFUSE, diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		static_cast<CStaticMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_NORMAL, normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		static_cast<CStaticMeshSceneNode*>(node)->SetMeshShadingType((EMeshShadingType)meshShading);
		static_cast<CStaticMeshSceneNode*>(node)->SetMeshMaterial(material);
		static_cast<CStaticMeshSceneNode*>(node)->SetBoundingBox(boundingBox);
	}

	/**
	* Load an animated mesh node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadAnimatedMeshNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		bool enabled, shadowCaster;
		int meshShading;
		std::string diffuseTexture, normalTexture, material, file, boundingBox;
		CVector3F position, rotation, scale, nodeDefaultScale;

		// Search for defined values in the element or use default values
		GetNodeData(handle, "enabled", enabled, true);
		GetNodeData(handle, "shadow_caster", shadowCaster, true);
		GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
		GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
		nodeDefaultScale = CVector3F::GetFrom(this->m_nodesConf.GetValueString("IStaticMeshSceneNode", "scale", "1.0f:1.0f:1.0f"));
		GetNodeData(handle, "scale", scale, CVector3F(nodeDefaultScale.x, nodeDefaultScale.y, nodeDefaultScale.z));
		GetNodeData(handle, "diffuse_texture", diffuseTexture, "default.png");
		GetNodeData(handle, "normal_texture", normalTexture, "default.png");
		GetNodeData(handle, "meshShading", meshShading, 0);
		GetNodeData(handle, "material", material, "default.mtl");
		GetNodeData(handle, "boundingBox", boundingBox, "");
		file = handle.ToElement()->Attribute("file");

		// Create the node
		node = new CAnimatedMeshSceneNode(scene, parent, name, file, position, rotation, scale);
		node->SetEnabled(enabled);
		node->SetShadowCaster(shadowCaster);
		static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_DIFFUSE, diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_NORMAL, normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
		static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshShadingType((EMeshShadingType)meshShading);
		static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshMaterial(material);
		static_cast<CAnimatedMeshSceneNode*>(node)->SetBoundingBox(boundingBox);
	}

	/**
	* Load a sound node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadSoundNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		bool relativeToListener, looped, playOnStartup;
		int soundType, soundCategory;
		float coneInnerAngle, coneOuterAngle, referenceDistance, maxDistance;
		std::string file;
		CVector3F position, rotation;
		
		GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
		GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));
		GetNodeData(handle, "type", soundType, this->m_nodesConf.GetValueBool("ISoundSceneNode", "type", 0));
		GetNodeData(handle, "category", soundCategory, this->m_nodesConf.GetValueBool("ISoundSceneNode", "category", 0));
		GetNodeData(handle, "cone_inner_angle", coneInnerAngle, this->m_nodesConf.GetValueFloat("ISoundSceneNode", "cone_inner_angle", 360.0f));
		GetNodeData(handle, "cone_outer_angle", coneOuterAngle, this->m_nodesConf.GetValueFloat("ISoundSceneNode", "cone_outer_angle", 360.0f));
		GetNodeData(handle, "reference_distance", referenceDistance, this->m_nodesConf.GetValueFloat("ISoundSceneNode", "reference_distance", 2));
		GetNodeData(handle, "max_distance", maxDistance, this->m_nodesConf.GetValueFloat("ISoundSceneNode", "max_distance", 20));
		GetNodeData(handle, "relative_to_listener", relativeToListener, this->m_nodesConf.GetValueBool("ISoundSceneNode", "relative_to_listener", false));
		GetNodeData(handle, "looped", looped, this->m_nodesConf.GetValueBool("ISoundSceneNode", "looped", false));
		GetNodeData(handle, "play_on_startup", playOnStartup, this->m_nodesConf.GetValueBool("ISoundSceneNode", "play_on_startup", false));
		GetNodeData(handle, "file", file, "");

		node = new CSoundSceneNode(scene, file, static_cast<ESoundType>(soundType), static_cast<ESoundCategory>(soundCategory), parent, name);
		static_cast<CSoundSceneNode*>(node)->SetPosition(position);
		static_cast<CSoundSceneNode*>(node)->SetRotation(rotation);
		static_cast<CSoundSceneNode*>(node)->SetSoundConeInnerAngle(coneInnerAngle);
		static_cast<CSoundSceneNode*>(node)->SetSoundConeOuterAngle(coneInnerAngle);
		static_cast<CSoundSceneNode*>(node)->SetSoundReferenceDistance(referenceDistance);
		static_cast<CSoundSceneNode*>(node)->SetSoundMaxDistance(maxDistance);
		static_cast<CSoundSceneNode*>(node)->SetSoundRelativeToListener(relativeToListener);
		static_cast<CSoundSceneNode*>(node)->SetSoundLooped(looped);
		if(playOnStartup) {
			static_cast<CSoundSceneNode*>(node)->SetSoundAutoStart(true);
			static_cast<CSoundSceneNode*>(node)->Update();
			static_cast<CSoundSceneNode*>(node)->SetSoundStatus(SOUND_STATUS_PLAY);
		}
	}

	/**
	* Load an empty node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadEmptyNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		bool enabled;
		CVector3F position, rotation;

		// Search for defined values in the element or use default values
		GetNodeData(handle, "enabled", enabled, true);
		GetNodeData(handle, "position", position, CVector3F(0, 0, 0));
		GetNodeData(handle, "rotation", rotation, CVector3F(0, 0, 0));

		// Create the node
		node = new CEmptySceneNode(scene, parent, name, position);
		node->SetEnabled(enabled);
		static_cast<CEmptySceneNode*>(node)->SetRotation(rotation);
	}

	/**
	* Load a particle emitter node
	* @param scene The scene
	* @param parent The node parent
	* @param handle The xml handle
	* @param node The node to fill
	* @param name The name
	*/
	void CSceneLoader::LoadParticleEmitterNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name) {
		LOG_TRACE();

		bool emitterActive;
		int emitterTextureId;
		CVector3F emitterPosition;

		// Template attributes
		std::string templateName = "";
		if(handle.ToElement()->Attribute("templateName") != nullptr) {
			templateName = handle.ToElement()->Attribute("templateName");
		}
		std::string emitterName = "";
		if(handle.ToElement()->Attribute("emitterName") != nullptr) {
			emitterName = handle.ToElement()->Attribute("emitterName");
		}

		// Create the emitter with a template
		CParticleEmitter * emitter = scene->GetParticleSystem().InstanciateEmitter(templateName, emitterName);

		// Basic values
		GetNodeData(handle, "active", emitterActive, emitter->IsActive());
		GetNodeData(handle, "position", emitterPosition, CVector3F(0, 0, 0));
		GetNodeData(handle, "textureId", emitterTextureId, 0);
		
		node = new CParticleEmitterSceneNode(scene, parent, name, emitter);
		static_cast<CParticleEmitterSceneNode*>(node)->SetPosition(emitterPosition);
		static_cast<CParticleEmitterSceneNode*>(node)->SetTemplateName(templateName);
		static_cast<CParticleEmitterSceneNode*>(node)->SetEmitterName(emitterName);
		static_cast<CParticleEmitterSceneNode*>(node)->SetActive(emitterActive);
		static_cast<CParticleEmitterSceneNode*>(node)->SetTextureId(emitterTextureId);
	}

	void CSceneLoader::AddBSPBrushData(tinyxml2::XMLHandle & handle, SBSPBrushDefinition & brushDefinition) {
		LOG_TRACE();

		bool visible;
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

	/**
	* Add data into the bsp brush definition
	* @param file The mesh name
	* @param position The position
	* @param rotation The rotation
	* @param scale The scale
	* @param brushDefinition The brush definition
	*/
	void CSceneLoader::AddBSPBoundingBoxData(const std::string file, const CVector3F position, const CVector3F rotation, const CVector3F scale, SBSPBrushDefinition & brushDefinition) {
		LOG_TRACE();

		std::stringstream ss;

		// Modification matrixes
		CMatrix worldMatrix = CMatrix(scale, CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), position);
		CMatrix rotationMatrix = CMatrix(CVector3F(1, 1, 1), CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), CVector3F(0, 0, 0));

		CMaterial mat("default.mtl");

		// Create the geometry
		CStaticMesh mesh(file);
		for(int i = 0; i < mesh.GetMesh()->GetNumberOfGroups(); ++i) {

			for(int j = 0; j < mesh.GetMesh()->GetGroupSize(i); j += 3) {
				CBSPTriangle * triangle = new CBSPTriangle();
				triangle->SetId(brushDefinition.id);

				ss.str("");
				ss << "group_" << (i + 1);

				for(int k = 0; k < 3; ++k) {
					triangle->SetTexcoord(mesh.GetMesh()->GetTexcoords(i)[j + k], k);
					triangle->SetVertex(worldMatrix.Transform(mesh.GetMesh()->GetVertices(i)[j + k]), k);
					triangle->SetNormal(rotationMatrix.Transform(mesh.GetMesh()->GetNormals(i)[j + k], true), k);
				}

				triangle->SetVisible(false);
				triangle->SetMaterial(mat, this->m_materialsConf.GetValueString(file, ss.str(), "default"));
				triangle->CreatePlane();
				triangle->Update();

				// Add the triangle to the geometry
				brushDefinition.geometry.push_back(triangle);
			}
		}
	}

	void CSceneLoader::AddSimpleBrushData(tinyxml2::XMLHandle & handle, SSimpleBrush & brush) {
		LOG_TRACE();

		bool visible;
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

	void CSceneLoader::SaveNode(DE::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		if(node->IsSaved()) {
			switch(node->GetType()) {
			case SCENE_NODE_ROOT:
				if(this->m_structure == SCENE_NODE_ROOT_STRUCTURE_TYPE_SIMPLE) {
					SaveSimpleStructure(node, document, parent);
				} else {
					SaveBSPStructure(node, document, parent);
				}
				break;

			case SCENE_NODE_CAMERA:
				SaveCameraNode(node, document, parent);
				break;

			case SCENE_NODE_SPOTLIGHT:
				SaveSpotLightNode(node, document, parent);
				break;

			case SCENE_NODE_POINTLIGHT:
				SavePointLightNode(node, document, parent);
				break;

			case SCENE_NODE_DIRECTIONALLIGHT:
				SaveDirectionalLightNode(node, document, parent);
				break;

			case SCENE_NODE_STATICMESH:
				SaveStaticMeshNode(node, document, parent);
				break;

			case SCENE_NODE_ANIMATEDMESH:
				SaveAnimatedMeshNode(node, document, parent);
				break;

			case SCENE_NODE_SOUND:
				SaveSoundNode(node, document, parent);
				break;

			case SCENE_NODE_EMPTY:
				SaveEmptyNode(node, document, parent);
				break;

			case SCENE_NODE_PARTICLE_EMITTER:
				SaveParticleEmitterNode(node, document, parent);
				break;
			}
		}
	}

	void CSceneLoader::SaveParticleSystem(DE::I3DScene * scene, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" Particle System Parameters "));

		tinyxml2::XMLElement * particleSystemNode = document.NewElement("particleSystem");

		tinyxml2::XMLElement * atlasTextureFileNode = document.NewElement("atlasTextureFile");
		atlasTextureFileNode->InsertEndChild(document.NewText(scene->GetParticleSystem().GetAtlasTextureName().c_str()));
		particleSystemNode->InsertEndChild(atlasTextureFileNode);

		tinyxml2::XMLElement * atlasTextureSizeNode = document.NewElement("atlasTextureSize");
		atlasTextureSizeNode->InsertEndChild(document.NewText(scene->GetParticleSystem().GetAtlasTextureSize().ToString().c_str()));
		particleSystemNode->InsertEndChild(atlasTextureSizeNode);

		tinyxml2::XMLElement * systemSizeNode = document.NewElement("systemSize");
		systemSizeNode->InsertEndChild(document.NewText(std::to_string(scene->GetParticleSystem().GetSystemSize()).c_str()));
		particleSystemNode->InsertEndChild(systemSizeNode);

		std::vector<std::string> emitterTemplates = scene->GetParticleSystem().GetEmittersTemplatesName();
		if(emitterTemplates.size() > 0) {
			// Templates
			tinyxml2::XMLElement * particleSystemEmitterTemplatesNode = document.NewElement("particleEmittersTemplates");
			
			// Template
			for(auto it = emitterTemplates.begin(); it != emitterTemplates.end(); ++it) {
				tinyxml2::XMLElement * templateNode = document.NewElement("template");
				templateNode->InsertEndChild(document.NewText((*it).c_str()));
				particleSystemEmitterTemplatesNode->InsertEndChild(templateNode);
			}
			particleSystemNode->InsertEndChild(particleSystemEmitterTemplatesNode);
		}

		parent->InsertEndChild(particleSystemNode);
	}

	/**
	* Save the simple structure
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveSimpleStructure(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

	}

	/**
	* Save the bsp structure
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveBSPStructure(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

	}

	/**
	* Save a camera node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveCameraNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" Projection Camera Node "));

		std::stringstream ss;
		CProjectionCameraSceneNode * sceneNode = static_cast<CProjectionCameraSceneNode*>(node);
		tinyxml2::XMLElement * cameraNode = document.NewElement("node");

		// Type
		cameraNode->SetAttribute("type", SCENE_NODE_CAMERA);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			cameraNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		cameraNode->SetAttribute("name", sceneNode->GetName().c_str());
		
		// Position
		tinyxml2::XMLElement * child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		cameraNode->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(sceneNode->GetRotation().ToString().c_str()));
		cameraNode->InsertEndChild(child);

		// Scale
		child = document.NewElement("scale");
		child->InsertEndChild(document.NewText(sceneNode->GetScale().ToString().c_str()));
		cameraNode->InsertEndChild(child);

		// Fov
		child = document.NewElement("fov");
		ss << RadianToDegree(sceneNode->GetAngle());
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		cameraNode->InsertEndChild(child);

		// Near
		child = document.NewElement("near");
		ss.str("");
		ss << sceneNode->GetNearDistance();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		cameraNode->InsertEndChild(child);

		// Far
		child = document.NewElement("far");
		ss.str("");
		ss << sceneNode->GetFarDistance();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		cameraNode->InsertEndChild(child);

		// Attributes
		SaveNodeAttributes(sceneNode, document, cameraNode);

		parent->InsertEndChild(cameraNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, cameraNode);
		}
	}

	/**
	* Save a spot light node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveSpotLightNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" SpotLight Node "));

		std::stringstream ss;
		CSpotLightSceneNode * sceneNode = static_cast<CSpotLightSceneNode*>(node);
		tinyxml2::XMLElement * lightNode = document.NewElement("node");

		// Type
		lightNode->SetAttribute("type", SCENE_NODE_SPOTLIGHT);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			lightNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		lightNode->SetAttribute("name", sceneNode->GetName().c_str());

		// Enabled
		tinyxml2::XMLElement * child = document.NewElement("enabled");
		child->InsertEndChild(document.NewText(sceneNode->IsEnabled() ? "true" : "false"));
		lightNode->InsertEndChild(child);

		// Shadow
		child = document.NewElement("shadow_caster");
		child->InsertEndChild(document.NewText(sceneNode->IsShadowCaster() ? "true" : "false"));
		lightNode->InsertEndChild(child);

		// Position
		child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(sceneNode->GetRotation().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Color
		child = document.NewElement("color");
		child->InsertEndChild(document.NewText(sceneNode->GetColor().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Range
		child = document.NewElement("range");
		ss << sceneNode->GetRange();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		lightNode->InsertEndChild(child);

		// Radius
		ss.str("");
		child = document.NewElement("radius");
		ss << RadianToDegree(sceneNode->GetRadius());
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		lightNode->InsertEndChild(child);

		// Falloff
		ss.str("");
		child = document.NewElement("falloff");
		ss << sceneNode->GetFalloff();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		lightNode->InsertEndChild(child);

		// Tightness
		ss.str("");
		child = document.NewElement("tightness");
		ss << sceneNode->GetTightness();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		lightNode->InsertEndChild(child);

		// Attributes
		SaveNodeAttributes(sceneNode, document, lightNode);

		parent->InsertEndChild(lightNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, lightNode);
		}
	}

	/**
	* Save a point light node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SavePointLightNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" PointLight Node "));

		std::stringstream ss;
		CPointLightSceneNode * sceneNode = static_cast<CPointLightSceneNode*>(node);
		tinyxml2::XMLElement * lightNode = document.NewElement("node");

		// Type
		lightNode->SetAttribute("type", SCENE_NODE_POINTLIGHT);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			lightNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		lightNode->SetAttribute("name", sceneNode->GetName().c_str());

		// Enabled
		tinyxml2::XMLElement * child = document.NewElement("enabled");
		child->InsertEndChild(document.NewText(sceneNode->IsEnabled() ? "true" : "false"));
		lightNode->InsertEndChild(child);

		// Shadow
		child = document.NewElement("shadow_caster");
		child->InsertEndChild(document.NewText(sceneNode->IsShadowCaster() ? "true" : "false"));
		lightNode->InsertEndChild(child);

		// Position
		child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(sceneNode->GetRotation().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Color
		child = document.NewElement("color");
		child->InsertEndChild(document.NewText(sceneNode->GetColor().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Range
		child = document.NewElement("range");
		ss << sceneNode->GetRange();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		lightNode->InsertEndChild(child);

		// Falloff
		ss.str("");
		child = document.NewElement("falloff");
		ss << sceneNode->GetFalloff();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		lightNode->InsertEndChild(child);

		// Attributes
		SaveNodeAttributes(sceneNode, document, lightNode);

		parent->InsertEndChild(lightNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, lightNode);
		}
	}

	/**
	* Save a directional light node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveDirectionalLightNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" DirectionalLight Node "));

		std::stringstream ss;
		CDirectionalLightSceneNode * sceneNode = static_cast<CDirectionalLightSceneNode*>(node);
		tinyxml2::XMLElement * lightNode = document.NewElement("node");

		// Type
		lightNode->SetAttribute("type", SCENE_NODE_DIRECTIONALLIGHT);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			lightNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		lightNode->SetAttribute("name", sceneNode->GetName().c_str());

		// Enabled
		tinyxml2::XMLElement * child = document.NewElement("enabled");
		child->InsertEndChild(document.NewText(sceneNode->IsEnabled() ? "true" : "false"));
		lightNode->InsertEndChild(child);

		// Shadow
		child = document.NewElement("shadow_caster");
		child->InsertEndChild(document.NewText(sceneNode->IsShadowCaster() ? "true" : "false"));
		lightNode->InsertEndChild(child);

		// Position
		child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(sceneNode->GetRotation().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Color
		child = document.NewElement("color");
		child->InsertEndChild(document.NewText(sceneNode->GetColor().ToString().c_str()));
		lightNode->InsertEndChild(child);

		// Attributes
		SaveNodeAttributes(sceneNode, document, lightNode);

		parent->InsertEndChild(lightNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, lightNode);
		}
	}

	/**
	* Save a static mesh node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveStaticMeshNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" Static Mesh Node "));

		std::stringstream ss;
		CStaticMeshSceneNode * sceneNode = static_cast<CStaticMeshSceneNode*>(node);
		tinyxml2::XMLElement * meshNode = document.NewElement("node");

		// Type
		meshNode->SetAttribute("type", SCENE_NODE_STATICMESH);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			meshNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		meshNode->SetAttribute("name", sceneNode->GetName().c_str());

		// File
		meshNode->SetAttribute("file", sceneNode->GetMeshName().c_str());

		// Enabled
		tinyxml2::XMLElement * child = document.NewElement("enabled");
		child->InsertEndChild(document.NewText(sceneNode->IsEnabled() ? "true" : "false"));
		meshNode->InsertEndChild(child);

		// Shadow
		child = document.NewElement("shadow_caster");
		child->InsertEndChild(document.NewText(sceneNode->IsShadowCaster() ? "true" : "false"));
		meshNode->InsertEndChild(child);

		// Position
		child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		meshNode->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(sceneNode->GetRotation().ToString().c_str()));
		meshNode->InsertEndChild(child);

		// Scale
		child = document.NewElement("scale");
		child->InsertEndChild(document.NewText(sceneNode->GetScale().ToString().c_str()));
		meshNode->InsertEndChild(child);

		// Diffuse texture
		if(sceneNode->GetMeshTextureName(TEXTURE_TYPE_DIFFUSE) != "") {
			child = document.NewElement("diffuse_texture");
			child->InsertEndChild(document.NewText(sceneNode->GetMeshTextureName(TEXTURE_TYPE_DIFFUSE).c_str()));
			meshNode->InsertEndChild(child);
		}

		// Normal texture
		if(sceneNode->GetMeshTextureName(TEXTURE_TYPE_NORMAL) != "") {
			child = document.NewElement("normal_texture");
			child->InsertEndChild(document.NewText(sceneNode->GetMeshTextureName(TEXTURE_TYPE_NORMAL).c_str()));
			meshNode->InsertEndChild(child);
		}

		// Mesh shading
		child = document.NewElement("meshShading");
		child->InsertEndChild(document.NewText(sceneNode->GetMeshShadingType() == MESH_SHADING_FLAT ? "0" : "1"));
		meshNode->InsertEndChild(child);

		// Material
		child = document.NewElement("material");
		child->InsertEndChild(document.NewText(sceneNode->GetMeshMaterialName().c_str()));
		meshNode->InsertEndChild(child);

		// Bounding box
		if(sceneNode->GetBoundingBox() != "") {
			child = document.NewElement("boundingBox");
			child->InsertEndChild(document.NewText(sceneNode->GetBoundingBox().c_str()));
			meshNode->InsertEndChild(child);
		}

		// Attributes
		SaveNodeAttributes(sceneNode, document, meshNode);

		parent->InsertEndChild(meshNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, meshNode);
		}
	}

	/**
	* Save an animated mesh node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveAnimatedMeshNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" Animated Mesh Node "));

		std::stringstream ss;
		CAnimatedMeshSceneNode * sceneNode = static_cast<CAnimatedMeshSceneNode*>(node);
		tinyxml2::XMLElement * meshNode = document.NewElement("node");

		// Type
		meshNode->SetAttribute("type", SCENE_NODE_ANIMATEDMESH);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			meshNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		meshNode->SetAttribute("name", sceneNode->GetName().c_str());

		// File
		meshNode->SetAttribute("file", sceneNode->GetMeshName().c_str());

		// Enabled
		tinyxml2::XMLElement * child = document.NewElement("enabled");
		child->InsertEndChild(document.NewText(sceneNode->IsEnabled() ? "true" : "false"));
		meshNode->InsertEndChild(child);

		// Shadow
		child = document.NewElement("shadow_caster");
		child->InsertEndChild(document.NewText(sceneNode->IsShadowCaster() ? "true" : "false"));
		meshNode->InsertEndChild(child);

		// Position
		child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		meshNode->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(sceneNode->GetRotation().ToString().c_str()));
		meshNode->InsertEndChild(child);

		// Scale
		child = document.NewElement("scale");
		child->InsertEndChild(document.NewText(sceneNode->GetScale().ToString().c_str()));
		meshNode->InsertEndChild(child);

		// Diffuse texture
		if(sceneNode->GetMeshTextureName(TEXTURE_TYPE_DIFFUSE) != "") {
			child = document.NewElement("diffuse_texture");
			child->InsertEndChild(document.NewText(sceneNode->GetMeshTextureName(TEXTURE_TYPE_DIFFUSE).c_str()));
			meshNode->InsertEndChild(child);
		}

		// Normal texture
		if(sceneNode->GetMeshTextureName(TEXTURE_TYPE_NORMAL) != "") {
			child = document.NewElement("normal_texture");
			child->InsertEndChild(document.NewText(sceneNode->GetMeshTextureName(TEXTURE_TYPE_NORMAL).c_str()));
			meshNode->InsertEndChild(child);
		}

		// Mesh shading
		child = document.NewElement("meshShading");
		child->InsertEndChild(document.NewText(sceneNode->GetMeshShadingType() == MESH_SHADING_FLAT ? "0" : "1"));
		meshNode->InsertEndChild(child);

		// Material
		child = document.NewElement("material");
		child->InsertEndChild(document.NewText(sceneNode->GetMeshMaterialName().c_str()));
		meshNode->InsertEndChild(child);

		// Bounding box
		if(sceneNode->GetBoundingBox() != "") {
			child = document.NewElement("boundingBox");
			child->InsertEndChild(document.NewText(sceneNode->GetBoundingBox().c_str()));
			meshNode->InsertEndChild(child);
		}

		// Attributes
		SaveNodeAttributes(sceneNode, document, meshNode);

		parent->InsertEndChild(meshNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, meshNode);
		}
	}

	/**
	* Save a sound node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveSoundNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" Sound Node "));

		std::stringstream ss;
		CSoundSceneNode * sceneNode = static_cast<CSoundSceneNode*>(node);
		tinyxml2::XMLElement * soundNode = document.NewElement("node");

		// Type
		soundNode->SetAttribute("type", SCENE_NODE_SOUND);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			soundNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		soundNode->SetAttribute("name", sceneNode->GetName().c_str());

		// Position
		tinyxml2::XMLElement * child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		soundNode->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(sceneNode->GetRotation().ToString().c_str()));
		soundNode->InsertEndChild(child);

		// Type
		ss.str("");
		child = document.NewElement("type");
		ss << sceneNode->GetSoundType();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		soundNode->InsertEndChild(child);

		// Category
		ss.str("");
		child = document.NewElement("category");
		ss << sceneNode->GetSoundCategory();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		soundNode->InsertEndChild(child);

		// Cone inner angle
		ss.str("");
		child = document.NewElement("cone_inner_angle");
		ss << sceneNode->GetSoundConeInnerAngle();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		soundNode->InsertEndChild(child);

		// Cone outer angle
		ss.str("");
		child = document.NewElement("cone_outer_angle");
		ss << sceneNode->GetSoundConeOuterAngle();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		soundNode->InsertEndChild(child);

		// Reference distance
		ss.str("");
		child = document.NewElement("reference_distance");
		ss << sceneNode->GetSoundReferenceDistance();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		soundNode->InsertEndChild(child);

		// Max distance
		ss.str("");
		child = document.NewElement("max_distance");
		ss << sceneNode->GetSoundMaxDistance();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		soundNode->InsertEndChild(child);

		// Relative to listener
		child = document.NewElement("relative_to_listener");
		child->InsertEndChild(document.NewText(sceneNode->IsSoundRelativeToListener() ? "true" : "false"));
		soundNode->InsertEndChild(child);

		// Looped
		child = document.NewElement("looped");
		child->InsertEndChild(document.NewText(sceneNode->IsSoundLooped() ? "true" : "false"));
		soundNode->InsertEndChild(child);

		// Play on startup
		ss.str("");
		child = document.NewElement("play_on_startup");
		ss << sceneNode->IsSoundAutoStart();
		child->InsertEndChild(document.NewText(sceneNode->IsSoundAutoStart() ? "true" : "false"));
		soundNode->InsertEndChild(child);

		// File
		child = document.NewElement("file");
		child->InsertEndChild(document.NewText(sceneNode->GetSoundName().c_str()));
		soundNode->InsertEndChild(child);

		// Attributes
		SaveNodeAttributes(sceneNode, document, soundNode);

		parent->InsertEndChild(soundNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, soundNode);
		}
	}

	/**
	* Save an empty node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveEmptyNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" Empty Node "));

		std::stringstream ss;
		CEmptySceneNode * sceneNode = static_cast<CEmptySceneNode*>(node);
		tinyxml2::XMLElement * emptyNode = document.NewElement("node");

		// Type
		emptyNode->SetAttribute("type", SCENE_NODE_EMPTY);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			emptyNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		emptyNode->SetAttribute("name", sceneNode->GetName().c_str());

		// Position
		tinyxml2::XMLElement * child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		emptyNode->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(sceneNode->GetRotation().ToString().c_str()));
		emptyNode->InsertEndChild(child);

		// Attributes
		SaveNodeAttributes(sceneNode, document, emptyNode);

		parent->InsertEndChild(emptyNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, emptyNode);
		}
	}

	/**
	* Save a particle emitter node
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CSceneLoader::SaveParticleEmitterNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" Particle Emitter Node "));

		std::stringstream ss;
		CParticleEmitterSceneNode * sceneNode = static_cast<CParticleEmitterSceneNode*>(node);
		tinyxml2::XMLElement * emitterNode = document.NewElement("node");

		// Type
		emitterNode->SetAttribute("type", SCENE_NODE_PARTICLE_EMITTER);

		// Id
		if(sceneNode->GetId() != SCENE_DEFAULT_ID) {
			emitterNode->SetAttribute("id", sceneNode->GetId());
		}

		// Name
		emitterNode->SetAttribute("name", sceneNode->GetName().c_str());

		// Template name
		if(sceneNode->GetTemplateName() != "") {
			emitterNode->SetAttribute("templateName", sceneNode->GetTemplateName().c_str());
		}

		// Emitter name
		if(sceneNode->GetEmitterName() != "") {
			emitterNode->SetAttribute("emitterName", sceneNode->GetEmitterName().c_str());
		}

		// Active
		ss.str("");
		tinyxml2::XMLElement * child = document.NewElement("active");
		ss << sceneNode->IsActive();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		emitterNode->InsertEndChild(child);

		// Position
		child = document.NewElement("position");
		child->InsertEndChild(document.NewText(sceneNode->GetPosition().ToString().c_str()));
		emitterNode->InsertEndChild(child);

		// Texture id
		ss.str("");
		child = document.NewElement("textureId");
		ss << sceneNode->GetEmitter()->GetTextureId();
		child->InsertEndChild(document.NewText(ss.str().c_str()));
		emitterNode->InsertEndChild(child);

		// Attributes
		SaveNodeAttributes(sceneNode, document, emitterNode);

		parent->InsertEndChild(emitterNode);

		// Save the children
		for(auto it = node->GetChildren().begin(); it != node->GetChildren().end(); it++) {
			SaveNode((*it), document, emitterNode);
		}
	}

	/**
	* Save the node attributes
	* @param node The node
	* @param document The xml document
	* @param parent The element
	*/
	void CSceneLoader::SaveNodeAttributes(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		std::map<std::string, std::string> attributes = node->GetAttributes();

		if(!attributes.empty()) {
			tinyxml2::XMLElement * child = document.NewElement("attributes");

			for(auto it = attributes.begin(); it != attributes.end(); it++) {
				tinyxml2::XMLElement * element = document.NewElement((*it).first.c_str());
				element->InsertEndChild(document.NewText((*it).second.c_str()));
				child->InsertEndChild(element);
			}

			parent->InsertEndChild(child);
		}
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
