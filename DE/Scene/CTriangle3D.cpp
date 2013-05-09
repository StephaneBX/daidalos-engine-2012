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

#include "../Scene/CTriangle3D.h"

namespace DE
{
	CTriangle3D::CTriangle3D()
	{
		for(int i = 0 ; i < 3 ; ++i)
			this->m_colors[i] = CVector4F(1,1,1,1);

		this->m_material.LoadFromFile("default.mtl");
		this->m_materialElement = "default";
	}

	CTriangle3D::CTriangle3D(const CTriangle3D & triangle)
	{
		*this = triangle;
	}

	void CTriangle3D::SetVertex(const CVector3F & vertex, const int id)
	{
		this->m_vertices[id] = vertex;
	}

	void CTriangle3D::SetNormal(const CVector3F & normal, const int id)
	{
		this->m_normals[id] = normal;
	}

	void CTriangle3D::SetTexcoord(const CVector2F & texcoord, const int id)
	{
		this->m_texcoords[id] = texcoord;
	}

	void CTriangle3D::SetColor(const CVector4F & color, const int id)
	{
		this->m_colors[id] = color;
	}

	void CTriangle3D::SetTexture(const CTexture2D & texture, ETextureType type)
	{
		switch(type)
		{
		case TEXTURE_TYPE_DIFFUSE:
			this->m_diffuseTexture = texture;
			break;

		case TEXTURE_TYPE_NORMAL:
			this->m_normalTexture = texture;
			break;
		}
	}

	void CTriangle3D::SetMaterial(const CMaterial & material, const std::string & elementName)
	{
		this->m_material = material;

		if(this->m_material.GetMaterial()->GetElement(elementName) != nullptr)
			this->m_materialElement = elementName;
		else
			this->m_materialElement = "default";
	}

	void CTriangle3D::SetFlatNormal(const CVector3F & normal)
	{
		this->m_flatNormal = normal;
	}

	void CTriangle3D::Update()
	{
		// Circum sphere
		float alpha, beta, gamma;
		
		alpha	= (std::pow((this->m_vertices[1] - this->m_vertices[2]).Length(), 2) * CVector3F::DotProduct(this->m_vertices[0] - this->m_vertices[1], this->m_vertices[0] - this->m_vertices[2])) / (2 * std::pow(CVector3F::CrossProduct(this->m_vertices[0] - this->m_vertices[1], this->m_vertices[1] - this->m_vertices[2]).Length(), 2));
		beta	= (std::pow((this->m_vertices[0] - this->m_vertices[2]).Length(), 2) * CVector3F::DotProduct(this->m_vertices[1] - this->m_vertices[0], this->m_vertices[1] - this->m_vertices[2])) / (2 * std::pow(CVector3F::CrossProduct(this->m_vertices[0] - this->m_vertices[1], this->m_vertices[1] - this->m_vertices[2]).Length(), 2));
		gamma	= (std::pow((this->m_vertices[0] - this->m_vertices[1]).Length(), 2) * CVector3F::DotProduct(this->m_vertices[2] - this->m_vertices[0], this->m_vertices[2] - this->m_vertices[1])) / (2 * std::pow(CVector3F::CrossProduct(this->m_vertices[0] - this->m_vertices[1], this->m_vertices[1] - this->m_vertices[2]).Length(), 2));
		
		this->m_circumSphere.SetPosition(alpha * this->m_vertices[0] + beta * this->m_vertices[1] + gamma * this->m_vertices[2]);
		this->m_circumSphere.SetRadius((this->m_circumSphere.GetPosition() - this->m_vertices[0]).Length() * 1.5f);

		// Update the tangents and binormals
		CVector3F ab = this->m_vertices[1] - this->m_vertices[0];
		CVector3F ac = this->m_vertices[2] - this->m_vertices[0];
		CVector2F abuv = this->m_texcoords[1] - this->m_texcoords[0];
		CVector2F acuv = this->m_texcoords[2] - this->m_texcoords[0];
		float fDenominator = acuv.y * abuv.x - abuv.y * acuv.x;

		for(int j = 0 ; j < 3 ; ++j)
		{
			if(fDenominator >= -EPSILON && fDenominator <= EPSILON)
			{
				this->m_tangents[j] = CVector3F(1,0,0);
				this->m_binormals[j] = this->m_tangents[j].CrossProduct(this->m_normals[j]).Normalize();
			}
			else
			{
				float fScale1 = 1.0f / fDenominator;
				this->m_tangents[j] = ((acuv.y * ab - abuv.y * ac) * fScale1).Normalize();

				// Gram-Schmidt orthogonalization
				this->m_tangents[j] -= (this->m_normals[j] * this->m_tangents[j].DotProduct(this->m_normals[j])).Normalize();
				this->m_binormals[j] = this->m_tangents[j].CrossProduct(this->m_normals[j]).Normalize();
			}
		}
	}

	const CVector3F & CTriangle3D::GetVertex(const int id) const
	{
		return this->m_vertices[id];
	}

	const CVector3F & CTriangle3D::GetNormal(const int id) const
	{
		return this->m_normals[id];
	}

	const CVector3F & CTriangle3D::GetTangent(const int id) const
	{
		return this->m_tangents[id];
	}

	const CVector3F & CTriangle3D::GetBinormal(const int id) const
	{
		return this->m_binormals[id];
	}

	const CVector3F & CTriangle3D::GetFlatNormal() const
	{
		return this->m_flatNormal;
	}

	const CVector2F & CTriangle3D::GetTexcoord(const int id) const
	{
		return this->m_texcoords[id];
	}

	const CVector4F & CTriangle3D::GetColor(const int id) const
	{
		return this->m_colors[id];
	}

	const CVector3F CTriangle3D::GetIncenter() const
	{
		return (this->m_vertices[0] + this->m_vertices[1] + this->m_vertices[2]) / 3.0f;
	}

	CSphereF & CTriangle3D::GetCircumSphere()
	{
		return this->m_circumSphere;
	}

	ITexture2DBase * CTriangle3D::GetTexture(ETextureType type) const
	{
		switch(type)
		{
		case TEXTURE_TYPE_DIFFUSE:
			return this->m_diffuseTexture.GetTexture();
			break;

		case TEXTURE_TYPE_NORMAL:
			return this->m_normalTexture.GetTexture();
			break;

		default:
			return this->m_diffuseTexture.GetTexture();
			break;
		}
	}

	SMaterialElement * CTriangle3D::GetMaterialElement() const
	{
		return this->m_material.GetMaterial()->GetElement(this->m_materialElement);
	}
}