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

#include "../Core/Logger/ILogger.hpp"
#include "../Medias/ISoundBase.hpp"

namespace daidalosengine {
	/**
	* Destructor
	*/
	ISoundBase::~ISoundBase() {
		LOG_TRACE();

		alDeleteSources(1, &this->m_source);
	}

	/**
	* Set the sound status
	* @param status The status
	*/
	void ISoundBase::SetStatus(ESoundStatus status) {
		LOG_TRACE();

		switch(status) {
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

		default:
			break;
		}
	}

	/**
	* Set the gain (volume)
	* @param gain The gain between 0.0 and 1.0
	*/
	void ISoundBase::SetGain(const float gain) {
		LOG_TRACE();

		alSourcef(this->m_source, AL_GAIN, gain);
	}

	/**
	* Set the position
	* @param position The position
	*/
	void ISoundBase::SetPosition(const CVector3F & position) {
		LOG_TRACE();

		CVector3F pos(position.x, position.y, -position.z);
		alSourcefv(this->m_source, AL_POSITION, pos);
	}

	/**
	* Set the orientation
	* @param orientation A 6 float array for the at and the up vectors
	*/
	void ISoundBase::SetOrientation(float * orientation) {
		LOG_TRACE();

		float orientation2[] = { orientation[0], orientation[1], -orientation[2], orientation[3], orientation[4], -orientation[5] };
		alSourcefv(this->m_source, AL_DIRECTION, orientation2);
	}

	/**
	* Set the velocity
	* @param velocity The velocity
	*/
	void ISoundBase::SetVelocity(const CVector3F & velocity) {
		LOG_TRACE();

		alSourcefv(this->m_source, AL_VELOCITY, velocity);
	}

	/**
	* Set the cone inner angle
	* @param angle The cone angle
	*/
	void ISoundBase::SetConeInnerAngle(const float angle) {
		LOG_TRACE();

		alSourcef(this->m_source, AL_CONE_INNER_ANGLE, angle);
	}

	/**
	* Set the cone outer angle
	* @param angle The cone angle
	*/
	void ISoundBase::SetConeOuterAngle(const float angle) {
		LOG_TRACE();

		alSourcef(this->m_source, AL_CONE_OUTER_ANGLE, angle);
	}

	/**
	* Set the distance from wich the gain will be maximal
	* @param distance The distance
	*/
	void ISoundBase::SetReferenceDistance(const float distance) {
		LOG_TRACE();

		alSourcef(this->m_source, AL_REFERENCE_DISTANCE, distance);
	}

	/**
	* Set the distance from wich the gain will be minimal
	* @param distance The distance
	*/
	void ISoundBase::SetMaxDistance(const float distance) {
		LOG_TRACE();

		alSourcef(this->m_source, AL_MAX_DISTANCE, distance);
	}

	/**
	* Set the position relative to the listener
	* @param relative The relative value
	*/
	void ISoundBase::SetRelativeToListener(const bool relative) {
		LOG_TRACE();

		alSourcei(this->m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
	}

	/**
	* Set the sound as looped
	* @param loop The loop value
	*/
	void ISoundBase::SetLooped(const bool loop) {
		LOG_TRACE();

		alSourcei(this->m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	/**
	* Update a sound
	*/
	void ISoundBase::Update() {
		LOG_TRACE();
	}

	/**
	* Get the format
	* @return The format
	*/
	ESoundFormat ISoundBase::GetFormat() const {
		LOG_TRACE();

		return this->m_format;
	}

	/**
	* Get the status
	* @return The status
	*/
	ESoundStatus ISoundBase::GetStatus() const {
		LOG_TRACE();

		ALint status;
		alGetSourcei(this->m_source, AL_SOURCE_STATE, &status);

		switch(status) {
		case AL_INITIAL:
			return SOUND_STATUS_INITIAL;

		case AL_STOPPED:
			return SOUND_STATUS_STOP;

		case AL_PLAYING:
			return SOUND_STATUS_PLAY;

		case AL_PAUSED:
			return SOUND_STATUS_PAUSE;

		default:
			return SOUND_STATUS_UNKNOWN;
		}

		return SOUND_STATUS_STOP;
	}

	/**
	* Check if the position is relative to the listener
	* @return The relative position state
	*/
	const bool ISoundBase::IsRelativeToListener() const {
		LOG_TRACE();

		ALint status;
		alGetSourcei(this->m_source, AL_SOURCE_RELATIVE, &status);

		return status == AL_TRUE;
	}

	/**
	* Get the looped status
	* @return The looped status
	*/
	const bool ISoundBase::IsLooped() const {
		LOG_TRACE();

		ALint status;
		alGetSourcei(this->m_source, AL_LOOPING, &status);

		return status == AL_TRUE;
	}

	/**
	* Get the source
	* @return The source
	*/
	const ALuint ISoundBase::GetSource() const {
		LOG_TRACE();

		return this->m_source;
	}

	/**
	* Get the number of channels
	* @return The number of channels
	*/
	const int ISoundBase::GetChannelsCount() const {
		LOG_TRACE();

		int channels = 1;

		switch (this->m_format) {
			case SOUND_FORMAT_STEREO16:
				channels = 2;
				break;

			default:
				channels = 1;
				break;
		}

		return channels;
	}

	/**
	* Get the samples count
	* @return The samples count
	*/
	const int ISoundBase::GetSamplesCount() const {
		LOG_TRACE();

		return this->m_samplesCount;
	}

	/**
	* Get the sample rate
	* @return The sample rate
	*/
	const int ISoundBase::GetSampleRate() const {
		LOG_TRACE();

		return this->m_sampleRate;
	}

	/**
	* Get the gain
	* @return The gain
	*/
	const float ISoundBase::GetGain() const {
		LOG_TRACE();

		float result;

		alGetSourcef(this->m_source, AL_GAIN, &result);

		return result;
	}

	/**
	* Get the cone inner angle
	* @return The cone inner angle
	*/
	const float ISoundBase::GetConeInnerAngle() const {
		LOG_TRACE();

		float result;

		alGetSourcef(this->m_source, AL_CONE_INNER_ANGLE, &result);

		return result;
	}

	/**
	* Get the cone outer angle
	* @return The cone outer angle
	*/
	const float ISoundBase::GetConeOuterAngle() const {
		LOG_TRACE();

		float result;

		alGetSourcef(this->m_source, AL_CONE_OUTER_ANGLE, &result);

		return result;
	}

	/**
	* Get the distance from wich the gain will be maximal
	* @return The distance from wich the gain will be maximal
	*/
	const float ISoundBase::GetReferenceDistance() const {
		LOG_TRACE();

		float result;

		alGetSourcef(this->m_source, AL_REFERENCE_DISTANCE, &result);

		return result;
	}

	/**
	* Get the distance from wich the gain will be minimal
	* @return The distance from wich the gain will be minimal
	*/
	const float ISoundBase::GetMaxDistance() const {
		LOG_TRACE();

		float result;

		alGetSourcef(this->m_source, AL_MAX_DISTANCE, &result);

		return result;
	}

	/**
	* Constructor
	* @param name The filename
	* @param format The sound format
	* @param samplesCount The samples count
	* @param sampleRate The sample rate
	*/
	ISoundBase::ISoundBase(const std::string & name, ESoundFormat format, const int samplesCount, const int sampleRate) : IResource(name) {
		LOG_TRACE();

		this->m_format = format;
		this->m_samplesCount = samplesCount;
		this->m_sampleRate = sampleRate;
		alGenSources(1, &this->m_source);
	}

	/**
	* Get the OpenAL sound format
	* @return The format
	*/
	ALenum ISoundBase::GetALFormat() const {
		LOG_TRACE();

		ALenum format;

		switch(this->m_format) {
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
