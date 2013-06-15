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

#include <libsndfile/sndfile.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "../Core/Enums.h"
#include "../Core/Memory/CExceptionLoad.h"
#include "../Core/Logger/ILogger.h"
#include "../Loaders/CStreamSoundLoader.h"

namespace DE
{
	CStreamSoundLoader::CStreamSoundLoader() {}

	CStreamSoundLoader::~CStreamSoundLoader() {}

	IStreamSoundBase * CStreamSoundLoader::LoadFromFile(const std::string & filename)
	{
		SF_INFO fileInfos;
		SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &fileInfos);

		// Open the file
		if(!file)
		{
			ILogger::Log() << "DE:CStreamSoundLoader Error: Unable to load the sound '" << filename << "'.\n";
			ILogger::Kill();

			throw CExceptionLoad(filename);
		}

		// Get the sound information
		IStreamSoundBase * sound = nullptr;
		int nbSamples = static_cast<int>(fileInfos.channels * fileInfos.frames);
		int sampleRate = fileInfos.samplerate;
		ESoundFormat format;
		ALenum openALFormat;

		switch(fileInfos.channels)
		{
		case 1:
			format = SOUND_FORMAT_MONO16;
			openALFormat = AL_FORMAT_MONO16;
			break;

		case 2:
			format = SOUND_FORMAT_STEREO16;
			openALFormat = AL_FORMAT_STEREO16;
			break;

		default:
			ILogger::Log() << "DE:CStreamSoundLoader Error: Incompatible file '" << filename << "'. Only files with 1 or 2 channels are accepted.\n";
			ILogger::Kill();

			throw CExceptionLoad(filename);
		}

		return new IStreamSoundBase(file, format, nbSamples, sampleRate);
	}
}
