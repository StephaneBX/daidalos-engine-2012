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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/IRenderer.hpp"
#include "../../Core/Memory/CExceptionAssert.hpp"
#include "../../Scene/2D/CLabel.hpp"

namespace DE {
	CLabel::CLabel(const std::string & text, const std::string & font) {
		LOG_TRACE();

		SetText(text);
		SetFont(font);
		SetColor(CColor(0xFFFFFFFF));

		this->m_horizontalAlignment = TEXT_HORIZONTAL_ALIGNMENT_CENTERED;
		this->m_verticalAlignment = TEXT_VERTICAL_ALIGNMENT_TOP;
		this->m_size = 12;
	}

	CLabel::~CLabel() {

	}

	void CLabel::SetFont(const std::string & font) {
		LOG_TRACE();

		this->m_font.LoadFromFile(font);
		this->m_scale = this->m_size / (float)this->m_font.GetSize();
		this->m_changed = true;
	}

	void CLabel::SetText(const std::string & text) {
		LOG_TRACE();

		this->m_text = text;
		this->m_changed = true;
	}

	void CLabel::SetHorizontalAlignment(ETextHorizontalAlignment alignment) {
		LOG_TRACE();

		this->m_horizontalAlignment = alignment;
		this->m_changed = true;
	}

	void CLabel::SetVerticalAlignment(ETextVerticalAlignment alignment) {
		LOG_TRACE();

		this->m_verticalAlignment = alignment;
		this->m_changed = true;
	}

	void CLabel::SetSize(const unsigned int size) {
		LOG_TRACE();

		this->m_size = size;
		this->m_scale = this->m_size / (float)this->m_font.GetSize();
	}

	void CLabel::SetColor(const CColor & color) {
		LOG_TRACE();

		this->m_color = color;
		this->m_changed = true;
	}

	void CLabel::Update(const float time) {
		LOG_TRACE();

		if(this->m_changed) {
			IRenderer * renderer = IRenderer::GetRenderer();

			GenerateTextGeometry();

			if(this->m_data.size() > 0) {
				// Create a new buffer if the current one is too small
				if((int)this->m_buffer.GetSize() < this->m_data.size() * sizeof(SGUIData))
					this->m_buffer = renderer->CreateVertexBuffer(sizeof(SGUIData), this->m_data.size(), BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);

				// Lock and copy the data
				unsigned char * lock = (unsigned char*)this->m_buffer.Lock(sizeof(SGUIData), 0, 0, LOCK_WRITEONLY);
				memcpy(lock, &this->m_data[0], this->m_data.size() * sizeof(SGUIData));
				this->m_buffer.Unlock();
			}
		}

		// Update the subviews
		IView::Update(time);
	}

	void CLabel::Render() {
		LOG_TRACE();

		if(this->m_data.size() > 0) {
			IRenderer * renderer = IRenderer::GetRenderer();
			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), this->m_parent != nullptr ? this->m_parent->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), this->m_color);

			renderer->SetTexture(0, this->m_font.GetTexture().GetTexture());
			renderer->SetTexture(1, nullptr);
			renderer->SetVertexBuffer(0, this->m_buffer, sizeof(SGUIData));
			renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_data.size() / 3);
		}

		// Render the subviews
		IView::Render();
	}

	ETextHorizontalAlignment CLabel::GetHorizontalAlignment() const {
		LOG_TRACE();

		return this->m_horizontalAlignment;
	}

	ETextVerticalAlignment CLabel::GetVerticalAlignment() const {
		LOG_TRACE();

		return this->m_verticalAlignment;
	}

	const unsigned int CLabel::GetSize() const {
		LOG_TRACE();

		return this->m_size;
	}

	const std::string & CLabel::GetFontName() const {
		LOG_TRACE();

		return this->m_font.GetFontName();
	}

	const std::string & CLabel::GetText() const {
		LOG_TRACE();

		return this->m_text;
	}

	const CColor & CLabel::GetColor() const {
		LOG_TRACE();

		return this->m_color;
	}

	void CLabel::GenerateTextGeometry() {
		LOG_TRACE();

		this->m_data.clear();

		std::set<unsigned int> lineStart;
		lineStart.insert(0);
		CVector2I cursor;
		unsigned char currentCharacter;
		unsigned int lastWordStartAt = 0;

		for(unsigned int i = 0; i < this->m_text.length(); ++i) {
			switch(this->m_text[i]) {
			case '\n':

				// End of a word, check its placement
				if(this->m_data.size() > 0) {
					if(CheckWordPlacement(lastWordStartAt, cursor, lineStart))
						break;
				}

				// If it's not the end of the text
				if((i + 1) < this->m_text.length()) {
					lastWordStartAt = this->m_data.size();
					cursor.x = 0;
					cursor.y += (int)(this->m_font.GetLineHeight() * this->m_scale);

					// If a preferred height has been set and the text is too high, stop here
					if(this->m_preferredsize.y > 0 && (cursor.y + (int)(this->m_font.GetLineHeight() * this->m_scale)) > this->m_preferredsize.y)
						break;

					lineStart.insert(this->m_data.size());
				}
				continue;

			case '\t':
			case ' ':
				currentCharacter = ' ';

				// End of a word, check its placement
				if(CheckWordPlacement(lastWordStartAt, cursor, lineStart))
					break;

				lastWordStartAt = this->m_data.size();
				break;

			default:
				currentCharacter = this->m_text[i];
				break;
			}

			// Create the geometry
			const SCharDescriptor * descriptor = this->m_font.GetCharDescriptor(currentCharacter);

			// Check the character compatibility with the font
			if(descriptor != nullptr) {
				// No need to create geometry for ' '
				if(currentCharacter != ' ') {
					CRectangleF coord = descriptor->coordinates;
					CRectangleI pos = CRectangleI((int)(descriptor->position.GetLeft() * this->m_scale), (int)(descriptor->position.GetTop() * this->m_scale), (int)(descriptor->position.GetWidth() * this->m_scale), (int)(descriptor->position.GetHeight() * this->m_scale)) + cursor;

					// Add the geometry
					std::vector<SGUIData> data = GenerateQuad(pos, coord);
					this->m_data.insert(this->m_data.end(), data.begin(), data.end());
				}

				// Advance the cursor
				cursor.x += (int)(descriptor->xadvance * this->m_scale);
			}
		}

		CVector2F boundingBoxMin, boundingBoxMax;

		// Final calculation if there is a geometry 
		if(this->m_data.size() > 0) {
			// Last word, check its placement
			CheckWordPlacement(lastWordStartAt, cursor, lineStart);

			if(this->m_data.size() > 0) {
				// Perform alignment calculation
				int verticalDecal = 0;
				int horizontalDecal = 0;
				switch(this->m_verticalAlignment) {
				case TEXT_VERTICAL_ALIGNMENT_BOTTOM:
					verticalDecal = (int)(this->m_preferredsize.y - this->m_data[this->m_data.size() - 1].position.y);
					break;

				case TEXT_VERTICAL_ALIGNMENT_CENTERED:
					verticalDecal = (int)(this->m_preferredsize.y - this->m_data[this->m_data.size() - 1].position.y) / 2;
					break;
				}

				boundingBoxMin.x = this->m_data[0].position.x;
				boundingBoxMin.y = this->m_data[0].position.y;
				boundingBoxMax.x = this->m_data[this->m_data.size() - 1].position.x;
				boundingBoxMax.y = this->m_data[this->m_data.size() - 1].position.y;

				// Update each line
				for(auto it = lineStart.begin(); it != lineStart.end(); ++it) {
					unsigned int lineEnd = this->m_data.size() - 1;
					auto next = it;
					++next;

					if(next != lineStart.end())
						lineEnd = (*next - 1);

					switch(this->m_horizontalAlignment) {
					case TEXT_HORIZONTAL_ALIGNMENT_RIGHT:
						horizontalDecal = (int)(this->m_preferredsize.x - this->m_data[lineEnd].position.x);
						break;

					case TEXT_HORIZONTAL_ALIGNMENT_CENTERED:
						horizontalDecal = (int)(this->m_preferredsize.x - this->m_data[lineEnd].position.x) / 2;
						break;
					}

					for(unsigned int j = *it; j <= lineEnd; ++j) {
						// Apply horizontal and vertical alignment
						this->m_data[j].position.x = this->m_data[j].position.x + horizontalDecal + this->m_position.x;
						this->m_data[j].position.y = this->m_data[j].position.y + verticalDecal + this->m_position.y;

						// Get the min and max x value for the bounding box
						boundingBoxMin.x = std::min(boundingBoxMin.x, this->m_data[j].position.x);
						boundingBoxMax.x = std::max(boundingBoxMax.x, this->m_data[j].position.x);
					}
				}
			}
		}

		// Final bounding box calculation
		this->m_boundingRectangle = CRectangleI((int)boundingBoxMin.x + this->m_position.x, (int)boundingBoxMin.y + this->m_position.y, (int)(boundingBoxMax.x - boundingBoxMin.x), (int)(boundingBoxMax.y - boundingBoxMin.y));

		this->m_changed = false;
	}

	const bool CLabel::CheckWordPlacement(const unsigned int wordStartAt, CVector2I & cursor, std::set<unsigned int> & lineStart) {
		LOG_TRACE();

		// If a preferred width has been set and the line is too long for the last word
		if(this->m_preferredsize.x > 0 && cursor.x > this->m_preferredsize.x) {
			float decal = this->m_data[wordStartAt].position.x;
			cursor.x -= (int)decal;
			cursor.y += (int)(this->m_font.GetLineHeight() * this->m_scale);

			// If a preferred height has been set and the text is too high, stop here and remove the word
			if(this->m_preferredsize.y > 0 && (cursor.y + (int)(this->m_font.GetLineHeight() * this->m_scale)) > this->m_preferredsize.y) {
				this->m_data.erase(this->m_data.begin() + wordStartAt, this->m_data.end());
				return true;
			}

			// Add a line
			lineStart.insert(wordStartAt);

			// Replace the last word at the beginning of the next line
			for(unsigned int j = wordStartAt; j < this->m_data.size(); ++j) {
				this->m_data[j].position.x -= decal;
				this->m_data[j].position.y += (int)(this->m_font.GetLineHeight() * this->m_scale);
			}
		}

		return false;
	}
}
