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

#include "../Core/Core.h"
#include "../Core/Utility/CTokenizer.h"
#include "../Core/Utility/CVector4.h"
#include "../Core/Memory/CExceptionLoad.h"
#include "../Core/Logger/ILogger.h"
#include "../Scene/CProjectionCameraSceneNode.h"
#include "../Scene/CSpotLightSceneNode.h"
#include "../Scene/CPointLightSceneNode.h"
#include "../Scene/CDirectionalLightSceneNode.h"
#include "../Scene/CStaticMeshSceneNode.h"
#include "../Scene/CAnimatedMeshSceneNode.h"
#include "../Scene/CSoundSceneNode.h"
#include "../Scene/CEmptySceneNode.h"
#include "../Scene/CPlane3D.h"
#include "../Medias/CTexture2D.h"
#include "../Medias/CMaterial.h"
#include "../Loaders/CSceneLoader.h"

namespace DE
{
	CSceneLoader::CSceneLoader() 
	{
		// Nodes default configuration file
		this->m_nodesConf = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_NODES_CONFIGURATION);
		this->m_materialsConf = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_MATERIALS_CONFIGURATION);
		this->m_structure = SCENE_NODE_ROOT_STRUCTURE_TYPE_BSP;
		this->m_renderer = nullptr;
	}

	CSceneLoader::~CSceneLoader() {}

	ISceneNode * CSceneLoader::LoadFromFile(const std::string & filename)
	{
		if(this->m_renderer == nullptr)
			this->m_renderer = IRenderer::GetRenderer();

		this->m_filename = filename;
		ISceneNode * root = nullptr;
		tinyxml2::XMLElement * element = nullptr;

		tinyxml2::XMLDocument doc;
		int loadOk = doc.LoadFile(filename.c_str());
		if(loadOk == tinyxml2::XML_NO_ERROR)
		{
			tinyxml2::XMLHandle hDoc(&doc);

			int tmpInt = 0;
			hDoc.FirstChildElement("scene").ToElement()->QueryIntAttribute("structure", &tmpInt);
			this->m_structure = (ESceneNodeRootStructureType)tmpInt;

			element = hDoc.FirstChildElement("scene").FirstChildElement("node").ToElement();
			for(element ; element ; element = element->NextSiblingElement())
				LoadNode(root, tinyxml2::XMLHandle(element));
		}
		else
		{
			LOG_ERROR("Unable to load the scene file '" << this->m_filename << "' (" << doc.GetErrorStr1() << ")");
			ILogger::Kill();

			throw CExceptionLoad(this->m_filename);
		}

		return root;
	}

	void CSceneLoader::SaveToFile(ISceneNode * root, const std::string & filename)
	{

	}

	void CSceneLoader::LoadNode(ISceneNode *& parent, tinyxml2::XMLHandle & handle)
	{
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
		if(handle.ToElement()->Attribute("name"))
		{
			name = handle.ToElement()->Attribute("name");
		}
		
		CVector4F nodeDefaultColor;
		CVector3F nodeDefaultScale;

		switch(type)
		{
		case SCENE_NODE_ROOT:

			switch(this->m_structure)
			{
			// Simple structure
			case SCENE_NODE_ROOT_STRUCTURE_TYPE_SIMPLE:
				LoadSimpleStructure(parent, handle, node);
				if(parent == nullptr)
					parent = node;

				break;

			// BSP structure
			case SCENE_NODE_ROOT_STRUCTURE_TYPE_BSP:
				LoadBSPStructure(parent, handle, node);
				if(parent == nullptr)
					parent = node;

				break;
			}

			break;

		case SCENE_NODE_CAMERA:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "position", position, CVector3F(0,0,0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));
			GetNodeData(handle, "scale", scale, CVector3F(1,1,1));
			GetNodeData(handle, "fov", fov, DegreeToRadian(this->m_nodesConf.GetValueFloat("ICameraSceneNode", "fov", 55)));
			GetNodeData(handle, "ratio", ratio, this->m_renderer->GetResolution().x / (float)this->m_renderer->GetResolution().y);
			GetNodeData(handle, "near", near, this->m_nodesConf.GetValueFloat("ICameraSceneNode", "near", 0.1f));
			GetNodeData(handle, "far", far, this->m_nodesConf.GetValueFloat("ICameraSceneNode", "far", 100.0f));

			// Create the node
			node = new CProjectionCameraSceneNode(parent, name, position, rotation, DegreeToRadian(fov), ratio, near, far);
			node->SetScale(scale);
			break;

		case SCENE_NODE_SPOTLIGHT:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "enabled", enabled, true);
			GetNodeData(handle, "shadow_caster", shadowCaster, false);
			GetNodeData(handle, "position", position, CVector3F(0,0,0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));
			nodeDefaultColor = CVector4F::GetFrom(this->m_nodesConf.GetValueString("ISpotLightSceneNode", "color", "1.0f:1.0f:1.0f:1.0f"));
			GetNodeData(handle, "color", color, CColor(nodeDefaultColor.x, nodeDefaultColor.y, nodeDefaultColor.z, nodeDefaultColor.w));
			GetNodeData(handle, "range", range, this->m_nodesConf.GetValueFloat("ISpotLightSceneNode", "range", 100.0f));
			GetNodeData(handle, "radius", radius, this->m_nodesConf.GetValueFloat("ISpotLightSceneNode", "radius", 30.0f));
			GetNodeData(handle, "falloff", falloff, this->m_nodesConf.GetValueFloat("ISpotLightSceneNode", "falloff", 0.8f));
			GetNodeData(handle, "tightness", tightness, this->m_nodesConf.GetValueFloat("ISpotLightSceneNode", "tightness", 0.6f));

			// Create the node
			node = new CSpotLightSceneNode(parent, name, position, rotation, color, range, radius, falloff, tightness, shadowCaster, enabled);
			break;

		case SCENE_NODE_POINTLIGHT:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "enabled", enabled, true);
			GetNodeData(handle, "shadow_caster", shadowCaster, false);
			GetNodeData(handle, "position", position, CVector3F(0,0,0));
			nodeDefaultColor = CVector4F::GetFrom(this->m_nodesConf.GetValueString("IPointLightSceneNode", "color", "1.0f:1.0f:1.0f:1.0f"));
			GetNodeData(handle, "color", color, CColor(nodeDefaultColor.x, nodeDefaultColor.y, nodeDefaultColor.z, nodeDefaultColor.w));
			GetNodeData(handle, "range", range, this->m_nodesConf.GetValueFloat("IPointLightSceneNode", "range", 100.0f));
			GetNodeData(handle, "falloff", falloff, this->m_nodesConf.GetValueFloat("IPointLightSceneNode", "falloff", 0.8f));

			// Create the node
			node = new CPointLightSceneNode(parent, name, position, color, range, falloff, shadowCaster, enabled);
			break;

		case SCENE_NODE_DIRECTIONALLIGHT:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "enabled", enabled, true);
			GetNodeData(handle, "shadow_caster", shadowCaster, false);
			GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));
			nodeDefaultColor = CVector4F::GetFrom(this->m_nodesConf.GetValueString("IPointLightSceneNode", "color", "1.0f:1.0f:1.0f:1.0f"));
			GetNodeData(handle, "color", color, CColor(nodeDefaultColor.x, nodeDefaultColor.y, nodeDefaultColor.z, nodeDefaultColor.w));

			// Create the node
			node = new CDirectionalLightSceneNode(parent, name, rotation, color, shadowCaster, enabled);
			break;

		case SCENE_NODE_STATICMESH:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "visible", visible, this->m_nodesConf.GetValueBool("IStaticMeshSceneNode", "visible", true));
			GetNodeData(handle, "position", position, CVector3F(0,0,0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));
			nodeDefaultScale = CVector3F::GetFrom(this->m_nodesConf.GetValueString("IStaticMeshSceneNode", "scale", "1.0f:1.0f:1.0f"));
			GetNodeData(handle, "scale", scale, CVector3F(nodeDefaultScale.x, nodeDefaultScale.y, nodeDefaultScale.z));
			GetNodeData(handle, "diffuse_texture", diffuseTexture, "");
			GetNodeData(handle, "normal_texture", normalTexture, "");
			GetNodeData(handle, "flat_shading", flatShading, false);
			GetNodeData(handle, "material", material, "default.mtl");
			file = handle.ToElement()->Attribute("file");
			
			// Create the node
			node = new CStaticMeshSceneNode(parent, name, file, position, rotation, scale);
			if(diffuseTexture != "")
				static_cast<CStaticMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_DIFFUSE, diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			if(normalTexture != "")
				static_cast<CStaticMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_NORMAL, normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			if(flatShading)
				static_cast<CStaticMeshSceneNode*>(node)->SetMeshShadingType(DE::MESH_SHADING_FLAT);
			static_cast<CStaticMeshSceneNode*>(node)->SetMeshMaterial(material);

			break;

		case SCENE_NODE_ANIMATEDMESH:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "visible", visible, this->m_nodesConf.GetValueBool("IAnimatedMeshSceneNode", "visible", true));
			GetNodeData(handle, "position", position, CVector3F(0,0,0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));
			nodeDefaultScale = CVector3F::GetFrom(this->m_nodesConf.GetValueString("IStaticMeshSceneNode", "scale", "1.0f:1.0f:1.0f"));
			GetNodeData(handle, "scale", scale, CVector3F(nodeDefaultScale.x, nodeDefaultScale.y, nodeDefaultScale.z));
			GetNodeData(handle, "diffuse_texture", diffuseTexture, "");
			GetNodeData(handle, "normal_texture", normalTexture, "");
			GetNodeData(handle, "flat_shading", flatShading, false);
			GetNodeData(handle, "material", material, "default.mtl");
			file = handle.ToElement()->Attribute("file");
			
			// Create the node
			node = new CAnimatedMeshSceneNode(parent, name, file, position, rotation,  scale);
			if(diffuseTexture != "")
				static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_DIFFUSE, diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			if(normalTexture != "")
				static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshTexture(TEXTURE_TYPE_NORMAL, normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());
			if(flatShading)
				static_cast<CAnimatedMeshSceneNode*>(node)->SetMeshShadingType(DE::MESH_SHADING_FLAT);
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

			GetNodeData(handle, "position", position, CVector3F(0,0,0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));
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

			node = new CSoundSceneNode(file, static_cast<ESoundType>(soundType), parent, name);
			static_cast<CSoundSceneNode*>(node)->SetPosition(position);
			static_cast<CSoundSceneNode*>(node)->SetRotation(rotation);
			static_cast<CSoundSceneNode*>(node)->SetSoundGain(gain);
			static_cast<CSoundSceneNode*>(node)->SetSoundConeInnerAngle(coneInnerAngle);
			static_cast<CSoundSceneNode*>(node)->SetSoundConeOuterAngle(coneInnerAngle);
			static_cast<CSoundSceneNode*>(node)->SetSoundReferenceDistance(referenceDistance);
			static_cast<CSoundSceneNode*>(node)->SetSoundMaxDistance(maxDistance);
			static_cast<CSoundSceneNode*>(node)->SetSoundRelativeToListener(relativeToListener);
			static_cast<CSoundSceneNode*>(node)->SetSoundLooped(looped);
			if(playOnStartup)
				static_cast<CSoundSceneNode*>(node)->SetSoundStatus(SOUND_STATUS_PLAY);

			break;

		case SCENE_NODE_EMPTY:
			// Search for defined values in the element or use default values
			GetNodeData(handle, "position", position, CVector3F(0,0,0));
			GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));

			// Create the node
			node = new CEmptySceneNode(parent, name, position);
			static_cast<CEmptySceneNode*>(node)->SetRotation(rotation);
			break;

		default:
			// Invalid or missing type
			LOG_WARNING("Invalid or missing node attribute 'type' in '" << this->m_filename << "'");
			break;
		}

		// If a node has been successfully created
		if(node != nullptr)
		{
			// Get the node attributes
			attributes = GetNodeAttributes(handle);

			// Add the attributes
			node->SetAttributes(attributes);

			// Add the node to the parent
			if(parent != node)
				parent->AddChild(node);

			// Check for attached nodes
			child = handle.FirstChildElement("node").ToElement();
			for(child ; child ; child = child->NextSiblingElement())
			{
				// Read only nodes
				if(std::string(child->Name()) == "node")
					LoadNode(node, tinyxml2::XMLHandle(child));
			}
		}
	}

	void CSceneLoader::LoadBSPStructure(ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node)
	{
		tinyxml2::XMLElement * child;
		std::vector<SBSPBrushDefinition> brushDefinition;
		node = new CBSPSceneNode();
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
		for(child ; child ; child = child->NextSiblingElement())	
		{
			// A brush created with a bunch of static models
			if(std::string(child->Name()) == "multi_brush")
			{
				tinyxml2::XMLHandle bspHandle(child);
				tinyxml2::XMLElement * child2 = bspHandle.FirstChildElement("brush").ToElement();
				SBSPBrushDefinition brush;

				// Default values
				brush.id = -1;
				brush.collision = true;

				// Get the brush options
				bspHandle.ToElement()->QueryBoolAttribute("collisions", &brush.collision);
				bspHandle.ToElement()->QueryIntAttribute("id", &brush.id);

				for(child2 ; child2 ; child2 = child2->NextSiblingElement())	
				{
					if(std::string(child2->Name()) == "brush")
						AddBSPBrushData(tinyxml2::XMLHandle(child2), brush);
				}

				if(brush.geometry.size() > 0)
					brushDefinition.push_back(brush);
			}
		}

		child = handle.FirstChildElement("brush").ToElement();
		for(child ; child ; child = child->NextSiblingElement())	
		{
			// A simple brush
			if(std::string(child->Name()) == "brush")
			{
				tinyxml2::XMLHandle bspHandle(child);
				SBSPBrushDefinition brush;

				// Default values
				brush.id = -1;
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

	void CSceneLoader::LoadSimpleStructure(ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node)
	{
		tinyxml2::XMLElement * child;
		std::vector<SSimpleBrush> brushes;
		node = new CSimpleSceneNode();
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
		for(child ; child ; child = child->NextSiblingElement())	
		{
			// A brush created with a bunch of static models
			if(std::string(child->Name()) == "multi_brush")
			{
				tinyxml2::XMLHandle bspHandle(child);
				tinyxml2::XMLElement * child2 = bspHandle.FirstChildElement("brush").ToElement();
				SSimpleBrush brush;

				for(child2 ; child2 ; child2 = child2->NextSiblingElement())	
				{
					if(std::string(child2->Name()) == "brush")
						AddSimpleBrushData(tinyxml2::XMLHandle(child2), brush);
				}

				if(brush.geometry.size() > 0)
					brushes.push_back(brush);
			}
		}

		child = handle.FirstChildElement("brush").ToElement();
		for(child ; child ; child = child->NextSiblingElement())	
		{
			// A simple brush
			if(std::string(child->Name()) == "brush")
			{
				tinyxml2::XMLHandle bspHandle(child);
				SSimpleBrush brush;

				AddSimpleBrushData(bspHandle, brush);
				brushes.push_back(brush);
			}
		}

		// Add all the geometry in the bsp scene node
		static_cast<CSimpleSceneNode*>(node)->LoadGeometry(brushes);
	}

	void CSceneLoader::AddBSPBrushData(tinyxml2::XMLHandle & handle, SBSPBrushDefinition & brushDefinition)
	{
		bool visible, flatShading;
		CVector3F position, rotation, scale;
		std::string diffuseTexture, normalTexture, material;
		std::ostringstream ss;

		std::string name = handle.ToElement()->Attribute("file");
		GetNodeData(handle, "visible", visible, true);
		GetNodeData(handle, "position", position, CVector3F(0,0,0));
		GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));
		GetNodeData(handle, "scale", scale, CVector3F(1,1,1));
		GetNodeData(handle, "diffuse_texture", diffuseTexture, "");
		GetNodeData(handle, "normal_texture", normalTexture, "");
		GetNodeData(handle, "flat_shading", flatShading, false);
		GetNodeData(handle, "material", material, "default.mtl");

		// Modification matrixes
		CMatrix worldMatrix = CMatrix(scale, CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), position);
		CMatrix rotationMatrix = CMatrix(CVector3F(1,1,1), CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), CVector3F(0,0,0));

		CMaterial mat(material);

		// Create the geometry
		CStaticMesh mesh(name);
		for(int i = 0 ; i < mesh.GetMesh()->GetNumberOfGroups() ; ++i)
		{
			CTexture2D diffuse;
			CTexture2D normal;

			if(diffuseTexture != "")
					diffuse.CreateFromFile(diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());

			if(normalTexture != "")
					normal.CreateFromFile(normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());

			for(int j = 0 ; j < mesh.GetMesh()->GetGroupSize(i) ; j += 3)
			{
				CBSPTriangle * triangle = new CBSPTriangle();
				
				ss.str("");
				ss << "group_" << (i + 1);

				for(int k = 0 ; k < 3 ; ++k)
				{
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

	void CSceneLoader::AddSimpleBrushData(tinyxml2::XMLHandle & handle, SSimpleBrush & brush)
	{
		bool visible, flatShading;
		CVector3F position, rotation, scale;
		std::string diffuseTexture, normalTexture, material;
		std::ostringstream ss;

		std::string name = handle.ToElement()->Attribute("file");
		GetNodeData(handle, "visible", visible, true);
		GetNodeData(handle, "position", position, CVector3F(0,0,0));
		GetNodeData(handle, "rotation", rotation, CVector3F(0,0,0));
		GetNodeData(handle, "scale", scale, CVector3F(1,1,1));
		GetNodeData(handle, "diffuse_texture", diffuseTexture, "");
		GetNodeData(handle, "normal_texture", normalTexture, "");
		GetNodeData(handle, "flat_shading", flatShading, false);
		GetNodeData(handle, "material", material, "default.mtl");

		// Modification matrixes
		CMatrix worldMatrix = CMatrix(scale, CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), position);
		CMatrix rotationMatrix = CMatrix(CVector3F(1,1,1), CVector3F(DegreeToRadian(rotation.x), DegreeToRadian(rotation.y), DegreeToRadian(rotation.z)), CVector3F(0,0,0));

		CMaterial mat(material);

		// Create the geometry
		CStaticMesh mesh(name);
		for(int i = 0 ; i < mesh.GetMesh()->GetNumberOfGroups() ; ++i)
		{
			CTexture2D diffuse;
			CTexture2D normal;
			CVector3F minVector = CVector3F(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			CVector3F maxVector = CVector3F(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

			if(diffuseTexture != "")
				diffuse.CreateFromFile(diffuseTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());

			if(normalTexture != "")
				normal.CreateFromFile(normalTexture, PIXEL_FORMAT_A8R8G8B8, this->m_renderer->GetDefaultTextureFilter());

			for(int j = 0 ; j < mesh.GetMesh()->GetGroupSize(i) ; j += 3)
			{
				CTriangle3D * triangle = new CTriangle3D();

				ss.str("");
				ss << "group_" << (i + 1);

				for(int k = 0 ; k < 3 ; ++k)
				{
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
				CVector3F tmp[] = {triangle->GetVertex(0), triangle->GetVertex(1), triangle->GetVertex(2)};
				CPlane3DF plane = CPlane3DF(tmp);
				triangle->SetFlatNormal(plane.GetNormal());

				// Add the triangle to the geometry
				brush.geometry.push_back(triangle);
			}

			// Add the visibility shape to the geometry
			brush.visibilityShape = new CSphereF((maxVector + minVector) / 2.0f, (maxVector - minVector).Length() / 2.0f);
		}
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, bool & data, const bool defaultValue)
	{
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
		{
			std::string tmpText = TrimString(text->Value());
			std::transform(tmpText.begin(), tmpText.end(), tmpText.begin(), tolower);
			
			if(tmpText == "true" || tmpText == "1")
				data = true;
			if(tmpText == "false" || tmpText == "0")
				data = false;
		}
		else
			data = defaultValue;
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, int & data, const int defaultValue)
	{
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
		{
			std::istringstream ss(text->Value());
			ss >> data;
		}
		else
			data = defaultValue;
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector3F & data, const CVector3F & defaultValue)
	{
		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
			data = CVector3F::GetFrom(std::string(text->Value()), defaultValue);
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector4F & data, const CVector4F & defaultValue)
	{
		CTokenizer tokenizer;
		data = defaultValue;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
			data = CVector4F::GetFrom(std::string(text->Value()), defaultValue);
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CColor & data, const CColor & defaultValue)
	{
		CTokenizer tokenizer;
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
			tokenizer.AddLine(text->Value());

		if(tokenizer.Tokenize(":", 4) && tokenizer.GetTokens().size() == 4)
		{
			std::istringstream ss;
			float r, g, b, a;
			
			ss.str(tokenizer.GetTokens().at(0));
			ss >> r;
			ss.clear();
			ss.str(tokenizer.GetTokens().at(1));
			ss >> g;
			ss.clear();
			ss.str(tokenizer.GetTokens().at(2));
			ss >> b;
			ss.clear();
			ss.str(tokenizer.GetTokens().at(3));
			ss >> a;

			data = CColor(r, g, b, a);
		}
		else
			data = defaultValue;
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, float & data, const float defaultValue)
	{
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
		{
			std::istringstream ss(text->Value());
			ss >> data;
		}
		else
			data = defaultValue;
	}

	void CSceneLoader::GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, std::string & data, const std::string & defaultValue)
	{
		tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
		{
			data = text->Value();
		}
		else
			data = defaultValue;
	}

	const std::map<std::string, std::string> CSceneLoader::GetNodeAttributes(tinyxml2::XMLHandle & handle)
	{
		std::map<std::string, std::string> attributes;
		tinyxml2::XMLElement * child;
		std::string attributeName;

		child = handle.FirstChildElement("attributes").FirstChild().ToElement();
		for(child ; child ; child = child->NextSiblingElement())	
		{
			tinyxml2::XMLHandle attributeHandle(child);

			// Get the attribute name
			attributeName = attributeHandle.ToElement()->Name();

			// Get the attribute value
			tinyxml2::XMLText * valueText = attributeHandle.FirstChild().ToText();

			// Add the attribue only if the two elements are valid
			if(valueText != nullptr)
			{
				attributes[attributeName] = valueText->Value();
			}
		}

		return attributes;
	}
}
