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

#include "../Scene/IBoundedLightSceneNode.h"
#include "../Scene/CSceneManager.h"

namespace DE {
	IBoundedLightSceneNode::~IBoundedLightSceneNode() {}

	void IBoundedLightSceneNode::SetRange(const float range) {
		this->m_range = range;
	}

	void IBoundedLightSceneNode::SetFalloff(const float falloff) {
		this->m_falloff = falloff;
	}

	void IBoundedLightSceneNode::Update(const float time) {
		CSceneManager * scene = CSceneManager::GetInstance();
		ISceneNode::Update(time);

		// Update the activation shape
		this->m_visibilityShape->SetPosition(this->m_absolutePosition);

		CVector3F radius = this->m_absoluteScaleMatrix.Transform(CVector3F(this->m_range, this->m_range, this->m_range));
		this->m_visibilityShape->SetRadius(std::max(radius.x, std::max(radius.y, radius.z)));

		// Get the light scissor
		this->m_scissor = IRenderer::GetRenderer()->GetLightScissor(CVector4F(scene->GetActiveCamera()->GetViewMatrix().Transform(this->m_visibilityShape->GetPosition()), this->m_visibilityShape->GetInnerDepth()));

		// Activation check (frustum + scissor test)
		this->m_active = !scene->GetActiveCamera()->FrustumCull(this->m_visibilityShape.get()) && (this->m_scissor.GetWidth() * this->m_scissor.GetHeight() > 0);
	}

	const float IBoundedLightSceneNode::GetRange() const {
		return this->m_range;
	}

	const float IBoundedLightSceneNode::GetFalloff() const {
		return this->m_falloff;
	}

	const CRectangleI & IBoundedLightSceneNode::GetScissor() const {
		return this->m_scissor;
	}

	CSphereF * IBoundedLightSceneNode::GetVisibilitySphere() const {
		return this->m_visibilityShape.get();
	}

	IBoundedLightSceneNode::IBoundedLightSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation, const CColor & color, const bool shadowCaster, const float range, const float falloff, const bool enabled) : ILightSceneNode(parent, name, position, rotation, color, shadowCaster, enabled) {
		SetRange(range);
		SetFalloff(falloff);

		this->m_visibilityShape = std::unique_ptr<CSphereF>(new CSphereF(position, range));
	}
}
