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

#ifndef __IVIEW_HPP
#define __IVIEW_HPP

#include <vector>
#include "../../Core/Utility/CVector3.hpp"
#include "../../Core/Utility/CVector2.hpp"
#include "../../Core/Utility/CColor.hpp"
#include "../../Core/Utility/CRectangle.hpp"
#include "../../Medias/CTexture2D.hpp"
#include "../../Medias/CBuffer.hpp"

namespace DE {
#define GUI_VIEW_LEFT_CLICK 1 << 28
#define GUI_VIEW_WHEEL_UP 1 << 29
#define GUI_VIEW_WHEEL_DOWN 1 << 30

	/**
	* The GUI data structure for the shaders
	*/
	struct SGUIData {
		CVector3F position;
		CVector2F texcoords;
	};

	/**
	* Basic class for views
	*/
	class IView {
	protected:
		bool m_changed;
		bool m_visible;
		bool m_hitTest;
		std::vector<SGUIData> m_data;
		std::vector<IView*> m_subviews;
		CBuffer m_buffer;
		CVector2I m_position;
		CVector2I m_preferredsize;
		CRectangleI m_boundingRectangle;
		IView * m_parent;
		STexture2DFilterPolicy m_textureFilter;

	public:
		/**
		* Destructor
		*/
		virtual ~IView();

		/**
		* Update the text geometry
		* @param time The elapsed time
		*/
		virtual void Update(const float time);

		/**
		* Render the text
		*/
		virtual void Render();

		/**
		* Add a subview
		* @param view The subview to add
		*/
		virtual void AddSubview(IView * view);

		/**
		* Remove a subview
		* @param view The subview to remove
		*/
		virtual void RemoveSubview(IView * view);

		/**
		* Remove all the subviews
		*/
		virtual void RemoveAllSubviews();

		/**
		* Perform a hit test, checking if the point is inside one of the subview or in this view
		* The test is exclusive, only 1 view can succeed in the hit test
		* @param point The point to test in screen coordinate
		* @return True if the point is inside the view on inside one of its subviews
		*/
		virtual const bool HitTest(const CVector2I & point);

		/**
		* Set the changed value
		* @param changed The changed value
		*/
		virtual void SetChanged(const bool changed);

		/**
		* Set the visible value
		* @param visible The visible value
		*/
		virtual void SetVisible(const bool visible);

		/**
		* Set the position
		* @param position The position
		*/
		void SetPosition(const CVector2I & position);

		/**
		* Set the preferred size
		* @param size The preferred size
		*/
		void SetPreferredSize(const CVector2I & size);

		/**
		* Set the bounds (position + preferred size)
		* @param bounds The bounds
		*/
		void SetBounds(const CRectangleI & bounds);

		/**
		* Set the view parent
		* @param parent The view parent
		*/
		void SetParent(IView * parent);

		/**
		* Remove from the parent view
		*/
		void RemoveFromParentView();

		/**
		* Get the changed value
		* @return The changed value
		*/
		const bool IsChanged() const;

		/**
		* Get the visible value
		* @return The visible value
		*/
		const bool IsVisible() const;

		/**
		* Check if the previous hit test occurred in this view
		* @return True if the hit test occurred in this view
		*/
		const bool isHit() const;

		/**
		* Check if a point is inside the gui
		* @param point The point to test in screen coordinate
		* @return True if the point is inside the gui
		*/
		const bool IsInside(const CVector2I & point) const;

		/**
		* Check if a rectangle is inside the gui
		* @param rectangle The rectangle to test in screen coordinate
		* @return True if the rectangle is inside the gui
		*/
		const bool IsInside(const CRectangleI & rectangle) const;

		/**
		* Get the position relative to the parent
		* @return The position
		*/
		const CVector2I & GetPosition() const;

		/**
		* Get the absolute position
		* @return The absolute position
		*/
		const CVector2I GetAbsolutePosition() const;

		/**
		* Get the preferred size
		* @return The preferred size
		*/
		const CVector2I & GetPreferredSize() const;

		/**
		* Get the bounding rectangle relative to the parent
		* @return The bounding rectangle
		*/
		const CRectangleI & GetBoundingRectangle() const;

		/**
		* Get the absolute bounding rectangle
		* @return The absolute bounding rectangle
		*/
		const CRectangleI GetAbsoluteBoundingRectangle() const;

	protected:
		/**
		* Constructor
		*/
		IView();

		/**
		* Constructor
		* @param copy The element to copy
		*/
		IView(const IView & copy);

		/**
		* = operator override
		* @param copy A GUI element copy
		* @return The GUI element
		*/
		const IView & operator=(const IView & copy);

		/**
		* Generate and return a quad with a given position and texture coordinates
		* @param position The position
		* @param coordinates The texture coordinates
		* @param rotation The rotation
		* @return The quad
		*/
		const std::vector<SGUIData> GenerateQuad(const CRectangleI & position, const CRectangleF & coordinates, const float rotation = 0);
	};
}

#endif
