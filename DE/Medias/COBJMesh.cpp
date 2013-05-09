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

#include <algorithm>
#include "../Core/Core.h"
#include "../Medias/COBJMesh.h"

namespace DE
{
	COBJMesh::COBJMesh(const std::string & name, SOBJInternalData * data) : IMeshBase(name)
	{
		this->m_internalData = data;

		this->m_vertex = new CVector3F*[this->m_internalData->groupSize.size()];
		this->m_normal = new CVector3F*[this->m_internalData->groupSize.size()];
		this->m_tangent = new CVector3F*[this->m_internalData->groupSize.size()];
		this->m_binormal = new CVector3F*[this->m_internalData->groupSize.size()];
		this->m_texcoord = new CVector2F*[this->m_internalData->groupSize.size()];

		// Copy the data
		this->m_groupSize = this->m_internalData->groupSize;
		for(unsigned int i = 0 ; i < this->m_internalData->groupSize.size() ; ++i)
		{
			this->m_vertex[i] = new CVector3F[this->m_internalData->groupSize[i]];
			this->m_normal[i] = new CVector3F[this->m_internalData->groupSize[i]];
			this->m_tangent[i] = new CVector3F[this->m_internalData->groupSize[i]];
			this->m_binormal[i] = new CVector3F[this->m_internalData->groupSize[i]];
			this->m_texcoord[i] = new CVector2F[this->m_internalData->groupSize[i]];

			memcpy(this->m_vertex[i], this->m_internalData->vertex[i], sizeof(CVector3F) * this->m_internalData->groupSize[i]);
			memcpy(this->m_texcoord[i], this->m_internalData->texcoord[i], sizeof(CVector2F) * this->m_internalData->groupSize[i]);

			if(this->m_internalData->normal[i] == nullptr)
			{
				// If no normals are present, calculate them (flat)
				for(int j = 0 ; j < this->m_internalData->groupSize[i] ; j += 3)
				{
					CVector3F tmp = this->m_internalData->vertex[i][j + 1] - this->m_internalData->vertex[i][j];
					CVector3F normal = tmp.CrossProduct(this->m_internalData->vertex[i][j + 2] - this->m_internalData->vertex[i][j]).Normalize();

					for(int k = 0 ; k < 3 ; ++k)
						this->m_normal[i][j + k] = normal;
				}
			}
			else
				memcpy(this->m_normal[i], this->m_internalData->normal[i], sizeof(CVector3F) * this->m_internalData->groupSize[i]);
			
		}

		// Visibilty shape calculation
		CVector3F min, max;
		for(unsigned int i = 0 ; i < this->m_internalData->groupSize.size() ; ++i)
		{
			for(int j = 0 ; j < this->m_internalData->groupSize[i] ; ++j)
			{
				max.x = std::max(max.x, this->m_vertex[i][j].x);
				max.y = std::max(max.y, this->m_vertex[i][j].y);
				max.z = std::max(max.z, this->m_vertex[i][j].z);

				min.x = std::min(min.x, this->m_vertex[i][j].x);
				min.y = std::min(min.y, this->m_vertex[i][j].y);
				min.z = std::min(min.z, this->m_vertex[i][j].z);
			}
		}

		// Find the position
		this->m_visibilityShape->SetPosition((max + min) / 2.0f);

		// Find the radius
		this->m_visibilityShape->SetRadius((max - ((max + min) / 2.0f)).Length());

		TangentAndBinormalCalculation();
	}

	COBJMesh::~COBJMesh() 
	{
		for(unsigned int i = 0 ; i < this->m_internalData->groupSize.size() ; ++i)
		{
			if(this->m_internalData->vertex != nullptr)
				SAFE_DELETE_ARRAY(this->m_internalData->vertex[i]);
		
			if(this->m_internalData->normal != nullptr)
				SAFE_DELETE_ARRAY(this->m_internalData->normal[i]);

			if(this->m_internalData->texcoord != nullptr)
				SAFE_DELETE_ARRAY(this->m_internalData->texcoord[i]);
		}

		SAFE_DELETE_ARRAY(this->m_internalData->vertex);
		SAFE_DELETE_ARRAY(this->m_internalData->normal);
		SAFE_DELETE_ARRAY(this->m_internalData->texcoord);
		SAFE_DELETE(this->m_internalData);
	}

	void COBJMesh::FinalizeGeometry(EMeshShadingType shadingType) 
	{
		switch(shadingType)
		{
		case MESH_SHADING_FLAT:
			FlatShadingNormals();
			break;

		case MESH_SHADING_SMOOTH:
			for(unsigned int i = 0 ; i < this->m_internalData->groupSize.size() ; ++i)
				memcpy(this->m_normal[i], this->m_internalData->normal[i], sizeof(CVector3F) * this->m_internalData->groupSize[i]);
			break;
		}
	}
}
