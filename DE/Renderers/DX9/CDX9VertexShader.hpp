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

#ifndef __CDX9VERTEXSHADER_HPP
#define __CDX9VERTEXSHADER_HPP

#include <string>
#include <map>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../Renderers/DX9/CDX9Shader.hpp"

namespace DE {
	/**
	* DX9 class used for shaders
	*/
	class CDX9VertexShader : public CDX9Shader {
	private:
		IDirect3DVertexShader9 * m_vertexShader;

	public:
		/**
		* Constructor
		* @param name The shader name
		* @param vertexShader The vertex shader
		* @param constantTable The constant table
		*/
		CDX9VertexShader(const std::string & name, IDirect3DVertexShader9 * vertexShader, ID3DXConstantTable * constantTable);

		/**
		* Destructor
		*/
		~CDX9VertexShader();

		/**
		* Get the vertex shader
		* @return The vertex shader
		*/
		IDirect3DVertexShader9 * GetVertexShader() const;

	private:
		CDX9VertexShader();
		CDX9VertexShader(const CDX9VertexShader & copy);
		const CDX9VertexShader & operator=(const CDX9VertexShader & copy);
	};
}

#endif
