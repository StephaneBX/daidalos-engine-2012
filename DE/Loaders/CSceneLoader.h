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

#include <TinyXML/tinyxml.h>
#include <string>
#include <map>
#include "../Core/ILoader.h"
#include "../Core/IRenderer.h"
#include "../Core/CConfigurationManager.h"
#include "../Scene/Structure/BSP/CBSPSceneNode.h"
#include "../Scene/Structure/Simple/CSimpleSceneNode.h"
#include "../Scene/ISceneNode.h"

namespace DE
{
	/**
	* Loader for daidalos engine scene files (.des)
	*/
	class CSceneLoader : public ILoader<ISceneNode>
	{
	private:
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
		~CSceneLoader();

		/**
		* Load a scene from a file
		* @param filename The scene filename
		* @return The scene
		*/
		ISceneNode * LoadFromFile(const std::string & filename);

		/**
		* Save a scene into a file
		* @param root The scene root
		* @param filename The output file
		*/
		void SaveToFile(ISceneNode * root, const std::string & filename);

	private:
		/**
		* Load a node by reading its xml declaration
		* @param parent The node parent
		* @param xmlElement The xml element describing the node
		*/
		void LoadNode(ISceneNode *& parent, const TiXmlHandle & handle);

		/**
		* Load a BSP structure
		* @param parent The node parent
		* @param child The bsp node root
		* @param node The node to fill
		*/
		void LoadBSPStructure(ISceneNode *& parent, const TiXmlHandle & handle, ISceneNode *& node);

		/**
		* Load a simple structure
		* @param parent The node parent
		* @param child The bsp node root
		* @param node The node to fill
		*/
		void LoadSimpleStructure(ISceneNode *& parent, const TiXmlHandle & handle, ISceneNode *& node);

		/**
		* Add data into the bsp brush definition by reading the handle
		* @param handle The handle for the data
		* @param brushDefinition The brush definition
		*/
		void AddBSPBrushData(const TiXmlHandle & handle, SBSPBrushDefinition & brushDefinition);

		/**
		* Add data into the simple brush by reading the handle
		* @param handle The handle for the data
		* @param brushDefinition The brush definition
		*/
		void AddSimpleBrushData(const TiXmlHandle & handle, SSimpleBrush & brush);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(const TiXmlHandle & handle, const std::string & name, bool & data, const bool defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(const TiXmlHandle & handle, const std::string & name, int & data, const int defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(const TiXmlHandle & handle, const std::string & name, CVector3F & data, const CVector3F & defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(const TiXmlHandle & handle, const std::string & name, CVector4F & data, const CVector4F & defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(const TiXmlHandle & handle, const std::string & name, CColor & data, const CColor & defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(const TiXmlHandle & handle, const std::string & name, float & data, const float defaultValue);

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(const TiXmlHandle & handle, const std::string & name, std::string & data, const std::string & defaultValue);

		/**
		* Get the attributes map to a given child
		* @param handle The handle
		* @return The attributes map
		*/
		const std::map<std::string, std::string> GetNodeAttributes(const TiXmlHandle & handle);
	};
}

#endif
