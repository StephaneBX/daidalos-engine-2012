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

#ifndef __CFONT_H
#define __CFONT_H

#include <string>
#include <map>
#include <memory>
#include "../Medias/CTexture2D.h"
#include "../Medias/IFontBase.h"

namespace DE
{
	/**
	* Font enclosing class
	*/
	class CFont
	{
	private:
		IFontBase * m_font;

	public:
		/**
		* Constructor
		*/
		CFont();

		/**
		* Constructor
		* @param filename The file name
		*/
		CFont(const std::string & filename);

		/**
		* Constructor
		* @param copy The font to copy
		*/
		CFont(const CFont & copy);

		/**
		* Destructor
		*/
		~CFont();

		/**
		* Load a font file
		* @param filename The font file name
		*/
		void LoadFromFile(const std::string & filename);

		/**
		* Get the italic value
		* @return The italic value
		*/
		const bool IsBold() const;

		/**
		* Get the italic value
		* @return The italic value
		*/
		const bool IsItalic() const;

		/**
		* Get the size
		* @return The size
		*/
		const int GetSize() const;

		/**
		* Get the line height
		* @return The line height
		*/
		const int GetLineHeight() const;

		/**
		* Get the base
		* @return The base
		*/
		const int GetBase() const;

		/**
		* Get the font name
		* @return The font name
		*/
		const std::string & GetFontName() const;

		/**
		* Get the texture representing the font
		* @return The texture
		*/
		const CTexture2D & GetTexture() const;

		/**
		* Get the descriptor of the given character id
		* @param id The character id
		* @return The character descriptor or null if no descriptor is found
		*/
		const SCharDescriptor * GetCharDescriptor(const unsigned int id) const;

		/**
		* = operator override
		* @param copy A font
		* @return The font
		*/
		const CFont & operator=(const CFont & copy);

		/**
		* Get the font
		* @return The font
		*/
		IFontBase * GetFont() const;
	};
}

#endif
