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

#include "../Scene/ITriggerableSceneNode.h"
#include "../Scene/CPlane3D.h"
#include "../Core/Memory/CExceptionAssert.h"

namespace DE
{
	ITriggerableSceneNode::ITriggerableSceneNode(ISceneNode * parent, const std::string & name) : ISceneNode(parent, name) 
	{
		this->m_triggerEnabled = false;
		this->m_triggered = false;
	}

	ITriggerableSceneNode::ITriggerableSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation) : ISceneNode(parent, name, position, rotation) 
	{
		this->m_triggerEnabled = false;
		this->m_triggered = false;
	}

	ITriggerableSceneNode::~ITriggerableSceneNode() {}

	const bool ITriggerableSceneNode::CheckTrigger(const IShape3DF * shape)
	{
		Assert(this->m_triggerShape != nullptr && shape != nullptr);

		return !this->m_triggered && this->m_triggerShape->IsCollision(shape);
	}

	const bool ITriggerableSceneNode::IsTriggerEnabled() const
	{
		return this->m_triggerEnabled;
	}

	const bool ITriggerableSceneNode::IsTriggered() const
	{
		return this->m_triggered;
	}

	const bool ITriggerableSceneNode::IsTriggerable() const
	{
		return true;
	}

	const bool ITriggerableSceneNode::IsCollidable() const
	{
		return true;
	}

	void ITriggerableSceneNode::Trigger()
	{
		if(this->m_triggerListeners.size() > 0)
		{
			for(auto it = this->m_triggerListeners.begin() ; it != this->m_triggerListeners.end() ; it++)
			{
				(*it)->Trigger();
			}
		}
	}

	void ITriggerableSceneNode::AddTriggerListener(ITriggerListener * listener)
	{
		this->m_triggerListeners.push_back(listener);
	}

	void ITriggerableSceneNode::RemoveTriggerListener(ITriggerListener * listener)
	{
		auto it = std::find(this->m_triggerListeners.begin(), this->m_triggerListeners.end(), listener);

		if(it != this->m_triggerListeners.end())
		{
			this->m_triggerListeners.erase(it);
		}
	}

	void ITriggerableSceneNode::SetTriggerEnabled(const bool enabled)
	{
		this->m_triggerEnabled = enabled;
	}

	void ITriggerableSceneNode::SetTriggered(const bool triggered)
	{
		this->m_triggered = triggered;
	}
}
