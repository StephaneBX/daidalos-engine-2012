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
#include "../../../../Core/Logger/ILogger.h"
#include "../../../../Core/Memory/CExceptionAssert.h"
#include "../../../../Scene/3D/I3DScene.h"
#include "../../../../Scene/3D/Structure/Simple/CSimpleSceneNode.h"

#define BSP_MIN_MOVE 0.02f
#define BSP_HEIGHT_EPSILON 0.4f

namespace DE {
	CSimpleSceneNode::CSimpleSceneNode(I3DScene * scene) : ILevelSceneNode(scene) {
		LOG_TRACE();

		this->m_type = SCENE_NODE_ROOT;
		this->m_shadingType = MESH_SHADING_SMOOTH;
	}

	CSimpleSceneNode::~CSimpleSceneNode() {
		LOG_TRACE();
	}

	void CSimpleSceneNode::LoadGeometry(const std::vector<SSimpleBrush> & brushes) {
		LOG_TRACE();

		// Bounding box variables
		CVector3F minPosition = CVector3F(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		CVector3F maxPosition = CVector3F(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

		for(unsigned int i = 0; i < brushes.size(); ++i) {
			for(unsigned int j = 0; j < brushes.at(i).geometry.size(); ++j) {
				// Add the diffuse texture to the diffuse textures vector
				if(brushes.at(i).geometry.at(j)->GetTexture(TEXTURE_TYPE_DIFFUSE) != nullptr && std::find(this->m_diffuseTextures.begin(), this->m_diffuseTextures.end(), brushes.at(i).geometry.at(j)->GetTexture(TEXTURE_TYPE_DIFFUSE)) == this->m_diffuseTextures.end())
					this->m_diffuseTextures.push_back(brushes.at(i).geometry.at(j)->GetTexture(TEXTURE_TYPE_DIFFUSE));

				// Add the normal texture to the normal textures vector
				if(brushes.at(i).geometry.at(j)->GetTexture(TEXTURE_TYPE_NORMAL) != nullptr && std::find(this->m_normalTextures.begin(), this->m_normalTextures.end(), brushes.at(i).geometry.at(j)->GetTexture(TEXTURE_TYPE_NORMAL)) == this->m_normalTextures.end())
					this->m_normalTextures.push_back(brushes.at(i).geometry.at(j)->GetTexture(TEXTURE_TYPE_NORMAL));

				// Update the bounding box
				for(unsigned int k = 0; k < 3; k++) {
					minPosition = minPosition.Min(brushes.at(i).geometry.at(j)->GetVertex(k));
					maxPosition = maxPosition.Max(brushes.at(i).geometry.at(j)->GetVertex(k));
				}
			}
		}

		// Add the brush to the brush list
		this->m_brushes = brushes;

		// Set the bounding box
		this->m_boundingBox.SetMinMax(minPosition, maxPosition);
	}

	void CSimpleSceneNode::Render() {
		LOG_TRACE();

		// Reset the data
		this->m_visibleTriangles.clear();
		this->m_batchSize.clear();

		IRenderer * renderer = IRenderer::GetRenderer();
		IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
		Assert(shaderProgram != nullptr);

		this->m_activeCamera = this->m_scene->GetActiveCamera();
		RegisterBrushesForRendering();
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
				batchDiffuseTexture = batchTriangles.at(0)->GetTexture(TEXTURE_TYPE_DIFFUSE);
				batchNormalTexture = batchTriangles.at(0)->GetTexture(TEXTURE_TYPE_NORMAL);
				batchMaterialElement = batchTriangles.at(0)->GetMaterialElement();

				this->m_visibleTriangles.erase(this->m_visibleTriangles.begin());

				// Check for compatible triangles
				for(auto it = this->m_visibleTriangles.begin(); it != this->m_visibleTriangles.end();) {
					if(((this->m_levelBuffer.GetMeshFormat() & MESHBUFFER_ELEMENT_TEXCOORD) == 0) || ((*it)->GetTexture(TEXTURE_TYPE_DIFFUSE) == batchDiffuseTexture && (*it)->GetTexture(TEXTURE_TYPE_NORMAL) == batchNormalTexture && (*it)->GetMaterialElement() == batchMaterialElement)) {
						batchTriangles.push_back(*it);
						it = this->m_visibleTriangles.erase(it);
					} else
						++it;
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

		ISceneNode::Render();
	}

	const bool CSimpleSceneNode::Slide(IShape3DF & shape, const CVector3F & movement) {
		LOG_TRACE();

		shape.SetPosition(shape.GetPosition() + movement);
		return false;
	}

	const bool CSimpleSceneNode::GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point) {
		LOG_TRACE();

		return false;
	}

	void CSimpleSceneNode::RegisterBrushesForRendering() {
		LOG_TRACE();

		// Loop through each brush
		for(auto it = this->m_brushes.begin(); it != this->m_brushes.end(); it++) {
			// Check if the brush is visible
			//if(!this->m_activeCamera->FrustumCull((*it).visibilityShape))
			//{
			// Loop through each triangle
			for(auto it2 = (*it).geometry.begin(); it2 != (*it).geometry.end(); ++it2) {
				if(!this->m_activeCamera->FrustumCull(&(*it2)->GetCircumSphere())) {
					this->m_visibleTriangles.push_back((*it2));
				}
			}
			//}
		}
	}
}
