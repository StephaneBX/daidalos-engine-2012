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

#include "../GUI/IGUIView.h"
#include "../Core/IRenderer.h"
#include "../Core/CGUIManager.h"

namespace DE
{
	CGUIManager::~CGUIManager() {}

	void CGUIManager::Initialize()
	{
		this->m_renderer = IRenderer::GetRenderer();
		this->m_proj.OrthographicView(0, (float)this->m_renderer->GetResolution().x, (float)this->m_renderer->GetResolution().y, 0, 0.0f, 1.0f);

		// Vertex declaration used by every GUI element
		SDeclarationElement guiDecl[] =
		{
			{0, DE::DECLARATION_USAGE_POSITION0, DE::DECLARATION_TYPE_FLOAT3},
			{0, DE::DECLARATION_USAGE_TEXCOORD0, DE::DECLARATION_TYPE_FLOAT2}
		};
		this->m_declaration = this->m_renderer->CreateVertexDeclaration(guiDecl);

		// Shader program used for gui rendering
		this->m_shaderProgram.LoadFromFile("gui");
	}

	void CGUIManager::AddToMainView(IGUIView * element)
	{
		this->m_mainView.AddSubview(element);
	}

	void CGUIManager::RemoveFromMainView(IGUIView * element)
	{
		this->m_mainView.RemoveSubview(element);
	}

	void CGUIManager::Render()
	{
		// Global data for all elements
		this->m_renderer->SetVertexDeclaration(this->m_declaration.get());
		this->m_renderer->SetShaderProgram(this->m_shaderProgram.GetShaderProgram());
		this->m_shaderProgram.SetParameter(SHADERPROGRAM_FLAG_VERTEX, this->m_renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_PROJ_MATRIX), this->m_proj);

		// Draw the main element
		this->m_mainView.Update();
		this->m_mainView.Render();
	}

	/**
	* Constructor
	*/
	CGUIManager::CGUIManager()
	{
		this->m_renderer = nullptr;
	}
}
