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
#include "../Medias/ISoundBase.h"

namespace DE
{
	ISoundBase::~ISoundBase()
	{
		alDeleteSources(1, &this->m_source);
	}

	void ISoundBase::SetStatus(ESoundStatus status)
	{
		switch(status)
		{
		case SOUND_STATUS_INITIAL:
			alSourceRewind(this->m_source);
			break;

		case SOUND_STATUS_STOP:
			alSourceStop(this->m_source);
			break;

		case SOUND_STATUS_PLAY:
			alSourcePlay(this->m_source);
			break;

		case SOUND_STATUS_PAUSE:
			alSourcePause(this->m_source);
			break;
		}
	}

	void ISoundBase::SetGain(const float gain)
	{
		alSourcef(this->m_source, AL_GAIN, gain);
	}

	void ISoundBase::SetPosition(const CVector3F & position)
	{
		CVector3F pos(position.x, position.y, -position.z);
		alSourcefv(this->m_source, AL_POSITION , pos);
	}

	void ISoundBase::SetOrientation(float * orientation)
	{
		float orientation2[] = { orientation[0], orientation[1], -orientation[2], orientation[3], orientation[4], -orientation[5] };
		alSourcefv(this->m_source, AL_DIRECTION, orientation2);
	}

	void ISoundBase::SetVelocity(const CVector3F & velocity)
	{
		alSourcefv(this->m_source, AL_VELOCITY, velocity);
	}

	void ISoundBase::SetConeInnerAngle(const float angle)
	{
		alSourcef(this->m_source, AL_CONE_INNER_ANGLE, angle);
	}

	void ISoundBase::SetConeOuterAngle(const float angle)
	{
		alSourcef(this->m_source, AL_CONE_OUTER_ANGLE, angle);
	}

	void ISoundBase::SetReferenceDistance(const float distance)
	{
		alSourcef(this->m_source, AL_REFERENCE_DISTANCE, distance);
	}

	void ISoundBase::SetMaxDistance(const float distance)
	{
		alSourcef(this->m_source, AL_MAX_DISTANCE , distance);
	}

	void ISoundBase::SetRelativeToListener(const bool relative)
	{
		alSourcei(this->m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
	}

	void ISoundBase::SetLooped(const bool loop)
	{
		alSourcei(this->m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	void ISoundBase::Update() {}

	ESoundFormat ISoundBase::GetFormat() const
	{
		return this->m_format;
	}

	ESoundStatus ISoundBase::GetStatus() const
	{
		ALint status;
		alGetSourcei(this->m_source, AL_SOURCE_STATE, &status);

		switch(status)
		{
		case AL_INITIAL:
			return SOUND_STATUS_INITIAL;

		case AL_STOPPED:
			return SOUND_STATUS_STOP;

		case AL_PLAYING:
			return SOUND_STATUS_PLAY;

		case AL_PAUSED:
			return SOUND_STATUS_PAUSE;
		}

		return SOUND_STATUS_STOP;
	}

	const bool ISoundBase::IsRelativeToListener() const
	{
		ALint status;
		alGetSourcei(this->m_source, AL_SOURCE_RELATIVE, &status);

		return status == AL_TRUE;
	}

	const bool ISoundBase::IsLooped() const
	{
		ALint status;
		alGetSourcei(this->m_source, AL_LOOPING, &status);

		return status == AL_TRUE;
	}

	const int ISoundBase::GetChannelsCount() const
	{
		int channels = 1;

		switch(this->m_format)
		{
		case SOUND_FORMAT_STEREO16:
			channels = 2;
			break;

		default:
			channels = 1;
			break;
		}

		return channels;
	}

	const float ISoundBase::GetGain() const
	{
		float result;

		alGetSourcef(this->m_source, AL_GAIN, &result);

		return result;
	}

	const float ISoundBase::GetConeInnerAngle() const
	{
		float result;

		alGetSourcef(this->m_source, AL_CONE_INNER_ANGLE, &result);

		return result;
	}

	const float ISoundBase::GetConeOuterAngle() const
	{
		float result;

		alGetSourcef(this->m_source, AL_CONE_OUTER_ANGLE, &result);

		return result;
	}

	const float ISoundBase::GetReferenceDistance() const
	{
		float result;

		alGetSourcef(this->m_source, AL_REFERENCE_DISTANCE, &result);

		return result;
	}

	const float ISoundBase::GetMaxDistance() const
	{
		float result;

		alGetSourcef(this->m_source, AL_MAX_DISTANCE, &result);

		return result;
	}

	ISoundBase::ISoundBase()
	{
		this->m_format = SOUND_FORMAT_MONO16;
		this->m_samplesCount = 0;
		this->m_sampleRate = 0;
		alGenSources(1, &this->m_source);
	}

	ALenum ISoundBase::GetALFormat() const
	{
		ALenum format;

		switch(this->m_format)
		{
		case SOUND_FORMAT_STEREO16:
			format = AL_FORMAT_STEREO16;
			break;

		default:
			format = AL_FORMAT_MONO16;
			break;
		}

		return format;
	}
}
