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

#include "../../Scene/2D/IView.h"
#include "../../Core/IRenderer.h"
#include "../../Scene/2D/C2DScene.h"

namespace DE {
	/**
	* Constructor
	*/
	C2DScene::C2DScene() {
		this->m_cursor = nullptr;

		const IRenderer * renderer = IRenderer::GetRenderer();
		this->m_proj.OrthographicView(0, (float)renderer->GetResolution().x, (float)renderer->GetResolution().y, 0, 0.0f, 1.0f);

		// Create the main view
		this->m_mainView = std::unique_ptr<CSprite>(new CSprite());
		this->m_mainView->SetBounds(CRectangleI(0, 0, renderer->GetResolution().x, renderer->GetResolution().y));
	}

	C2DScene::~C2DScene() {}

	void C2DScene::AddToMainView(IView * element) {
		this->m_mainView->AddSubview(element);
	}

	void C2DScene::RemoveFromMainView(IView * element) {
		this->m_mainView->RemoveSubview(element);
	}

	void C2DScene::SetCursor(CCursor * cursor) {
		this->m_cursor = cursor;
	}

	void C2DScene::Render() {
		// Update the elements
		this->m_mainView->Update();

		// Hit test
		if(this->m_cursor != nullptr && this->m_cursor->IsVisible()) {
			this->m_mainView->HitTest(this->m_cursor->GetPosition());
		}

		// Draw all the elements
		this->m_mainView->Render();
	}

	const CMatrix & C2DScene::GetProjMatrix() const {
		return this->m_proj;
	}
}
