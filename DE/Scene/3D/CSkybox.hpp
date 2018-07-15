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

#ifndef __CSKYBOX_HPP
#define __CSKYBOX_HPP

#include <array>
#include "../../Medias/CTexture2D.hpp"
#include "../../Medias/CBuffer.hpp"
#include "../../Core/IDeclaration.hpp"
#include "../../Medias/CShaderProgram.hpp"

#define SKYBOX_FACES 6

namespace daidalosengine {
	class I3DScene;

	/**
	* Skybox class.
	*/
	class CSkybox {
	private:
		bool m_visible;
		I3DScene * m_scene;
		std::array<CTexture2D, SKYBOX_FACE_SIZE> m_textures;
		std::array<daidalosengine::CBuffer, SKYBOX_FACE_SIZE> m_buffers;
		CBuffer m_indexBuffer;
		DeclarationPtr m_vertexDeclaration;
		CShaderProgram m_shaderProgram;

	public:
		/**
		* Constructor
		*/
		CSkybox();

		/**
		* Constructor
		* @param name The start name without the extension and the face
		*/
		explicit CSkybox(const std::string & name);

		/**
		* Set the associated scene
		* @param scene The associated scene
		*/
		void SetScene(I3DScene * scene);

		/**
		* Set the skybox texture name
		* @param name The start name without the extension and the face
		*/
		void SetName(const std::string & name);

		/**
		* Render the skybox
		*/
		void Render();

		/**
		* If the skybox is visible
		* @return True is the skybox is visible and can be rendered
		*/
		bool IsVisible() const;

	private:
		/**
		* Create the necessary geometry
		*/
		void CreateGeometry();
	};
}

#endif
