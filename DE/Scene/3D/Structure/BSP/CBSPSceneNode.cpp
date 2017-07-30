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
#include "../../../../Core/IRenderer.hpp"
#include "../../../../Core/Logger/ILogger.hpp"
#include "../../../../Core/Memory/CExceptionAssert.hpp"
#include "../../../../Scene/3D/I3DScene.hpp"
#include "../../../../Scene/3D/Structure/BSP/CBSPSceneNode.hpp"

#define BSP_MIN_MOVE 0.02f
#define BSP_HEIGHT_EPSILON 0.4f

namespace DE {
	CBSPSceneNode::CBSPSceneNode(I3DScene * scene) : ILevelSceneNode(scene) {
		LOG_TRACE();

		this->m_type = SCENE_NODE_ROOT;
		this->m_bsp = nullptr;
		this->m_collisionBsp = nullptr;
		this->m_shadingType = MESH_SHADING_SMOOTH;
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
		CVector3F minPosition = CVector3F(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		CVector3F maxPosition = CVector3F(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

		// Loop through the brushes
		for(unsigned int i = 0; i < brushDefinition.size(); ++i) {
			brushGeometry.clear();
			for(unsigned int j = 0; j < brushDefinition[i].geometry.size(); ++j) {
				// Add the diffuse texture to the diffuse textures vector
				if(brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_DIFFUSE) != nullptr && std::find(this->m_diffuseTextures.begin(), this->m_diffuseTextures.end(), brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_DIFFUSE)) == this->m_diffuseTextures.end())
					this->m_diffuseTextures.push_back(brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_DIFFUSE));

				// Add the normal texture to the normal textures vector
				if(brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_NORMAL) != nullptr && std::find(this->m_normalTextures.begin(), this->m_normalTextures.end(), brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_NORMAL)) == this->m_normalTextures.end())
					this->m_normalTextures.push_back(brushDefinition[i].geometry[j]->GetTexture(TEXTURE_TYPE_NORMAL));

				// Add a copy in the global vector
				global.push_back(new CBSPTriangle(*brushDefinition[i].geometry[j]));

				// Add a copy in the brush vector
				brushGeometry.push_back(new CBSPTriangle(*brushDefinition[i].geometry[j]));
			}

			// Create the brush and add it to the brush list
			SBSPBrush * brush = new SBSPBrush();
			brush->enabled = brushDefinition[i].collision;
			brush->tested = false;
			brush->id = brushDefinition[i].id;
			brush->root = std::unique_ptr<CBSPTreeNode>(new CBSPTreeNode());
			brush->root->LoadTriangleList(brushGeometry, true);

			this->m_brushes.push_back(std::unique_ptr<SBSPBrush>(brush));
		}

		// Add the triangles to the bsp geometry
		this->m_bsp = std::unique_ptr<CBSPTreeNode>(new CBSPTreeNode());
		this->m_bsp->LoadTriangleList(global, true);
		RegisterLeafs(this->m_bsp.get());

		// Create the collision bsp by cloning the main bsp nodes
		this->m_collisionBsp = std::unique_ptr<CBSPBrushNode>(new CBSPBrushNode(this->m_bsp.get()));

		for(unsigned int i = 0; i < this->m_brushes.size(); ++i) {
			// Add the brushes to the collision bsp
			this->m_collisionBsp->AddBrush(this->m_brushes[i].get(), brushDefinition[i].geometry);

			// Update the bounding box
			for(unsigned int j = 0; j < brushDefinition[i].geometry.size(); ++j) {
				for(unsigned int k = 0; k < 3; k++) {
					minPosition = minPosition.Min(brushDefinition[i].geometry[j]->GetVertex(k));
					maxPosition = maxPosition.Max(brushDefinition[i].geometry[j]->GetVertex(k));
				}
			}

			// Delete the triangles as they're no longer needed
			for(unsigned int j = 0; j < brushDefinition[i].geometry.size(); ++j)
				delete brushDefinition[i].geometry[j];
		}

		// Set the bounding box
		this->m_boundingBox.SetMinMax(minPosition, maxPosition);
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

		IRenderer * renderer = IRenderer::GetRenderer();
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

		for(auto it = this->m_brushes.begin(); it != this->m_brushes.end(); it++) {
			if((*it)->id == brushId) {
				(*it)->enabled = collision;
			}
		}
	}

	const bool CBSPSceneNode::Slide(IShape3DF & shape, const CVector3F & movement) {
		LOG_TRACE();

		CVector3F endPoint, edge, distance;
		bool isCollision = false;
		float dot;
		float fraction = 1;
		int normalIndex = 0;
		int i = 0;
		int j = 0;
		CVector3F startPoint = shape.GetPosition();
		CVector3F velocity = movement;
		CVector3F remainingDistance = movement;

		CImpact impact;
		CImpact iBrush;

		bool elevated = false;

		for(int impactLoop = 0; impactLoop < BSP_MAX_IMPACT; ++impactLoop) {
			endPoint = startPoint + remainingDistance;

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
					CVector3F shapePositionOld = shape.GetPosition();
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
			distance = impact.GetDistance() * impact.GetImpactFraction();

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
						if(remainingDistance.DotProduct(this->m_normals[j]) < 0)
							break;
					}
				}

				if(j == normalIndex)
					break;
			}

			// Position modification depending on the number of normals
			if(i == normalIndex) {
				if(normalIndex == 2) {
					edge = this->m_normals[0].CrossProduct(this->m_normals[1]).Normalize();
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

	const bool CBSPSceneNode::GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		CImpact impact;
		CImpact iBrush;

		impact.Initialize(shape.GetPosition(), shape.GetPosition() + movement);
		iBrush.Initialize(shape.GetPosition(), shape.GetPosition() + movement);

		this->m_collisionBsp->CollisionDetection(shape, impact, iBrush);
		point = shape.GetPosition() + impact.GetDistance() * impact.GetImpactFraction();

		return impact.IsImpact();
	}

	const bool CBSPSceneNode::GetCollisionPoint(const CVector3F & position, const CVector3F & movement, CVector3F & point) {
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
		this->m_batchSize.clear();
		//this->m_bsp->ResetDraw();

		IRenderer * renderer = IRenderer::GetRenderer();
		IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		this->m_activeCamera = this->m_scene->GetActiveCamera();
		RegisterNodeForRendering(this->m_bsp.get(),filterId);
		this->m_numberOfVisibleTriangles = this->m_visibleTriangles.size();
		if(this->m_visibleTriangles.size() > 0) {

			// World matrixes
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_WORLD_MATRIX), this->m_worldMatrix);
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_ROTATION_MATRIX), this->m_absoluteRotationMatrix);

			// Sort the triangles
			std::vector<CTriangle3D*> batchTriangles;
			ITexture2DBase * batchDiffuseTexture;
			ITexture2DBase * batchNormalTexture;
			SMaterialElement * batchMaterialElement;

			// Sort with the diffuse texture and the material
			while(this->m_visibleTriangles.size() > 0) {
				batchTriangles.clear();
				batchTriangles.push_back(this->m_visibleTriangles[0]);
				batchDiffuseTexture = batchTriangles[0]->GetTexture(TEXTURE_TYPE_DIFFUSE);
				batchNormalTexture = batchTriangles[0]->GetTexture(TEXTURE_TYPE_NORMAL);
				batchMaterialElement = batchTriangles[0]->GetMaterialElement();

				this->m_visibleTriangles.erase(this->m_visibleTriangles.begin());

				// Batch per texture
				if((this->m_levelBuffer.GetMeshFormat() & MESHBUFFER_ELEMENT_TEXCOORD) == 0) {
					// The textures are disabled so no batching needed, we take all the triangles
					batchTriangles.insert(std::end(batchTriangles), std::begin(this->m_visibleTriangles), std::end(this->m_visibleTriangles));
					this->m_visibleTriangles.clear();
				} else {
					// Check for compatible triangles
					for(auto it = this->m_visibleTriangles.begin(); it != this->m_visibleTriangles.end();) {
						if((*it)->GetTexture(TEXTURE_TYPE_DIFFUSE) == batchDiffuseTexture && (*it)->GetTexture(TEXTURE_TYPE_NORMAL) == batchNormalTexture && (*it)->GetMaterialElement() == batchMaterialElement) {
							batchTriangles.push_back(*it);
							it = this->m_visibleTriangles.erase(it);
						} else {
							++it;
						}
					}
				}

				// Transform
				this->m_levelBuffer.Transform(batchTriangles, this->m_shadingType);

				// Material
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_DIFFUSE), batchMaterialElement->m_kDiffuse);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_AMBIENT), batchMaterialElement->m_kAmbient);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_SPECULAR), batchMaterialElement->m_kSpecular);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_EMISSION), batchMaterialElement->m_kEmission);
				shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_MATERIAL_NS), batchMaterialElement->m_ns);

				if((this->m_levelBuffer.GetMeshFormat() & MESHBUFFER_ELEMENT_TEXCOORD) != 0) {
					renderer->SetTexture(0, batchDiffuseTexture);
					renderer->SetTexture(1, batchNormalTexture);
				}

				renderer->SetVertexBuffer(0, this->m_levelBuffer.GetVertexBuffer(), this->m_levelBuffer.GetStride());
				renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_levelBuffer.GetNumberOfElements() / 3);
			}
		}

		if(filterId == SCENE_DEFAULT_ID || filterId == SCENE_TOP_LAYER_ID) {
			ISceneNode::Render(filterId);
		}
	}

	void CBSPSceneNode::RegisterNodeForRendering(const int filterId) {
		LOG_TRACE();

		bool isFiltered = filterId != SCENE_DEFAULT_ID;

		// Loop through each leaf
		for(auto it = this->m_geometryLeafs.begin(); it != m_geometryLeafs.end(); ++it) {

			// Check the visibility for the leaf before testing each triangle
			if(!this->m_activeCamera->FrustumCull(&(*it)->GetVisibilitySphere())) {
				// Loop through each triangle
				for(auto it2 = (*it)->GetTriangles().begin(); it2 != (*it)->GetTriangles().end(); ++it2) {
					if((*it2)->IsVisible()/* && !(*it2)->IsDrawn()*/ && !this->m_activeCamera->FrustumCull(&(*it2)->GetCircumSphere()) && ((isFiltered && (*it2)->GetId() == filterId) || (!isFiltered && (*it2)->GetId() >= -1))) {
						this->m_visibleTriangles.push_back((*it2).get());
						//(*it2)->SetDrawn(true);
					}
				}
			}	
		}
	}

	void CBSPSceneNode::RegisterNodeForRendering(CBSPTreeNode * node, const int filterId) {
		LOG_TRACE();

		if(this->m_activeCamera->FrustumCull(&node->GetVisibilitySphere())) {
			return;
		}

		bool isFiltered = filterId != SCENE_DEFAULT_ID;

		if(node->GetType() == BSP_NODE_TYPE_EMPTY_LEAF) {
			for(auto it = node->GetTriangles().begin(); it != node->GetTriangles().end(); ++it) {
				if((*it)->IsVisible()/* && !(*it)->IsDrawn()*/ && !this->m_activeCamera->FrustumCull(&(*it)->GetCircumSphere()) && ((isFiltered && (*it)->GetId() == filterId) || (!isFiltered && (*it)->GetId() >= -1))) {
					this->m_visibleTriangles.push_back((*it).get());
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

		if(node->GetType() == BSP_NODE_TYPE_EMPTY_LEAF)
			this->m_geometryLeafs.push_back(node);

		if(node->GetChild(BSP_NODE_CHILD_FRONT) != nullptr)
			RegisterLeafs(node->GetChild(BSP_NODE_CHILD_FRONT));

		if(node->GetChild(BSP_NODE_CHILD_BACK) != nullptr)
			RegisterLeafs(node->GetChild(BSP_NODE_CHILD_BACK));
	}
}
