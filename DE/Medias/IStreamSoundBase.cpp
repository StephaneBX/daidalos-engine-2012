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
#include "../Medias/IStreamSoundBase.hpp"

namespace daidalosengine {
	IStreamSoundBase::IStreamSoundBase(const std::string & name, SNDFILE * file, ESoundFormat format, const int samplesCount, const int sampleRate) : ISoundBase(name, format, samplesCount, sampleRate) {
		LOG_TRACE();

		this->m_framesRead = 0;
		this->m_file = file;

		alGenBuffers(STREAM_SOUND_BUFFER_SIZE, this->m_buffers);
		for(int i = 0; i < STREAM_SOUND_BUFFER_SIZE; ++i)
			ReadChunk(this->m_buffers[i]);
	}

	IStreamSoundBase::~IStreamSoundBase() {
		LOG_TRACE();

		ALint nbQueued;
		ALuint buffer;
		alGetSourcei(GetSource(), AL_BUFFERS_QUEUED, &nbQueued);
		for(ALint i = 0; i < nbQueued; ++i)
			alSourceUnqueueBuffers(GetSource(), 1, &buffer);

		sf_close(this->m_file);
		alDeleteBuffers(STREAM_SOUND_BUFFER_SIZE, this->m_buffers);
	}

	void IStreamSoundBase::SetStatus(ESoundStatus status) {
		LOG_TRACE();

		switch(status) {
		case SOUND_STATUS_STOP:
		case SOUND_STATUS_INITIAL:
			if(status == SOUND_STATUS_STOP)
				alSourceStop(GetSource());

			this->m_framesRead = 0;

			ALint nbQueued;
			ALuint buffer;
			alGetSourcei(GetSource(), AL_BUFFERS_QUEUED, &nbQueued);
			for(ALint i = 0; i < nbQueued; ++i)
				alSourceUnqueueBuffers(GetSource(), 1, &buffer);

			sf_seek(this->m_file, 0, SEEK_SET);

			for(int i = 0; i < STREAM_SOUND_BUFFER_SIZE; ++i)
				ReadChunk(this->m_buffers[i]);
			break;

		case SOUND_STATUS_PLAY:
			alSourcePlay(GetSource());
			break;

		case SOUND_STATUS_PAUSE:
			alSourcePause(GetSource());
			break;

		default:
			break;
		}
	}

	void IStreamSoundBase::Update() {
		LOG_TRACE();

		if(GetStatus() == SOUND_STATUS_PLAY) {
			ALint nbProcessed;
			int read = 0;
			alGetSourcei(GetSource(), AL_BUFFERS_PROCESSED, &nbProcessed);

			for(ALint i = 0; i < nbProcessed; ++i) {
				ALuint buffer;
				alSourceUnqueueBuffers(GetSource(), 1, &buffer);

				read = ReadChunk(buffer);
			}

			if(read == 0 && nbProcessed > 0)
				SetStatus(SOUND_STATUS_STOP);

		} else if(GetStatus() == SOUND_STATUS_STOP && IsLooped()) {
			SetStatus(SOUND_STATUS_STOP);
			SetStatus(SOUND_STATUS_PLAY);
		}
	}

	void IStreamSoundBase::SetLooped(const bool loop) {
		LOG_TRACE();

		this->m_looped = loop;
	}

	bool IStreamSoundBase::IsLooped() const {
		LOG_TRACE();

		return this->m_looped;
	}

	int IStreamSoundBase::ReadChunk(ALuint buffer) {
		LOG_TRACE();

		int nbSamples = GetSampleRate() * GetChannelsCount();
		int read = 0;
		std::vector<ALshort> samples(nbSamples);

		read = (int)sf_read_short(this->m_file, &samples[0], nbSamples);

		if(read > 0) {
			alBufferData(buffer, GetALFormat(), &samples[0], read * sizeof(ALushort), GetSampleRate());
			alSourceQueueBuffers(GetSource(), 1, &buffer);
			this->m_framesRead += read;
		}

		return read;
	}
}
