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

#ifndef __C3DSCENE_H
#define __C3DSCENE_H

#include "../../Scene/3D/I3DScene.h"

namespace DE {
	/**
	* Handle a 3d scene
	*/
	class C3DScene {
	private:
		I3DScene * m_scene;

	public:
		/**
		* Constructor
		*/
		C3DScene();

		/**
		* Destructor
		*/
		~C3DScene();

		/**
		* Load a Daidalos Engine Scene File (.des)
		* @param sceneFile The des file
		*/
		void LoadScene(const std::string & sceneFile);

		/**
		* Save a Daidalos Engine Scene file (.des)
		* @param destination The destination
		*/
		void SaveScene(const std::string & destination);

		/**
		* Clear the scene
		*/
		void ClearAll();

		/**
		* Add a node to the scene
		* @param parent The parent node or nullptr
		* @param node The node
		*/
		void AddSceneNode(ISceneNode * parent, ISceneNode * node);

		/**
		* Set the active camera
		* @param camera The active camera
		*/
		void SetActiveCamera(ICameraSceneNode * camera);

		/**
		* Update all the nodes
		* /!\ Also perform view test based on the currently active camera
		* @param time The elapsed time
		*/
		void UpdateAll(const float time = 0);

		/**
		* Render the geometry that will let the skybox pass through
		*/
		void RenderSkyboxMask();

		/**
		* Render the skybox
		*/
		void RenderSkybox();

		/**
		* Render all the nodes
		*/
		void RenderAll();

		/**
		* Render all the transparent nodes
		*/
		void RenderTransparent();

		/**
		* Render the debug information
		*/
		void RenderDebugInformation();

		/**
		* Set the mesh format for all nodes
		* @param format The mesh format
		*/
		void SetMeshFormat(const int format);

		/**
		* Send a spot light data to the current shader
		* @param id The light id
		*/
		void SendSpotLightDataToShader(const int id);

		/**
		* Send a point light data to the current shader
		* @param id The light id
		*/
		void SendPointLightDataToShader(const int id);

		/**
		* Send a directional light data to the current shader
		* @param id The light id
		*/
		void SendDirectionalLightDataToShader(const int id);

		/**
		* Set the collision state of a brush id
		*/
		void SetBrushCollision(const int brushId, const bool collision);

		/**
		* Slide a shape on the scene
		* @param shape The shape to slide
		* @param movement A CVector3F describing the shape movement
		* @return True if a collision occurred
		*/
		const bool Slide(IShape3DF & shape, const CVector3F & movement);

		/**
		* Get the collision point of a moving point
		* @param position The point
		* @param movement A CVector3F describing the shape movement
		* @param point The CVector3F that will hold the collision position
		* @return True if a collision occurred
		*/
		const bool GetCollisionPoint(const CVector3F & position, const CVector3F & movement, CVector3F & point);

		/**
		* Get the collision point of a moving shape
		* @param shape The shape
		* @param movement A CVector3F describing the shape movement
		* @param point The CVector3F that will hold the collision position
		* @return True if a collision occurred
		*/
		const bool GetCollisionPoint(IShape3DF & shape, const CVector3F & movement, CVector3F & point);

		/**
		* Get the number of active spot lights
		* @return The number of active spot lights
		*/
		const int GetNumberOfActiveSpotLights() const;

		/**
		* Get the number of active point lights
		* @return The number of active point lights
		*/
		const int GetNumberOfActivePointLights() const;

		/**
		* Get the number of active directional lights
		* @return The number of active spot lights
		*/
		const int GetNumberOfActiveDirectionalLights() const;

		/**
		* Get the number of visible triangles
		* @return The number of visible triangles
		*/
		const int GetNumberOfVisibleTriangles() const;

		/**
		* Get the scene filename
		* @return The scene filename
		*/
		const std::string & GetFilename() const;

		/**
		* Get the ambient color of the scene
		* @return The ambient color
		*/
		const CVector4F & GetAmbientColor() const;

		/**
		* Get the sky ambient color of the scene
		* @return The sky ambient color
		*/
		const CVector4F & GetSkyAmbientColor() const;

		/**
		* Get the scene bounding box
		* @return The scene bounding box
		*/
		const CAABoundingBoxF GetSceneBoundingBox();

		/**
		* Get the particle system
		* @return The particle system
		*/
		CParticleSystem & GetParticleSystem();

		/**
		* Get the first node found with the given name
		* @param name The node name
		* @return A node or nullptr if no node is found
		*/
		ISceneNode * GetNode(const std::string & name) const;

		/**
		* Get the scene
		* @return The scene
		*/
		I3DScene * GetScene() const;

		/**
		* Get all the nodes with the given name
		* @param name The node name
		* @return A vector containing all the nodes with the given name
		*/
		std::vector<ISceneNode*> GetNodeArray(const std::string & name) const;

		/**
		* Get all the nodes with the given type
		* @param nodeType The node type
		* @return A vector containing all the nodes with the given type
		*/
		std::vector<ISceneNode*> GetNodeArray(ESceneNodeType nodeType) const;

		/**
		* Get the root node
		@return The root node
		*/
		ILevelSceneNode * GetRootNode() const;

		/**
		* Get the active camera
		* @return The active camera
		*/
		ICameraSceneNode * GetActiveCamera() const;

		/**
		* Get the active spot lights buffer
		* @return The active spot lights buffer
		*/
		SSPotLightsShaderData * GetActiveSpotLightsBuffer() const;

		/**
		* Get the active point lights buffer
		* @return The active point lights buffer
		*/
		SPointLightsShaderData * GetActivePointLightsBuffer() const;

		/**
		* Get the active directional lights buffer
		* @return The active directional lights buffer
		*/
		SDirectionalLightsShaderData * GetActiveDirectionalLightsBuffer() const;

	private:
		/**
		* Constructor
		*/
		C3DScene(const C3DScene & copy);
		const C3DScene & operator=(const C3DScene & copy);
	};
}

#endif
