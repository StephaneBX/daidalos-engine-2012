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

#ifndef __ISTREAMSOUNDBASE_HPP
#define __ISTREAMSOUNDBASE_HPP

#include <libsndfile/sndfile.h>
#include "../Medias/ISoundBase.hpp"

#define STREAM_SOUND_BUFFER_SIZE 4

namespace DE {
	/**
	* Basic class for stream sounds
	*/
	class IStreamSoundBase : public ISoundBase {
	private:
		bool m_looped;
		int m_framesRead;
		ALuint m_buffers[STREAM_SOUND_BUFFER_SIZE];
		SNDFILE * m_file;

	public:
		/**
		* Constructor
		* @param name The file name
		* @param file The sound file
		* @param format The format
		* @param samplesCount The number of samples
		* @param sampleRate The sample rate
		*/
		IStreamSoundBase(const std::string & name, SNDFILE * file, ESoundFormat format, const int samplesCount, const int sampleRate);

		/**
		* Destructor
		*/
		~IStreamSoundBase();

		/**
		* Set the sound status
		* @param status The status
		*/
		void SetStatus(ESoundStatus status);

		/**
		* Update a sound
		*/
		void Update();

		/**
		* Set the sound as looped
		* @param loop The loop value
		*/
		void SetLooped(const bool loop);

		/**
		* Get the looped status
		* @return The looped status
		*/
		const bool IsLooped() const;

	private:
		/**
		* Read a chunk of data
		* @param buffer The buffer to put the data into
		*/
		const int ReadChunk(ALuint buffer);

	private:
		IStreamSoundBase();
		IStreamSoundBase(const IStreamSoundBase & copy);
		const IStreamSoundBase & operator=(const IStreamSoundBase & copy);
	};
}

#endif
