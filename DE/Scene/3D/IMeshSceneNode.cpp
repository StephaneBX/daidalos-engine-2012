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
#include "../../Core/IRenderer.h"
#include "../../Core/CConfigurationManager.h"
#include "../../Core/Memory/CExceptionAssert.h"
#include "../../Core/Logger/ILogger.h"
#include "../../Scene/3D/CSphere.h"
#include "../../Scene/3D/I3DScene.h"
#include "../../Scene/3D/IMeshSceneNode.h"

namespace DE {
	IMeshSceneNode::IMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name) : ISceneNode(scene, parent, name) {
		LOG_TRACE();

		this->m_shadingType = MESH_SHADING_SMOOTH;
		this->m_visibilityShape = new CSphereF();
		this->m_needGeometryUpdate = true;
	}

	IMeshSceneNode::IMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CVector3F & scale) : ISceneNode(scene, parent, name, position, rotation) {
		LOG_TRACE();

		this->m_shadingType = MESH_SHADING_SMOOTH;
		SetScale(scale);
		this->m_visibilityShape = new CSphereF();
		this->m_needGeometryUpdate = true;
	}

	IMeshSceneNode::~IMeshSceneNode() {
		LOG_TRACE();

		SAFE_DELETE_ARRAY(this->m_buffer);
		SAFE_DELETE_ARRAY(this->m_diffuseTexture);
		SAFE_DELETE_ARRAY(this->m_normalTexture);

		SAFE_DELETE(this->m_visibilityShape);
	}

	void IMeshSceneNode::SetMeshMaterial(const std::string & filename) {
		LOG_TRACE();

		std::ostringstream ss;
		this->m_material.LoadFromFile(filename);
		this->m_defaultMaterialElement.clear();
		this->m_activeMaterialElement.clear();

		SiniP materialConf = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_MATERIALS_CONFIGURATION);
		for(int i = 0; i < this->m_numberOfGroups; ++i) {
			ss.str("");
			ss << "group_" << (i + 1);

			std::string name = materialConf.GetValueString(this->m_filename, ss.str(), "default");
			SMaterialElement * element = this->m_material.GetMaterial()->GetElement(name);

			// If the element is not found, choose the default one
			if(element == nullptr) {
				this->m_defaultMaterialElement.push_back(this->m_material.GetMaterial()->GetElement("default"));
			} else {
				this->m_defaultMaterialElement.push_back(element);
			}
			this->m_activeMaterialElement.push_back(this->m_defaultMaterialElement[i]);
		}
	}

	void IMeshSceneNode::SetActiveMeshMaterialElement(const std::string & name) {
		LOG_TRACE();

		SMaterialElement * element = this->m_material.GetMaterial()->GetElement(name);
		if(element != nullptr) {
			for(int i = 0; i < this->m_numberOfGroups; ++i) {
				this->m_activeMaterialElement[i] = element;
			}
		}
	}

	void IMeshSceneNode::RestoreDefaultMeshMaterialElement() {
		LOG_TRACE();

		for(int i = 0; i < this->m_numberOfGroups; ++i) {
			this->m_activeMaterialElement[i] = this->m_defaultMaterialElement[i];
		}
	}

	void IMeshSceneNode::SetMeshTexture(ETextureType textureType, const std::string & filename, EPixelFormat format, const STexture2DFilterPolicy * filter, const int group) {
		LOG_TRACE();

		if(group == MESHSCENE_ALL_GROUPS) {
			// Set the texture in all the groups
			switch(textureType) {
			case TEXTURE_TYPE_DIFFUSE:
				for(int i = 0; i < this->m_numberOfGroups; ++i) {
					this->m_diffuseTexture[i].CreateFromFile(filename, format, filter);
				}
				break;

			case TEXTURE_TYPE_NORMAL:
				for(int i = 0; i < this->m_numberOfGroups; ++i) {
					this->m_normalTexture[i].CreateFromFile(filename, format, filter);
				}
				break;
			}
		} else {
			// Set the texture to the chosen group
			Assert(group >= 0 && group < this->m_numberOfGroups);

			switch(textureType) {
			case TEXTURE_TYPE_DIFFUSE:
				this->m_diffuseTexture[group].CreateFromFile(filename, format, filter);
				break;

			case TEXTURE_TYPE_NORMAL:
				this->m_normalTexture[group].CreateFromFile(filename, format, filter);
				break;
			}
		}
	}

	void IMeshSceneNode::Update(const float time) {
		LOG_TRACE();

		ISceneNode::Update(time);
	}

	void IMeshSceneNode::SetMeshFormat(const int format) {
		LOG_TRACE();

		for(int i = 0; i < this->m_numberOfGroups; ++i) {
			this->m_buffer[i].SetMeshFormat(format);
		}

		this->m_needGeometryUpdate = true;

		ISceneNode::SetMeshFormat(format);
	}

	void IMeshSceneNode::SetMeshShadingType(EMeshShadingType shadingType) {
		LOG_TRACE();

		this->m_shadingType = shadingType;

		ISceneNode::SetMeshShadingType(shadingType);
	}

	void IMeshSceneNode::Render() {
		LOG_TRACE();

		// Activation check
		this->m_active = !this->m_scene->GetActiveCamera()->FrustumCull(this->m_visibilityShape);

		// If the camera is bound to a light, it means that the render is used for a lightning process.
		// Render the mesh only if it's a shadow caster
		if(this->m_active && this->m_scene->GetActiveCamera()->GetParent() != nullptr && (this->m_scene->GetActiveCamera()->GetParent()->GetType() == SCENE_NODE_SPOTLIGHT || this->m_scene->GetActiveCamera()->GetParent()->GetType() == SCENE_NODE_DIRECTIONALLIGHT || this->m_scene->GetActiveCamera()->GetParent()->GetType() == SCENE_NODE_POINTLIGHT)) {
			if(!IsShadowCaster()) {
				this->m_active = false;
			}
		}

		if (this->m_active && this->m_enabled) {
			if (this->m_needGeometryUpdate) {
				FinalizeGeometry();
			}

			IRenderer * renderer = IRenderer::GetRenderer();
			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();

			Assert(shaderProgram != nullptr);

			// World matrixes
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_WORLD_MATRIX), this->m_worldMatrix);
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_ROTATION_MATRIX), this->m_absoluteRotationMatrix);

			// Group paramaters
			for(int i = 0; i < this->m_numberOfGroups; ++i) {
				// Material
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_DIFFUSE), this->m_activeMaterialElement[i]->m_kDiffuse);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_SPECULAR), this->m_activeMaterialElement[i]->m_kSpecular);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_EMISSION), this->m_activeMaterialElement[i]->m_kEmission);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_NS), this->m_activeMaterialElement[i]->m_ns);

				// Can override material ambient color
				if(this->m_scene->GetRootNode()->GetUseGlobalAmbientColor()) {
					shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_AMBIENT), this->m_scene->GetRootNode()->GetAmbientColor());
				} else {
					shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_AMBIENT), this->m_activeMaterialElement[i]->m_kAmbient);
				}
				

				if((this->m_buffer[i].GetMeshFormat() & MESHBUFFER_ELEMENT_TEXCOORD) != 0) {
					renderer->SetTexture(0, this->m_diffuseTexture[i].GetTexture());
					renderer->SetTexture(1, this->m_normalTexture[i].GetTexture());
				}

				renderer->SetVertexBuffer(0, this->m_buffer[i].GetVertexBuffer(), this->m_buffer[i].GetStride());
				renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_buffer[i].GetNumberOfElements() / 3);
			}

		}

		ISceneNode::Render();
	}

	const EMeshShadingType IMeshSceneNode::GetMeshShadingType() const {
		LOG_TRACE();

		return this->m_shadingType;
	}

	CSphereF * IMeshSceneNode::GetVisibilityShape() const {
		LOG_TRACE();

		return this->m_visibilityShape;
	}

	const std::string IMeshSceneNode::GetMeshTextureName(ETextureType textureType) const {
		LOG_TRACE();

		std::string name = "";
		switch(textureType) {
		case TEXTURE_TYPE_DIFFUSE:
			if(this->m_diffuseTexture != nullptr) {
				name = this->m_diffuseTexture->GetTexture()->GetName();
			}
			
			break;

		case TEXTURE_TYPE_NORMAL:
			if(this->m_normalTexture != nullptr) {
				name = this->m_normalTexture->GetTexture()->GetName();
			}
			break;
		}

		return name;
	}

	const std::string IMeshSceneNode::GetMeshMaterialName() const {
		LOG_TRACE();

		std::string name = "";

		if(this->m_material.GetMaterial() != nullptr) {
			name = this->m_material.GetMaterial()->GetName();
		}

		return name;
	}

	void IMeshSceneNode::FinalizeGeometry() {
		LOG_TRACE();
	}
}
