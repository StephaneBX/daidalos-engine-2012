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

#ifndef __ISCENE_H
#define __ISCENE_H

#include "../Core/IRenderer.h"
#include "../Core/CConfigurationManager.h"
#include "../Scene/ICameraSceneNode.h"
#include "../Scene/ILevelSceneNode.h"
#include "../Scene/CStaticMeshSceneNode.h"
#include "../Scene/CAnimatedMeshSceneNode.h"
#include "../Scene/CAABoundingBox.h"
#include "../Scene/CSkybox.h"
#include "../Core/CSingletonManager.h"

#define MAX_LIGHTS_IN_SHADERS 200

namespace DE {
	/**
	* Spot lights data structure for shaders.
	* Also used by the point lights
	*/
	struct SSPotLightsShaderData {
		int numberOfLights;
		int enabled[MAX_LIGHTS_IN_SHADERS];
		int shadowCaster[MAX_LIGHTS_IN_SHADERS];
		CVector4F position[MAX_LIGHTS_IN_SHADERS];
		CVector4F direction[MAX_LIGHTS_IN_SHADERS];
		CVector4F rangeRadiusFalloffTightness[MAX_LIGHTS_IN_SHADERS];
		CVector4F color[MAX_LIGHTS_IN_SHADERS];
		CRectangleI scissor[MAX_LIGHTS_IN_SHADERS];
		ICameraSceneNode * node[MAX_LIGHTS_IN_SHADERS];

		// Visibility data
		int visibilityOrder[MAX_LIGHTS_IN_SHADERS];
		float distanceToCamera[MAX_LIGHTS_IN_SHADERS];
	};

	/**
	* Point lights data structure for shaders
	*/
	struct SPointLightsShaderData {
		int numberOfLights;
		int enabled[MAX_LIGHTS_IN_SHADERS];
		int shadowCaster[MAX_LIGHTS_IN_SHADERS];
		CVector4F positionRange[MAX_LIGHTS_IN_SHADERS];
		CVector4F colorFalloff[MAX_LIGHTS_IN_SHADERS];
		CRectangleI scissor[MAX_LIGHTS_IN_SHADERS];

		// Visibility data
		int visibilityOrder[MAX_LIGHTS_IN_SHADERS];
		float distanceToCamera[MAX_LIGHTS_IN_SHADERS];
	};

	/**
	* Directional lights data structure for shaders
	*/
	struct SDirectionalLightsShaderData {
		int numberOfLights;
		int enabled[MAX_LIGHTS_IN_SHADERS];
		int shadowCaster[MAX_LIGHTS_IN_SHADERS];
		CVector4F position[MAX_LIGHTS_IN_SHADERS];
		CVector4F direction[MAX_LIGHTS_IN_SHADERS];
		CVector4F color[MAX_LIGHTS_IN_SHADERS];
		ICameraSceneNode * node[MAX_LIGHTS_IN_SHADERS];
	};

	/**
	* Handle the scene
	*/
	class IScene {
	private:
		float m_lighteningUpdateTimer;
		SSPotLightsShaderData * m_activeSpotLights;
		SPointLightsShaderData * m_activePointLights;
		SDirectionalLightsShaderData * m_activeDirectionalLights;

		CSkybox m_skybox;
		ISceneNode * m_root;
		ICameraSceneNode * m_activeCamera;
		SiniP m_options;

		CShaderProgram m_debugShader;
		DeclarationPtr m_debugVertexDeclaration;

	public:
		/**
		* Constructor
		*/
		IScene();

		/**
		* Destructor
		*/
		~IScene();

		/**
		* Clear the scene
		*/
		void ClearAll();

		/**
		* Set the root node
		*/
		void SetRoot(ISceneNode * root);

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
		* Render the skybox
		*/
		void RenderSkybox();

		/**
		* Render all the nodes
		*/
		void RenderAll();

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
		* Slide a shape on the scene
		* @param shape The shape to slide
		* @param movement A CVector3F describing the shape movement
		* @return True if a collision occurred
		*/
		const bool Slide(IShape3DF & shape, const CVector3F & movement);

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
		* Get the root node
		*/
		ISceneNode * GetRoot() const;

		/**
		* Get the first node found with the given name
		* @param name The node name
		* @return A node or nullptr if no node is found
		*/
		ISceneNode * GetNode(const std::string & name) const;

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
		* Order the lights in order the allow the most relevant one to be visible
		*/
		void SortLights();

		/**
		* Make sure that the number of active lights do not exceed the values in the configuration file
		*/
		void ActiveLightsCheck();

		/**
		* Find all the nodes with a given name and store them in a vector
		* @param currentNode The current node
		* @param name The node name
		* @param vector The vector to store the nodes into
		*/
		void FindAllNodes(ISceneNode * currentNode, const std::string & name, std::vector<ISceneNode*> & vector) const;

		/**
		* Find all the nodes with a given name and store them in a vector
		* @param currentNode The current node
		* @param nodeType The node type
		* @param vector The vector to store the nodes into
		*/
		void FindAllNodes(ISceneNode * currentNode, ESceneNodeType nodeType, std::vector<ISceneNode*> & vector) const;

		/**
		* Find a node with the given name
		* @param currentNode The current node
		* @param name The node name
		* @return A node or nullptr if no node is found
		*/
		ISceneNode * FindNode(ISceneNode * currentNode, const std::string & name) const;

	private:
		/**
		* Constructor
		*/
		IScene(const IScene & copy);
		const IScene & operator=(const IScene & copy);
	};
}

#endif
