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

#include <Scene/3D/CStaticMeshSceneNode.h>
#include <Scene/3D/CSoundSceneNode.h>
#include "../3D/CEditorLevelSceneNode.h"
#include "../Loaders/CEditorSceneLoader.h"
#include "../CEditorConfiguration.h"

namespace EDITOR {

	/**
	* Override and use a simple node system
	* @param parent The node parent
	* @param child The bsp node root
	* @param node The node to fill
	*/
	void CEditorSceneLoader::LoadBSPStructure(DE::I3DScene * scene, DE::ISceneNode *& parent, tinyxml2::XMLHandle & handle, DE::ISceneNode *& node) {
		LoadEditorLevelStucture(scene, parent, handle, node);
	}

	/**
	* Override and use a simple node system
	* @param parent The node parent
	* @param child The bsp node root
	* @param node The node to fill
	*/
	void CEditorSceneLoader::LoadSimpleStructure(DE::I3DScene * scene, DE::ISceneNode *& parent, tinyxml2::XMLHandle & handle, DE::ISceneNode *& node) {
		LoadEditorLevelStucture(scene, parent, handle, node);
	}

	/**
	* Load a sound node
	* @param parent The node parent
	* @param child The bsp node root
	* @param node The node to fill
	*/
	void CEditorSceneLoader::LoadSoundNode(DE::I3DScene * scene, DE::ISceneNode *& parent, tinyxml2::XMLHandle & handle, DE::ISceneNode *& node, const std::string & name) {

		DE::CSceneLoader::LoadSoundNode(scene, parent, handle, node, name);
		static_cast<DE::CSoundSceneNode*>(node)->SetSoundStatus(DE::SOUND_STATUS_PAUSE);
	}

	/**
	* Save the simple structure
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CEditorSceneLoader::SaveSimpleStructure(DE::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {
		SaveBSPStructure(node, document, parent);
	}

	/**
	* Save the bsp structure
	* @param node The node
	* @param document The xml document
	* @param parent The element parent
	*/
	void CEditorSceneLoader::SaveBSPStructure(DE::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		parent->InsertEndChild(document.NewComment(" BSP Level Node "));

		DE::ILevelSceneNode * levelNode = static_cast<DE::ILevelSceneNode*>(node);

		// Level node
		tinyxml2::XMLElement * levelElement = document.NewElement("node");

		// Type
		levelElement->SetAttribute("type", DE::SCENE_NODE_ROOT);

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
	void CEditorSceneLoader::LoadEditorLevelStucture(DE::I3DScene * scene, DE::ISceneNode *& parent, tinyxml2::XMLHandle & handle, DE::ISceneNode *& node) {
		tinyxml2::XMLElement * child;
		node = new CEditorLevelSceneNode(scene);
		std::string skyName;
		DE::CVector3F ambientColor, skyAmbientColor;

		// Get the ambient color
		if(handle.ToElement()->Attribute("ambientColor") != nullptr)
			ambientColor = DE::CVector3F::GetFrom(handle.ToElement()->Attribute("ambientColor"));
		static_cast<CEditorLevelSceneNode*>(node)->SetAmbientColor(DE::CVector4F(ambientColor.x, ambientColor.y, ambientColor.z, 1));

		// Get the sky name
		if(handle.ToElement()->Attribute("skyName") != nullptr)
			skyName = handle.ToElement()->Attribute("skyName");
		static_cast<CEditorLevelSceneNode*>(node)->SetSkyName(skyName);

		// Get the sky ambient color
		if(handle.ToElement()->Attribute("skyAmbientColor") != nullptr)
			skyAmbientColor = DE::CVector3F::GetFrom(handle.ToElement()->Attribute("skyAmbientColor"));
		static_cast<CEditorLevelSceneNode*>(node)->SetSkyAmbientColor(DE::CVector4F(skyAmbientColor.x, skyAmbientColor.y, skyAmbientColor.z, 1));

		// Read the bsp geometry
		child = handle.FirstChildElement("multi_brush").ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			// A brush created with a bunch of static models
			if(std::string(child->Name()) == "multi_brush") {
				tinyxml2::XMLElement * child2 = tinyxml2::XMLHandle(child).FirstChildElement("brush").ToElement();

				for(child2; child2; child2 = child2->NextSiblingElement()) {
					if(std::string(child2->Name()) == "brush") {
						tinyxml2::XMLHandle brushHandle(child2);

						DE::CVector3F position, rotation, scale;
						std::string diffuseTexture, normalTexture;

						// Search for defined values in the element or use default values
						GetNodeData(brushHandle, "position", position, DE::CVector3F(0, 0, 0));
						GetNodeData(brushHandle, "rotation", rotation, DE::CVector3F(0, 0, 0));
						GetNodeData(brushHandle, "scale", scale, DE::CVector3F(1, 1, 1));
						GetNodeData(brushHandle, "diffuse_texture", diffuseTexture, "default.png");
						GetNodeData(brushHandle, "normal_texture", normalTexture, "default.png");
						std::string file = brushHandle.ToElement()->Attribute("file");

						int id = SCENE_DEFAULT_ID;
						brushHandle.ToElement()->QueryIntAttribute("id", &id);

						// Create the node
						DE::CStaticMeshSceneNode * meshNode = new DE::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_MULTIBRUSH, file, position, rotation, scale);
						meshNode->SetId(id);
						meshNode->SetMeshTexture(DE::TEXTURE_TYPE_DIFFUSE, diffuseTexture, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
						meshNode->SetMeshTexture(DE::TEXTURE_TYPE_NORMAL, normalTexture, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
						node->AddChild(meshNode);
					}
				}
			}
		}

		child = handle.FirstChildElement("brush").ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			// A simple brush
			if(std::string(child->Name()) == "brush") {
				tinyxml2::XMLHandle brushHandle(child);

				DE::CVector3F position, rotation, scale;
				std::string diffuseTexture, normalTexture;

				// Search for defined values in the element or use default values
				GetNodeData(brushHandle, "position", position, DE::CVector3F(0, 0, 0));
				GetNodeData(brushHandle, "rotation", rotation, DE::CVector3F(0, 0, 0));
				GetNodeData(brushHandle, "scale", scale, DE::CVector3F(1, 1, 1));
				GetNodeData(brushHandle, "diffuse_texture", diffuseTexture, "default.png");
				GetNodeData(brushHandle, "normal_texture", normalTexture, "default.png");
				std::string file = brushHandle.ToElement()->Attribute("file");

				int id = SCENE_DEFAULT_ID;
				brushHandle.ToElement()->QueryIntAttribute("id", &id);

				// Create the node
				DE::CStaticMeshSceneNode * meshNode = new DE::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_BRUSH, file, position, rotation, scale);
				meshNode->SetId(id);
				meshNode->SetMeshTexture(DE::TEXTURE_TYPE_DIFFUSE, diffuseTexture, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
				meshNode->SetMeshTexture(DE::TEXTURE_TYPE_NORMAL, normalTexture, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
				node->AddChild(meshNode);
			}
		}

		child = handle.FirstChildElement("node").ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			// A node with a brush bounding box declared
			if(std::string(child->Name()) == "node") {
				tinyxml2::XMLHandle nodeHandle(child);

				DE::CVector3F position, rotation, scale;
				std::string boundingBox;

				int type = -1;
				nodeHandle.ToElement()->QueryIntAttribute("type", &type);

				int id = SCENE_DEFAULT_ID;
				nodeHandle.ToElement()->QueryIntAttribute("id", &id);

				// Search for defined values in the element or use default values
				GetNodeData(nodeHandle, "position", position, DE::CVector3F(0, 0, 0));
				GetNodeData(nodeHandle, "rotation", rotation, DE::CVector3F(0, 0, 0));
				GetNodeData(nodeHandle, "scale", scale, DE::CVector3F(1, 1, 1));
				GetNodeData(nodeHandle, "boundingBox", boundingBox, "");

				// Create a bounding box if necessary
				if(boundingBox != "") {
					// Create the node
					DE::CStaticMeshSceneNode * meshNode = new DE::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_BOUNDINGBOX, boundingBox, position, rotation, scale);
					meshNode->SetId(id);
					meshNode->SetSaved(false);
					meshNode->SetMeshTexture(DE::TEXTURE_TYPE_DIFFUSE, EDITOR_COLLISION_TEXTURE_NAME, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
					meshNode->SetMeshTexture(DE::TEXTURE_TYPE_NORMAL, EDITOR_COLLISION_TEXTURE_NAME, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
					node->AddChild(meshNode);
				}

				// Add a visible element if the node is a sound
				if(type == DE::SCENE_NODE_SOUND) {
					DE::CStaticMeshSceneNode * soundNode = new DE::CStaticMeshSceneNode(scene, node, EDITOR_NODE_NAME_SOUND, "cube.obj", position, rotation, DE::CVector3F(2, 2, 2));
					soundNode->SetId(id);
					soundNode->SetSaved(false);
					soundNode->SetMeshTexture(DE::TEXTURE_TYPE_DIFFUSE, EDITOR_SOUND_TEXTURE_NAME, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
					soundNode->SetMeshTexture(DE::TEXTURE_TYPE_NORMAL, EDITOR_SOUND_TEXTURE_NAME, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
					node->AddChild(soundNode);
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
	void CEditorSceneLoader::SaveBrush(DE::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent) {

		DE::IMeshSceneNode * meshNode = static_cast<DE::IMeshSceneNode *>(node);
		tinyxml2::XMLElement * brush = document.NewElement("brush");

		// Id
		if(meshNode->GetId() != SCENE_DEFAULT_ID) {
			brush->SetAttribute("id", meshNode->GetId());
		}

		// File
		brush->SetAttribute("file", meshNode->GetMeshName().c_str());

		// Position
		tinyxml2::XMLElement * child = document.NewElement("position");
		child->InsertEndChild(document.NewText((meshNode->GetPosition().ToString().c_str())));
		brush->InsertEndChild(child);

		// Rotation
		child = document.NewElement("rotation");
		child->InsertEndChild(document.NewText((meshNode->GetRotation().ToString().c_str())));
		brush->InsertEndChild(child);

		// Scale
		child = document.NewElement("scale");
		child->InsertEndChild(document.NewText((meshNode->GetScale().ToString().c_str())));
		brush->InsertEndChild(child);

		// Diffuse texture
		if(meshNode->GetMeshTextureName(DE::TEXTURE_TYPE_DIFFUSE) != "") {
			child = document.NewElement("diffuse_texture");
			child->InsertEndChild(document.NewText((meshNode->GetMeshTextureName(DE::TEXTURE_TYPE_DIFFUSE).c_str())));
			brush->InsertEndChild(child);
		}

		// Normal texture
		if(meshNode->GetMeshTextureName(DE::TEXTURE_TYPE_NORMAL) != "") {
			child = document.NewElement("normal_texture");
			child->InsertEndChild(document.NewText((meshNode->GetMeshTextureName(DE::TEXTURE_TYPE_NORMAL).c_str())));
			brush->InsertEndChild(child);
		}

		parent->InsertEndChild(brush);
	}
}
