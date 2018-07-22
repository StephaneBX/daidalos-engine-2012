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

#include <algorithm>
#include "../../../../Core/Logger/ILogger.hpp"
#include "../../../../Core/Memory/CExceptionAssert.hpp"
#include "../../../../Scene/3D/I3DScene.hpp"
#include "../../../../Scene/3D/Structure/Simple/CSimpleSceneNode.hpp"

#define BSP_MIN_MOVE 0.02F
#define BSP_HEIGHT_EPSILON 0.4F

namespace daidalosengine {
	CSimpleSceneNode::CSimpleSceneNode(I3DScene * scene) : ILevelSceneNode(scene) {
		LOG_TRACE();

		SetType(SCENE_NODE_ROOT);
		CSimpleSceneNode::SetMeshShadingType(MESH_SHADING_SMOOTH);
	}

	CSimpleSceneNode::~CSimpleSceneNode() {
		LOG_TRACE();
	}

	void CSimpleSceneNode::LoadGeometry(const std::vector<SSimpleBrush> & brushes) {
		LOG_TRACE();

		// Bounding box variables
		auto minPosition = CVector3F(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		auto maxPosition = CVector3F(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

		for (const auto& brushe : brushes) {
			for (auto j : brushe.geometry) {
				// Add the diffuse texture to the diffuse textures vector
				//if(brushes[i].geometry[j]->GetTexture(TEXTURE_TYPE_DIFFUSE) != nullptr && std::find(this->m_diffuseTextures.begin(), this->m_diffuseTextures.end(), brushes[i].geometry[j]->GetTexture(TEXTURE_TYPE_DIFFUSE)) == this->m_diffuseTextures.end())
				AddDiffuseTexture(j->GetTexture(TEXTURE_TYPE_DIFFUSE));

				// Add the normal texture to the normal textures vector
				//if(brushes[i].geometry[j]->GetTexture(TEXTURE_TYPE_NORMAL) != nullptr && std::find(this->m_normalTextures.begin(), this->m_normalTextures.end(), brushes[i].geometry[j]->GetTexture(TEXTURE_TYPE_NORMAL)) == this->m_normalTextures.end())
				AddNormalTexture(j->GetTexture(TEXTURE_TYPE_NORMAL));

				// Update the bounding box
				for(unsigned int k = 0; k < 3; k++) {
					minPosition = minPosition.Min(j->GetVertex(k));
					maxPosition = maxPosition.Max(j->GetVertex(k));
				}
			}
		}

		// Add the brush to the brush list
		this->m_brushes = brushes;

		// Set the bounding box
		CAABoundingBoxF boundingBox;
		boundingBox.SetMinMax(minPosition, maxPosition);
		SetLevelBoundingBox(boundingBox);
	}

	/**
	* Render the transparent elements of the level
	*/
	void CSimpleSceneNode::Render(const int filterId) {
		LOG_TRACE();

		RenderWithOptions(filterId);
	}

	void CSimpleSceneNode::RenderTransparent() {
		LOG_TRACE();

		auto renderer = IRenderer::GetRenderer();
		renderer->SetRenderState(RENDER_STATE_TYPE_ALPHATEST, true);
		RenderWithOptions(SCENE_TRANSPARENT_ID);
		renderer->SetRenderState(RENDER_STATE_TYPE_ALPHATEST, false);
	}

	void CSimpleSceneNode::RenderSkyboxMask() {
		LOG_TRACE();

		RenderWithOptions(SCENE_SKYBOX_ID);
	}

	void CSimpleSceneNode::SetBrushCollision(const int brushId, const bool collision) {
		// Nothing to do
	}

	bool CSimpleSceneNode::Slide(IShape3DF & shape, const CVector3F & movement) {
		LOG_TRACE();

		shape.SetPosition(shape.GetPosition() + movement);
		return false;
	}

	bool CSimpleSceneNode::GetCollisionPoint(const CVector3F & position, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		return false;
	}

	bool CSimpleSceneNode::GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		return false;
	}

	void CSimpleSceneNode::RenderWithOptions(const int filterId) {
		LOG_TRACE();

		// Reset the data
		this->m_visibleTriangles.clear();

		auto renderer = IRenderer::GetRenderer();
		auto shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		SetActiveCamera(GetScene()->GetActiveCamera());
		RegisterBrushesForRendering(filterId);
		SetNumberOfVisibleTriangles(this->m_visibleTriangles.size());
		if(!this->m_visibleTriangles.empty()) {
			// World matrixes
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_WORLD_MATRIX), GetAbsoluteTransformationMatrix());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_ROTATION_MATRIX), GetAbsoluteRotationMatrix());

			// Sort the triangles
			std::vector<CTriangle3D*> batchTriangles;

			// Sort with the diffuse texture and the material
			while (!this->m_visibleTriangles.empty()) {
				batchTriangles.clear();
				batchTriangles.push_back(this->m_visibleTriangles[0]);
				const auto batchDiffuseTexture = batchTriangles[0]->GetTexture(TEXTURE_TYPE_DIFFUSE);
				const auto batchNormalTexture = batchTriangles[0]->GetTexture(TEXTURE_TYPE_NORMAL);
				const auto batchMaterialElement = batchTriangles[0]->GetMaterialElement();

				this->m_visibleTriangles.erase(this->m_visibleTriangles.begin());

				// Batch per texture
				if (!IsTexturingEnabled()) {
					// The textures are disabled so no batching needed, we take all the triangles
					batchTriangles.insert(std::end(batchTriangles), std::begin(this->m_visibleTriangles), std::end(this->m_visibleTriangles));
					this->m_visibleTriangles.clear();
				} else {
					// Check for compatible triangles
					for (auto it = this->m_visibleTriangles.begin(); it != this->m_visibleTriangles.end();) {
						if ((*it)->GetTexture(TEXTURE_TYPE_DIFFUSE) == batchDiffuseTexture && (*it)->GetTexture(TEXTURE_TYPE_NORMAL) == batchNormalTexture && (*it)->GetMaterialElement() == batchMaterialElement) {
							batchTriangles.push_back(*it);
							it = this->m_visibleTriangles.erase(it);
						} else {
							++it;
						}
					}
				}

				// Material
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_DIFFUSE), batchMaterialElement->m_kDiffuse);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_SPECULAR), batchMaterialElement->m_kSpecular);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_EMISSION), batchMaterialElement->m_kEmission);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_NS), batchMaterialElement->m_ns);

				// Can override material ambient color
				if(GetUseGlobalAmbientColor()) {
					shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_AMBIENT), GetAmbientColor());
				} else {
					shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_AMBIENT), batchMaterialElement->m_kAmbient);
				}

				if (IsTexturingEnabled()) {
					renderer->SetTexture(0, batchDiffuseTexture);
					renderer->SetTexture(1, batchNormalTexture);
				}

				// Transform & draw
				SetLevelBuffer(batchTriangles);
				RenderPrimitives();
			}
		}

		if (filterId == SCENE_DEFAULT_ID || filterId == SCENE_TOP_LAYER_ID) {
			ISceneNode::Render(filterId);
		}
	}

	void CSimpleSceneNode::RegisterBrushesForRendering(const int filterId) {
		LOG_TRACE();

		const auto isFiltered = filterId != SCENE_DEFAULT_ID;

		// Loop through each brush
		for (auto& brush : this->m_brushes) {
			// Loop through each triangle
			for (auto& it2 : brush.geometry) {
				if(!GetActiveCamera()->FrustumCull(&it2->GetCircumSphere()) && ((isFiltered && it2->GetId() == filterId) || (!isFiltered && it2->GetId() >= -1))) {
					this->m_visibleTriangles.push_back(it2);
				}
			}
		}
	}
}
