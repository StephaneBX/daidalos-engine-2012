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

#ifndef __CEDITORSCENELOADER_HPP
#define __CEDITORSCENELOADER_HPP

#include <Loaders/CSceneLoader.hpp>

namespace daidaloseditor {
	/**
	* Loader for daidalos engine scene files (.des), modified for the editor
	*/
	class CEditorSceneLoader : public daidalosengine::CSceneLoader {

	public:

	protected:
		/**
		* Override and use a simple node system
		* @param parent The node parent
		* @param child The bsp node root
		* @param node The node to fill
		*/
		virtual void LoadBSPStructure(daidalosengine::I3DScene * scene, daidalosengine::ISceneNode *& parent, tinyxml2::XMLHandle & handle, daidalosengine::ISceneNode *& node);

		/**
		* Override and use a simple node system
		* @param parent The node parent
		* @param child The bsp node root
		* @param node The node to fill
		*/
		virtual void LoadSimpleStructure(daidalosengine::I3DScene * scene, daidalosengine::ISceneNode *& parent, tinyxml2::XMLHandle & handle, daidalosengine::ISceneNode *& node);

		/**
		* Load a sound node
		* @param parent The node parent
		* @param child The bsp node root
		* @param node The node to fill
		*/
		virtual void LoadSoundNode(daidalosengine::I3DScene * scene, daidalosengine::ISceneNode *& parent, tinyxml2::XMLHandle & handle, daidalosengine::ISceneNode *& node, const std::string & name);

		/**
		* Save the simple structure
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveSimpleStructure(daidalosengine::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

		/**
		* Save the bsp structure
		* @param node The node
		* @param document The xml document
		* @param parent The element parent
		*/
		virtual void SaveBSPStructure(daidalosengine::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);

	private:
		/**
		* Replace the bsp and simple structure with an editor specific one
		* @param parent The node parent
		* @param child The bsp node root
		* @param node The node to fill
		*/
		void LoadEditorLevelStucture(daidalosengine::I3DScene * scene, daidalosengine::ISceneNode *& parent, tinyxml2::XMLHandle & handle, daidalosengine::ISceneNode *& node);

		/**
		* Save a node as a brush
		* @param parent The node parent
		* @param child The bsp node root
		* @param node The node to fill
		*/
		void SaveBrush(daidalosengine::ISceneNode * node, tinyxml2::XMLDocument & document, tinyxml2::XMLElement * parent);
	};
}

#endif
