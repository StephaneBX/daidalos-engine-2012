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

#include "../Core/CMediaManager.h"
#include "../Core/Memory/CExceptionAssert.h"
#include "../Medias/IStaticSoundBase.h"
#include "../Medias/IStreamSoundBase.h"
#include "../Medias/CSound.h"

namespace DE
{
	CSound::CSound() {}

	CSound::CSound(const std::string & filename, ESoundType type)
	{
		LoadFromFile(filename, type);
	}

	CSound::~CSound() {}

	void CSound::LoadFromFile(const std::string & filename, ESoundType type)
	{
		this->m_type = type;
		switch(type)
		{
		case SOUND_TYPE_STATIC:
			this->m_sound = std::unique_ptr<IStaticSoundBase>(CMediaManager::GetInstance()->LoadMediaFromFile<IStaticSoundBase>(filename));
			break;

		case SOUND_TYPE_STREAM:
			this->m_sound = std::unique_ptr<IStreamSoundBase>(CMediaManager::GetInstance()->LoadMediaFromFile<IStreamSoundBase>(filename));
			break;
		}
	}

	void CSound::SetStatus(ESoundStatus status)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetStatus(status);
	}

	void CSound::SetGain(const float gain)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetGain(gain);
	}

	void CSound::SetPosition(const CVector3F & position)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetPosition(position);
	}

	void CSound::SetOrientation(float * orientation)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetOrientation(orientation);
	}

	void CSound::SetVelocity(const CVector3F & velocity)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetVelocity(velocity);
	}

	void CSound::SetConeInnerAngle(const float angle)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetConeInnerAngle(angle);
	}

	void CSound::SetConeOuterAngle(const float angle)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetConeOuterAngle(angle);
	}

	void CSound::SetReferenceDistance(const float distance)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetReferenceDistance(distance);
	}

	void CSound::SetMaxDistance(const float distance)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetMaxDistance(distance);
	}

	void CSound::SetRelativeToListener(const bool relative)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetRelativeToListener(relative);
	}

	void CSound::SetLooped(const bool loop)
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->SetLooped(loop);
	}

	void CSound::Update()
	{
		Assert(this->m_sound != nullptr);
		this->m_sound->Update();
	}

	ESoundFormat CSound::GetFormat() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->GetFormat();
	}

	ESoundStatus CSound::GetStatus() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->GetStatus();
	}
		
	ESoundType CSound::GetType() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_type;
	}

	const bool CSound::IsRelativeToListener() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->IsRelativeToListener();
	}

	const bool CSound::IsLooped() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->IsLooped();
	}

	const int CSound::GetChannelsCount() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->GetChannelsCount();
	}

	const float CSound::GetGain() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->GetGain();
	}

	const float CSound::GetConeInnerAngle() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->GetConeInnerAngle();
	}

	const float CSound::GetConeOuterAngle() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->GetConeOuterAngle();
	}

	const float CSound::GetReferenceDistance() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->GetReferenceDistance();
	}

	const float CSound::GetMaxDistance() const
	{
		Assert(this->m_sound != nullptr);
		return this->m_sound->GetMaxDistance();
	}

	ISoundBase * CSound::GetSound() const
	{
		return this->m_sound.get();
	}
}
