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

#ifndef __ISHADERPROGRAMBASE_HPP
#define __ISHADERPROGRAMBASE_HPP

#include <string>
#include "../Core/Enums.hpp"
#include "../Medias/IShaderBase.hpp"
#include "../Core/IResource.hpp"

namespace daidalosengine {
	/**
	* Basic class used for shader programs
	*/
	class IShaderProgramBase : public IResource {
	private:
		IShaderBase * m_vertexShader;
		IShaderBase * m_pixelShader;

	public:
		/**
		* Constructor
		* @param name The shader program base name
		*/
		explicit IShaderProgramBase(const std::string & name);

		/**
		* Destructor
		*/
		virtual ~IShaderProgramBase();

		/**
		* Set the shader
		* @param name The shader base name
		*/
		void SetShader(const std::string & name);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param value The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const bool value);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param value The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const int value);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param value The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const float value);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const bool * value, const int size);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const int * value, const int size);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const float * value, const int size);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param vec The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector2F & vec);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param vec The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector2I & vec);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param vec The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector3F & vec);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param vec The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector4F & vec);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param color The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const CColor & color);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param mat The value
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const CMatrix & mat);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param vec The value
		* @param size The number of elements
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const CVector4F * vec, const int size);

		/**
		* Set a shader parameter
		* @param flags The flags for the parameter target(s)
		* @param name The parameter name
		* @param mat The value
		* @param size The number of elements
		*/
		void SetParameter(const EShaderProgramFlag flags, const std::string & name, const CMatrix * mat, const int size);

		/**
		* Get the vertex shader
		* @return The vertex shader
		*/
		IShaderBase * GetVertexShader() const;

		/**
		* Get the pixel shader
		* @return The pixel shader
		*/
		IShaderBase * GetPixelShader() const;

	private:
		/**
		* Load a shader
		* @param shader The shader
		* @param name The name
		*/
		void Load(IShaderBase *& shader, const std::string & name);

	public:
		IShaderProgramBase() = delete;
		IShaderProgramBase(const IShaderProgramBase & copy) = delete;
		const IShaderProgramBase & operator=(const IShaderProgramBase & copy) = delete;
	};
}

#endif
