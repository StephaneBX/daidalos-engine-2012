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

#include "../../../../Core/IRenderer.hpp"
#include "../../../../Core/Logger/ILogger.hpp"
#include "../../../../Core/Memory/CExceptionAssert.hpp"
#include "../../../../Scene/3D/I3DScene.hpp"
#include "../../../../Scene/3D/Structure/BSP/CBSPSceneNode.hpp"

#define BSP_MIN_MOVE 0.02f
#define BSP_HEIGHT_EPSILON 0.4f

namespace daidalosengine {
	CBSPSceneNode::CBSPSceneNode(I3DScene * scene) : ILevelSceneNode(scene) {
		LOG_TRACE();

		SetType(SCENE_NODE_ROOT);
		this->m_bsp = nullptr;
		this->m_collisionBsp = nullptr;
		CBSPSceneNode::SetMeshShadingType(MESH_SHADING_SMOOTH);
	}

	CBSPSceneNode::~CBSPSceneNode() {
		LOG_TRACE();
	}

	void CBSPSceneNode::LoadGeometry(const std::vector<SBSPBrushDefinition> & brushDefinition) {
		LOG_TRACE();

		// Reset triangles count
		this->m_geometryLeafs.clear();

		// Global geometry vector
		std::vector<CBSPTriangle*> global;
		std::vector<CBSPTriangle*> brushGeometry;

		// Bounding box variables
		auto minPosition = CVector3F(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		auto maxPosition = CVector3F(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

		// Loop through the brushes
		for (const auto& i : brushDefinition) {
			brushGeometry.clear();
			for (auto j : i.geometry) {
				// Add the diffuse texture to the diffuse textures vector
				//if(brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_DIFFUSE) != nullptr && std::find(this->m_diffuseTextures.begin(), this->m_diffuseTextures.end(), brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_DIFFUSE)) == this->m_diffuseTextures.end())
				AddDiffuseTexture(j->GetTexture(TEXTURE_TYPE_DIFFUSE));

				// Add the normal texture to the normal textures vector
				//if(brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_NORMAL) != nullptr && std::find(this->m_normalTextures.begin(), this->m_normalTextures.end(), brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_NORMAL)) == this->m_normalTextures.end())
				AddNormalTexture(j->GetTexture(TEXTURE_TYPE_NORMAL));

				// Add a copy in the global vector
				global.push_back(new CBSPTriangle(*j));

				// Add a copy in the brush vector
				brushGeometry.push_back(new CBSPTriangle(*j));
			}

			// Create the brush and add it to the brush list
			auto* brush = new SBSPBrush();
			brush->enabled = i.collision;
			brush->tested = false;
			brush->id = i.id;
			brush->root = std::make_unique<CBSPTreeNode>();
			brush->root->LoadTriangleList(brushGeometry, true);

			this->m_brushes.push_back(std::unique_ptr<SBSPBrush>(brush));
		}

		// Add the triangles to the bsp geometry
		this->m_bsp = std::make_unique<CBSPTreeNode>();
		this->m_bsp->LoadTriangleList(global, true);
		RegisterLeafs(this->m_bsp.get());

		// Create the collision bsp by cloning the main bsp nodes
		this->m_collisionBsp = std::make_unique<CBSPBrushNode>(this->m_bsp.get());

		for(unsigned int i = 0; i < this->m_brushes.size(); ++i) {
			// Add the brushes to the collision bsp
			this->m_collisionBsp->AddBrush(this->m_brushes[i].get(), brushDefinition[i].geometry);

			// Update the bounding box
			for (auto j : brushDefinition[i].geometry) {
				for(auto k = 0U; k < 3; k++) {
					minPosition = minPosition.Min(j->GetVertex(k));
					maxPosition = maxPosition.Max(j->GetVertex(k));
				}
			}

			// Delete the triangles as they're no longer needed
			for (auto j : brushDefinition[i].geometry) {
				delete j;
			}
		}

		// Set the bounding box
		CAABoundingBoxF boundingBox;
		boundingBox.SetMinMax(minPosition, maxPosition);
		SetLevelBoundingBox(boundingBox);
	}

	/**
	* Render the transparent elements of the level
	*/
	void CBSPSceneNode::Render(const int filterId) {
		LOG_TRACE();

		RenderWithOptions(filterId);
	}

	void CBSPSceneNode::RenderTransparent() {
		LOG_TRACE();

		auto* renderer = IRenderer::GetRenderer();
		renderer->SetRenderState(RENDER_STATE_TYPE_ALPHATEST, true);
		RenderWithOptions(SCENE_TRANSPARENT_ID);
		renderer->SetRenderState(RENDER_STATE_TYPE_ALPHATEST, false);
	}

	void CBSPSceneNode::RenderSkyboxMask() {
		LOG_TRACE();

		RenderWithOptions(SCENE_SKYBOX_ID);
	}

	void CBSPSceneNode::SetBrushCollision(const int brushId, const bool collision) {
		LOG_TRACE();

		for (auto& brush : this->m_brushes) {
			if(brush->id == brushId) {
				brush->enabled = collision;
			}
		}
	}

	bool CBSPSceneNode::Slide(IShape3DF & shape, const CVector3F & movement) {
		LOG_TRACE();

		auto isCollision = false;
		float dot;
		float fraction = 1;
		auto normalIndex = 0;
		auto i = 0;
		auto j = 0;
		auto startPoint = shape.GetPosition();
		const auto& velocity = movement;
		auto remainingDistance = movement;

		CImpact impact;
		CImpact iBrush;

		auto elevated = false;

		for(auto impactLoop = 0; impactLoop < BSP_MAX_IMPACT; ++impactLoop) {
			const auto endPoint = startPoint + remainingDistance;

			// Impacts initialization
			impact.Initialize(startPoint, endPoint);
			iBrush.Initialize(startPoint, endPoint);

			// Collision test
			this->m_collisionBsp->CollisionDetection(shape, impact, iBrush);

			if(impact.IsImpact()) {
				// Test with an slightly elevated position for wall type collisions
				if(!elevated && impact.GetPlaneNormal().y <= 0) {
					CImpact impactEpsilon;
					CImpact iBrushEpsilon;
					const auto shapePositionOld = shape.GetPosition();
					elevated = true;

					impactEpsilon.Initialize(CVector3F(startPoint.x, startPoint.y + BSP_HEIGHT_EPSILON, startPoint.z), CVector3F(endPoint.x, endPoint.y + BSP_HEIGHT_EPSILON, endPoint.z));
					iBrushEpsilon.Initialize(CVector3F(startPoint.x, startPoint.y + BSP_HEIGHT_EPSILON, startPoint.z), CVector3F(endPoint.x, endPoint.y + BSP_HEIGHT_EPSILON, endPoint.z));
					shape.SetPosition(CVector3F(shapePositionOld.x, shapePositionOld.y + BSP_HEIGHT_EPSILON, shapePositionOld.z));

					// elevated collision test
					this->m_collisionBsp->CollisionDetection(shape, impactEpsilon, iBrushEpsilon);

					// Use the elevated position only if the impact fraction is bigger (more distance traveled)
					if(impactEpsilon.GetImpactFraction() > impact.GetImpactFraction()) {
						impact = impactEpsilon;
						iBrush = iBrushEpsilon;

						// If the elevated position causes no collision, stop here
						if(!impactEpsilon.IsImpact()) {
							shape.SetPosition(impactEpsilon.GetEndPoint());
							return isCollision;
						}
					} else {
						// Use the old position
						shape.SetPosition(shapePositionOld);
					}
				}

				isCollision = true;
			} else {
				shape.SetPosition(endPoint);
				return isCollision;
			}

			fraction -= fraction * impact.GetImpactFraction();
			auto distance = impact.GetDistance() * impact.GetImpactFraction();

			// Reset the normal index if the distance shape-impact is too big
			if(distance.Length() > BSP_MIN_MOVE) {
				normalIndex = 0;
			}

			// Memorize the plane normal
			this->m_normals[normalIndex] = impact.GetPlaneNormal();
			++normalIndex;

			startPoint += distance;

			// Normals verification
			for(i = 0; i < normalIndex; ++i) {
				dot = velocity.DotProduct(this->m_normals[i]);
				remainingDistance = velocity - this->m_normals[i] * dot;

				for(j = 0; j < normalIndex; ++j) {
					if(i != j) {
						if (remainingDistance.DotProduct(this->m_normals[j]) < 0) {
							break;
						}
					}
				}

				if (j == normalIndex) {
					break;
				}
			}

			// Position modification depending on the number of normals
			if(i == normalIndex) {
				if(normalIndex == 2) {
					auto edge = this->m_normals[0].CrossProduct(this->m_normals[1]).Normalize();
					dot = edge.DotProduct(velocity);
					remainingDistance = edge * dot;
				} else {
					shape.SetPosition(startPoint);
					return isCollision;
				}
			}

			remainingDistance *= fraction;

			// Don't move if the sliding movement is toward the velocity or too small
			if(velocity.DotProduct(remainingDistance) < 0 || remainingDistance.Length() < BSP_MIN_MOVE) {
				/*CVector3F distance = startPoint - shape.GetPosition();
				if(distance.Length() >= BSP_MIN_MOVE)
				shape.SetPosition(startPoint);*/

				shape.SetPosition(startPoint);

				return isCollision;
			}
		}

		return isCollision;
	}

	bool CBSPSceneNode::GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		CImpact impact;
		CImpact iBrush;

		impact.Initialize(shape.GetPosition(), shape.GetPosition() + movement);
		iBrush.Initialize(shape.GetPosition(), shape.GetPosition() + movement);

		this->m_collisionBsp->CollisionDetection(shape, impact, iBrush);
		point = shape.GetPosition() + impact.GetDistance() * impact.GetImpactFraction();

		return impact.IsImpact();
	}

	bool CBSPSceneNode::GetCollisionPoint(const CVector3F & position, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		CImpact impact;
		CImpact iBrush;

		impact.Initialize(position, position + movement);
		iBrush.Initialize(position, position + movement);

		this->m_collisionBsp->CollisionDetection(position, impact, iBrush);
		point = position + impact.GetDistance() * impact.GetImpactFraction();

		return impact.IsImpact();
	}

	void CBSPSceneNode::RenderWithOptions(const int filterId) {
		LOG_TRACE();

		// Reset the data
		this->m_visibleTriangles.clear();
		//this->m_bsp->ResetDraw();

		auto* renderer = IRenderer::GetRenderer();
		auto* shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		SetActiveCamera(GetScene()->GetActiveCamera());
		RegisterNodeForRendering(this->m_bsp.get(),filterId);
		SetNumberOfVisibleTriangles(this->m_visibleTriangles.size());
		if (!this->m_visibleTriangles.empty()) {

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
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_AMBIENT), batchMaterialElement->m_kAmbient);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_SPECULAR), batchMaterialElement->m_kSpecular);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_EMISSION), batchMaterialElement->m_kEmission);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_NS), batchMaterialElement->m_ns);

				if (IsTexturingEnabled()) {
					renderer->SetTexture(0, batchDiffuseTexture);
					renderer->SetTexture(1, batchNormalTexture);
				}

				//Transform & draw
				SetLevelBuffer(batchTriangles);
				RenderPrimitives();
			}
		}

		if (filterId == SCENE_DEFAULT_ID || filterId == SCENE_TOP_LAYER_ID) {
			ISceneNode::Render(filterId);
		}
	}

	void CBSPSceneNode::RegisterNodeForRendering(const int filterId) {
		LOG_TRACE();

		const auto isFiltered = filterId != SCENE_DEFAULT_ID;

		// Loop through each leaf
		for(auto it = this->m_geometryLeafs.begin(); it != m_geometryLeafs.end(); ++it) {

			// Check the visibility for the leaf before testing each triangle
			if(!GetActiveCamera()->FrustumCull(&(*it)->GetVisibilitySphere())) {
				// Loop through each triangle
				for (const auto& it2 : (*it)->GetTriangles()) {
					if(it2->IsVisible()/* && !(*it2)->IsDrawn()*/ && !GetActiveCamera()->FrustumCull(&it2->GetCircumSphere()) && ((isFiltered && it2->GetId() == filterId) || (!isFiltered &&
						it2->GetId() >= -1))) {
						this->m_visibleTriangles.push_back(it2.get());
						//(*it2)->SetDrawn(true);
					}
				}
			}	
		}
	}

	void CBSPSceneNode::RegisterNodeForRendering(CBSPTreeNode * node, const int filterId) {
		LOG_TRACE();

		if(GetActiveCamera()->FrustumCull(&node->GetVisibilitySphere())) {
			return;
		}

		const auto isFiltered = filterId != SCENE_DEFAULT_ID;

		if(node->GetType() == BSP_NODE_TYPE_EMPTY_LEAF) {
			for (const auto& it : node->GetTriangles()) {
				if(it->IsVisible()/* && !(*it)->IsDrawn()*/ && !GetActiveCamera()->FrustumCull(&it->GetCircumSphere()) && ((isFiltered && it->GetId() == filterId) || (!isFiltered &&
					it->GetId() >= -1))) {
					this->m_visibleTriangles.push_back(it.get());
					//(*it)->SetDrawn(true);
				}
			}
		} else {
			if(node->GetChild(BSP_NODE_CHILD_FRONT) != nullptr) {
				RegisterNodeForRendering(node->GetChild(BSP_NODE_CHILD_FRONT), filterId);
			}

			if(node->GetChild(BSP_NODE_CHILD_BACK) != nullptr) {
				RegisterNodeForRendering(node->GetChild(BSP_NODE_CHILD_BACK), filterId);
			}
		}
	}

	void CBSPSceneNode::RegisterLeafs(CBSPTreeNode * node) {
		LOG_TRACE();

		if (node->GetType() == BSP_NODE_TYPE_EMPTY_LEAF) {
			this->m_geometryLeafs.push_back(node);
		}

		if (node->GetChild(BSP_NODE_CHILD_FRONT) != nullptr) {
			RegisterLeafs(node->GetChild(BSP_NODE_CHILD_FRONT));
		}

		if (node->GetChild(BSP_NODE_CHILD_BACK) != nullptr) {
			RegisterLeafs(node->GetChild(BSP_NODE_CHILD_BACK));
		}
	}
}
