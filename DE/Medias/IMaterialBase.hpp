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

#ifndef __IMATERIALBASE_HPP
#define __IMATERIALBASE_HPP

#include <string>
#include <map>
#include <memory>
#include "../Core/IResource.hpp"
#include "../Core/Utility/CVector3.hpp"

namespace daidalosengine {
	/**
	* Material element structure
	*/
	struct SMaterialElement {
		CVector3F m_kDiffuse;
		CVector3F m_kAmbient;
		CVector3F m_kSpecular;
		CVector3F m_kEmission;
		float m_ns;
	};

	/**
	* Basic class used for materials
	*/
	class IMaterialBase : public IResource {
	public:
		std::map<std::string, std::unique_ptr<SMaterialElement>> m_elements;

	public:
		/**
		* Constructor
		* @param name The resource name
		*/
		explicit IMaterialBase(const std::string & name);

		/**
		* Add an element to the material
		* @param name The element name
		* @param element The element data
		*/
		void AddElement(const std::string & name, SMaterialElement * element);

		/**
		* Get an element or nullptr if not key is found
		* @param name The element name
		* @return An element or nullptr
		*/
		SMaterialElement * GetElement(const std::string & name) const;

	private:
		IMaterialBase();
		IMaterialBase(const IMaterialBase & copy);
		const IMaterialBase & operator=(const IMaterialBase & copy);
	};
}

#endif
