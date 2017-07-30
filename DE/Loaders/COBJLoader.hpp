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

#ifndef __COBJLOADER_HPP
#define __COBJLOADER_HPP

#include <string>
#include "../Core/ILoader.hpp"
#include "../Medias/COBJMesh.hpp"

namespace DE {
	/**
	* Loader for wavefront files (.obj)
	*/
	class COBJLoader : public ILoader < IMeshBase > {
	private:
		std::string m_filename;
		SOBJInternalData * m_internalData;

	public:
		/**
		* Constructor
		*/
		COBJLoader();

		/**
		* Destructor
		*/
		virtual ~COBJLoader();

		/**
		* Load an obj mesh from a file
		* @param filename The obj file path
		* @return The mesh
		*/
		virtual IMeshBase * LoadFromFile(const std::string & filename) override;

	private:
		/**
		* Fill the final buffers with the corresponding information.
		* When the list of vertices, normals, textures coordinates and the faces order are read from the file, this fonction create the associated data.
		* @param tmpVertex A vector containing the vertices
		* @param tmpNormal A vector containing the normals
		* @param tmpTexture A vector containing the textures
		* @param vertexOrder A vector containing the order of the vertices to use
		* @param normalOrder A vector containing the order of the normal to use
		* @param textureOrder A vector containing the order of the texture to use
		* @param groupCounter The group number for this data
		*/
		void FillBuffers(const std::vector<CVector3F> & tmpVertex, const std::vector<CVector3F> & tmpNormal, const std::vector<CVector2F> & tmpTexture, const std::vector<int> vertexOrder, const std::vector<int> normalOrder, const std::vector<int> & textureOrder, const int groupCounter);
	};
}

#endif
