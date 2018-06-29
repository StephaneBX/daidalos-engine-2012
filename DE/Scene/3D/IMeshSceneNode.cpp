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
#include "../../Core/IRenderer.hpp"
#include "../../Core/CConfigurationManager.hpp"
#include "../../Core/Memory/CExceptionAssert.hpp"
#include "../../Core/Logger/ILogger.hpp"
#include "../../Scene/3D/CSphere.hpp"
#include "../../Scene/3D/I3DScene.hpp"
#include "../../Scene/3D/IMeshSceneNode.hpp"

namespace daidalosengine {
	IMeshSceneNode::IMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name) : ISceneNode(scene, parent, name) {
		LOG_TRACE();

		this->m_shadingType = MESH_SHADING_SMOOTH;
		this->m_visibilityShape = new CSphereF();
		this->m_needGeometryUpdate = true;

		this->m_numberOfGroups = 0;
		this->m_buffer = nullptr;
		this->m_diffuseTexture = nullptr;
		this->m_normalTexture = nullptr;
	}

	IMeshSceneNode::IMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CVector3F & scale) : ISceneNode(scene, parent, name, position, rotation) {
		LOG_TRACE();

		this->m_shadingType = MESH_SHADING_SMOOTH;
		SetScale(scale);
		this->m_visibilityShape = new CSphereF();
		this->m_needGeometryUpdate = true;

		this->m_numberOfGroups = 0;
		this->m_buffer = nullptr;
		this->m_diffuseTexture = nullptr;
		this->m_normalTexture = nullptr;
	}

	IMeshSceneNode::~IMeshSceneNode() {
		LOG_TRACE();

		SAFE_DELETE_ARRAY(this->m_buffer);
		SAFE_DELETE_ARRAY(this->m_diffuseTexture);
		SAFE_DELETE_ARRAY(this->m_normalTexture);

		SAFE_DELETE(this->m_visibilityShape);
	}

	/**
	* Set the filename
	* @param filename The filename
	*/
	void IMeshSceneNode::SetFileName(const std::string & filename) {
		LOG_TRACE();

		this->m_filename = filename;
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

			default:
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

			default:
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

	/**
	* Render the node
	* @param filterId The filter id to apply the select the rendered nodes
	*/
	void IMeshSceneNode::Render(const int filterId) {
		LOG_TRACE();

		// Activation check
		SetActive((filterId == SCENE_DEFAULT_ID && GetId()!= SCENE_TOP_LAYER_ID && !GetScene()->GetActiveCamera()->FrustumCull(this->m_visibilityShape)) || (GetId() == filterId && filterId == SCENE_TOP_LAYER_ID));

		// If the camera is bound to a light, it means that the render is used for a lightning shadow process.
		// Render the mesh only if it's a shadow caster
		if(IsActive() && GetScene()->GetActiveCamera()->GetParent() != nullptr && (GetScene()->GetActiveCamera()->GetParent()->GetType() == SCENE_NODE_SPOTLIGHT || GetScene()->GetActiveCamera()->GetParent()->GetType() == SCENE_NODE_DIRECTIONALLIGHT || GetScene()->GetActiveCamera()->GetParent()->GetType() == SCENE_NODE_POINTLIGHT)) {
			if(!IsShadowCaster()) {
				SetActive(false);
			}
		}

		if (IsActive() && IsEnabled()) {
			if (this->m_needGeometryUpdate) {
				FinalizeGeometry();
			}

			IRenderer * renderer = IRenderer::GetRenderer();
			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();

			Assert(shaderProgram != nullptr);

			// World matrixes
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_WORLD_MATRIX), GetAbsoluteTransformationMatrix());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_ROTATION_MATRIX), GetAbsoluteRotationMatrix());

			// Group paramaters
			for(int i = 0; i < this->m_numberOfGroups; ++i) {
				// Material
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_DIFFUSE), this->m_activeMaterialElement[i]->m_kDiffuse);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_SPECULAR), this->m_activeMaterialElement[i]->m_kSpecular);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_EMISSION), this->m_activeMaterialElement[i]->m_kEmission);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_NS), this->m_activeMaterialElement[i]->m_ns);

				// Can override material ambient color
				if(GetScene()->GetRootNode()->GetUseGlobalAmbientColor()) {
					shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_AMBIENT), GetScene()->GetRootNode()->GetAmbientColor());
				} else {
					shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_AMBIENT), this->m_activeMaterialElement[i]->m_kAmbient);
				}
				

				if((this->m_buffer[i].GetMeshFormat() & MESHBUFFER_ELEMENT_TEXCOORD) != 0) {
					renderer->SetTexture(0, this->m_diffuseTexture[i].GetTexture());
					renderer->SetTexture(1, this->m_normalTexture[i].GetTexture());
				}

				renderer->SetVertexBuffer(0, this->m_buffer[i].GetVertexBuffer().GetBuffer(), this->m_buffer[i].GetStride());
				renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_buffer[i].GetNumberOfElements() / 3);
			}

		}

		ISceneNode::Render(filterId);
	}

	/**
	* Get the number of groups
	*/
	const int IMeshSceneNode::GetNumberOfGroups() const {
		LOG_TRACE();

		return this->m_numberOfGroups;
	}

	const EMeshShadingType IMeshSceneNode::GetMeshShadingType() const {
		LOG_TRACE();

		return this->m_shadingType;
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

		default:
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

	CSphereF * IMeshSceneNode::GetVisibilityShape() {
		LOG_TRACE();

		return this->m_visibilityShape;
	}

	/**
	* Initialize the geometry data
	* @param numberOfGroups The number of groups
	*/
	void IMeshSceneNode::InitializeGeometry(const int numberOfGroups) {
		LOG_TRACE();

		SAFE_DELETE_ARRAY(this->m_buffer);
		SAFE_DELETE_ARRAY(this->m_diffuseTexture);
		SAFE_DELETE_ARRAY(this->m_normalTexture);

		// Set the number of groups
		this->m_numberOfGroups = numberOfGroups;

		// Initialize the buffers
		this->m_buffer = new CMeshBuffer[this->m_numberOfGroups];
		for (int i = 0; i < this->m_numberOfGroups; i++) {
			this->m_buffer[i].SetScene(GetScene());
		}

		// Initialize the textures arrays
		this->m_diffuseTexture = new CTexture2D[this->m_numberOfGroups];
		this->m_normalTexture = new CTexture2D[this->m_numberOfGroups];
	}

	/**
	* Push the mesh data into the buffer.
	* @param mesh The mesh
	* @param group The group index
	*/
	void IMeshSceneNode::PushMeshData(IMeshBase * mesh, const int group) {
		LOG_TRACE();

		this->m_buffer[group].Transform(mesh->GetVertices(group), mesh->GetTexcoords(group), mesh->GetNormals(group), mesh->GetTangents(group), mesh->GetBinormals(group), mesh->GetGroupSize(group));
	}

	/**
	* Set if a geometry update is required.
	* @param needGeometryUpdate If a geometry update is required
	*/
	void IMeshSceneNode::SetNeedGeometryUpdate(const bool needGeometryUpdate) {
		LOG_TRACE();

		this->m_needGeometryUpdate = needGeometryUpdate;
	}

	void IMeshSceneNode::FinalizeGeometry() {
		LOG_TRACE();
	}
}
