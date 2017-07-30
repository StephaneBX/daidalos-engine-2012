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

#ifndef __CDX9DECLARATION_HPP
#define __CDX9DECLARATION_HPP

#include <string>
#include "../../Core/IDeclaration.hpp"

struct IDirect3DVertexDeclaration9;

namespace DE {
	/**
	* CDX9Declaration class.
	* DX9 vertex declaration class.
	*/
	class CDX9Declaration : public IDeclaration {
	private:
		IDirect3DVertexDeclaration9 * m_declaration;

	public:
		/**
		* Constructor
		* @param declaration The DX9 declaration
		*/
		CDX9Declaration(IDirect3DVertexDeclaration9 * declaration);

		/**
		* Destructor
		*/
		~CDX9Declaration();

		/**
		* Get the DX9 declaration
		* @return The DX9 decalration
		*/
		IDirect3DVertexDeclaration9 * GetDeclaration() const;

	private:
		CDX9Declaration();
		CDX9Declaration(const CDX9Declaration & copy);
		const CDX9Declaration & operator=(const CDX9Declaration & copy);
	};
}

#endif
