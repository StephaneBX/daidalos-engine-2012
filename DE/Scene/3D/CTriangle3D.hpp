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

#ifndef __CTRIANGLE3D_HPP
#define __CTRIANGLE3D_HPP

#include "../../Core/Utility/CVector2.hpp"
#include "../../Core/Utility/CVector3.hpp"
#include "../../Core/Utility/CVector4.hpp"
#include "../../Medias/CTexture2D.hpp"
#include "../../Medias/CMaterial.hpp"
#include "../../Scene/3D/CPlane3D.hpp"
#include "../../Scene/3D/CSphere.hpp"

namespace DE {
	/**
	* Handle a 3d triangle
	*/
	class CTriangle3D {
	protected:
		int m_id;
		CVector3F m_vertices[3];
		CVector3F m_normals[3];
		CVector3F m_tangents[3];
		CVector3F m_binormals[3];
		CVector3F m_flatNormal;
		CVector2F m_texcoords[3];
		CVector4F m_colors[3];
		CTexture2D m_diffuseTexture;
		CTexture2D m_normalTexture;
		CMaterial m_material;
		CSphereF m_circumSphere;
		std::string m_materialElement;

	public:
		/**
		* Constructor
		*/
		CTriangle3D();

		/**
		* Constructor
		* @param triangle The triangle to copy
		*/
		CTriangle3D(const CTriangle3D & triangle);

		/**
		* Set the id 
		*/
		void SetId(const int id);

		/**
		* Set a vertex
		* @param vertex A vertex
		* @param index The index
		*/
		void SetVertex(const CVector3F & vertex, const int index);

		/**
		* Set a normal
		* @param normal A normal
		* @param index The index
		*/
		void SetNormal(const CVector3F & normal, const int index);

		/**
		* Set a texcoord
		* @param texcoord A texcoord
		* @param index The index
		*/
		void SetTexcoord(const CVector2F & texcoord, const int index);

		/**
		* Set a color
		* @param color A color
		* @param index The index
		*/
		void SetColor(const CVector4F & color, const int index);

		/**
		* Set a texture
		* @param texture A texture
		* @param type The texture type
		*/
		void SetTexture(const CTexture2D & texture, ETextureType type);

		/**
		* Set the material
		* @param material The material
		* @param elementName elementName The element name in the material
		*/
		void SetMaterial(const CMaterial & material, const std::string & elementName = "default");

		/**
		* Set the flat normal
		* @param normal The flat normal
		*/
		void SetFlatNormal(const CVector3F & normal);

		/**
		* Update inner data
		*/
		void Update();

		/**
		* Get the id
		*/
		const int GetId() const;

		/**
		* Get a vertex
		* @param index The index
		* @return A vertex
		*/
		const CVector3F & GetVertex(const int index) const;

		/**
		* Get a normal
		* @param index The index
		* @return A normal
		*/
		const CVector3F & GetNormal(const int index) const;

		/**
		* Get a tangent
		* @param index The index
		* @return A tangent
		*/
		const CVector3F & GetTangent(const int index) const;

		/**
		* Get a binormal
		* @param index The index
		* @return A binormal
		*/
		const CVector3F & GetBinormal(const int index) const;

		/**
		* Get the flat normal
		* @return The flat normal
		*/
		const CVector3F & GetFlatNormal() const;

		/**
		* Get a texcoord
		* @param index The index
		* @return A texcoord
		*/
		const CVector2F & GetTexcoord(const int index) const;

		/**
		* Get a color
		* @param index The index
		* @return A color
		*/
		const CVector4F & GetColor(const int index) const;

		/**
		* Get the incenter (center of gravity) of the triangle
		* @return The incenter
		*/
		const CVector3F GetIncenter() const;

		/**
		* Get the circum Sphere of the triangles
		* Used in the engine to encompass the triangle in a visibility sphere
		*/
		CSphereF & GetCircumSphere();

		/**
		* Get a texture
		* @param type The texture type
		* @return A texture
		*/
		ITexture2DBase * GetTexture(ETextureType type) const;

		/**
		* Get the material
		* @return The material
		*/
		SMaterialElement * GetMaterialElement() const;
	};
}

#endif
