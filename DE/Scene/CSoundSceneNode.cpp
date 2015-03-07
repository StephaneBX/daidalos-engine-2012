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

#include "../Scene/CSoundSceneNode.h"

namespace DE {
	CSoundSceneNode::CSoundSceneNode(const std::string & filename, ESoundType type, ISceneNode * parent, const std::string & name) : ISceneNode(parent, name) {
		this->m_sound.LoadFromFile(filename, type);
	}

	CSoundSceneNode::~CSoundSceneNode() {}

	void CSoundSceneNode::Update(const float time) {
		ISceneNode::Update(time);

		// Update the sound position and orientation
		this->m_sound.SetPosition(this->m_absolutePosition);
		if(this->m_sound.GetConeInnerAngle() < 360.0f || this->m_sound.GetConeOuterAngle() < 360.0f) {
			CVector3F lookAt(0, 0, this->m_sound.GetMaxDistance());
			CVector3F up(0, 1, 0);
			lookAt = this->m_absoluteRotationMatrix.Transform(lookAt);
			up = this->m_absoluteRotationMatrix.Transform(lookAt, true);

			float orientation[] = { lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z };
			this->m_sound.SetOrientation(orientation);
		}
		this->m_sound.Update();
	}

	void CSoundSceneNode::SetSoundStatus(ESoundStatus status) {
		this->m_sound.SetStatus(status);
	}

	void CSoundSceneNode::SetSoundGain(const float gain) {
		this->m_sound.SetGain(gain);
	}

	void CSoundSceneNode::SetSoundVelocity(const CVector3F & velocity) {
		this->m_sound.SetVelocity(velocity);
	}

	void CSoundSceneNode::SetSoundConeInnerAngle(const float angle) {
		this->m_sound.SetConeInnerAngle(angle);
	}

	void CSoundSceneNode::SetSoundConeOuterAngle(const float angle) {
		this->m_sound.SetConeOuterAngle(angle);
	}

	void CSoundSceneNode::SetSoundReferenceDistance(const float distance) {
		this->m_sound.SetReferenceDistance(distance);
	}

	void CSoundSceneNode::SetSoundMaxDistance(const float distance) {
		this->m_sound.SetMaxDistance(distance);
	}

	void CSoundSceneNode::SetSoundRelativeToListener(const bool relative) {
		this->m_sound.SetRelativeToListener(relative);
	}

	void CSoundSceneNode::SetSoundLooped(const bool loop) {
		this->m_sound.SetLooped(loop);
	}

	ESoundFormat CSoundSceneNode::GetSoundFormat() const {
		return this->m_sound.GetFormat();
	}

	ESoundStatus CSoundSceneNode::GetSoundStatus() const {
		return this->m_sound.GetStatus();
	}

	ESoundType CSoundSceneNode::GetSoundType() const {
		return this->m_sound.GetType();
	}

	const bool CSoundSceneNode::IsSoundLooped() const {
		return this->m_sound.IsLooped();
	}

	const int CSoundSceneNode::GetSoundChannelsCount() const {
		return this->m_sound.GetChannelsCount();
	}
}
