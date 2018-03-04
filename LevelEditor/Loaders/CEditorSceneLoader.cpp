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

#include <Scene/3D/CStaticMeshSceneNode.hpp>
#include <Scene/3D/CSoundSceneNode.hpp>
#include <Core/IRenderer.hpp>
#include "../3D/CEditorLevelSceneNode.hpp"
#include "../Loaders/CEditorSceneLoader.hpp"
#include "../CEditorConfiguration.hpp"

namespace daidaloseditor {

	/**
	* Override and use a simple node system
	* @param parent The node parent
	* @param child The bsp node root
	* @param node The node to fill
	*/
	void CEditorSceneLoader::LoadBSPStructure(daidalosengine::I3DScene * scene, daidalosengine::ISceneNode *& parent, tinyxml2::XMLHandle & handle, daidalosengine::ISceneNode *& node) {
		LoadEditorLevelStucture(scene, parent, handle, node);
	}

	/**
	* Override and use a simple node system
	* @param parent The node parent
	* @param child The bsp node root
	* @param node The node to fill
	*/
	void CEditorSceneLoader::LoadSimpleStructure(daidalosengine::I3DScene * scene, daidalosengine::ISceneNode *& parent, tinyxml2::XMLHandle & handle, daidalosengine::ISceneNode *& node) {
		LoadEditorLevelStucture(scene, parent, handle, node);
	}

	/**
	* Load a sound node
	* @param parent The node parent
	* @param child The bsp node root
	* @param node The node to fill
	*/
	void CEditorSceneLoader::LoadSoundNode(daidalosengine::I3DScene * scene, daidalosengine::ISceneNode *& parent, tinyxml2::XMLHandle & handle, daidalosengine::ISceneNode *& node, const std::string & name) {

		daidalosengine::CSceneLoader::LoadSoundNode(scene, parent, handle, node, name);
		static_cast<daidalosengine::CSoundSceneNode*>(node)->SetSoundStatus(daidalosengine::SOUND_STATUS_PAUSE);
	}

	/**
	* Save the simple structure
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CEditorSceneLoader::SaveSimpleStructure(daidalosengine::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {
		SaveBSPStructure(node, document, parent);
	}

	/**
	* Save the bsp structure
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CEditorSceneLoader::SaveBSPStructure(daidalosengine::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" BSP Level Node "));

		daidalosengine::ILevelSceneNode * levelNode = static_cast<daidalosengine::ILevelSceneNode*>(node);

		// Level node
		tinyxml2::XMLElement * levelElement = document.NewElement("node");

		// Type
		levelElement->SetAttribute("type", daidalosengine::SCENE_NODE_ROOT);

		// Name
		levelElement->SetAttribute("name", "level");

		// Ambient color
		levelElement->SetAttribute("ambientColor", levelNode->GetAmbientColor().ToString().c_str());

		// Skybox name
		levelElement->SetAttribute("skyName", levelNode->GetSkyName().c_str());

		// Skybox ambient color
		levelElement->SetAttribute("skyAmbientColor", levelNode->GetSkyAmbientColor().ToString().c_str());

		// Multibrush
		tinyxml2::XMLElement * multiBrush = document.NewElement("multi_brush");

		// Search for multi brush elements
		for(auto it = levelNode->GetChildren().begin(); it != levelNode->GetChildren().end(); it++) {

			if(std::string(EDITOR_NODE_NAME_MULTIBRUSH).compare((*it)->GetName()) == 0) {
				// The child is part of the multi brush, add it to the multi_brush element
				SaveBrush((*it), document, multiBrush);
			} else if(std::string(EDITOR_NODE_NAME_BRUSH).compare((*it)->GetName()) == 0) {
				// The child is a brush, add it to the level node
				SaveBrush((*it), document, levelElement);
			} else {
				// The child is not part on multi brush, add it to the level node
				SaveNode((*it), document, levelElement);
			}
		}

		levelElement->InsertFirstChild(multiBrush);
		parent->InsertEndChild(levelElement);
	}

	/**
	* Replace the bsp and simple structure with an editor specific one
	* @param parent The node parent
	* @param child The bsp node root
	* @param node The node to fill
	*/
	void CEditorSceneLoader::LoadEditorLevelStucture(daidalosengine::I3DScene * scene, daidalosengine::ISceneNode *& parent, tinyxml2::XMLHandle & handle, daidalosengine::ISceneNode *& node) {
		tinyxml2::XMLElement * child;
		node = new CEditorLevelSceneNode(scene);
		std::string skyName;
		daidalosengine::CVector3F ambientColor;
		daidalosengine::CVector3F skyAmbientColor;
		daidalosengine::IRenderer * renderer = daidalosengine::IRenderer::GetRenderer();

		// Get the ambient color
		if(handle.ToElement()->Attribute("ambientColor") != nullptr)
			ambientColor = daidalosengine::CVector3F::GetFrom(handle.ToElement()->Attribute("ambientColor"));
		static_cast<CEditorLevelSceneNode*>(node)->SetAmbientColor(daidalosengine::CVector4F(ambientColor.x, ambientColor.y, ambientColor.z, 1));

		// Get the sky name
		if(handle.ToElement()->Attribute("skyName") != nullptr)
			skyName = handle.ToElement()->Attribute("skyName");
		static_cast<CEditorLevelSceneNode*>(node)->SetSkyName(skyName);

		// Get the sky ambient color
		if(handle.ToElement()->Attribute("skyAmbientColor") != nullptr)
			skyAmbientColor = daidalosengine::CVector3F::GetFrom(handle.ToElement()->Attribute("skyAmbientColor"));
		static_cast<CEditorLevelSceneNode*>(node)->SetSkyAmbientColor(daidalosengine::CVector4F(skyAmbientColor.x, skyAmbientColor.y, skyAmbientColor.z, 1));

		// Read the bsp geometry
		child = handle.FirstChildElement("multi_brush").ToElement();
		for(tinyxml2::XMLElement * childIt = child; childIt != nullptr; childIt = childIt->NextSiblingElement()) {
			// A brush created with a bunch of static models
			if(std::string(childIt->Name()) == "multi_brush") {
				tinyxml2::XMLElement * child2 = tinyxml2::XMLHandle(childIt).FirstChildElement("brush").ToElement();

				for(tinyxml2::XMLElement * child2It = child2; child2It != nullptr; child2It = child2It->NextSiblingElement()) {
					if(std::string(child2It->Name()) == "brush") {
						tinyxml2::XMLHandle brushHandle(child2It);

						daidalosengine::CVector3F position;
						daidalosengine::CVector3F rotation;
						daidalosengine::CVector3F scale;
						std::string diffuseTexture;
						std::string normalTexture;

						// Search for defined values in the element or use default values
						GetNodeData(brushHandle, "position", position, daidalosengine::CVector3F(0, 0, 0));
						GetNodeData(brushHandle, "rotation", rotation, daidalosengine::CVector3F(0, 0, 0));
						GetNodeData(brushHandle, "scale", scale, daidalosengine::CVector3F(1, 1, 1));
						GetNodeData(brushHandle, "diffuse_texture", diffuseTexture, "default.png");
						GetNodeData(brushHandle, "normal_texture", normalTexture, "default.png");
						std::string file = brushHandle.ToElement()->Attribute("file");

						int id = SCENE_DEFAULT_ID;
						brushHandle.ToElement()->QueryIntAttribute("id", &id);

						// Create the node
						daidalosengine::CStaticMeshSceneNode * meshNode = new daidalosengine::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_MULTIBRUSH, file, position, rotation, scale);
						meshNode->SetId(id);
						meshNode->SetMeshShadingType(daidalosengine::MESH_SHADING_FLAT);
						meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, diffuseTexture, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
						meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, normalTexture, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
						node->AddChild(meshNode);
					}
				}
			}
		}

		child = handle.FirstChildElement("brush").ToElement();
		for(tinyxml2::XMLElement * childIt = child; childIt != nullptr; childIt = childIt->NextSiblingElement()) {
			// A simple brush
			if(std::string(childIt->Name()) == "brush") {
				tinyxml2::XMLHandle brushHandle(childIt);

				daidalosengine::CVector3F position;
				daidalosengine::CVector3F rotation;
				daidalosengine::CVector3F scale;
				std::string diffuseTexture;
				std::string normalTexture;

				// Search for defined values in the element or use default values
				GetNodeData(brushHandle, "position", position, daidalosengine::CVector3F(0, 0, 0));
				GetNodeData(brushHandle, "rotation", rotation, daidalosengine::CVector3F(0, 0, 0));
				GetNodeData(brushHandle, "scale", scale, daidalosengine::CVector3F(1, 1, 1));
				GetNodeData(brushHandle, "diffuse_texture", diffuseTexture, "default.png");
				GetNodeData(brushHandle, "normal_texture", normalTexture, "default.png");
				std::string file = brushHandle.ToElement()->Attribute("file");

				int id = SCENE_DEFAULT_ID;
				brushHandle.ToElement()->QueryIntAttribute("id", &id);

				// Create the node
				daidalosengine::CStaticMeshSceneNode * meshNode = new daidalosengine::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_BRUSH, file, position, rotation, scale);
				meshNode->SetId(id);
				meshNode->SetMeshShadingType(daidalosengine::MESH_SHADING_FLAT);
				meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, diffuseTexture, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
				meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, normalTexture, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
				node->AddChild(meshNode);
			}
		}

		child = handle.FirstChildElement("node").ToElement();
		for(tinyxml2::XMLElement * childIt = child; childIt != nullptr; childIt = childIt->NextSiblingElement()) {
			// A node with a brush bounding box declared
			if(std::string(childIt->Name()) == "node") {
				tinyxml2::XMLHandle nodeHandle(childIt);

				daidalosengine::CVector3F position;
				daidalosengine::CVector3F rotation;
				daidalosengine::CVector3F scale;
				std::string boundingBox;

				int type = -1;
				nodeHandle.ToElement()->QueryIntAttribute("type", &type);

				int id = SCENE_DEFAULT_ID;
				nodeHandle.ToElement()->QueryIntAttribute("id", &id);

				// Search for defined values in the element or use default values
				GetNodeData(nodeHandle, "position", position, daidalosengine::CVector3F(0, 0, 0));
				GetNodeData(nodeHandle, "rotation", rotation, daidalosengine::CVector3F(0, 0, 0));
				GetNodeData(nodeHandle, "scale", scale, daidalosengine::CVector3F(1, 1, 1));
				GetNodeData(nodeHandle, "boundingBox", boundingBox, "");

				// Create a bounding box if necessary
				if(boundingBox != "") {
					// Create the node
					daidalosengine::CStaticMeshSceneNode * meshNode = new daidalosengine::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_BOUNDINGBOX, boundingBox, position, rotation, scale);
					meshNode->SetId(id);
					meshNode->SetSaved(false);
					meshNode->SetShadowCaster(false);
					meshNode->SetMeshShadingType(daidalosengine::MESH_SHADING_FLAT);
					meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, EDITOR_COLLISION_TEXTURE_NAME, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
					meshNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, EDITOR_COLLISION_TEXTURE_NAME, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
					node->AddChild(meshNode);
				}

				// Add a visible element if the node is a sound
				if(type == daidalosengine::SCENE_NODE_SOUND) {
					daidalosengine::CStaticMeshSceneNode * soundNode = new daidalosengine::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_SOUND, "cube.obj", position, rotation, daidalosengine::CVector3F(2, 2, 2));
					soundNode->SetId(id);
					soundNode->SetSaved(false);
					soundNode->SetShadowCaster(false);
					soundNode->SetMeshShadingType(daidalosengine::MESH_SHADING_FLAT);
					soundNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, EDITOR_SOUND_TEXTURE_NAME, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
					soundNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, EDITOR_SOUND_TEXTURE_NAME, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
					node->AddChild(soundNode);
				}

				// Add a visible element if the node is a light
				if(type == daidalosengine::SCENE_NODE_SPOTLIGHT) {
					daidalosengine::CStaticMeshSceneNode * lightNode = new daidalosengine::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_SOUND, "cube.obj", position, rotation, daidalosengine::CVector3F(2, 2, 2));
					lightNode->SetId(id);
					lightNode->SetSaved(false);
					lightNode->SetShadowCaster(false);
					lightNode->SetMeshShadingType(daidalosengine::MESH_SHADING_FLAT);
					lightNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_DIFFUSE, EDITOR_SPOTLIGHT_TEXTURE_NAME, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
					lightNode->SetMeshTexture(daidalosengine::TEXTURE_TYPE_NORMAL, EDITOR_SPOTLIGHT_TEXTURE_NAME, daidalosengine::PIXEL_FORMAT_A8R8G8B8, renderer->GetDefaultTextureFilter());
					node->AddChild(lightNode);
				}
			}
		}
	}

	/**
	* Save a node as a brush
	* @param parent The node parent
	* @param child The bsp node root
	* @param node The node to fill
	*/
	void CEditorSceneLoader::SaveBrush(daidalosengine::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		daidalosengine::IMeshSceneNode * meshNode = static_cast<daidalosengine::IMeshSceneNode *>(node);
		tinyxml2::XMLElement * brush = document.NewElement("brush");

		// Id
		if(meshNode->GetId() != SCENE_DEFAULT_ID) {
			brush->SetAttribute("id", meshNode->GetId());
		}

		// File
		brush->SetAttribute("file", meshNode->GetMeshName().c_str());

		// Position
		tinyxml2::XMLElement * child = document.NewElement("position");
		child->InsertEndChild(document.NewText(meshNode->GetPosition().ToString().c_str()));
		brush->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText(meshNode->GetRotation().ToString().c_str()));
		brush->InsertEndChild(child);

		// Scale
		child = document.NewElement("scale");
		child->InsertEndChild(document.NewText(meshNode->GetScale().ToString().c_str()));
		brush->InsertEndChild(child);

		// Diffuse texture
		if(meshNode->GetMeshTextureName(daidalosengine::TEXTURE_TYPE_DIFFUSE) != "") {
			child = document.NewElement("diffuse_texture");
			child->InsertEndChild(document.NewText(meshNode->GetMeshTextureName(daidalosengine::TEXTURE_TYPE_DIFFUSE).c_str()));
			brush->InsertEndChild(child);
		}

		// Normal texture
		if(meshNode->GetMeshTextureName(daidalosengine::TEXTURE_TYPE_NORMAL) != "") {
			child = document.NewElement("normal_texture");
			child->InsertEndChild(document.NewText(meshNode->GetMeshTextureName(daidalosengine::TEXTURE_TYPE_NORMAL).c_str()));
			brush->InsertEndChild(child);
		}

		parent->InsertEndChild(brush);
	}
}
