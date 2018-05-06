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

#ifndef __IWIN32APPLICATION_HPP
#define __IWIN32APPLICATION_HPP

#define DIRECTINPUT_VERSION 0x0800

#include <string>
#include <dinput.h>
#include <Windows.h>
#include "../Core/CInputDevice.hpp"
#include "../Core/IApplication.hpp"

namespace daidalosengine {
	/**
	* Win32 implementation of an application
	*/
	class IWin32Application : public IApplication {
	private:
		bool m_hasFocus;
		int m_nCmdShow;
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		WNDCLASSEX m_wc;
		LPDIRECTINPUT m_directInput;
		std::vector<LPDIRECTINPUTDEVICEA> m_gamepad;
		
		static IWin32Application * s_instance;
		static EInputKey s_keyCode[];

	public:
		/**
		* Constructor
		* @param hInstance The program instance
		* @param nCmdShow The commands
		* @param title The application title
		*/
		IWin32Application(const HINSTANCE hInstance, const int nCmdShow, const std::string & title = APPLICATION_NAME);

		/**
		* The destructor
		*/
		virtual ~IWin32Application();

	protected:
		/**
		* Initialize the window
		*/
		virtual void InitializeWindow();

		/**
		* Initialize the gamepads
		*/
		virtual void InitializeGamepad();

		/**
		* The main loop of the application
		*/
		virtual void MainLoop();

		/**
		* The update method
		* @param time Elapsed time
		*/
		virtual void Update(const float time);

		/**
		* Update the gamepad inputs
		*/
		virtual void UpdateGamepads();

		/**
		* The render method
		*/
		virtual void Render();

		/**
		* Resize the window
		* @param width The new width
		* @param height The new height
		*/
		virtual void Resize(const int width, const int height);

		/**
		* Get the gamepads vector
		*/
		std::vector<LPDIRECTINPUTDEVICEA> & GetGamepad();

		/**
		* Get the Direct Input context
		*/
		const LPDIRECTINPUT GetDirectInput() const;

		/**
		* The global window procedure for messages
		*/
		static LRESULT CALLBACK GlobalWindowProc(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam);

		/**
		* The window procedure for messages
		*/
		LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		/**
		* Called for each enumerated devices by Direct Input
		*/
		static BOOL CALLBACK CreateDeviceCallback(LPCDIDEVICEINSTANCE instance, void * reference);

	private:
		IWin32Application(const IWin32Application & copy);
		const IWin32Application & operator=(const IWin32Application & copy);
	};
}

#endif
