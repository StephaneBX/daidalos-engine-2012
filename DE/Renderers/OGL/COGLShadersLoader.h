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

#ifndef __COGLSHADERSLOADER_H
#define __COGLSHADERSLOADER_H

#include <string>
#include "../../Core/ILoader.h"
#include "../../Medias/IShaderBase.h"

namespace DE
{
	/**
	* Loader for OGL shaders files (.vs and .fs)
	*/
	class COGLShadersLoader : public ILoader<IShaderBase> 
	{
	public:
		/**
		* Constructor
		*/
		COGLShadersLoader();

		/**
		* Destructor
		*/
		~COGLShadersLoader();

		/**
		* Load an OGL shader from a file
		* @param filename The pixel shader filename
		* @return The shader
		*/
		IShaderBase * LoadFromFile(const std::string & filename);

	private:
		/**
		* Load the shader source code.
		* @param filename The shader filename.
		* @return The source code
		*/
		char * LoadSource(const std::string & filename) const;
	};
}

#endif
