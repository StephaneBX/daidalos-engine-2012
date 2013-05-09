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

#ifdef APPLICATION_USE_SDL

#ifdef WIN32
	#include "../Renderers/OGL/COGLRenderer.h"
#endif

#include "../Renderers/DX9/CDX9Renderer.h"

#include "../Core/IRenderer.h"
#include "../Core/Utility/CTimer.h"
#include "../Applications/ISDLApplication.h"

#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib")

namespace DE
{
	EInputKey ISDLApplication::s_keyCode[] =
	{
		INPUT_KEY_UNDEFINED, // 0
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 5
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_BACKSPACE,
		INPUT_KEY_KEYBOARD_TAB,
		INPUT_KEY_UNDEFINED, // 10
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_CLEAR,
		INPUT_KEY_KEYBOARD_RETURN,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 15
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_PAUSE,
		INPUT_KEY_UNDEFINED, // 20
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
		INPUT_KEY_KEYBOARD_EXCLAIMATION,
		INPUT_KEY_KEYBOARD_QUOTEDOUBLE,
		INPUT_KEY_KEYBOARD_HASH, // 35
		INPUT_KEY_KEYBOARD_DOLLAR,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_AMPERSAND,
		INPUT_KEY_KEYBOARD_QUOTE,
		INPUT_KEY_KEYBOARD_LEFTPARENTHESIS, // 40
		INPUT_KEY_KEYBOARD_RIGHTPARENTHESIS,
		INPUT_KEY_KEYBOARD_ASTERISK,
		INPUT_KEY_KEYBOARD_PLUS,
		INPUT_KEY_KEYBOARD_COMMA,
		INPUT_KEY_KEYBOARD_MINUS, // 45
		INPUT_KEY_KEYBOARD_PERIOD,
		INPUT_KEY_KEYBOARD_SLASH,
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
		INPUT_KEY_KEYBOARD_COLON,
		INPUT_KEY_KEYBOARD_SEMICOLON,
		INPUT_KEY_KEYBOARD_LESS, // 60
		INPUT_KEY_KEYBOARD_EQUALS,
		INPUT_KEY_KEYBOARD_GREATER,
		INPUT_KEY_KEYBOARD_QUESTION,
		INPUT_KEY_KEYBOARD_AT,
		INPUT_KEY_UNDEFINED, // 65
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 70
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 75
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 80
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 85
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 90
		INPUT_KEY_KEYBOARD_LEFTBRACKET,
		INPUT_KEY_KEYBOARD_BACKSLASH,
		INPUT_KEY_KEYBOARD_RIGHTBRACKET,
		INPUT_KEY_KEYBOARD_CARET,
		INPUT_KEY_KEYBOARD_UNDERSCORE, // 95
		INPUT_KEY_KEYBOARD_BACKQUOTE,
		INPUT_KEY_KEYBOARD_A,
		INPUT_KEY_KEYBOARD_B,
		INPUT_KEY_KEYBOARD_C,
		INPUT_KEY_KEYBOARD_D, // 100
		INPUT_KEY_KEYBOARD_E,
		INPUT_KEY_KEYBOARD_F,
		INPUT_KEY_KEYBOARD_G,
		INPUT_KEY_KEYBOARD_H,
		INPUT_KEY_KEYBOARD_I, // 105
		INPUT_KEY_KEYBOARD_J,
		INPUT_KEY_KEYBOARD_K,
		INPUT_KEY_KEYBOARD_L,
		INPUT_KEY_KEYBOARD_M,
		INPUT_KEY_KEYBOARD_N, // 110
		INPUT_KEY_KEYBOARD_O,
		INPUT_KEY_KEYBOARD_P,
		INPUT_KEY_KEYBOARD_Q,
		INPUT_KEY_KEYBOARD_R,
		INPUT_KEY_KEYBOARD_S, // 115
		INPUT_KEY_KEYBOARD_T,
		INPUT_KEY_KEYBOARD_U,
		INPUT_KEY_KEYBOARD_V,
		INPUT_KEY_KEYBOARD_W,
		INPUT_KEY_KEYBOARD_X, // 120
		INPUT_KEY_KEYBOARD_Y,
		INPUT_KEY_KEYBOARD_Z,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 125
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_DELETE,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 130
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 140
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 150
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 160
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 170
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 180
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 190
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 200
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 210
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 220
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 230
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 240
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED, // 250
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_KP0,
		INPUT_KEY_KEYBOARD_KP1,
		INPUT_KEY_KEYBOARD_KP2,
		INPUT_KEY_KEYBOARD_KP3,
		INPUT_KEY_KEYBOARD_KP4, // 260
		INPUT_KEY_KEYBOARD_KP5,
		INPUT_KEY_KEYBOARD_KP6,
		INPUT_KEY_KEYBOARD_KP7,
		INPUT_KEY_KEYBOARD_KP8,
		INPUT_KEY_KEYBOARD_KP9,
		INPUT_KEY_KEYBOARD_KP_PERIOD,
		INPUT_KEY_KEYBOARD_KP_DIVIDE,
		INPUT_KEY_KEYBOARD_KP_MULTIPLY,
		INPUT_KEY_KEYBOARD_KP_MINUS,
		INPUT_KEY_KEYBOARD_KP_PLUS, // 270
		INPUT_KEY_KEYBOARD_KP_ENTER,
		INPUT_KEY_KEYBOARD_KP_EQUALS,
		INPUT_KEY_KEYBOARD_UP,
		INPUT_KEY_KEYBOARD_DOWN,
		INPUT_KEY_KEYBOARD_RIGHT,
		INPUT_KEY_KEYBOARD_LEFT,
		INPUT_KEY_KEYBOARD_INSERT,
		INPUT_KEY_KEYBOARD_HOME,
		INPUT_KEY_KEYBOARD_END,
		INPUT_KEY_KEYBOARD_PAGEUP, // 280
		INPUT_KEY_KEYBOARD_PAGEDOWN,
		INPUT_KEY_KEYBOARD_F1,
		INPUT_KEY_KEYBOARD_F2,
		INPUT_KEY_KEYBOARD_F3,
		INPUT_KEY_KEYBOARD_F4,
		INPUT_KEY_KEYBOARD_F5,
		INPUT_KEY_KEYBOARD_F6,
		INPUT_KEY_KEYBOARD_F7,
		INPUT_KEY_KEYBOARD_F8,
		INPUT_KEY_KEYBOARD_F9, // 290
		INPUT_KEY_KEYBOARD_F10,
		INPUT_KEY_KEYBOARD_F11,
		INPUT_KEY_KEYBOARD_F12,
		INPUT_KEY_KEYBOARD_F13,
		INPUT_KEY_KEYBOARD_F14,
		INPUT_KEY_KEYBOARD_F15,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_UNDEFINED,
		INPUT_KEY_KEYBOARD_NUMLOCK, // 300
		INPUT_KEY_KEYBOARD_CAPSLOCK,
		INPUT_KEY_KEYBOARD_SCROLLOCK,
		INPUT_KEY_KEYBOARD_RSHIFT,
		INPUT_KEY_KEYBOARD_LSHIFT,
		INPUT_KEY_KEYBOARD_RCTRL,
		INPUT_KEY_KEYBOARD_LCTRL,
		INPUT_KEY_KEYBOARD_RALT,
		INPUT_KEY_KEYBOARD_LALT,
		INPUT_KEY_KEYBOARD_RMETA,
		INPUT_KEY_KEYBOARD_LMETA, // 310
		INPUT_KEY_KEYBOARD_LSUPER,
		INPUT_KEY_KEYBOARD_RSUPER,
		INPUT_KEY_KEYBOARD_ALTGR,
		INPUT_KEY_KEYBOARD_COMPOSE
	};

	ISDLApplication::ISDLApplication(const std::string & title) : IApplication()
	{
		this->m_name = title;

		#ifdef _WIN32
			_putenv((const char*)"SDL_VIDEO_WINDOW_POS=center");
		#else
			putenv((const char*)"SDL_VIDEO_WINDOW_POS=center");
		#endif

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
		atexit(SDL_Quit);
		SDL_WM_SetCaption(this->m_name.c_str(), nullptr);

		InitializeWindow();

		// Open the plugged gamepad
		this->m_gamepads.assign(nullptr);
		for(int i = 0 ; i < SDL_NumJoysticks() ; i++)
		{
			this->m_gamepads[i] = SDL_JoystickOpen(i);
		}
	}

	ISDLApplication::~ISDLApplication() 
	{
		for(unsigned int i = 0 ; i < this->m_gamepads.size() ; i++)
		{
			if(this->m_gamepads[i] != nullptr)
			{
				SDL_JoystickClose(this->m_gamepads[i]);
			}
		}
	}


	void ISDLApplication::InitializeWindow()
	{
		if(this->m_fullscreen)
			this->m_screen = SDL_SetVideoMode(this->m_width, this->m_height,32, SDL_SWSURFACE | SDL_HWSURFACE | SDL_FULLSCREEN);
		else
			this->m_screen = SDL_SetVideoMode(this->m_width, this->m_height,32, SDL_SWSURFACE | SDL_HWSURFACE);

		this->m_displayBounds = CRectangleI(this->m_screen->clip_rect.x, this->m_screen->clip_rect.y, this->m_screen->clip_rect.w, this->m_screen->clip_rect.h);
	
		SDL_ShowCursor(0);
		SDL_WM_SetCaption(this->m_name.c_str(), nullptr);
		SDL_WM_GrabInput(SDL_GRAB_ON);

		// Renderer creation
		#ifdef _WIN32
			this->m_hWnd = GetActiveWindow();

			if(this->m_rendererName == "dx9")
				IRenderer::SetRenderer(new CDX9Renderer(this->m_verticalSynchro, this->m_anisotropicFiltering, this->m_hWnd));
			else
				IRenderer::SetRenderer(new COGLRenderer(this->m_verticalSynchro, this->m_anisotropicFiltering, this->m_hWnd));
		#else
			IRenderer::SetRenderer(new COGLRenderer(this->m_verticalSynchro, this->m_anisotropicFiltering, this->m_hWnd));
		#endif

		this->m_renderer = DE::IRenderer::GetRenderer();
		this->m_renderer->SetDisplayBounds(this->m_displayBounds);

		this->m_gui->Initialize();
	}

	void ISDLApplication::MainLoop()
	{
		CTimer timer;
		float time;
		float realTime;
		double previousTime = timer.Elapsed();

		const float maxValue = 32767;
		float axisFactor = 0;

		while(this->m_alive)
		{
			time = (float)((timer.Elapsed() - previousTime) * 1000);
			realTime = time;
			previousTime = timer.Elapsed();

			if(time > MAXIMUM_FRAME_TIME)
				time = MAXIMUM_FRAME_TIME;


			SDL_Event event;
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
				case SDL_KEYDOWN:
					this->m_input->SetKeyDown(s_keyCode[event.key.keysym.sym]);
					break;

				case SDL_KEYUP:
					this->m_input->SetKeyUp(s_keyCode[event.key.keysym.sym]);
					break;

				case SDL_MOUSEBUTTONDOWN:
					switch(event.button.button)
					{
					case SDL_BUTTON_WHEELUP:
						this->m_input->SetKeyDown(INPUT_KEY_MOUSE_WHEELUP);
						break;

					case SDL_BUTTON_WHEELDOWN:
						this->m_input->SetKeyDown(INPUT_KEY_MOUSE_WHEELDOWN);
						break;

					default:
						this->m_input->SetKeyDown(s_keyCode[INPUT_KEY_MOUSE_BUTTON0 + event.button.button]);
						break;
					}
					break;

				case SDL_MOUSEBUTTONUP:
					switch(event.button.button)
					{
					case SDL_BUTTON_WHEELUP:
						this->m_input->SetKeyUp(INPUT_KEY_MOUSE_WHEELUP);
						break;

					case SDL_BUTTON_WHEELDOWN:
						this->m_input->SetKeyUp(INPUT_KEY_MOUSE_WHEELDOWN);
						break;

					default:
						this->m_input->SetKeyUp(s_keyCode[INPUT_KEY_MOUSE_BUTTON0 + event.button.button]);
						break;
					}
					break;

				case SDL_MOUSEMOTION:
					if(!(event.motion.x == event.motion.xrel && event.motion.y == event.motion.yrel))
						this->m_input->MoveMouse(CVector2I(event.motion.x, event.motion.y), CVector2I(event.motion.xrel, event.motion.yrel));
					break;

				case SDL_JOYAXISMOTION:
					
					if(event.jaxis.value < -maxValue * GAMEPAD_AXIS_THRESHOLD)
					{
						axisFactor = std::min(1.0f, -(event.jaxis.value + (maxValue * GAMEPAD_AXIS_THRESHOLD)) / (maxValue * (1.0f - GAMEPAD_AXIS_THRESHOLD)));

						// Call the right axis
						switch(event.jaxis.axis)
						{
						case 0:
							this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LXN, axisFactor);
							break;

						case 1:
							this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LYN, axisFactor);
							break;

						case 2:
							this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LZN, axisFactor);
							break;

						case 3:
							this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RXN, axisFactor);
							break;

						case 4:
							this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RYN, axisFactor);
							break;

						default:
							this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RZN, axisFactor);
							break;
						}
						
					}
					else
					{
						if(event.jaxis.value > maxValue * GAMEPAD_AXIS_THRESHOLD)
						{
							axisFactor = std::min(1.0f, (event.jaxis.value - (maxValue * GAMEPAD_AXIS_THRESHOLD)) / (maxValue * (1.0f - GAMEPAD_AXIS_THRESHOLD)));

							// Call the right axis
							switch(event.jaxis.axis)
							{
							case 0:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LXP, axisFactor);
								break;

							case 1:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LYP, axisFactor);
								break;

							case 2:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LZP, axisFactor);
								break;

							case 3:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RXP, axisFactor);
								break;

							case 4:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RYP, axisFactor);
								break;

							default:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RZP, axisFactor);
								break;
							}
						}
						else
						{
							axisFactor = 0;
							// Call the right axis
							switch(event.jaxis.axis)
							{
							case 0:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LXN, axisFactor);
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LXP, axisFactor);
								break;

							case 1:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LYN, axisFactor);
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LYP, axisFactor);
								break;

							case 2:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LZN, axisFactor);
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_LZP, axisFactor);
								break;

							case 3:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RXN, axisFactor);
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RXP, axisFactor);
								break;

							case 4:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RYN, axisFactor);
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RYP, axisFactor);
								break;

							default:
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RZN, axisFactor);
								this->m_input->SetGamepadKey(event.jaxis.which, INPUT_KEY_GAMEPAD_AXIS_RZP, axisFactor);
								break;
							}
						}
					}

					break;

				case SDL_JOYBUTTONDOWN:
					// Boundary check
					if(event.jbutton.which < INPUT_KEY_GAMEPAD_BUTTON_END)
					{
						this->m_input->SetGamepadKey(event.jbutton.which, (EInputKeyGamepad)(INPUT_KEY_GAMEPAD_BUTTON0 + event.jbutton.button), 1.0f);
					}
					break;

				case SDL_JOYBUTTONUP:
					this->m_input->SetGamepadKey(event.jbutton.which, (EInputKeyGamepad)(INPUT_KEY_GAMEPAD_BUTTON0 + event.jbutton.button), 0);
					break;

				case SDL_QUIT:
					this->m_alive = false;
					break;

				case SDL_VIDEORESIZE:
					Resize(event.resize.w, event.resize.h);
				}
			}

			Update(time);
			Render();
		}
	}

	void ISDLApplication::Update(const float time) {}

	void ISDLApplication::UpdateGamepads()
	{

	}

	void ISDLApplication::Render() {}

	void ISDLApplication::Resize(const int width, const int height) 
	{
		if(this->m_renderer)
			this->m_renderer->SetDisplayBounds(CRectangleI(this->m_screen->clip_rect.x, this->m_screen->clip_rect.y, this->m_screen->clip_rect.w, this->m_screen->clip_rect.h));
	}
}

#endif
