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

#ifndef __CDX9SHADER_HPP
#define __CDX9SHADER_HPP

#include <string>
#include <map>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../Medias/IShaderBase.hpp"

namespace daidalosengine {
	/**
	* DX9 class used for shaders
	*/
	class CDX9Shader : public IShaderBase {
	private:
		std::map<std::string, D3DXHANDLE> m_handles;
		ID3DXConstantTable * m_constantTable;

	public:
		/**
		* Constructor
		* @param name The shader name
		* @param constantTable The constant table
		*/
		CDX9Shader(const std::string & name, ID3DXConstantTable * constantTable);

		/**
		* Destructor
		*/
		virtual ~CDX9Shader();

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		*/
		virtual void SetParameter(const std::string & name, const bool value) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		*/
		virtual void SetParameter(const std::string & name, const int value) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		*/
		virtual void SetParameter(const std::string & name, const float value) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		virtual void SetParameter(const std::string & name, const bool * value, const int size) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		virtual void SetParameter(const std::string & name, const int * value, const int size) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param value The value
		* @param size The number of elements
		*/
		virtual void SetParameter(const std::string & name, const float * value, const int size) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		*/
		virtual void SetParameter(const std::string & name, const CVector2F & vec) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		*/
		virtual void SetParameter(const std::string & name, const CVector2I & vec) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		*/
		virtual void SetParameter(const std::string & name, const CVector3F & vec) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		*/
		virtual void SetParameter(const std::string & name, const CVector4F & vec) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param color The value
		*/
		virtual void SetParameter(const std::string & name, const CColor & color) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param mat The value
		*/
		virtual void SetParameter(const std::string & name, const CMatrix & mat) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param vec The value
		* @param size The number of elements
		*/
		virtual void SetParameter(const std::string & name, const CVector4F * vec, const int size) override;

		/**
		* Set a shader parameter
		* @param name The parameter name
		* @param mat The value
		* @param size The number of elements
		*/
		virtual void SetParameter(const std::string & name, const CMatrix * mat, const int size) override;

	protected:
		/**
		* Check the existence of the handle and create it if necessary
		* @param name The handle name
		*/
		void CheckHandle(const std::string & name);

	private:
		CDX9Shader();
		CDX9Shader(const CDX9Shader & copy);
		const CDX9Shader & operator=(const CDX9Shader & copy);
	};
}

#endif
