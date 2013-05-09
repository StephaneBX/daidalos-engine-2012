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

#ifndef __COGLSHADERPROGRAM_H
#define __COGLSHADERPROGRAM_H

#include <string>
#include "../../Medias/IShaderProgramBase.h"

namespace DE
{
	/**
	* OGL class used for shader programs
	*/
	class COGLShaderProgram : public IShaderProgramBase
	{
	private:
		int m_samplers[MAXIMUM_TEXTURE_UNIT];
		unsigned int m_programId;

	public:
		/**
		* Constructor
		* @param name The shader base name
		*/
		COGLShaderProgram(const std::string & name);

		/**
		* Set the shader
		* @param name The shader base name
		*/
		void SetShader(const std::string & name);

		/**
		* Get the shader program id
		* @return The shader program id
		*/
		const unsigned int GetProgramId() const;

		/**
		* Get the samplers
		* @return The samplers
		*/
		int * GetSamplers();

	private:
		/**
		* Link the shaders to the shader program
		*/
		void LinkProgram();

		/**
		* Check the program status.
		* @param type The type of check
		* @param oglFunction The OGL function name
		*/
		void CheckProgramStatus(const int type, const std::string & oglFunction);

		COGLShaderProgram();
		COGLShaderProgram(const COGLShaderProgram & copy);
		const COGLShaderProgram & operator=(const COGLShaderProgram & copy);
	};
}

#endif
