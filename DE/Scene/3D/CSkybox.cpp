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

#include <sstream>
#include "../../Scene/3D/CSkybox.h"
#include "../../Core/IRenderer.h"
#include "../../Core/Logger/ILogger.h"
#include "../../Scene/3D/I3DScene.h"

namespace DE {

	#define SIZE_FACTOR 1.5f
	#define SHADER_NAME "skybox"

	/**
	* Skybox element
	*/
	struct SSkyboxElement {
		CVector3F vertex;
		CVector2F texcoord;
	};

	/**
	* Constructor
	*/
	CSkybox::CSkybox() {
		LOG_TRACE();

		this->m_scene = nullptr;
		this->m_visible = false;
		CreateGeometry();
	}

	/**
	* Constructor
	* @param name The start name without the extension and the face
	*/
	CSkybox::CSkybox(const std::string & name) {
		LOG_TRACE();

		this->m_scene = nullptr;
		this->m_visible = true;
		SetName(name);
		CreateGeometry();
	}

	void CSkybox::SetScene(I3DScene * scene) {
		LOG_TRACE();

		this->m_scene = scene;
	}

	/**
	* Set the skybox texture name
	* @param name The start name without the extension and the face
	*/
	void CSkybox::SetName(const std::string & name) {
		LOG_TRACE();

		std::ostringstream ss;

		ss << name << "_front.png";
		this->m_textures[SKYBOX_FACE_FRONT].CreateFromFile(ss.str(), PIXEL_FORMAT_A8R8G8B8);

		ss.str("");
		ss << name << "_right.png";
		this->m_textures[SKYBOX_FACE_RIGHT].CreateFromFile(ss.str(), PIXEL_FORMAT_A8R8G8B8);

		ss.str("");
		ss << name << "_back.png";
		this->m_textures[SKYBOX_FACE_BACK].CreateFromFile(ss.str(), PIXEL_FORMAT_A8R8G8B8);

		ss.str("");
		ss << name << "_left.png";
		this->m_textures[SKYBOX_FACE_LEFT].CreateFromFile(ss.str(), PIXEL_FORMAT_A8R8G8B8);

		ss.str("");
		ss << name << "_up.png";
		this->m_textures[SKYBOX_FACE_UP].CreateFromFile(ss.str(), PIXEL_FORMAT_A8R8G8B8);

		ss.str("");
		ss << name << "_down.png";
		this->m_textures[SKYBOX_FACE_DOWN].CreateFromFile(ss.str(), PIXEL_FORMAT_A8R8G8B8);

		this->m_visible = true;
	}

	/**
	* Render the skybox
	*/
	void CSkybox::Render() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();

		// Disable writing to the z-buffer
		renderer->SetRenderState(RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, false);

		// Set the renderer
		renderer->SetVertexDeclaration(this->m_vertexDeclaration.get());
		renderer->SetShaderProgram(this->m_shaderProgram.GetShaderProgram());
		renderer->SetIndexBuffer(this->m_indexBuffer, sizeof(int));

		// Shader values to reposition the skybox and change its size
		this->m_shaderProgram.SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SKYBOX_POSITION), this->m_scene->GetActiveCamera()->GetAbsolutePosition());
		this->m_shaderProgram.SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_SKYBOX_SIZE), (this->m_scene->GetActiveCamera()->GetNearDistance() * SIZE_FACTOR));
		this->m_shaderProgram.SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_VIEWPROJ_MATRIX), this->m_scene->GetActiveCamera()->GetViewProjMatrix());

		// Draw each part of the skybox
		for(int i = 0; i < SKYBOX_FACE_SIZE; i++) {
			renderer->SetVertexBuffer(0, this->m_buffers.at(i), sizeof(DE::SSkyboxElement), 0, 4);
			renderer->SetTexture(0, this->m_textures.at(i).GetTexture());
			renderer->DrawIndexedPrimitives(DE::PRIMITIVE_TYPE_TRIANGLE_LIST, 0, 2);
		}

		// Enable back writing to the z-buffer
		renderer->SetRenderState(RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE, true);
	}

	/**
	* If the skybox is visible
	* @return True is the skybox is visible and can be rendered
	*/
	const bool CSkybox::IsVisible() const {
		LOG_TRACE();

		return this->m_visible;
	}

	/**
	* Create the necessary geometry
	*/
	void CSkybox::CreateGeometry() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();

		// Create the buffers
		SSkyboxElement front[] =
		{
			{ CVector3F(-1, 1, 1), CVector2F(0, 0) },
			{ CVector3F(1, 1, 1), CVector2F(1, 0) },
			{ CVector3F(1, -1, 1), CVector2F(1, 1) },
			{ CVector3F(-1, -1, 1), CVector2F(0, 1) }
		};
		this->m_buffers[SKYBOX_FACE_FRONT] = renderer->CreateVertexBuffer(sizeof(SSkyboxElement), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, front);

		SSkyboxElement right[] =
		{
			{ CVector3F(1, 1, 1), CVector2F(0, 0) },
			{ CVector3F(1, 1, -1), CVector2F(1, 0) },
			{ CVector3F(1, -1, -1), CVector2F(1, 1) },
			{ CVector3F(1, -1, 1), CVector2F(0, 1) }
		};
		this->m_buffers[SKYBOX_FACE_RIGHT] = renderer->CreateVertexBuffer(sizeof(SSkyboxElement), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, right);

		SSkyboxElement back[] =
		{
			{ CVector3F(1, 1, -1), CVector2F(0, 0) },
			{ CVector3F(-1, 1, -1), CVector2F(1, 0) },
			{ CVector3F(-1, -1, -1), CVector2F(1, 1) },
			{ CVector3F(1, -1, -1), CVector2F(0, 1) }
		};
		this->m_buffers[SKYBOX_FACE_BACK] = renderer->CreateVertexBuffer(sizeof(SSkyboxElement), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, back);

		SSkyboxElement left[] =
		{
			{ CVector3F(-1, 1, -1), CVector2F(0, 0) },
			{ CVector3F(-1, 1, 1), CVector2F(1, 0) },
			{ CVector3F(-1, -1, 1), CVector2F(1, 1) },
			{ CVector3F(-1, -1, -1), CVector2F(0, 1) }
		};
		this->m_buffers[SKYBOX_FACE_LEFT] = renderer->CreateVertexBuffer(sizeof(SSkyboxElement), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, left);

		SSkyboxElement up[] =
		{
			{ CVector3F(-1, 1, -1), CVector2F(0, 0) },
			{ CVector3F(1, 1, -1), CVector2F(1, 0) },
			{ CVector3F(1, 1, 1), CVector2F(1, 1) },
			{ CVector3F(-1, 1, 1), CVector2F(0, 1) }
		};
		this->m_buffers[SKYBOX_FACE_UP] = renderer->CreateVertexBuffer(sizeof(SSkyboxElement), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, up);

		SSkyboxElement down[] =
		{
			{ CVector3F(-1, -1, 1), CVector2F(0, 0) },
			{ CVector3F(1, -1, 1), CVector2F(1, 0) },
			{ CVector3F(1, -1, -1), CVector2F(1, 1) },
			{ CVector3F(-1, -1, -1), CVector2F(0, 1) }
		};
		this->m_buffers[SKYBOX_FACE_DOWN] = renderer->CreateVertexBuffer(sizeof(SSkyboxElement), 4, DE::BUFFER_DYNAMIC | DE::BUFFER_WRITEONLY, down);

		// Create the vertex declaration
		DE::SDeclarationElement declaration[] =
		{
			{ 0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3 },
			{ 0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2 }
		};
		this->m_vertexDeclaration = renderer->CreateVertexDeclaration(declaration);

		// Create the index buffer
		int indices[] = { 0, 1, 2, 2, 3, 0 };
		this->m_indexBuffer = renderer->CreateIndexBuffer(6, 0, indices);

		// Load the shader program
		this->m_shaderProgram.LoadFromFile(SHADER_NAME);
	}
}