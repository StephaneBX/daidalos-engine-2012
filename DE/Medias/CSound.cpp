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

#include <SiniP/SiniP.h>
#include "../Core/CMediaManager.hpp"
#include "../Core/Logger/ILogger.hpp"
#include "../Core/Memory/CExceptionAssert.hpp"
#include "../Core/CConfigurationManager.hpp"
#include "../Medias/IStaticSoundBase.hpp"
#include "../Medias/IStreamSoundBase.hpp"
#include "../Medias/CSound.hpp"

namespace daidalosengine {
	CSound::CSound() {}

	CSound::CSound(const std::string & filename, ESoundType type, ESoundCategory category) {
		LOG_TRACE();

		LoadFromFile(filename, type, category);
	}

	CSound::~CSound() {
		LOG_TRACE();
	}

	void CSound::LoadFromFile(const std::string & filename, ESoundType type, ESoundCategory category) {
		LOG_TRACE();

		this->m_type = type;
		this->m_category = category;

		// Load the sound
		switch(type) {
		case SOUND_TYPE_STATIC:
			this->m_sound = std::unique_ptr<IStaticSoundBase>(CMediaManager::GetInstance()->LoadMediaFromFile<IStaticSoundBase>(filename));
			break;

		case SOUND_TYPE_STREAM:
			this->m_sound = std::unique_ptr<IStreamSoundBase>(CMediaManager::GetInstance()->LoadMediaFromFile<IStreamSoundBase>(filename));
			break;
		}

		// Set the gain
		SiniP options = daidalosengine::CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);
		switch(category) {
		case SOUND_CATEGORY_MUSIC:
			this->m_sound->SetGain(options.GetValueFloat(APPLICATION_OPTION_SECTION_SOUND, APPLICATION_OPTION_SOUND_MUSIC, 1.0F));
			break;

		case SOUND_CATEGORY_ENVIRONMENT:
			this->m_sound->SetGain(options.GetValueFloat(APPLICATION_OPTION_SECTION_SOUND, APPLICATION_OPTION_SOUND_ENVIRONMENT, 1.0F));
			break;

		case SOUND_CATEGORY_DIALOGUE:
			this->m_sound->SetGain(options.GetValueFloat(APPLICATION_OPTION_SECTION_SOUND, APPLICATION_OPTION_SOUND_DIALOGUE, 1.0F));
			break;
		}
	}

	void CSound::SetStatus(ESoundStatus status) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetStatus(status);
	}

	void CSound::SetGain(const float gain) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetGain(gain);
	}

	void CSound::SetPosition(const CVector3F & position) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetPosition(position);
	}

	void CSound::SetOrientation(float * orientation) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetOrientation(orientation);
	}

	void CSound::SetVelocity(const CVector3F & velocity) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetVelocity(velocity);
	}

	void CSound::SetConeInnerAngle(const float angle) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetConeInnerAngle(angle);
	}

	void CSound::SetConeOuterAngle(const float angle) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetConeOuterAngle(angle);
	}

	void CSound::SetReferenceDistance(const float distance) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetReferenceDistance(distance);
	}

	void CSound::SetMaxDistance(const float distance) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetMaxDistance(distance);
	}

	void CSound::SetRelativeToListener(const bool relative) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetRelativeToListener(relative);
	}

	void CSound::SetLooped(const bool loop) {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->SetLooped(loop);
	}

	void CSound::Update() {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		this->m_sound->Update();
	}

	ESoundFormat CSound::GetFormat() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->GetFormat();
	}

	ESoundStatus CSound::GetStatus() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->GetStatus();
	}

	ESoundType CSound::GetType() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_type;
	}

	ESoundCategory CSound::GetCategory() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_category;
	}

	const bool CSound::IsRelativeToListener() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->IsRelativeToListener();
	}

	const bool CSound::IsLooped() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->IsLooped();
	}

	const int CSound::GetChannelsCount() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->GetChannelsCount();
	}

	const float CSound::GetGain() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->GetGain();
	}

	const float CSound::GetConeInnerAngle() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->GetConeInnerAngle();
	}

	const float CSound::GetConeOuterAngle() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->GetConeOuterAngle();
	}

	const float CSound::GetReferenceDistance() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->GetReferenceDistance();
	}

	const float CSound::GetMaxDistance() const {
		LOG_TRACE();

		Assert(this->m_sound != nullptr);
		return this->m_sound->GetMaxDistance();
	}

	ISoundBase * CSound::GetSound() const {
		LOG_TRACE();

		return this->m_sound.get();
	}
}
