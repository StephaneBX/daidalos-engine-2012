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

#include "../../Core/IRenderer.hpp"
#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/Utility/CMatrix.hpp"
#include "../../Scene/2D/IView.hpp"

namespace daidalosengine {
	/**
	* Destructor
	*/
	IView::~IView() {
		LOG_TRACE();

		if(this->m_parent != nullptr) {
			RemoveFromParentView();
		}
	}

	/**
	* Update the text geometry
	* @param time The elapsed time
	*/
	void IView::Update(const float time) {
		LOG_TRACE();

		for(auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if((*it)->IsVisible()) {
				(*it)->Update(time);
			}
		}
	}

	/**
	* Render the text
	*/
	void IView::Render() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();

		for(auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if((*it)->IsVisible()) {
				renderer->SetScissor(GetAbsoluteBoundingRectangle());
				(*it)->Render();
			}
		}
	}

	/**
	* Add a subview
	* @param view The subview to add
	*/
	void IView::AddSubview(IView * view) {
		LOG_TRACE();

		this->m_subviews.push_back(view);
		view->SetParent(this);
	}

	/**
	* Remove a subview
	* @param view The subview to remove
	*/
	void IView::RemoveSubview(IView * view) {
		LOG_TRACE();

		for (auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if ((*it) == view) {
				this->m_subviews.erase(it);
				view->SetParent(nullptr);
				return;
			}
		}
	}

	/**
	* Remove all the subviews
	*/
	void IView::RemoveAllSubviews() {
		LOG_TRACE();

		this->m_subviews.clear();
	}

	/**
	* Perform a hit test, checking if the point is inside one of the subview or in this view
	* The test is exclusive, only 1 view can succeed in the hit test
	* @param point The point to test in screen coordinate
	* @return True if the point is inside the view on inside one of its subviews
	*/
	bool IView::HitTest(const CVector2I & point) {
		return HitTest(this, point);
	}

	/**
	* Perform a hit test, checking if the point is inside one of the subview or in this view
	* The test is exclusive, only 1 view can succeed in the hit test
	* @param target the target view to check
	* @param point The point to test in screen coordinate
	* @return True if the point is inside the view on inside one of its subviews
	*/
	bool IView::HitTest(IView * target, const CVector2I & point) {
		LOG_TRACE();

		this->m_hitTest = false;

		for (auto rit = this->m_subviews.rbegin(); rit != this->m_subviews.rend(); rit++) {
			if (target->IsInside((*rit)->GetAbsoluteBoundingRectangle()) && (*rit)->HitTest(point)) {
				return true;
			}
		}

		this->m_hitTest = IsInside(point);
		return this->m_hitTest;
	}

	/**
	* Set the changed value
	* @param changed The changed value
	*/
	void IView::SetChanged(const bool changed) {
		LOG_TRACE();

		this->m_changed = changed;
	}

	/**
	* Set the visible value
	* @param visible The visible value
	*/
	void IView::SetVisible(const bool visible) {
		LOG_TRACE();

		this->m_visible = visible;
	}

	/**
	* Set the hit
	* @param hit The hit
	*/
	void IView::SetHit(const bool hit) {
		LOG_TRACE();

		this->m_hitTest = hit;
	}

	/**
	* Set the position
	* @param position The position
	*/
	void IView::SetPosition(const CVector2I & position) {
		LOG_TRACE();

		this->m_position = position;
		this->m_boundingRectangle = CRectangleI(this->m_position.x, this->m_position.y, this->m_preferredsize.x, this->m_preferredsize.y);
	}

	/**
	* Set the preferred size
	* @param size The preferred size
	*/
	void IView::SetPreferredSize(const CVector2I & size) {
		LOG_TRACE();

		this->m_preferredsize = size;
		this->m_boundingRectangle = CRectangleI(this->m_position.x, this->m_position.y, this->m_preferredsize.x, this->m_preferredsize.y);
	}

	/**
	* Set the bounding rectangle
	* @param bounds The bounds
	*/
	void IView::SetBoundingRectangle(const CRectangleI & bounds) {
		LOG_TRACE();

		this->m_boundingRectangle = bounds;
	}

	/**
	* Set the bounds (position + preferred size)
	* @param bounds The bounds
	*/
	void IView::SetBounds(const CRectangleI & bounds) {
		LOG_TRACE();

		SetPosition(CVector2I(bounds.GetLeft(), bounds.GetTop()));
		SetPreferredSize(CVector2I(bounds.GetWidth(), bounds.GetHeight()));
		this->m_boundingRectangle = CRectangleI(this->m_position.x, this->m_position.y, this->m_preferredsize.x, this->m_preferredsize.y);
	}

	/**
	* Set the view parent
	* @param parent The view parent
	*/
	void IView::SetParent(IView * parent) {
		LOG_TRACE();

		this->m_parent = parent;
	}

	/**
	* Remove from the parent view
	*/
	void IView::RemoveFromParentView() {
		LOG_TRACE();

		if(this->m_parent != nullptr) {
			this->m_parent->RemoveSubview(this);
		}
	}

	/**
	* Get the changed value
	* @return The changed value
	*/
	bool IView::IsChanged() const {
		LOG_TRACE();

		return this->m_changed;
	}

	/**
	* Get the visible value
	* @return The visible value
	*/
	bool IView::IsVisible() const {
		LOG_TRACE();

		return this->m_visible;
	}

	/**
	* Check if the previous hit test occurred in this view
	* @return True if the hit test occurred in this view
	*/
	bool IView::IsHit() const {
		LOG_TRACE();

		return this->m_hitTest;
	}

	/**
	* Check if a point is inside the gui
	* @param point The point to test in screen coordinate
	* @return True if the point is inside the gui
	*/
	bool IView::IsInside(const CVector2I & point) const {
		LOG_TRACE();

		return GetAbsoluteBoundingRectangle().Intersect(point) == RECTANGLE_INTERSECTION_INSIDE;
	}

	/**
	* Check if a rectangle is inside the gui
	* @param rectangle The rectangle to test in screen coordinate
	* @return True if the rectangle is inside the gui
	*/
	bool IView::IsInside(const CRectangleI & rectangle) const {
		LOG_TRACE();

		return GetAbsoluteBoundingRectangle().Intersect(rectangle) != RECTANGLE_INTERSECTION_OUTSIDE;
	}

	/**
	* Get the position relative to the parent
	* @return The position
	*/
	const CVector2I & IView::GetPosition() const {
		LOG_TRACE();

		return this->m_position;
	}

	/**
	* Get the absolute position
	* @return The absolute position
	*/
	const CVector2I IView::GetAbsolutePosition() const {
		LOG_TRACE();

		if(this->m_parent != nullptr) {
			return this->m_parent->GetAbsolutePosition() + this->m_position;
		}
		return m_position;
	}

	/**
	* Get the preferred size
	* @return The preferred size
	*/
	const CVector2I & IView::GetPreferredSize() const {
		LOG_TRACE();

		return this->m_preferredsize;
	}

	/**
	* Get the bounding rectangle relative to the parent
	* @return The bounding rectangle
	*/
	const CRectangleI & IView::GetBoundingRectangle() const {
		LOG_TRACE();

		return this->m_boundingRectangle;
	}

	/**
	* Get the absolute bounding rectangle
	* @return The absolute bounding rectangle
	*/
	const CRectangleI IView::GetAbsoluteBoundingRectangle() const {
		LOG_TRACE();

		if(this->m_parent != nullptr) {
			return this->m_boundingRectangle + this->m_parent->GetAbsolutePosition();
		}
		return m_boundingRectangle;
	}

	/**
	* Get the parent
	* @return The parent
	*/
	IView * IView::GetParent() {
		return this->m_parent;
	}

	/**
	* Get the texture filter
	* @return The texture filter
	*/
	STexture2DFilterPolicy * IView::GetTextureFilter() {
		return &this->m_textureFilter;
	}

	/**
	* Constructor
	*/
	IView::IView() {
		LOG_TRACE();

		this->m_changed = false;
		this->m_visible = true;
		this->m_hitTest = false;
		this->m_parent = nullptr;

		// Default 2D texture filter
		this->m_textureFilter.minFilter = daidalosengine::TEXTURE_FILTER_TYPE_NONE;
		this->m_textureFilter.magFilter = daidalosengine::TEXTURE_FILTER_TYPE_NONE;
		this->m_textureFilter.mipFilter = daidalosengine::TEXTURE_FILTER_TYPE_NONE;
		this->m_textureFilter.addressU = daidalosengine::TEXTURE_ADDRESS_CLAMP;
		this->m_textureFilter.addressV = daidalosengine::TEXTURE_ADDRESS_CLAMP;
		this->m_textureFilter.maxAnisotropy = 0;
	}

	/**
	* Constructor
	* @param copy The element to copy
	*/
	IView::IView(const IView & copy) {
		LOG_TRACE();

		*this = copy;
	}

	/**
	* Update the buffer if necessary
	*/
	void IView::UpdateBuffer() {
		LOG_TRACE();

		if (this->m_data.size() > 0) {
			// Create a new buffer if the current one is too small
			if ((int)this->m_buffer.GetSize() < this->m_data.size() * sizeof(SGUIData))
				this->m_buffer = IRenderer::GetRenderer()->CreateVertexBuffer(sizeof(SGUIData), this->m_data.size(), BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);

			// Lock and copy the data
			unsigned char * lock = (unsigned char*)this->m_buffer.Lock(sizeof(SGUIData), 0, 0, LOCK_WRITEONLY);
			memcpy(lock, &this->m_data[0], this->m_data.size() * sizeof(SGUIData));
			this->m_buffer.Unlock();
		}
	}

	/**
	* Render the primtives from the buffer
	*/
	void IView::RenderPrimitives() {
		LOG_TRACE();

		IRenderer * renderer = IRenderer::GetRenderer();
		renderer->SetVertexBuffer(0, this->m_buffer.GetBuffer(), sizeof(SGUIData));
		renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_data.size() / 3);
	}

	/**
	* Render the subviews
	* @param scissor If the scissor test should be modified
	*/
	void IView::RenderSubviews(const bool scissor) {
		LOG_TRACE();

		// Set the scissor if enabled
		IRenderer * renderer = IRenderer::GetRenderer();
		for (auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			if ((*it)->IsVisible()) {
				if (scissor) {
					renderer->SetScissor(this->m_parent->GetAbsoluteBoundingRectangle());
				}
				(*it)->Render();
			}
		}
	}

	/**
	* Set the data
	* @param data The data
	*/
	void IView::SetData(const std::vector<SGUIData> & data) {
		this->m_data = data;
	}

	/**
	* Get the data size
	* @return The data size
	*/
	int IView::GetDataSize() const {
		LOG_TRACE();

		return this->m_data.size();
	}

	/**
	* = operator override
	* @param copy A GUI element copy
	* @return The GUI element
	*/
	IView & IView::operator=(const IView & copy) {
		LOG_TRACE();

		if (this != &copy) {
			this->m_changed = copy.m_changed;
			this->m_visible = copy.m_visible;
			this->m_position = copy.m_position;
			this->m_preferredsize = copy.m_preferredsize;
			this->m_boundingRectangle = copy.m_boundingRectangle;
			this->m_parent = nullptr;
		}

		return *this;
	}

	/**
	* Perform a hit test, checking if the point is inside one of the subview or in this view
	* @param target the target view to check
	* @param point The point to test in screen coordinate
	* @return True if the point is inside the view on inside one of its subviews
	*/
	bool IView::HitTestAny(IView * target, const CVector2I & point) {
		this->m_hitTest = false;

		for (auto rit = this->m_subviews.rbegin(); rit != this->m_subviews.rend(); rit++) {
			if (target->IsInside((*rit)->GetAbsoluteBoundingRectangle()) && (*rit)->HitTest(point)) {
				this->m_hitTest = true;
			}
		}
		return this->m_hitTest;
	}

	/**
	* Generate and return a quad with a given position and texture coordinates
	* @param position The position
	* @param coordinates The texture coordinates
	* @param rotation The rotation
	* @return The quad
	*/
	const std::vector<SGUIData> IView::GenerateQuad(const CRectangleI & position, const CRectangleF & coordinates, const float rotation) {
		LOG_TRACE();

		CRectangleI centeredPosition = CRectangleI(-(position.GetWidth() / 2), -(position.GetHeight() / 2), position.GetWidth(), position.GetHeight());
		CMatrix rotationMatrix = CMatrix(CVector3F(1, 1, 1), CVector3F(0, 0, DegreeToRadian(rotation)), CVector3F(0, 0, 0));
		CVector3F translation = CVector3F((float)(position.GetLeft() + position.GetWidth() / 2), (float)(position.GetTop() + (position.GetHeight() / 2)), 0);
		std::vector<SGUIData> dataSet;
		SGUIData data;

		// First triangle
		data.position = CVector3F((float)centeredPosition.GetLeft(), (float)centeredPosition.GetTop(), 0);
		data.texcoords = CVector2F(coordinates.GetLeft(), coordinates.GetTop());
		dataSet.push_back(data);

		data.position = CVector3F((float)centeredPosition.GetRight(), (float)centeredPosition.GetBottom(), 0);
		data.texcoords = CVector2F(coordinates.GetRight(), coordinates.GetBottom());
		dataSet.push_back(data);

		data.position = CVector3F((float)centeredPosition.GetLeft(), (float)centeredPosition.GetBottom(), 0);
		data.texcoords = CVector2F(coordinates.GetLeft(), coordinates.GetBottom());
		dataSet.push_back(data);

		// Second triangle
		data.position = CVector3F((float)centeredPosition.GetLeft(), (float)centeredPosition.GetTop(), 0);
		data.texcoords = CVector2F(coordinates.GetLeft(), coordinates.GetTop());
		dataSet.push_back(data);

		data.position = CVector3F((float)centeredPosition.GetRight(), (float)centeredPosition.GetTop(), 0);
		data.texcoords = CVector2F(coordinates.GetRight(), coordinates.GetTop());
		dataSet.push_back(data);

		data.position = CVector3F((float)centeredPosition.GetRight(), (float)centeredPosition.GetBottom(), 0);
		data.texcoords = CVector2F(coordinates.GetRight(), coordinates.GetBottom());
		dataSet.push_back(data);

		for(auto it = dataSet.begin(); it != dataSet.end(); it++) {
			(*it).position = rotationMatrix.Transform((*it).position) + translation;
		}

		return dataSet;
	}

	/**
	* Calculate the subviews bounding rectangle
	* @return The bounding rectangle
	*/
	const CRectangleI IView::GetSubviewsBoundingRectangle() const {
		LOG_TRACE();

		CVector2I minSize;
		CVector2I maxSize;
		for (auto it = this->m_subviews.begin(); it != this->m_subviews.end(); it++) {
			minSize.x = std::min(minSize.x, (*it)->GetPosition().x);
			minSize.y = std::min(minSize.y, (*it)->GetPosition().y);

			maxSize.x = std::max(maxSize.x, (*it)->GetPosition().x + (*it)->GetPreferredSize().x);
			maxSize.y = std::max(maxSize.y, (*it)->GetPosition().y + (*it)->GetPreferredSize().y);
		}

		return CRectangleI(minSize, maxSize - minSize);
	}
}
