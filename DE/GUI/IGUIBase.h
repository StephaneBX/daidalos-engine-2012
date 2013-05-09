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

#ifndef __IGUIBASE_H
#define __IGUIBASE_H

#include <vector>
#include "../Core/Utility/CVector3.h"
#include "../Core/Utility/CVector2.h"
#include "../Core/Utility/CColor.h"
#include "../Core/Utility/CRectangle.h"
#include "../Medias/CBuffer.h"

namespace DE
{
	/**
	* The GUI data structure for the shaders
	*/
	struct SGUIData
	{
		CVector3F position;
		CVector2F texcoords;
	};

	/**
	* Basic class for gui
	*/
	class IGUIBase
	{
	protected:
		bool m_changed;
		bool m_visible;
		std::vector<SGUIData> m_data;
		CBuffer m_buffer;
		CVector2I m_position;
		CVector2I m_preferredsize;
		CRectangleI m_boundingRectangle;

	public:
		/**
		* Update the text geometry
		*/
		virtual void Update() = 0;

		/**
		* Render the text
		*/
		virtual void Render() = 0;
		
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
		* Check if a point is inside the gui
		* @param point The point to test
		* @return Trueif the point is inside the gui
		*/
		const bool IsInside(const CVector2I & point) const;

		/**
		* Get the position
		* @return The position
		*/
		const CVector2I & GetPosition() const;

		/**
		* Get the preferred size
		* @return The preferred size
		*/
		const CVector2I & GetPreferredSize() const;

		/**
		* Get the bounding rectangle
		* @return The bounding rectangle
		*/
		const CRectangleI & GetBoundingRectangle() const;

	protected:
		/**
		* Constructor
		*/
		IGUIBase();

		/**
		* Constructor
		* @param copy The element to copy
		*/
		IGUIBase(const IGUIBase & copy);

		/**
		* = operator override
		* @param copy A GUI element copy
		* @return The GUI element
		*/
		const IGUIBase & operator=(const IGUIBase & copy);
	};
}

#endif
