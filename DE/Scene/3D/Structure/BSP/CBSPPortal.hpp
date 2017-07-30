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

#ifndef __CBSPPORTAL_HPP
#define __CBSPPORTAL_HPP

#include <array>
#include "../../../../Scene/3D/Structure/BSP/CBSPTriangle.hpp"

namespace DE {
	/**
	* The bsp portal class
	* Used during the pvs calculation to improve performance
	*/
	class CBSPPortal {
	private:
		std::array<CBSPTriangle, 2> m_triangles;
		unsigned int m_numberOfLeafs;
		std::array<unsigned int, 2> m_leafsIndex;

	public:
		/**
		* Set the triangle at the given index
		* @param triangle The triangle
		* @param index The index: 0 or 1
		*/
		void SetTriangle(const CBSPTriangle & triangle, const unsigned int index);

		/**
		* Set the number of leafs
		* @param leafs The number of leafs
		*/
		void SetNumberOfLeafs(const unsigned int leafs);

		/**
		* Set the leaf index
		* @param leafMasterIndex The leaf index in the master leaf array
		* @param index The leaf: 0 or 1
		*/
		void SetLeafIndex(const unsigned int leafMasterIndex, const unsigned int index);

		/**
		* Get the number of leafs attached to the portal
		* @return The number of leafs
		*/
		const unsigned int GetNumberOfLeafs() const;

		/**
		* Get the leaf master array index
		* @param index The index: 0 or 1
		* @return The master leaf array index
		*/
		const unsigned int GetLeafIndex(const unsigned int index) const;

		/**
		* Get the triangle
		* @param index The index: 0 or 1
		* @return The index
		*/
		const CBSPTriangle & GetTriangle(const unsigned int index) const;
	};
}

#endif
