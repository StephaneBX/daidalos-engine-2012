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

#ifndef __IRESOURCE_HPP
#define __IRESOURCE_HPP

#include <string>
#include "../Core/Utility/CFile.hpp"

namespace daidalosengine {
	/**
	* Basic class used for every resources
	*/
	class IResource {
	private:
		static int s_resourceCounter;

	private:
		int m_referenceCount;
		CFile m_name;

	public:
		/**
		* Constructor
		*/
		IResource();

		/**
		* Constructor
		* @param name The resource name
		*/
		explicit IResource(const std::string & name);

		/**
		* Destructor
		*/
		virtual ~IResource();

		/**
		* Get the ressource name
		* @return The name
		*/
		const std::string GetName() const;

		/**
		* Remove a reference and delete the ressource if not used anymore
		* @return The reference count
		*/
		int Release();

		/**
		* Set the name
		* @param name The name
		*/
		void SetName(const std::string & name);

		/**
		* Add a reference on the ressource
		*/
		void AddReference();

		/**
		* Called when the device is lost
		*/
		virtual void OnDeviceLost();

		/**
		* Called when the device is reset
		*/
		virtual void OnDeviceReset();

	public:
		IResource(const IResource & resource) = delete;
		const IResource & operator=(const IResource & copy) = delete;
	};
}

#endif
