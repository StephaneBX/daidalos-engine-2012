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

#ifndef __CBSPTRIANGLE_HPP
#define __CBSPTRIANGLE_HPP

#include "../../../../Scene/3D/CTriangle3D.hpp"

namespace daidalosengine {
	/**
	* Handle a 3d triangle for the bsp
	*/
	class CBSPTriangle : public CTriangle3D {
	protected:
		bool m_separationPlane;
		bool m_visible;
		bool m_drawn;
		CPlane3DF m_plane;

	public:
		/**
		* Constructor
		*/
		CBSPTriangle();

		/**
		* Set the separation plane state
		* @param separationPlane The separation plane state
		*/
		void SetSeparationPlane(const bool separationPlane);

		/**
		* Set the triangle visibility
		* @param visible The triangle visibility
		*/
		void SetVisible(const bool visible);

		/**
		* Set the triangle drawn state
		* @param drawn The triangle drawn state
		*/
		void SetDrawn(const bool drawn);

		/**
		* Set the plane
		* @param plane The plane
		*/
		void SetPlane(const CPlane3DF & plane);

		/**
		* Create the plane with the vertices information
		*/
		void CreatePlane();

		/**
		* Check if the triangle is a separation plane
		* @return True if the triangle is a separation plane
		*/
		const bool IsSeparationPlane() const;

		/**
		* Check if the triangle is visible
		* @return True if the triangle is visible
		*/
		const bool IsVisible() const;

		/**
		* Check if the triangle is drawn
		* @return True if the triangle is drawn
		*/
		const bool IsDrawn() const;

		/**
		* Get the plane
		* @return The plane
		*/
		const CPlane3DF & GetPlane() const;

		/**
		* Classify a triangle regarding a plane
		* @param plane The plane used as a reference
		* @return The triangle position toward the plane
		*/
		EBSPTriangleOrientation Classify(const CPlane3DF & plane);

		/**
		* Get the plane
		* @return The plane
		*/
		CPlane3DF * GetPlanePointer();
	};
}

#endif
