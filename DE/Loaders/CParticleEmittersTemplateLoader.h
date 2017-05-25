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

#ifndef __CPARTICLEEMITTERSTEMPLATELOADER_H
#define __CPARTICLEEMITTERSTEMPLATELOADER_H

#include <string>
#include <TinyXML2/tinyxml2.h> 
#include "../Core/ILoader.h"
#include "../Scene/Particles/IParticleEmittersTemplate.h"

namespace DE {
	/**
	* Loader for particle emitters template file (.dept)
	*/
	class CParticleEmittersTemplateLoader : public ILoader < IParticleEmittersTemplate > {
	private:
		std::string m_filename;
		std::map<std::string, CParticleEmitter*> m_emittersTemplate;

	public:
		/**
		* Constructor
		*/
		CParticleEmittersTemplateLoader();

		/**
		* Destructor
		*/
		virtual ~CParticleEmittersTemplateLoader();

		/**
		* Load an obj mesh from a file
		* @param filename The obj file path
		* @return The mesh
		*/
		IParticleEmittersTemplate * LoadFromFile(const std::string & filename) override;

	private:
		/**
		* Load the particle emitter template
		* @param handle The handle
		*/
		void LoadParticleEmitterNode(tinyxml2::XMLHandle & handle);

		/**
		* Load a particle emitter initializers
		* @param emitter The emitter 
		* @param initializersElement The initializers
		*/
		void LoadParticleEmitterInitializers(CParticleEmitter * emitter, tinyxml2::XMLElement * initializersElement);

		/**
		* Load a particle emitter initializers
		* @param emitter The emitter
		* @param updatersElement The initializers
		*/
		void LoadParticleEmitterUpdaters(CParticleEmitter * emitter, tinyxml2::XMLElement * updatersElement);

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
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector2F & data, const CVector2F & defaultValue);

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
	};
}

#endif
