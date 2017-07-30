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

#include <IL/il.h>
#include "../Core/Core.hpp"
#include "../Core/Memory/CExceptionLoad.hpp"
#include "../Core/Memory/CExceptionSave.hpp"
#include "../Core/Logger/ILogger.hpp"
#include "../Loaders/CImagesLoader.hpp"

namespace DE {
	CImagesLoader::CImagesLoader() {
		LOG_TRACE();

		ilInit();

		ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
		ilEnable(IL_ORIGIN_SET);

		ilEnable(IL_FILE_OVERWRITE);

		ilSetInteger(IL_FORMAT_MODE, IL_BGRA);
		ilEnable(IL_FORMAT_SET);
	}

	CImagesLoader::~CImagesLoader() {
		LOG_TRACE();

		ilShutDown();
	}

	CImage * CImagesLoader::LoadFromFile(const std::string & filename) {
		LOG_TRACE();

		ILuint texture;
		ilGenImages(1, &texture);
		ilBindImage(texture);

		if(!ilLoadImage(const_cast<ILstring>(filename.c_str()))) {
			ILenum devilError = ilGetError();
			LOG_ERROR("Unable to load the image '" << filename << "'");
			ILogger::Kill();

			throw CExceptionLoad(filename);
		}

		const CVector2I size = CVector2I(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
		const unsigned char * pixels = ilGetData();

		CImage * image = new CImage(size, PIXEL_FORMAT_A8R8G8B8, pixels);
		//image->Mirror();

		ilBindImage(0);
		ilDeleteImages(1, &texture);

		return image;
	}

	void CImagesLoader::SaveToFile(CImage * object, const std::string & filename) {
		LOG_TRACE();

		CImage image(object->GetSize(), PIXEL_FORMAT_A8R8G8B8);
		image.Copy(*object);

		image.Flip();

		ILuint texture;
		ilGenImages(1, &texture);
		ilBindImage(texture);

		const int width = image.GetWidth();
		const int height = image.GetHeight();

		if(!ilTexImage(width, height, 1, GetBytesPerPixel(image.GetFormat()), IL_BGRA, IL_UNSIGNED_BYTE, (void*)image.GetData())) {
			LOG_ERROR("Unable to save the file '" << filename << "'");
			ILogger::Kill();

			throw CExceptionSave(filename);
		}

		if(!ilSaveImage(const_cast<ILstring>(filename.c_str()))) {
			LOG_ERROR("Unable to save the file '" << filename << "'");
			ILogger::Kill();

			throw CExceptionSave(filename);
		}

		ilBindImage(0);
		ilDeleteImages(1, &texture);
	}
}