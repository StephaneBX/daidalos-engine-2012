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

namespace daidalosengine {
	CLabel::CLabel(const std::string & text, const std::string & font) {
		LOG_TRACE();

		SetText(text);
		SetFont(font);
		SetColor(CColor(1.0F, 1.0F, 1.0F, 1.0F));
		SetSize(26);

		this->m_horizontalAlignment = TEXT_HORIZONTAL_ALIGNMENT_CENTERED;
		this->m_verticalAlignment = TEXT_VERTICAL_ALIGNMENT_TOP;
	}

	CLabel::~CLabel() = default;

	/**
	* Initialize the content
	*/
	void CLabel::Initialize() {
		// Nothing to do
	}

	void CLabel::SetFont(const std::string & font) {
		LOG_TRACE();

		this->m_font.LoadFromFile(font);
		this->m_scale = this->m_size / static_cast<float>(this->m_font.GetSize());
		SetChanged(true);
	}

	void CLabel::SetText(const std::string & text) {
		LOG_TRACE();

		this->m_text = text;
		SetChanged(true);
	}

	void CLabel::SetHorizontalAlignment(const ETextHorizontalAlignment alignment) {
		LOG_TRACE();

		this->m_horizontalAlignment = alignment;
		SetChanged(true);
	}

	void CLabel::SetVerticalAlignment(const ETextVerticalAlignment alignment) {
		LOG_TRACE();

		this->m_verticalAlignment = alignment;
		SetChanged(true);
	}

	void CLabel::SetSize(const unsigned int size) {
		LOG_TRACE();

		this->m_size = size;
		this->m_scale = this->m_size / static_cast<float>(this->m_font.GetSize());
	}

	void CLabel::SetColor(const CColor & color) {
		LOG_TRACE();

		this->m_color = color;
		SetChanged(true);
	}

	void CLabel::Update(const float time) {
		LOG_TRACE();

		if (IsChanged()) {
			GenerateTextGeometry();
			UpdateBuffer();
		}

		// Update the subviews
		IView::Update(time);
	}

	void CLabel::Render() {
		LOG_TRACE();

		if (GetDataSize() > 0) {
			IRenderer * renderer = IRenderer::GetRenderer();

			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), GetParent() != nullptr ? GetParent()->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, daidalosengine::IRenderer::GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), this->m_color);

			renderer->SetTexture(0, this->m_font.GetTexture().GetTexture());
			renderer->SetTexture(1, nullptr);
			RenderPrimitives();
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

	unsigned int CLabel::GetSize() const {
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

		std::vector<SGUIData> data;
		std::set<unsigned int> lineStart;
		lineStart.insert(0);
		CVector2I cursor;
		unsigned char currentCharacter = 0;
		unsigned int lastWordStartAt = 0;

		for (unsigned int i = 0; i < this->m_text.length(); ++i) {
			switch (this->m_text[i]) {
				case '\n':

					// End of a word, check its placement
					if (!data.empty()) {
						if (CheckWordPlacement(data, lastWordStartAt, cursor, lineStart))
							break;
					}

					// If it's not the end of the text
					if ((i + 1) < this->m_text.length()) {
						lastWordStartAt = data.size();
						cursor.x = 0;
						cursor.y += static_cast<int>(this->m_font.GetLineHeight() * this->m_scale);

						// If a preferred height has been set and the text is too high, stop here
						if (GetPreferredSize().y > 0 && (cursor.y + static_cast<int>(this->m_font.GetLineHeight() * this->m_scale)) > GetPreferredSize().y) {
							break;
						}

						lineStart.insert(data.size());
					}
					continue;

				case '\t':
				case ' ':
					currentCharacter = ' ';

					// End of a word, check its placement
					if (CheckWordPlacement(data, lastWordStartAt, cursor, lineStart))
						break;

					lastWordStartAt = data.size();
					break;

				default:
					currentCharacter = this->m_text[i];
					break;
			}

			// Create the geometry
			const auto descriptor = this->m_font.GetCharDescriptor(currentCharacter);

			// Check the character compatibility with the font
			if (descriptor != nullptr) {
				// No need to create geometry for ' '
				if (currentCharacter != ' ') {
					const auto coord = descriptor->coordinates;
					const auto pos = CRectangleI(static_cast<int>(descriptor->position.GetLeft() * this->m_scale), static_cast<int>(descriptor->position.GetTop() * this->m_scale), static_cast<int>(descriptor->position.GetWidth() * this->m_scale), static_cast<int>(descriptor->position.GetHeight() * this->m_scale)) + cursor;

					// Add the geometry
					auto tmpData = GenerateQuad(pos, coord);
					data.insert(data.end(), tmpData.begin(), tmpData.end());
				}

				// Advance the cursor
				cursor.x += static_cast<int>(descriptor->xadvance * this->m_scale);
			}
		}

		CVector2F boundingBoxMin;
		CVector2F boundingBoxMax;

		// Final calculation if there is a geometry 
		if (!data.empty()) {
			// Last word, check its placement
			CheckWordPlacement(data, lastWordStartAt, cursor, lineStart);

			if (!data.empty()) {
				// Perform alignment calculation
				auto verticalDecal = 0;
				auto horizontalDecal = 0;
				switch (this->m_verticalAlignment) {
					case TEXT_VERTICAL_ALIGNMENT_BOTTOM:
						verticalDecal = static_cast<int>(GetPreferredSize().y - data[data.size() - 1].position.y);
						break;

					case TEXT_VERTICAL_ALIGNMENT_CENTERED:
						verticalDecal = static_cast<int>(GetPreferredSize().y - data[data.size() - 1].position.y) / 2;
						break;

					default:
						break;
				}

				boundingBoxMin.x = data[0].position.x;
				boundingBoxMin.y = data[0].position.y;
				boundingBoxMax.x = data[data.size() - 1].position.x;
				boundingBoxMax.y = data[data.size() - 1].position.y;

				// Update each line
				for (auto it = lineStart.begin(); it != lineStart.end(); ++it) {
					auto lineEnd = data.size() - 1;
					auto next = it;
					++next;

					if (next != lineStart.end())
						lineEnd = (*next - 1);

					switch (this->m_horizontalAlignment) {
						case TEXT_HORIZONTAL_ALIGNMENT_RIGHT:
							horizontalDecal = static_cast<int>(GetPreferredSize().x - data[lineEnd].position.x);
							break;

						case TEXT_HORIZONTAL_ALIGNMENT_CENTERED:
							horizontalDecal = static_cast<int>(GetPreferredSize().x - data[lineEnd].position.x) / 2;
							break;

						default:
							break;
					}

					for (auto j = *it; j <= lineEnd; ++j) {
						// Apply horizontal and vertical alignment
						data[j].position.x = data[j].position.x + horizontalDecal + GetPosition().x;
						data[j].position.y = data[j].position.y + verticalDecal + GetPosition().y;

						// Get the min and max x value for the bounding box
						boundingBoxMin.x = std::min(boundingBoxMin.x, data[j].position.x);
						boundingBoxMax.x = std::max(boundingBoxMax.x, data[j].position.x);
					}
				}
			}
		}

		// Add the data
		SetData(data);

		// Final bounding box calculation
		SetBoundingRectangle(CRectangleI(GetPosition().x, GetPosition().y, static_cast<int>(boundingBoxMax.x - boundingBoxMin.x), static_cast<int>(boundingBoxMax.y - boundingBoxMin.y)));

		SetChanged(false);
	}

	bool CLabel::CheckWordPlacement(std::vector<SGUIData> & data, const unsigned int wordStartAt, CVector2I & cursor, std::set<unsigned int> & lineStart) {
		LOG_TRACE();

		// If a preferred width has been set and the line is too long for the last word
		if (GetPreferredSize().x > 0 && cursor.x > GetPreferredSize().x) {
			const auto decal = data[wordStartAt].position.x;
			cursor.x -= static_cast<int>(decal);
			cursor.y += static_cast<int>(this->m_font.GetLineHeight() * this->m_scale);

			// If a preferred height has been set and the text is too high, stop here and remove the word
			if (GetPreferredSize().y > 0 && (cursor.y + static_cast<int>(this->m_font.GetLineHeight() * this->m_scale)) > GetPreferredSize().y) {
				data.erase(data.begin() + wordStartAt, data.end());
				return true;
			}

			// Add a line
			lineStart.insert(wordStartAt);

			// Replace the last word at the beginning of the next line
			for (auto j = wordStartAt; j < data.size(); ++j) {
				data[j].position.x -= decal;
				data[j].position.y += static_cast<int>(this->m_font.GetLineHeight() * this->m_scale);
			}
		}

		return false;
	}
}
