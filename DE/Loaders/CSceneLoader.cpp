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
		TiXmlElement * element = nullptr;

		TiXmlDocument doc(filename.c_str());
		bool loadOk = doc.LoadFile();
		if(loadOk)
		{
			TiXmlHandle hDoc(&doc);

			int tmpInt = 0;
			hDoc.FirstChild("scene").Element()->QueryIntAttribute("structure", &tmpInt);
			this->m_structure = (ESceneNodeRootStructureType)tmpInt;

			element = hDoc.FirstChild("scene").FirstChild("node").ToElement();
			for(element ; element ; element = element->NextSiblingElement())
				LoadNode(root, TiXmlHandle(element));
		}
		else
		{
			ILogger::Log() << "DE:CSceneLoader Error: Unable to load the scene file '" << this->m_filename << "' (" << doc.ErrorDesc() << ").\n";
			ILogger::Kill();

			throw CExceptionLoad(this->m_filename);
		}

		return root;
	}

	void CSceneLoader::SaveToFile(ISceneNode * root, const std::string & filename)
	{

	}

	void CSceneLoader::LoadNode(ISceneNode *& parent, const TiXmlHandle & handle)
	{
		// Global data variables
		bool visible, flatShading, enabled, shadowCaster;
		float fov, ratio, near, far, range, radius, falloff, tightness;
		CVector3F position, rotation, scale;
		std::map<std::string, std::string> attributes;
		CColor color;
		TiXmlElement * child = nullptr;
		ISceneNode * node = nullptr;
		int type = -1;
		handle.Element()->QueryIntAttribute("type", &type);
		std::string file, diffuseTexture, normalTexture, material;
		CTokenizer tokenizer;

		std::string name = "undefined";
		if(handle.Element()->Attribute("name"))
		{
			name = handle.Element()->Attribute("name");
		}
		
		CVector4F nodeDefaultColor;
		CVector3F nodeDefaultScale;

		switch(type)
		{
		case SCENE_NODE_ROOT:

			switch(this->m_structure)
			{
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
			file = handle.Element()->Attribute("file");
			
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
			file = handle.Element()->Attribute("file");
			
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
			ILogger::Log() << "DE:CSceneLoader Error: Invalid or missing node attribute 'type' in '" << this->m_filename << "'.\n";
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
				if(child->ValueStr() == "node")
					LoadNode(node, TiXmlHandle(child));
			}
		}
	}

	void CSceneLoader::LoadBSPStructure(ISceneNode *& parent, const TiXmlHandle & handle, ISceneNode *& node)
	{
		TiXmlElement * child;
		std::vector<SBSPBrushDefinition> brushDefinition;
		node = new CBSPSceneNode();
		CVector3F ambientColor;
		if(handle.Element()->Attribute("ambientColor") != nullptr)
			ambientColor = CVector3F::GetFrom(handle.Element()->Attribute("ambientColor"));
		static_cast<CBSPSceneNode*>(node)->SetAmbientColor(CVector4F(ambientColor.x, ambientColor.y, ambientColor.z, 1));

		// Read the bsp geometry
		child = handle.FirstChildElement("multi_brush").ToElement();
		for(child ; child ; child = child->NextSiblingElement())	
		{
			// A brush created with a bunch of static models
			if(child->ValueStr() == "multi_brush")
			{
				TiXmlHandle bspHandle(child);
				TiXmlElement * child2 = bspHandle.FirstChildElement("brush").ToElement();
				SBSPBrushDefinition brush;

				// Default values
				brush.id = -1;
				brush.collision = true;

				// Get the brush options
				bspHandle.Element()->QueryBoolAttribute("collisions", &brush.collision);
				bspHandle.Element()->QueryIntAttribute("id", &brush.id);

				for(child2 ; child2 ; child2 = child2->NextSiblingElement())	
				{
					if(child2->ValueStr() == "brush")
						AddBrushData(TiXmlHandle(child2), brush);
				}

				if(brush.geometry.size() > 0)
					brushDefinition.push_back(brush);
			}
		}

		child = handle.FirstChildElement("brush").ToElement();
		for(child ; child ; child = child->NextSiblingElement())	
		{
			// A simple brush
			if(child->ValueStr() == "brush")
			{
				TiXmlHandle bspHandle(child);
				SBSPBrushDefinition brush;

				// Default values
				brush.id = -1;
				brush.collision = true;

				// Get the brush options
				bspHandle.Element()->QueryBoolAttribute("collisions", &brush.collision);
				bspHandle.Element()->QueryIntAttribute("id", &brush.id);

				AddBrushData(bspHandle, brush);
				brushDefinition.push_back(brush);
			}
		}

		// Add all the geometry in the bsp scene node
		static_cast<CBSPSceneNode*>(node)->LoadGeometry(brushDefinition);
	}

	void CSceneLoader::AddBrushData(const TiXmlHandle & handle, SBSPBrushDefinition & brushDefinition)
	{
		bool visible, flatShading;
		CVector3F position, rotation, scale;
		std::string diffuseTexture, normalTexture, material;
		std::ostringstream ss;

		std::string name = handle.Element()->Attribute("file");
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

	void CSceneLoader::GetNodeData(const TiXmlHandle & handle, const std::string & name, bool & data, const bool defaultValue)
	{
		TiXmlText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
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

	void CSceneLoader::GetNodeData(const TiXmlHandle & handle, const std::string & name, int & data, const int defaultValue)
	{
		TiXmlText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
		if(text != nullptr)
		{
			std::istringstream ss(text->Value());
			ss >> data;
		}
		else
			data = defaultValue;
	}

	void CSceneLoader::GetNodeData(const TiXmlHandle & handle, const std::string & name, CVector3F & data, const CVector3F & defaultValue)
	{
		CTokenizer tokenizer;
		data = defaultValue;
		TiXmlText * text = handle.FirstChild(name.c_str()).FirstChild().Text();
		if(text != nullptr)
			data = CVector3F::GetFrom(std::string(text->Value()), defaultValue);
	}

	void CSceneLoader::GetNodeData(const TiXmlHandle & handle, const std::string & name, CVector4F & data, const CVector4F & defaultValue)
	{
		CTokenizer tokenizer;
		data = defaultValue;
		TiXmlText * text = handle.FirstChild(name.c_str()).FirstChild().Text();
		if(text != nullptr)
			data = CVector4F::GetFrom(std::string(text->Value()), defaultValue);
	}

	void CSceneLoader::GetNodeData(const TiXmlHandle & handle, const std::string & name, CColor & data, const CColor & defaultValue)
	{
		CTokenizer tokenizer;
		TiXmlText * text = handle.FirstChild(name.c_str()).FirstChild().Text();
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

	void CSceneLoader::GetNodeData(const TiXmlHandle & handle, const std::string & name, float & data, const float defaultValue)
	{
		TiXmlText * text = handle.FirstChild(name.c_str()).FirstChild().Text();
		if(text != nullptr)
		{
			std::istringstream ss(text->Value());
			ss >> data;
		}
		else
			data = defaultValue;
	}

	void CSceneLoader::GetNodeData(const TiXmlHandle & handle, const std::string & name, std::string & data, const std::string & defaultValue)
	{
		TiXmlText * text = handle.FirstChild(name.c_str()).FirstChild().Text();
		if(text != nullptr)
		{
			data = text->Value();
		}
		else
			data = defaultValue;
	}

	const std::map<std::string, std::string> CSceneLoader::GetNodeAttributes(const TiXmlHandle & handle)
	{
		std::map<std::string, std::string> attributes;
		TiXmlElement * child;
		std::string attributeName;

		child = handle.FirstChildElement("attributes").FirstChild().ToElement();
		for(child ; child ; child = child->NextSiblingElement())	
		{
			TiXmlHandle attributeHandle(child);

			// Get the attribute name
			attributeName = attributeHandle.Element()->ValueStr();

			// Get the attribute value
			TiXmlText * valueText = attributeHandle.FirstChild().Text();

			// Add the attribue only if the two elements are valid
			if(valueText != nullptr)
			{
				attributes[attributeName] = valueText->ValueStr();
			}
		}

		return attributes;
	}
}
