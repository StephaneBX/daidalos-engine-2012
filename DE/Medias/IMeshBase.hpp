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

#ifndef __IMESHBASE_HPP
#define __IMESHBASE_HPP

#include <vector>
#include "../Core/Enums.hpp"
#include "../Core/Utility/CVector3.hpp"
#include "../Core/Utility/CVector2.hpp"
#include "../Scene/3D/CSphere.hpp"
#include "../Medias/CMaterial.hpp"
#include "../Core/IResource.hpp"

namespace daidalosengine {
	/**
	* Basic class used for meshs
	*/
	class IMeshBase : public IResource {
	protected:
		std::vector<int> m_groupSize;
		CVector3F ** m_vertex;
		CVector3F ** m_normal;
		CVector3F ** m_tangent;
		CVector3F ** m_binormal;
		CVector2F ** m_texcoord;
		CSphereF * m_visibilityShape;

	public:
		/**
		* Destructor
		*/
		 virtual ~IMeshBase();

		/**
		* Finalize the geometry.
		* Copy the geometry to the output buffers.
		* @param shadingType The shading type
		*/
		virtual void FinalizeGeometry(EMeshShadingType shadingType) = 0;

		/**
		* Get the number of groups.
		* @return The number of groups.
		*/
		const int GetNumberOfGroups() const;

		/**
		* Get the size of a group.
		* @param group Th group number.
		* @return The size of the group.
		*/
		const int GetGroupSize(const int group = 0) const;

		/**
		* Get the vertices of a group.
		* @param group The group number.
		* @return The vertices.
		*/
		CVector3F * GetVertices(const int group = 0) const;

		/**
		* Get the normals of a group.
		* @param group The group number.
		* @return The normals.
		*/
		CVector3F * GetNormals(const int group = 0) const;

		/**
		* Get the normals of a group.
		* @param group The group number.
		* @return The normals.
		*/
		CVector3F * GetTangents(const int group = 0) const;

		/**
		* Get the normals of a group.
		* @param group Th group number.
		* @return The normals.
		*/
		CVector3F * GetBinormals(const int group = 0) const;

		/**
		* Get the texcoord of a group.
		* @param group Th group number.
		* @return The texcoord.
		*/
		CVector2F * GetTexcoords(const int group = 0) const;

		/**
		* Get the visibility shape
		* @return the visibility shape
		*/
		CSphereF * GetVisibilityShape() const;

	protected:
		/**
		* Constructor
		* @param name The resource name.
		*/
		explicit IMeshBase(const std::string & name);

		/**
		* Flat the normals in the normal geometry output.
		*/
		void FlatShadingNormals();

		/**
		* Calculate the tangents and the binormals
		*/
		void TangentAndBinormalCalculation();

	private:
		IMeshBase();
		IMeshBase(const IMeshBase & copy);
		const IMeshBase & operator=(const IMeshBase & copy);
	};
}

#endif
