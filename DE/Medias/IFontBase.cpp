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

#include "../Medias/IFontBase.h"

namespace DE {
	IFontBase::IFontBase(const std::string & name, SFontInternalData * data) : IResource(name) {
		this->m_internalData = std::unique_ptr<SFontInternalData>(data);

		// Create the char descriptors
		for(auto it = this->m_internalData->charData.begin(); it != this->m_internalData->charData.end(); ++it) {
			SCharDescriptor descriptor;
			descriptor.id = it->second.id;
			descriptor.xadvance = it->second.xadvance;
			descriptor.kerning = it->second.kerning;
			descriptor.coordinates = CRectangleF(it->second.x / (float)this->m_internalData->texture.GetSize().x, it->second.y / (float)this->m_internalData->texture.GetSize().y, it->second.width / (float)this->m_internalData->texture.GetSize().x, it->second.height / (float)this->m_internalData->texture.GetSize().y);
			descriptor.position = CRectangleI(it->second.xoffset, it->second.yoffset, it->second.width, it->second.height);

			this->m_charDescriptor[descriptor.id] = descriptor;
		}
	}

	const bool IFontBase::IsBold() const {
		return this->m_internalData->bold;
	}

	const bool IFontBase::IsItalic() const {
		return this->m_internalData->italic;
	}

	const int IFontBase::GetSize() const {
		return this->m_internalData->size;
	}

	const int IFontBase::GetLineHeight() const {
		return this->m_internalData->lineHeight;
	}

	const int IFontBase::GetBase() const {
		return this->m_internalData->base;
	}

	const std::string & IFontBase::GetFontName() const {
		return this->m_internalData->fontName;
	}

	const CTexture2D & IFontBase::GetTexture() const {
		return this->m_internalData->texture;
	}

	const SCharDescriptor * IFontBase::GetCharDescriptor(const unsigned int id) const {
		auto it = this->m_charDescriptor.find(id);
		if(it != this->m_charDescriptor.end())
			return &it->second;
		else
			return nullptr;
	}
}
