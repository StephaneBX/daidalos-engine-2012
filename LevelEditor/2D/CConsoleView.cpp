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

#include <Core/Utility/CTokenizer.hpp>
#include "../2D/CConsoleView.hpp"

#define LABEL_HEIGHT 20
#define MARGIN 10
#define CONSOLE_INPUT_TEXT ">"

#define CONSOLE_INPUT_LOAD ">LOAD"
#define CONSOLE_INPUT_SAVE ">SAVE"

namespace EDITOR {
	/**
	* Constructor
	* @param texture The view bounds
	*/
	CConsoleView::CConsoleView(const int numberOfLines) {

		// Set the bounds
		SetBounds(DE::CRectangleI(0, 0, 400, (numberOfLines + 1) * LABEL_HEIGHT));

		// Set background color
		SetBackgroundColor(DE::CColor(0.2F, 0.2F, 0.2F));

		// Create the input label
		this->m_inputLabel.SetBounds(DE::CRectangleI(MARGIN, this->m_boundingRectangle.GetHeight() - LABEL_HEIGHT, this->m_boundingRectangle.GetWidth() - (MARGIN * 2), LABEL_HEIGHT));
		this->m_inputLabel.SetSize(LABEL_HEIGHT);
		this->m_inputLabel.SetColor(DE::CColor(255, 255, 255, 255));
		this->m_inputLabel.SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
		this->m_inputLabel.SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_CENTERED);
		this->m_inputLabel.SetText(CONSOLE_INPUT_TEXT);
		AddSubview(&this->m_inputLabel);

		// Create the history labels
		this->m_historyLabel.resize(numberOfLines);
		int i = 0;
		for(auto it = this->m_historyLabel.begin(); it != this->m_historyLabel.end(); it++) {
			(*it).SetBounds(DE::CRectangleI(MARGIN, i * LABEL_HEIGHT, this->m_boundingRectangle.GetWidth() - (MARGIN * 2), LABEL_HEIGHT));
			(*it).SetSize(LABEL_HEIGHT);
			(*it).SetColor(DE::CColor(255, 255, 255, 255));
			(*it).SetHorizontalAlignment(DE::TEXT_HORIZONTAL_ALIGNMENT_LEFT);
			(*it).SetVerticalAlignment(DE::TEXT_VERTICAL_ALIGNMENT_CENTERED);
			(*it).SetText("");
			AddSubview(&(*it));
			i++;
		}

		SetVisible(false);
	}

	/**
	* Set the visible value
	* @param visible The visible value
	*/
	void CConsoleView::SetVisible(const bool visible) {
		IView::SetVisible(visible);

		if(visible) {
			DE::CInputDevice::GetInstance()->AddKeyboardListener(this);
		} else {
			DE::CInputDevice::GetInstance()->RemoveKeyboardListener(this);

			// Clean the console
			this->m_inputLabel.SetText(CONSOLE_INPUT_TEXT);
		}
	}

	/**
	* Add a listener
	* @param listener The listener to add
	*/
	void CConsoleView::AddListener(IConsoleListener * listener) {
		this->m_listeners.push_back(listener);
	}

	/**
	* Remove a listener
	* @param listener The listener to remove
	*/
	void CConsoleView::RemoveListener(IConsoleListener * listener) {

		auto it = std::find(this->m_listeners.begin(), this->m_listeners.end(), listener);
		if(it != this->m_listeners.end()) {
			this->m_listeners.erase(it);
		}
	}

	/**
	* Called when a keyboard action is started
	* @param actionId The action id
	*/
	void CConsoleView::OnInputActionStarted(const unsigned int actionId) {
		// Nothing to do
	}

	/**
	* Called when a keyboard action is stopped
	* @param actionId The action id
	*/
	void CConsoleView::OnInputActionStopped(const unsigned int actionId) {
		// Nothing to do
	}

	/**
	* Called when a key has been pressed
	* @param key The key
	*/
	void CConsoleView::OnKeyPressed(const unsigned int key) {

		// Add a key
		if(key >= '0' && key <= '9' || key >= 'A' && key <= 'Z' || key == ' ') {

			if((this->m_inputLabel.GetBoundingRectangle().GetWidth() + LABEL_HEIGHT) < this->m_boundingRectangle.GetWidth()) {
				std::ostringstream ss;
				ss << this->m_inputLabel.GetText() << (char)key;

				this->m_inputLabel.SetText(ss.str());
			}
		}

		// Backspace
		if(key == 8 && this->m_inputLabel.GetText().size() > 1) {
			this->m_inputLabel.SetText(this->m_inputLabel.GetText().substr(0, this->m_inputLabel.GetText().size() - 1));
		}

		// Carriage return
		if(key == 13) {
			ValidInputLabel();
		}
	}

	/**
	* Check the input label for a valid command
	*/
	void CConsoleView::ValidInputLabel() {
		DE::CTokenizer tokenizer;

		tokenizer.AddLine(this->m_inputLabel.GetText());
		tokenizer.Tokenize(" ");

		const DE::TToken tokens = tokenizer.GetTokens();
		if(tokens.size() == 2) {
			// Check for known commands
			if(std::string(CONSOLE_INPUT_LOAD).compare(tokens[0]) == 0) {
				// Load a scene
				for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); it++) {
					std::string filename = tokens[1] + ".des";
					std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
					(*it)->OnConsoleCommandLoad(filename);
				}

			} else if(std::string(CONSOLE_INPUT_SAVE).compare(tokens[0]) == 0) {
				// Save a scene
				for(auto it = this->m_listeners.begin(); it != this->m_listeners.end(); it++) {
					std::string filename = tokens[1] + ".des";
					std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
					(*it)->OnConsoleCommandSave(filename);
				}
			}
		}

		// Move the input to history and shift the history labels
		ShiftLabels();
	}

	/**
	* Move the current input in the history labels and shift them
	*/
	void CConsoleView::ShiftLabels() {

		for(unsigned int i = 0; i < this->m_historyLabel.size(); i++) {

			if((i + 1) < this->m_historyLabel.size()) {
				// Shift the history label
				this->m_historyLabel[i].SetText(this->m_historyLabel[i + 1].GetText());
			} else {
				// Take the input label text
				this->m_historyLabel[i].SetText(this->m_inputLabel.GetText());
			}
		}
		this->m_inputLabel.SetText(CONSOLE_INPUT_TEXT);
	}
}
