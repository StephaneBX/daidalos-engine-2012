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

#include "../Core/Logger/ILogger.h"
#include "../Core/Memory/CExceptionAssert.h"
#include "../Core/CMediaManager.h"
#include "../Core/CResourceManager.h"
#include "../Medias/CFont.h"

namespace DE
{
	CFont::CFont()
	{
		this->m_font = nullptr;
	}

	CFont::CFont(const std::string & filename)
	{
		this->m_font = nullptr;
		LoadFromFile(filename);
	}

	CFont::CFont(const CFont & copy)
	{
		*this = copy;
	}

	CFont::~CFont()
	{
		if(this->m_font != nullptr)
			CResourceManager::GetInstance()->ReleaseResource(this->m_font->GetName());
	}

	void CFont::LoadFromFile(const std::string & filename)
	{
		// Release previously used resource
		if(this->m_font != nullptr)
			CResourceManager::GetInstance()->ReleaseResource(this->m_font->GetName());

		// Check the existence of the resource and load it if necessary
		IFontBase * tmp = CResourceManager::GetInstance()->GetResource<IFontBase>(filename);

		if(tmp == nullptr)
		{
			this->m_font = CMediaManager::GetInstance()->LoadMediaFromFile<IFontBase>(filename);
			CResourceManager::GetInstance()->AddResource(filename, this->m_font);
		}
		else
			this->m_font = tmp;
	}

	const bool CFont::IsBold() const
	{
		Assert(this->m_font != nullptr);
		return this->m_font->IsBold();
	}

	const bool CFont::IsItalic() const
	{
		Assert(this->m_font != nullptr);
		return this->m_font->IsItalic();
	}

	const int CFont::GetSize() const
	{
		Assert(this->m_font != nullptr);
		return this->m_font->GetSize();
	}

	const int CFont::GetLineHeight() const
	{
		Assert(this->m_font != nullptr);
		return this->m_font->GetLineHeight();
	}

	const int CFont::GetBase() const
	{
		Assert(this->m_font != nullptr);
		return this->m_font->GetBase();
	}

	const std::string & CFont::GetFontName() const
	{
		Assert(this->m_font != nullptr);
		return this->m_font->GetFontName();
	}

	const CTexture2D & CFont::GetTexture() const
	{
		Assert(this->m_font != nullptr);
		return this->m_font->GetTexture();
	}

	const SCharDescriptor * CFont::GetCharDescriptor(const unsigned int id) const
	{
		Assert(this->m_font != nullptr);
		return this->m_font->GetCharDescriptor(id);
	}

	const CFont & CFont::operator=(const CFont & copy)
	{
		if(this->m_font != nullptr)
			CResourceManager::GetInstance()->ReleaseResource(this->m_font->GetName());

		this->m_font = copy.GetFont();

		if(this->m_font != nullptr)
			this->m_font->AddReference();

		return *this;
	}

	IFontBase * CFont::GetFont() const
	{
		return this->m_font;
	}
}
