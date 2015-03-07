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

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "../Core/Logger/ILogger.h"
#include "../Core/CSoundContextManager.h"

namespace DE {
	CSoundContextManager::CSoundContextManager() {
		ALCdevice * device = alcOpenDevice(nullptr);
		if(!device) {
			LOG_ERROR("Unable to open the default sound device");
			return;
		}

		// Context creation
		ALCcontext * context = alcCreateContext(device, nullptr);
		if(!context) {
			LOG_ERROR("Unable to create the sound context");
			return;
		}

		// Context activation
		if(!alcMakeContextCurrent(context)) {
			LOG_ERROR("Unable to activate the sound context");
			return;
		}
	}

	CSoundContextManager::~CSoundContextManager() {
		ALCcontext * context = alcGetCurrentContext();
		ALCdevice *  device = alcGetContextsDevice(context);

		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void CSoundContextManager::SetListenerGain(const float gain) {
		alListenerf(AL_GAIN, gain);
	}

	void CSoundContextManager::SetListenerPosition(const CVector3F & position) {
		CVector3F pos(position.x, position.y, -position.z);
		alListenerfv(AL_POSITION, pos);
	}

	void CSoundContextManager::SetListenerOrientation(float * orientation) {
		float orientation2[] = { orientation[0], orientation[1], -orientation[2], orientation[3], orientation[4], -orientation[5] };
		alListenerfv(AL_ORIENTATION, orientation2);
	}

	void CSoundContextManager::SetListenerVelocity(const CVector3F & velocity) {
		alListenerfv(AL_VELOCITY, velocity);
	}
}
