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

#ifndef __CTEXTURE2D_HPP
#define __CTEXTURE2D_HPP

#include "../Medias/ITexture2DBase.hpp"

namespace daidalosengine {
	/**
	* Basic class used for textures
	*/
	class CTexture2D {
	private:
		ITexture2DBase * m_texture;

	public:
		/**
		* Constructor
		*/
		CTexture2D();

		/**
		* Constructor
		* @param texture The texture to copy
		*/
		CTexture2D(const CTexture2D & texture);

		/**
		* Destructor
		*/
		~CTexture2D();

		/**
		* = operator overrides
		* @param texture The texture
		* @return The texture
		*/
		CTexture2D & operator=(const CTexture2D & texture);

		/**
		* Get the texture size
		* @return The texture size
		*/
		const CVector2I & GetSize() const;

		/**
		* Get the texture image
		* @return The texture image
		*/
		const CImage & GetImage() const;

		/**
		* Get the texture filter policy
		* @return The texture filter policy
		*/
		const STexture2DFilterPolicy & GetTextureFilterPolicy() const;

		/**
		* Get the texture format
		* @return The texture format
		*/
		EPixelFormat GetFormat() const;

		/**
		* Get the texture texture
		* @return The texture texture
		*/
		ITexture2DBase * GetTexture() const;

		/**
		* == operator override
		* @param texture the texture to compare with
		* @return The comparison result
		*/
		bool operator ==(const CTexture2D & texture) const;

		/**
		* != operator override
		* @param texture the texture to compare with
		* @return The comparison result
		*/
		bool operator !=(const CTexture2D & texture) const;

		/**
		* Create an empty texture
		* @param name The resource name
		* @param size The size
		* @param format The texture format
		* @param filter The filter policy
		* @param flags The flags
		*/
		void CreateEmpty(const std::string name, const CVector2I & size, EPixelFormat format, const STexture2DFilterPolicy * filter = nullptr, const unsigned long flags = 0);

		/**
		* Create a texture from a file
		* @param filename The filename
		* @param format The texture format
		* @param filter The filter policy
		* @param flags The flags
		*/
		void CreateFromFile(const std::string & filename, EPixelFormat format, const STexture2DFilterPolicy * filter = nullptr, const unsigned long flags = 0);

		/**
		* Create a texture from an image
		* @param image The image
		* @param format The texture format
		* @param filter The filter policy
		* @param flags The flags
		* @param name The texture name
		*/
		void CreateFromImage(const CImage & image, EPixelFormat format, const STexture2DFilterPolicy * filter = nullptr, const unsigned long flags = 0, const std::string & name = "");

		/**
		* Save the texture to a file
		* @param filename The filename
		*/
		void SaveToFile(const std::string & filename) const;

		/**
		* Update the texture
		* @param rect A rectangle representing the surface to update
		*/
		void Update(const CRectangleI & rect = CRectangleI(-1, -1, -1, -1));

	private:
		/**
		* Load an image.
		* @param image The image
		* @param filter The texture filter
		* @param format The texture format
		* @param flags The flags
		* @param name The image name
		*/
		void Load(const CImage & image, const STexture2DFilterPolicy & filter, EPixelFormat format = PIXEL_FORMAT_A8R8G8B8, const unsigned long flags = 0, const std::string name = "");
	};
}

#endif
