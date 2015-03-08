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

#ifndef __IRENDERTARGETBASE_H
#define __IRENDERTARGETBASE_H

#include <string>
#include "../Core/IResource.h"

namespace DE {
	enum ERenderTargetType {
		RENDER_TARGET_TYPE_COLOR,
		RENDER_TARGET_TYPE_DEPTH
	};

	/**
	* Basic class used for render targets
	*/
	class IRenderTargetBase : public IResource {
	private:
		ERenderTargetType m_type;

	public:
		/**
		* Destructor
		*/
		virtual ~IRenderTargetBase();

		/**
		* Get the render target type
		* @return The type
		*/
		ERenderTargetType GetType() const;

	protected:
		/**
		* Constructor
		* @param type The render target type
		*/
		IRenderTargetBase(ERenderTargetType type);

	private:
		IRenderTargetBase();
		IRenderTargetBase(const IRenderTargetBase & copy);
		const IRenderTargetBase & operator=(const IRenderTargetBase & copy);
	};
}

#endif
