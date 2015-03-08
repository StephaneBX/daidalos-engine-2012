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

#ifndef __CRESOURCEMANAGER_H
#define __CRESOURCEMANAGER_H

#include <map>
#include "../Core/IResource.h"
#include "../Core/CSingletonManager.h"

namespace DE {
	/**
	* Resource manager
	* Handle the resources and delete them when unused
	*/
	class CResourceManager : public CSingletonManager < CResourceManager > {
		friend class CSingletonManager < CResourceManager > ;

	private:
		std::map<std::string, IResource *> m_resources;

	public:
		/**
		* Destructor
		*/
		virtual ~CResourceManager();

		/**
		* Get a resource or nullptr if it's not present in the registered resources
		* @param name The resource name
		* @return The resource or nullptr if it's not present in the database
		*/
		template <class T> T * GetResource(const std::string & name) const;

		/**
		* Add a resource to the database
		* @param name The name of the resource
		* @param resource The resource
		*/
		void AddResource(const std::string & name, IResource * resource);

		/**
		* Release a resource if its reference count drop to 0
		* @param name The resource name
		*/
		void ReleaseResource(const std::string & name);

		/**
		* Release a resource if its reference count drop to 0
		* @param resource The resource
		*/
		void ReleaseResource(IResource * resource);

	private:
		/**
		* Constructor
		*/
		CResourceManager();
		CResourceManager(const CResourceManager & copy);
		const CResourceManager & operator=(const CResourceManager & copy);
	};

	template <class T> T * CResourceManager::GetResource(const std::string & name) const {
		T * ret = nullptr;

		auto it = this->m_resources.find(name);

		// Resource found, we add a reference and return it
		if(it != m_resources.end()) {
			it->second->AddReference();
			ret = static_cast<T *>(it->second);
		}

		return ret;
	}
}

#endif
