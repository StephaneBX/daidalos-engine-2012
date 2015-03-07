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

#include <TinyXML2/tinyxml2.h>
#include "../Core/IRenderer.h"
#include "../Core/Utility/CVector2.h"
#include "../Core/Utility/CVector4.h"
#include "../Core/Logger/ILogger.h"
#include "../Core/Memory/CExceptionLoad.h"
#include "../Loaders/CFontLoader.h"

namespace DE {
	CFontLoader::CFontLoader() {}

	CFontLoader::~CFontLoader() {}

	IFontBase * CFontLoader::LoadFromFile(const std::string & filename) {
		SFontInternalData * data = nullptr;
		tinyxml2::XMLDocument doc;
		int loadOk = doc.LoadFile(filename.c_str());
		if(loadOk == tinyxml2::XML_NO_ERROR) {
			data = new SFontInternalData;
			std::string tmpString;
			tinyxml2::XMLHandle hDoc(&doc);

			/**
			* General informations
			*/
			tinyxml2::XMLElement * info = hDoc.FirstChildElement("font").FirstChildElement("info").ToElement();
			data->fontName = info->Attribute("face");
			info->QueryBoolAttribute("bold", &data->bold);
			info->QueryBoolAttribute("italic", &data->italic);
			info->QueryIntAttribute("size", &data->size);

			tmpString = info->Attribute("padding");
			data->padding = CVector4I::GetFrom(tmpString, CVector4I(), ",");

			tmpString = info->Attribute("spacing");
			data->spacing = CVector2I::GetFrom(tmpString, CVector2I(), ",");

			tinyxml2::XMLElement * common = hDoc.FirstChildElement("font").FirstChildElement("common").ToElement();
			common->QueryIntAttribute("lineHeight", &data->lineHeight);
			common->QueryIntAttribute("base", &data->base);

			tinyxml2::XMLElement * page = hDoc.FirstChildElement("font").FirstChildElement("pages").FirstChildElement("page").ToElement();
			tmpString = page->Attribute("file");
			data->texture.CreateFromFile(tmpString, PIXEL_FORMAT_A8R8G8B8, IRenderer::GetRenderer()->GetDefaultTextureFilter());

			/**
			* Load the chars
			*/
			tinyxml2::XMLElement * character = hDoc.FirstChildElement("font").FirstChildElement("chars").FirstChildElement("char").ToElement();
			for(character; character; character = character->NextSiblingElement()) {
				SCharData charData;
				character->QueryUnsignedAttribute("id", &charData.id);
				character->QueryIntAttribute("x", &charData.x);
				character->QueryIntAttribute("y", &charData.y);
				character->QueryIntAttribute("width", &charData.width);
				character->QueryIntAttribute("height", &charData.height);
				character->QueryIntAttribute("xoffset", &charData.xoffset);
				character->QueryIntAttribute("yoffset", &charData.yoffset);
				character->QueryIntAttribute("xadvance", &charData.xadvance);

				data->charData[charData.id] = charData;
			}

			/**
			* Load the kernings
			*/
			tinyxml2::XMLElement * kerning = hDoc.FirstChildElement("font").FirstChildElement("kernings").FirstChildElement("kerning").ToElement();
			for(kerning; kerning; kerning = kerning->NextSiblingElement()) {
				SKerningPair kerningData;
				kerning->QueryUnsignedAttribute("first", &kerningData.first);
				kerning->QueryUnsignedAttribute("second", &kerningData.second);
				kerning->QueryIntAttribute("amount", &kerningData.amount);

				data->charData[kerningData.first].kerning[kerningData.second] = kerningData;
			}
		} else {
			LOG_ERROR("Unable to load the font file '" << filename << "' (" << doc.GetErrorStr1() << ")");
			ILogger::Kill();

			throw CExceptionLoad(filename);
		}

		return new IFontBase(filename, data);
	}

	void CFontLoader::SaveToFile(const IFontBase * object, const std::string & filename) {

	}
}
