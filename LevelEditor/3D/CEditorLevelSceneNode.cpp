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

#include <Core/IRenderer.hpp>
#include "../3D/CEditorLevelSceneNode.hpp"

namespace daidaloseditor {
	CEditorLevelSceneNode::CEditorLevelSceneNode(daidalosengine::I3DScene * scene) : ILevelSceneNode(scene) {
		SetType(daidalosengine::SCENE_NODE_ROOT);
		SetMeshShadingType(daidalosengine::MESH_SHADING_FLAT);
	}

	CEditorLevelSceneNode::~CEditorLevelSceneNode() = default;
	
	/**
	* Render the node
	* @param filterId The filter id to apply the select the rendered nodes
	*/
	void CEditorLevelSceneNode::Render(const int filterId) {
		RenderWithOptions(filterId);
	}

	/**
	* Render the transparent elements of the level
	*/
	void CEditorLevelSceneNode::RenderTransparent() {

		daidalosengine::IRenderer * renderer = daidalosengine::IRenderer::GetRenderer();
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, true);
		renderer->SetAlphaReference(0x00000001);
		renderer->SetAlphaFunction(daidalosengine::COMPARE_GREATEREQUAL);
		RenderWithOptions(SCENE_TRANSPARENT_ID);
		renderer->SetRenderState(daidalosengine::RENDER_STATE_TYPE_ALPHATEST, false);
	}

	void CEditorLevelSceneNode::RenderSkyboxMask() {
		RenderWithOptions(SCENE_SKYBOX_ID);
	}

	void CEditorLevelSceneNode::SetBrushCollision(const int brushId, const bool collision) {
		// Nothing to do
	}

	bool CEditorLevelSceneNode::Slide(daidalosengine::IShape3DF & shape, const daidalosengine::CVector3F & movement) {

		shape.SetPosition(shape.GetPosition() + movement);
		return false;
	}

	bool CEditorLevelSceneNode::GetCollisionPoint(const daidalosengine::CVector3F & position, const daidalosengine::CVector3F & movement, daidalosengine::CVector3F & point) {
		return false;
	}

	bool CEditorLevelSceneNode::GetCollisionPoint(daidalosengine::IShape3DF & shape, const daidalosengine::CVector3F & movement, daidalosengine::CVector3F & point) {
		return false;
	}

	void CEditorLevelSceneNode::RenderWithOptions(const int filterId) {
		if(filterId == SCENE_DEFAULT_ID) {
			ISceneNode::Render();
		}
	}
}
