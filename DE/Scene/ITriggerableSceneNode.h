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

#ifndef __ITRIGGERABLESCENENODE_H
#define __ITRIGGERABLESCENENODE_H

#include <vector>
#include "../Core/Enums.h"
#include "../Scene/ISceneNode.h"
#include "../Scene/IShape3D.h"
#include "../Scene/ITriggerListener.h"

namespace DE
{
	/**
	* Basic class for trigger nodes
	*/
	class ITriggerableSceneNode : public ISceneNode
	{
	protected:
		bool m_triggerEnabled;
		bool m_triggered;
		std::unique_ptr<IShape3DF> m_triggerShape;
		std::vector<ITriggerListener*> m_triggerListeners;
		
	public:
		/**
		* Constructor
		* @param parent The parent
		* @param name The node name
		*/
		ITriggerableSceneNode(ISceneNode * parent = nullptr, const std::string & name = "");

		/**
		* Constructor
		* @param parent The parent
		* @param name The node name
		* @param position The local position
		* @param rotation The rotation
		* @param scale The scale
		*/
		ITriggerableSceneNode(ISceneNode * parent, const std::string & name, const CVector3F & position, const CVector3F & rotation);

		/**
		* Destructor
		*/
		virtual ~ITriggerableSceneNode();

		/**
		* Check if a given shape triggers the node
		* @param shape The shape to test with
		* @return True if the node can be triggered
		*/
		const bool CheckTrigger(const IShape3DF * shape);

		/**
		* Check if the trigger is enabled
		* @return True if the trigger is enabled
		*/
		const bool IsTriggerEnabled() const;

		/**
		* Check if the trigger activated
		* @return True if the trigger is activated
		*/
		const bool IsTriggered() const;

		/**
		* Check if the node is triggerable
		* @return True if the node is triggerable
		*/
		const bool IsTriggerable() const;

		/**
		* Check if the node is collidable
		*/
		const bool IsCollidable() const;

		/**
		* Trigger the node
		*/
		void Trigger();

		/**
		* Add a trigger listener
		* @param listener The listener to add
		*/
		void AddTriggerListener(ITriggerListener * listener);

		/**
		* Remove a trigger listener
		* @param listener The listener to remove
		*/
		void RemoveTriggerListener(ITriggerListener * listener);

		/**
		* Set the enabled state
		* @param enabled The trigger state
		*/
		void SetTriggerEnabled(const bool enabled);

		/**
		* Set the activated state
		* @param triggered The trigger activation state
		*/
		void SetTriggered(const bool triggered);
	};
}

#endif
