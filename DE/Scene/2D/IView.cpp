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

namespace DE {
	IView::~IView() {
		if(this->m_parent != nullptr) {
			RemoveFromParentView();
		}
	}

	void IView::Update() {
		for(auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if((*it)->IsVisible()) {
				(*it)->Update();
			}
		}
	}

	void IView::Render() {
		for(auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if((*it)->IsVisible()) {
				(*it)->Render();
			}
		}
	}

	const bool IView::HitTest(const CVector2I & point) {
		this->m_hitTest = false;

		for(auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if((*it)->HitTest(point)) {
				return true;
			}
		}

		this->m_hitTest = IsInside(point);
		return this->m_hitTest;
	}

	void IView::SetChanged(const bool changed) {
		this->m_changed = changed;
	}

	void IView::SetVisible(const bool visible) {
		this->m_visible = visible;
	}

	void IView::SetPosition(const CVector2I & position) {
		this->m_position = position;
	}

	void IView::SetPreferredSize(const CVector2I & size) {
		this->m_preferredsize = size;
	}

	void IView::SetBounds(const CRectangleI & bounds) {
		SetPosition(CVector2I(bounds.GetLeft(), bounds.GetTop()));
		SetPreferredSize(CVector2I(bounds.GetWidth(), bounds.GetHeight()));
		this->m_boundingRectangle = CRectangleI(this->m_position.x, this->m_position.y, this->m_preferredsize.x, this->m_preferredsize.y);
	}

	void IView::SetParent(IView * parent) {
		this->m_parent = parent;
	}

	void IView::AddSubview(IView * view) {
		this->m_subviews.push_back(view);
		view->SetParent(this);
	}

	void IView::RemoveSubview(IView * view) {
		for(auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if((*it) == view) {
				this->m_subviews.erase(it);
				view->SetParent(nullptr);
				return;
			}
		}

	}

	void IView::RemoveFromParentView() {
		if(this->m_parent != nullptr) {
			this->m_parent->RemoveSubview(this);
		}
	}

	void IView::RemoveAllSubviews() {
		this->m_subviews.clear();
	}

	const bool IView::IsChanged() const {
		return this->m_changed;
	}

	const bool IView::IsVisible() const {
		return this->m_visible;
	}

	const bool IView::isHit() const {
		return this->m_hitTest;
	}

	const bool IView::IsInside(const CVector2I & point) const {
		return GetAbsoluteBoundingRectangle().Intersect(point) == RECTANGLE_INTERSECTION_INSIDE;
	}

	const CVector2I & IView::GetPosition() const {
		return this->m_position;
	}

	const CVector2I IView::GetAbsolutePosition() const {
		if(this->m_parent != nullptr) {
			return this->m_parent->GetAbsolutePosition() + this->m_position;
		}
		return m_position;
	}

	const CVector2I & IView::GetPreferredSize() const {
		return this->m_preferredsize;
	}

	const CRectangleI & IView::GetBoundingRectangle() const {
		return this->m_boundingRectangle;
	}

	const CRectangleI IView::GetAbsoluteBoundingRectangle() const {
		if(this->m_parent != nullptr) {
			return this->m_boundingRectangle + this->m_parent->GetAbsolutePosition();
		}
		return m_boundingRectangle;
	}

	IView::IView() {
		this->m_changed = false;
		this->m_visible = true;
		this->m_hitTest = false;
		this->m_parent = nullptr;

		// Default 2D texture filter
		this->m_textureFilter.minFilter = DE::TEXTURE_FILTER_TYPE_NONE;
		this->m_textureFilter.magFilter = DE::TEXTURE_FILTER_TYPE_NONE;
		this->m_textureFilter.mipFilter = DE::TEXTURE_FILTER_TYPE_NONE;
		this->m_textureFilter.addressU = DE::TEXTURE_ADDRESS_CLAMP;
		this->m_textureFilter.addressV = DE::TEXTURE_ADDRESS_CLAMP;
		this->m_textureFilter.maxAnisotropy = 0;
	}

	IView::IView(const IView & copy) {
		*this = copy;
	}

	const IView & IView::operator=(const IView & copy) {
		this->m_changed = copy.m_changed;
		this->m_visible = copy.m_visible;
		this->m_position = copy.m_position;
		this->m_preferredsize = copy.m_preferredsize;
		this->m_boundingRectangle = copy.m_boundingRectangle;
		this->m_parent = nullptr;
		return *this;
	}

	const std::vector<SGUIData> IView::GenerateQuad(const CRectangleI & position, const CRectangleF & coordinates) {
		std::vector<SGUIData> dataSet;
		SGUIData data;

		// First triangle
		data.position = CVector3F((float)position.GetLeft(), (float)position.GetTop(), 0);
		data.texcoords = CVector2F(coordinates.GetLeft(), coordinates.GetTop());
		dataSet.push_back(data);

		data.position = CVector3F((float)position.GetRight(), (float)position.GetBottom(), 0);
		data.texcoords = CVector2F(coordinates.GetRight(), coordinates.GetBottom());
		dataSet.push_back(data);

		data.position = CVector3F((float)position.GetLeft(), (float)position.GetBottom(), 0);
		data.texcoords = CVector2F(coordinates.GetLeft(), coordinates.GetBottom());
		dataSet.push_back(data);

		// Second triangle
		data.position = CVector3F((float)position.GetLeft(), (float)position.GetTop(), 0);
		data.texcoords = CVector2F(coordinates.GetLeft(), coordinates.GetTop());
		dataSet.push_back(data);

		data.position = CVector3F((float)position.GetRight(), (float)position.GetTop(), 0);
		data.texcoords = CVector2F(coordinates.GetRight(), coordinates.GetTop());
		dataSet.push_back(data);

		data.position = CVector3F((float)position.GetRight(), (float)position.GetBottom(), 0);
		data.texcoords = CVector2F(coordinates.GetRight(), coordinates.GetBottom());
		dataSet.push_back(data);

		return dataSet;
	}
}
