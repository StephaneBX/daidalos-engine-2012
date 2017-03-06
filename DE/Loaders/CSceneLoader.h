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

#ifndef __CSCENELOADER_H
#define __CSCENELOADER_H

#include <TinyXML2/tinyxml2.h>
#include <string>
#include <map>
#include "../Core/ILoader.h"
#include "../Core/IRenderer.h"
#include "../Core/CConfigurationManager.h"
#include "../Scene/3D/Structure/BSP/CBSPSceneNode.h"
#include "../Scene/3D/Structure/Simple/CSimpleSceneNode.h"
#include "../Scene/3D/ISceneNode.h"

namespace DE {

	/**
	* Loader for daidalos engine scene files (.des)
	*/
	class CSceneLoader : public ILoader < I3DScene > {
	protected:
		std::string m_filename;
		ESceneNodeRootStructureType m_structure;
		SiniP m_nodesConf;
		SiniP m_materialsConf;
		IRenderer * m_renderer;

	public:
		/**
		* Constructor
		*/
		CSceneLoader();

		/**
		* Destructor
		*/
		virtual ~CSceneLoader();

		/**
		* Load a scene from a file
		* @param filename The scene filename
		* @return The scene
		*/
		virtual I3DScene * LoadFromFile(const std::string & filename) override;

		/**
		* Save a scene into a file
		* @param object The scene
		* @param filename The output file
		*/
		virtual void SaveToFile(I3DScene * object, const std::string & filename) override;

	protected:
		/**
		* Load a node by reading its xml declaration
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		*/
		virtual void LoadNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle);

		/**
		* Load a BSP structure
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		*/
		virtual void LoadBSPStructure(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node);

		/**
		* Load a simple structure
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		*/
		virtual void LoadSimpleStructure(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node);

		/**
		* Load the particle system declaration
		* @param scene The scene
		* @param handle The xml handle
		*/
		virtual void LoadParticleSystem(I3DScene * scene, tinyxml2::XMLHandle & handle);

		/**
		* Load a default particle system
		* @param scene The scene
		*/
		virtual void LoadDefaultParticleSystem(I3DScene * scene);

		/**
		* Load a camera node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadCameraNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Load a spot light node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadSpotLightNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Load a point light node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadPointLightNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Load a directional light node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadDirectionalLightNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Load a static mesh node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadStaticMeshNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Load a animated mesh node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadAnimatedMeshNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Load a sound node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadSoundNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Load an empty node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadEmptyNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Load a particle emitter node
		* @param scene The scene
		* @param parent The node parent
		* @param handle The xml handle
		* @param node The node to fill
		* @param name The name
		*/
		virtual void LoadParticleEmitterNode(I3DScene * scene, ISceneNode *& parent, tinyxml2::XMLHandle & handle, ISceneNode *& node, const std::string & name);

		/**
		* Add data into the bsp brush definition by reading the handle
		* @param handle The handle for the data
		* @param brushDefinition The brush definition
		*/
		virtual void AddBSPBrushData(tinyxml2::XMLHandle & handle, SBSPBrushDefinition & brushDefinition);

		/**
		* Add data into the bsp brush definition
		* @param file The mesh name
		* @param position The position
		* @param rotation The rotation
		* @param scale The scale
		* @param brushDefinition The brush definition
		*/
		virtual void AddBSPBoundingBoxData(const std::string file, const CVector3F position, const CVector3F rotation, const CVector3F scale, SBSPBrushDefinition & brushDefinition);

		/**
		* Add data into the simple brush by reading the handle
		* @param handle The handle for the data
		* @param brush The brush definition
		*/
		virtual void AddSimpleBrushData(tinyxml2::XMLHandle & handle, SSimpleBrush & brush);

		/**
		* Save a node into an xml element
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save the particle system configuration into an xml element
		* @param document The xml document
		* @param parent The element parent
		* @param scene The scene holding the particle system
		*/
		virtual void SaveParticleSystem(I3DScene * scene, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save the simple structure
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveSimpleStructure(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save the bsp structure
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveBSPStructure(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save a camera node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveCameraNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save a spot light node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveSpotLightNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save a point light node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SavePointLightNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save a directional light node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveDirectionalLightNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save a static mesh node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveStaticMeshNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save an animated mesh node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveAnimatedMeshNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save a sound node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveSoundNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save an empty node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveEmptyNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save the node attributes
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		void SaveNodeAttributes(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save a particle emitter node
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveParticleEmitterNode(ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, bool & data, const bool defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, int & data, const int defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector2I & data, const CVector2I & defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector3F & data, const CVector3F & defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector4F & data, const CVector4F & defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CColor & data, const CColor & defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, float & data, const float defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, std::string & data, const std::string & defaultValue);

		/**
		* Get the attributes map to a given child
		* @param handle The handle
		* @return The attributes map
		*/
		const std::map<std::string, std::string> GetNodeAttributes(tinyxml2::XMLHandle & handle);
	};
}

#endif
