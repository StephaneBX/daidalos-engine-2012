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

#ifdef _WIN32

#define NOMINMAX

#include "../Renderers/DX9/CDX9Renderer.h"
#include "../Core/Logger/ILogger.h"
#include "../Core/IRenderer.h"
#include "../Core/Utility/CTimer.h"
#include "../Applications/IWin32Application.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

namespace DE {
	IWin32Application * IWin32Application::s_instance = nullptr;

	EInputKey IWin32Application::s_keyCode[] =
	{
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_MOUSE_BUTTON0,
		INPUT_KEY_MOUSE_BUTTON1,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_MOUSE_BUTTON2,
		INPUT_KEY_MOUSE_BUTTON3,  // 5
		INPUT_KEY_MOUSE_BUTTON4,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_BACKSPACE,
		INPUT_KEY_KEYBOARD_TAB,
		INPUT_KEY_UNDEFINED, // 10
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_CLEAR,
		INPUT_KEY_KEYBOARD_RETURN,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 15
		INPUT_KEY_KEYBOARD_LSHIFT,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_PAUSE,
		INPUT_KEY_KEYBOARD_CAPSLOCK, // 20
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 25
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_ESCAPE,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 30
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_SPACE,
		INPUT_KEY_KEYBOARD_PAGEUP,
		INPUT_KEY_KEYBOARD_PAGEDOWN,
		INPUT_KEY_KEYBOARD_END, // 35
		INPUT_KEY_KEYBOARD_HOME,
		INPUT_KEY_KEYBOARD_LEFT,
		INPUT_KEY_KEYBOARD_UP,
		INPUT_KEY_KEYBOARD_RIGHT,
		INPUT_KEY_KEYBOARD_DOWN, // 40
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 45
		INPUT_KEY_KEYBOARD_DELETE,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_0,
		INPUT_KEY_KEYBOARD_1,
		INPUT_KEY_KEYBOARD_2, // 50
		INPUT_KEY_KEYBOARD_3,
		INPUT_KEY_KEYBOARD_4,
		INPUT_KEY_KEYBOARD_5,
		INPUT_KEY_KEYBOARD_6,
		INPUT_KEY_KEYBOARD_7, // 55
		INPUT_KEY_KEYBOARD_8,
		INPUT_KEY_KEYBOARD_9,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 60
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_A, // 65
		INPUT_KEY_KEYBOARD_B,
		INPUT_KEY_KEYBOARD_C,
		INPUT_KEY_KEYBOARD_D,
		INPUT_KEY_KEYBOARD_E,
		INPUT_KEY_KEYBOARD_F, // 70
		INPUT_KEY_KEYBOARD_G,
		INPUT_KEY_KEYBOARD_H,
		INPUT_KEY_KEYBOARD_I,
		INPUT_KEY_KEYBOARD_J,
		INPUT_KEY_KEYBOARD_K, // 75
		INPUT_KEY_KEYBOARD_L,
		INPUT_KEY_KEYBOARD_M,
		INPUT_KEY_KEYBOARD_N,
		INPUT_KEY_KEYBOARD_O,
		INPUT_KEY_KEYBOARD_P, // 80
		INPUT_KEY_KEYBOARD_Q,
		INPUT_KEY_KEYBOARD_R,
		INPUT_KEY_KEYBOARD_S,
		INPUT_KEY_KEYBOARD_T,
		INPUT_KEY_KEYBOARD_U, // 85
		INPUT_KEY_KEYBOARD_V,
		INPUT_KEY_KEYBOARD_W,
		INPUT_KEY_KEYBOARD_X,
		INPUT_KEY_KEYBOARD_Y,
		INPUT_KEY_KEYBOARD_Z, // 90
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 95
		INPUT_KEY_KEYBOARD_KP0,
		INPUT_KEY_KEYBOARD_KP1,
		INPUT_KEY_KEYBOARD_KP2,
		INPUT_KEY_KEYBOARD_KP3,
		INPUT_KEY_KEYBOARD_KP4, // 100
		INPUT_KEY_KEYBOARD_KP5,
		INPUT_KEY_KEYBOARD_KP6,
		INPUT_KEY_KEYBOARD_KP7,
		INPUT_KEY_KEYBOARD_KP8,
		INPUT_KEY_KEYBOARD_KP9, // 105
		INPUT_KEY_KEYBOARD_KP_MULTIPLY,
		INPUT_KEY_KEYBOARD_KP_PLUS,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_KP_MINUS,
		INPUT_KEY_KEYBOARD_KP_PERIOD, // 110
		INPUT_KEY_KEYBOARD_KP_DIVIDE,
		INPUT_KEY_KEYBOARD_F1,
		INPUT_KEY_KEYBOARD_F2,
		INPUT_KEY_KEYBOARD_F3,
		INPUT_KEY_KEYBOARD_F4, // 115
		INPUT_KEY_KEYBOARD_F5,
		INPUT_KEY_KEYBOARD_F6,
		INPUT_KEY_KEYBOARD_F7,
		INPUT_KEY_KEYBOARD_F8,
		INPUT_KEY_KEYBOARD_F9, // 120
		INPUT_KEY_KEYBOARD_F10,
		INPUT_KEY_KEYBOARD_F11,
		INPUT_KEY_KEYBOARD_F12,
		INPUT_KEY_KEYBOARD_F13,
		INPUT_KEY_KEYBOARD_F14, // 125
		INPUT_KEY_KEYBOARD_F15,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 130
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 135
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 140
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_NUMLOCK,
		INPUT_KEY_KEYBOARD_SCROLLOCK, // 145
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 150
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 155
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_LSHIFT, // 160
		INPUT_KEY_KEYBOARD_RSHIFT,
		INPUT_KEY_KEYBOARD_LCTRL,
		INPUT_KEY_KEYBOARD_RCTRL,
		INPUT_KEY_KEYBOARD_LMETA,
		INPUT_KEY_KEYBOARD_RMETA, // 165
		INPUT_KEY_UNDEFINED
	};

	/**
	* Constructor
	* @param hInstance The program instance
	* @param nCmdShow The commands
	* @param title The application title
	*/
	IWin32Application::IWin32Application(const HINSTANCE hInstance, const int nCmdShow, const std::string & title) : IApplication() {
		LOG_TRACE();

		this->m_name = title;
		s_instance = this;

		this->m_hasFocus = false;
		this->m_hInstance = hInstance;
		this->m_nCmdShow = nCmdShow;

		ZeroMemory(&this->m_wc, sizeof(WNDCLASSEX));

		this->m_wc.cbSize = sizeof(WNDCLASSEX);
		this->m_wc.style = CS_HREDRAW | CS_VREDRAW;
		this->m_wc.lpfnWndProc = GlobalWindowProc;
		this->m_wc.hInstance = this->m_hInstance;
		this->m_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		this->m_wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		this->m_wc.lpszClassName = this->m_name.c_str();

		RegisterClassEx(&this->m_wc);

		InitializeWindow();

		InitializeGamepad();
	}

	/**
	* The destructor
	*/
	IWin32Application::~IWin32Application() {
		LOG_TRACE();

		if(this->m_hWnd) {
			DestroyWindow(this->m_hWnd);
			UnregisterClass(APPLICATION_NAME, this->m_hInstance);
		}

		// Release Direct Input related data
		SAFE_RELEASE(this->m_directInput);

		for(unsigned int i = 0; i < this->m_gamepad.size(); i++) {
			SAFE_RELEASE(this->m_gamepad[i]);
		}
	}

	/**
	* Initialize the window
	*/
	void IWin32Application::InitializeWindow() {
		LOG_TRACE();

		if(this->m_fullscreen) {
			this->m_hWnd = CreateWindowEx(NULL, this->m_name.c_str(), this->m_name.c_str(), WS_EX_TOPMOST | WS_POPUP, 0, 0, this->m_width, this->m_height, NULL, NULL, this->m_hInstance, NULL);
		} else {
			this->m_hWnd = CreateWindowEx(NULL, this->m_name.c_str(), this->m_name.c_str(), WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 0, 0, this->m_width, this->m_height, NULL, NULL, this->m_hInstance, NULL);
		}
		ShowWindow(this->m_hWnd, this->m_nCmdShow);

		RECT rect;
		GetClientRect(this->m_hWnd, &rect);

		this->m_displayBounds = CRectangleI(0, 0, rect.right - rect.left, rect.bottom - rect.top);

		SetCursorPos(this->m_width / 2, this->m_height / 2);
		this->m_input->SetMousePosition(DE::CVector2I(this->m_displayBounds.GetWidth() / 2, this->m_displayBounds.GetHeight() / 2), false);

		// Renderer creation
		IRenderer::SetRenderer(new CDX9Renderer(this->m_verticalSynchro, this->m_anisotropicFiltering, this->m_antialiasing, this->m_hWnd));

		this->m_renderer = IRenderer::GetRenderer();
		this->m_renderer->SetDisplayBounds(this->m_displayBounds);
	}

	/**
	* Initialize the gamepads
	*/
	void IWin32Application::InitializeGamepad() {
		LOG_TRACE();

		// Create the direct input context
		DirectInput8Create(this->m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->m_directInput, NULL);

		// Enumerate the plugged gamepad and create a device for each one (maximum MAX_GAMEPADS)
		this->m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, CreateDeviceCallback, NULL, DIEDFL_ATTACHEDONLY);

		// Change the gamepad data format and acquire them
		for(unsigned int i = 0; i < this->m_gamepad.size(); i++) {
			this->m_gamepad[i]->SetDataFormat(&c_dfDIJoystick);
			this->m_gamepad[i]->Acquire();
		}
	}

	/**
	* The main loop of the application
	*/
	void IWin32Application::MainLoop() {
		LOG_TRACE();

		MSG msg;
		CTimer timer;
		float time;
		float realTime;
		double previousTime = timer.Elapsed();

		// Ignore the first messages
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		while(this->m_alive) {
			time = (float)((timer.Elapsed() - previousTime) * 1000);
			realTime = time;
			previousTime = timer.Elapsed();
			POINTS point;

			if(time > MAXIMUM_FRAME_TIME)
				time = MAXIMUM_FRAME_TIME;

			// Disable any previous mouse wheel movement
			this->m_input->SetKeyUp(INPUT_KEY_MOUSE_WHEELUP);
			this->m_input->SetKeyUp(INPUT_KEY_MOUSE_WHEELDOWN);

			while(PeekMessage(&msg, this->m_hWnd, 0, 0, PM_REMOVE) > 0) {
				switch(msg.message) {
				case WM_KEYDOWN:
					if(msg.wParam < 167)
						this->m_input->SetKeyDown(s_keyCode[msg.wParam], msg.wParam);
					break;

				case WM_KEYUP:
					if(msg.wParam < 167)
						this->m_input->SetKeyUp(s_keyCode[msg.wParam], msg.wParam);
					break;

				case WM_MOUSEMOVE:
					point = MAKEPOINTS(msg.lParam);

					this->m_input->SetMousePosition(DE::CVector2I(point.x, point.y), this->m_hasFocus);

					if(this->m_hasFocus && (point.x < this->m_displayBounds.GetWidth() * 0.1f || point.x > this->m_displayBounds.GetWidth() * 0.9f || point.y < this->m_displayBounds.GetHeight() * 0.1f || point.y > this->m_displayBounds.GetHeight() * 0.9f)) {
						SetCursorPos(this->m_width / 2, this->m_height / 2);
						this->m_input->SetMousePosition(DE::CVector2I(this->m_displayBounds.GetWidth() / 2, this->m_displayBounds.GetHeight() / 2), false);
					}
					break;

				case WM_MOUSEWHEEL:
					if(HIWORD(msg.wParam) > 0) {
						this->m_input->SetKeyDown(INPUT_KEY_MOUSE_WHEELUP);
					} else {
						this->m_input->SetKeyDown(INPUT_KEY_MOUSE_WHEELDOWN);
					}
					break;

				case WM_LBUTTONDOWN:
					this->m_input->SetKeyDown(INPUT_KEY_MOUSE_BUTTON0);
					break;

				case WM_LBUTTONUP:
					this->m_input->SetKeyUp(INPUT_KEY_MOUSE_BUTTON0);
					break;

				case WM_RBUTTONDOWN:
					this->m_input->SetKeyDown(INPUT_KEY_MOUSE_BUTTON1);
					break;

				case WM_RBUTTONUP:
					this->m_input->SetKeyUp(INPUT_KEY_MOUSE_BUTTON1);
					break;

				case WM_MBUTTONDOWN:
					this->m_input->SetKeyDown(INPUT_KEY_MOUSE_BUTTON2);
					break;

				case WM_MBUTTONUP:
					this->m_input->SetKeyUp(INPUT_KEY_MOUSE_BUTTON2);
					break;

				case WM_QUIT:
					this->m_alive = false;
					break;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// Update the gamepad
			UpdateGamepads();

			Update(time);
			Render();

			this->m_input->ResetMouseMovement();
		}
	}

	/**
	* The update method
	* @param time Elapsed time
	*/
	void IWin32Application::Update(const float time) {
		LOG_TRACE();
	}

	/**
	* Update the gamepad inputs
	*/
	void IWin32Application::UpdateGamepads() {
		LOG_TRACE();

		// Loop through the active gamepads
		for(unsigned int gamepadId = 0; gamepadId < this->m_gamepad.size(); gamepadId++) {
			// Get the gamepad state
			DIJOYSTATE state;
			HRESULT res = this->m_gamepad[gamepadId]->GetDeviceState(sizeof(state), (LPVOID)&state);

			if(SUCCEEDED(res)) {
				// If the call succeed, update the gamepad state

				// Update the buttons
				for(int i = 0; i < INPUT_KEY_GAMEPAD_BUTTON_END; i++) {
					float buttonState = 0;

					// If the button is down
					if(state.rgbButtons[i] & 0x80) {
						buttonState = 1;
					}
					this->m_input->SetGamepadKey(gamepadId, (EInputKeyGamepad)(INPUT_KEY_GAMEPAD_BUTTON0 + i), buttonState);
				}

				// Update the pov - the values are angles
				this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_POV_UP, state.rgdwPOV[0] == 0 ? 1.0f : 0.0f);
				this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_POV_RIGHT, state.rgdwPOV[0] == 9000 ? 1.0f : 0.0f);
				this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_POV_DOWN, state.rgdwPOV[0] == 18000 ? 1.0f : 0.0f);
				this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_POV_LEFT, state.rgdwPOV[0] == 27000 ? 1.0f : 0.0f);

				// Update the axis
				const float axisCenter = 32767;
				const float axisThreshold = 1.0f - (GAMEPAD_AXIS_THRESHOLD);
				float axisFactor = 0;

				// X
				if(state.lX < axisCenter * axisThreshold) {
					axisFactor = std::min(1.0f, 1.0f - (state.lX / (axisCenter * axisThreshold)));
					this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LXN, axisFactor);
				} else {
					if(state.lX > axisCenter + (axisCenter * (1.0f - axisThreshold))) {
						axisFactor = std::min(1.0f, (state.lX - (axisCenter + (axisCenter * (1.0f - axisThreshold)))) / (axisCenter * axisThreshold));
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LXP, axisFactor);
					} else {
						axisFactor = 0;
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LXN, axisFactor);
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LXP, axisFactor);
					}
				}

				// Y
				if(state.lY < axisCenter * axisThreshold) {
					axisFactor = std::min(1.0f, 1.0f - (state.lY / (axisCenter * axisThreshold)));
					this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LYN, axisFactor);
				} else {
					if(state.lY > axisCenter + (axisCenter * (1.0f - axisThreshold))) {
						axisFactor = std::min(1.0f, (state.lY - (axisCenter + (axisCenter * (1.0f - axisThreshold)))) / (axisCenter * axisThreshold));
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LYP, axisFactor);
					} else {
						axisFactor = 0;
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LYN, axisFactor);
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LYP, axisFactor);
					}
				}

				// Z
				if(state.lZ < axisCenter * axisThreshold) {
					axisFactor = std::min(1.0f, 1.0f - (state.lZ / (axisCenter * axisThreshold)));
					this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LZN, axisFactor);
				} else {
					if(state.lZ > axisCenter + (axisCenter * (1.0f - axisThreshold))) {
						axisFactor = std::min(1.0f, (state.lZ - (axisCenter + (axisCenter * (1.0f - axisThreshold)))) / (axisCenter * axisThreshold));
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LZP, axisFactor);
					} else {
						axisFactor = 0;
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LZN, axisFactor);
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_LZP, axisFactor);
					}
				}

				// R
				if(state.lRx < axisCenter * axisThreshold) {
					axisFactor = std::min(1.0f, 1.0f - (state.lRx / (axisCenter * axisThreshold)));
					this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RXN, axisFactor);
				} else {
					if(state.lRx > axisCenter + (axisCenter * (1.0f - axisThreshold))) {
						axisFactor = std::min(1.0f, (state.lRx - (axisCenter + (axisCenter * (1.0f - axisThreshold)))) / (axisCenter * axisThreshold));
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RXP, axisFactor);
					} else {
						axisFactor = 0;
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RXN, axisFactor);
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RXP, axisFactor);
					}
				}

				// U
				if(state.lRy < axisCenter * axisThreshold) {
					axisFactor = std::min(1.0f, 1.0f - (state.lRy / (axisCenter * axisThreshold)));
					this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RYN, axisFactor);
				} else {
					if(state.lRy > axisCenter + (axisCenter * (1.0f - axisThreshold))) {
						axisFactor = std::min(1.0f, (state.lRy - (axisCenter + (axisCenter * (1.0f - axisThreshold)))) / (axisCenter * axisThreshold));
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RYP, axisFactor);
					} else {
						axisFactor = 0;
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RYN, axisFactor);
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RYP, axisFactor);
					}
				}

				// V
				if(state.lRz < axisCenter * axisThreshold && state.lRz > 0) {
					axisFactor = std::min(1.0f, 1.0f - (state.lRz / (axisCenter * axisThreshold)));
					this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RZN, axisFactor);
				} else {
					if(state.lRz > axisCenter + (axisCenter * (1.0f - axisThreshold))) {
						axisFactor = std::min(1.0f, (state.lRz - (axisCenter + (axisCenter * (1.0f - axisThreshold)))) / (axisCenter * axisThreshold));
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RZP, axisFactor);
					} else {
						axisFactor = 0;
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RZN, axisFactor);
						this->m_input->SetGamepadKey(gamepadId, INPUT_KEY_GAMEPAD_AXIS_RZP, axisFactor);
					}
				}
			} else {
				// Try to acquire back the gamepad
				this->m_gamepad[gamepadId]->Acquire();
			}
		}
	}

	/**
	* The render method
	*/
	void IWin32Application::Render() {
		LOG_TRACE();
	}

	/**
	* Resize the window
	* @param width The new width
	* @param height The new height
	*/
	void IWin32Application::Resize(const int width, const int height) {
		LOG_TRACE();

		RECT rect;
		GetWindowRect(this->m_hWnd, &rect);
		this->m_displayBounds = CRectangleI(0, 0, rect.right - rect.left, rect.bottom - rect.top);
	}

	/**
	* Get the gamepads vector
	*/
	std::vector<LPDIRECTINPUTDEVICEA> & IWin32Application::GetGamepad() {
		LOG_TRACE();
		return this->m_gamepad;
	}

	/**
	* Get the Direct Input context
	*/
	const LPDIRECTINPUT IWin32Application::GetDirectInput() const {
		LOG_TRACE();
		return this->m_directInput;
	}

	/**
	* The global window procedure for messages
	*/
	LRESULT CALLBACK IWin32Application::GlobalWindowProc(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam) {
		LOG_TRACE();
		return s_instance->WindowProc(Hwnd, Message, WParam, LParam);
	}

	/**
	* The window procedure for messages
	*/
	LRESULT CALLBACK IWin32Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		LOG_TRACE();

		switch(message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;

		case WM_SETCURSOR:
			SetCursor(nullptr);
			return TRUE;
			break;

		case WM_SETFOCUS:
			this->m_hasFocus = true;
			ShowCursor(false);
			break;

		case WM_KILLFOCUS:
			this->m_hasFocus = false;
			ShowCursor(true);
			break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	/**
	* Called for each enumerated devices by Direct Input
	*/
	BOOL CALLBACK IWin32Application::CreateDeviceCallback(LPCDIDEVICEINSTANCE instance, void * reference) {
		LOG_TRACE();

		// Handle a maximum of MAX_GAMEPADS gamepad
		if(s_instance->GetGamepad().size() < MAX_GAMEPADS) {
			// Create the device
			LPDIRECTINPUTDEVICEA device;
			HRESULT result = s_instance->GetDirectInput()->CreateDevice(instance->guidInstance, &device, NULL);

			// If the creation succeeded
			if(SUCCEEDED(result)) {
				// Add the gamepad to the vector
				s_instance->GetGamepad().push_back(device);
			}
			return DIENUM_CONTINUE;
		}
		return DIENUM_STOP;
	}
}

#endif
