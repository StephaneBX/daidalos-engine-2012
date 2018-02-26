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

#ifndef __CLABEL_HPP
#define __CLABEL_HPP

#include <string>
#include <set>
#include "../../Medias/CFont.hpp"
#include "../../Core/Utility/CColor.hpp"
#include "../../Core/Enums.hpp"
#include "../../Scene/2D/IView.hpp"

namespace daidalosengine {
	/**
	* Basic class for gui text
	*/
	class CLabel : public IView {
	private:
		unsigned int m_size;
		float m_scale;
		CFont m_font;
		std::string m_text;
		CColor m_color;
		ETextHorizontalAlignment m_horizontalAlignment;
		ETextVerticalAlignment m_verticalAlignment;

	public:
		/**
		* Constructor
		* @param text The text
		* @param font The font name
		*/
		CLabel(const std::string & text = "", const std::string & font = "arial.fnt");

		/**
		* Destructor
		*/
		virtual ~CLabel();

		/**
		* Initialize the content
		*/
		virtual void Initialize() final;

		/**
		* Set the font
		* @param font The font name
		*/
		void SetFont(const std::string & font);

		/**
		* Set the text
		* @param text The text
		*/
		void SetText(const std::string & text);

		/**
		* Set the horizontal alignment
		* @param alignment The horizontal alignment
		*/
		void SetHorizontalAlignment(ETextHorizontalAlignment alignment);

		/**
		* Set the vertical alignment
		* @param alignment The vertical alignment
		*/
		void SetVerticalAlignment(ETextVerticalAlignment alignment);

		/**
		* Set the text size
		* @param size The size
		*/
		void SetSize(const unsigned int size);

		/**
		* Set the color
		* @param color The color
		*/
		void SetColor(const CColor & color);

		/**
		* Update the state and the geometry
		* @param time The elapsed time
		*/
		virtual void Update(const float time) override;

		/**
		* Render the text
		*/
		virtual void Render() override;

		/**
		* Get the horizontal alignment
		* @return The horizontal alignment
		*/
		ETextHorizontalAlignment GetHorizontalAlignment() const;

		/**
		* Get the vertical alignment
		* @return The vertical alignment
		*/
		ETextVerticalAlignment GetVerticalAlignment() const;

		/**
		* Get the text size
		* @return The text size
		*/
		const unsigned int GetSize() const;

		/**
		* Get the font name
		* @return The font name
		*/
		const std::string & GetFontName() const;

		/**
		* Get the text
		* @return The text
		*/
		const std::string & GetText() const;

		/**
		* Get the color
		* @return The color
		*/
		const CColor & GetColor() const;

	private:
		/**
		* Generate the text geometry
		*/
		void GenerateTextGeometry();

		/**
		* Check the word placement regarding the preferred size and move it to the next line if necessary
		* @param wordStartAt The index of the word in th data
		* @param cursor The current cursor position
		* @param lineStart The index of each line in the data
		*/
		const bool CheckWordPlacement(const unsigned int wordStartAt, CVector2I & cursor, std::set<unsigned int> & lineStart);
	};
}

#endif
