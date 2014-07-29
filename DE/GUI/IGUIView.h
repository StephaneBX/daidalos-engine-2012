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

#ifndef __IGUIVIEW_H
#define __IGUIVIEW_H

#include <vector>
#include "../Core/Utility/CVector3.h"
#include "../Core/Utility/CVector2.h"
#include "../Core/Utility/CColor.h"
#include "../Core/Utility/CRectangle.h"
#include "../Medias/CBuffer.h"

namespace DE
{
	#define GUI_VIEW_LEFT_CLICK 1 << 30

	/**
	* The GUI data structure for the shaders
	*/
	struct SGUIData
	{
		CVector3F position;
		CVector2F texcoords;
	};

	/**
	* Basic class for views
	*/
	class IGUIView
	{
	protected:
		bool m_changed;
		bool m_visible;
		bool m_hitTest;
		std::vector<SGUIData> m_data;
		std::vector<IGUIView*> m_subviews;
		CBuffer m_buffer;
		CVector2I m_position;
		CVector2I m_preferredsize;
		CRectangleI m_boundingRectangle;
		IGUIView * m_parent;

	public:
		/**
		* Constructor
		*/
		IGUIView();

		/**
		* Update the text geometry
		*/
		virtual void Update();

		/**
		* Render the text
		*/
		virtual void Render();

		/**
		* Perform a hit test, checking if the point is inside in the the subview or in this view
		* The test is exclusive, only 1 view can succeed in the hit test
		* @param point The point to test in screen coordinate
		* @return True if the point is inside the view on inside one of its subviews
		*/
		virtual const bool HitTest(const CVector2I & point);
		
		/**
		* Set the changed value
		* @param changed The changed value
		*/
		void SetChanged(const bool changed);

		/**
		* Set the visible value
		* @param visible The visible value
		*/
		void SetVisible(const bool visible);

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
		void SetParent(IGUIView * parent);

		/**
		* Add a subview
		* @param view The subview to add
		*/
		void AddSubview(IGUIView * view);

		/**
		* Remove a subview
		* @param view The subview to remove
		*/
		void RemoveSubview(IGUIView * view);

		/**
		* Remove from the parent view
		*/
		void RemoveFromParentView();

		/**
		* Remove all the subviews
		*/
		void RemoveAllSubviews();

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
		* @param copy The element to copy
		*/
		IGUIView(const IGUIView & copy);

		/**
		* = operator override
		* @param copy A GUI element copy
		* @return The GUI element
		*/
		const IGUIView & operator=(const IGUIView & copy);

		/**
		* Generate and return a quad with a given position and texture coordinates
		*/
		const std::vector<SGUIData> GenerateQuad(const CRectangleI & position, const CRectangleF & coordinates);
	};
}

#endif
