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
#include "CEditorLevelSceneNode.h"
#include "CEditorSceneLoader.h"

namespace EDITOR {

	/**
	* Save a scene into a file
	* @param root The scene root
	* @param filename The output file
	*/
	void CEditorSceneLoader::SaveToFile(DE::ISceneNode * root, const std::string & filename) {

	}

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

						DE::CVector3F position, rotation;
						std::string diffuseTexture, normalTexture;

						// Search for defined values in the element or use default values
						GetNodeData(brushHandle, "position", position, DE::CVector3F(0, 0, 0));
						GetNodeData(brushHandle, "rotation", rotation, DE::CVector3F(0, 0, 0));
						GetNodeData(brushHandle, "diffuse_texture", diffuseTexture, "default.png");
						GetNodeData(brushHandle, "normal_texture", normalTexture, "default.png");
						std::string file = brushHandle.ToElement()->Attribute("file");

						int id = SCENE_DEFAULT_ID;
						brushHandle.ToElement()->QueryIntAttribute("id", &id);

						// Create the node
						DE::CStaticMeshSceneNode * meshNode = new DE::CStaticMeshSceneNode(scene, node, "__editor_node_multibrush", file, position, rotation, DE::CVector3F(1.0f, 1.0f, 1.0f));
						meshNode->SetId(id);
						meshNode->SetMeshTexture(DE::TEXTURE_TYPE_DIFFUSE, diffuseTexture, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
						meshNode->SetMeshTexture(DE::TEXTURE_TYPE_NORMAL, normalTexture, DE::PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
						node->AddChild(meshNode);
					}
				}
			}
		}
	}
}
