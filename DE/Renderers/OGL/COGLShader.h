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

#ifndef __COGLSHADER_H
#define __COGLSHADER_H

#include <string>
#include <map>
#include "../../Medias/IShaderBase.h"

namespace DE
{
	/**
	* OGL class used for shaders
	*/
	class COGLShader : public IShaderBase
	{
	private:
		std::map<std::string, int> m_variables;
		unsigned int m_shader;
		unsigned int m_associatedProgram;

	public:
		/**
		* Constructor
		* @param name The shader name
		* @param shader The shader id
		*/
		COGLShader(const std::string & name, const unsigned int shader);

		/**
		* Destructor
		*/
		~COGLShader();

		/**
		* Set the program id that the shader is associated to
		* @param program The program id
		*/
		void SetProgramId(const unsigned int program);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		*/
		void SetParameter(const std::string & name, const bool value);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		*/
		void SetParameter(const std::string & name, const int value);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		*/
		void SetParameter(const std::string & name, const float value);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		void SetParameter(const std::string & name, const bool * value, const int size);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		void SetParameter(const std::string & name, const int * value, const int size);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		void SetParameter(const std::string & name, const float * value, const int size);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		*/
		void SetParameter(const std::string & name, const CVector2F & vec);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		*/
		void SetParameter(const std::string & name, const CVector3F & vec);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		*/
		void SetParameter(const std::string & name, const CVector4F & vec);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param color The value
		*/
		void SetParameter(const std::string & name, const CColor & color);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param mat The value
		*/
		void SetParameter(const std::string & name, const CMatrix & mat);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		* @param size The number of elements
		*/
		void SetParameter(const std::string & name, const CVector4F * vec, const int size);

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param mat The value
		* @param size The number of elements
		*/
		void SetParameter(const std::string & name, const CMatrix * mat, const int size);

		/**
		* Clear the variables.
		*/
		void ClearVariables();

		/**
		* Get the vertex shader
		* @return The vertex shader
		*/
		const unsigned int GetShader() const;

	private:
		/**
		* Check the existence of the variable and create it if necessary
		* @param name The handle name
		*/
		void CheckVariable(const std::string & name);

	private:
		COGLShader();
		COGLShader(const COGLShader & copy);
		const COGLShader & operator=(const COGLShader & copy);
	};
}

#endif
