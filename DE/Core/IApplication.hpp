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

#ifndef __IAPPLICATION_HPP
#define __IAPPLICATION_HPP

#include <string>
#include "../Core/Utility/CRectangle.hpp"
#include "../Core/CConfigurationManager.hpp"
#include "../Core/CSoundContextManager.hpp"

#pragma comment(lib, "SiniP.lib")
#pragma comment(lib, "tinyxml2.lib")
#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "libsndfile-1.lib")

#define APPLICATION_NAME "Daidalos Engine"
#define MAXIMUM_FRAME_TIME 50

namespace daidalosengine {
	/**
	* Basic class for the application
	*/
	class IApplication {
	private:
		bool m_alive;
		bool m_fullscreen;
		bool m_verticalSynchro;
		int m_width;
		int m_height;
		int m_anisotropicFiltering;
		int m_antialiasing;
		ERenderStateFillMode m_fillMode;
		std::string m_rendererName;
		std::string m_name;
		CRectangleI m_displayBounds;
		SiniP m_options;
		IRenderer * m_renderer;
		CSoundContextManager * m_soundContext;

	public:
		/**
		* Constructor
		* @param name The application name
		*/
		explicit IApplication(const std::string & name);

		/**
		* The destructor
		*/
		virtual ~IApplication();

	protected:
		/**
		* Check if the application is alive
		* @return True if the application is alive
		*/
		const bool IsAlive() const;

		/**
		* Check if the fullscreen is enabled
		* @return True if the fullscreen is enabled
		*/
		const bool IsFullScreen() const;

		/**
		* Check if the vertical synchro is enabled
		* @return True if the vertical synchro is enabled
		*/
		const bool IsVerticalSynchro() const;

		/**
		* Get the width
		* @return The width
		*/
		const int GetWidth() const;

		/**
		* Get the height
		* @return The height
		*/
		const int GetHeight() const;

		/**
		* Get the anisotropic filterting level
		* @return The anisotropic filterting level
		*/
		const int GetAnisotropicFiltering() const;

		/**
		* Get the antialiasing level
		* @return The antialiasing level
		*/
		const int GetAntialiasing() const;

		/**
		* Get the fill mode
		* @return The fill mode
		*/
		const ERenderStateFillMode GetFillMode() const;

		/**
		* Get the name
		* @return The name
		*/
		const std::string & GetName();

		/**
		* Get the options
		* @return The options
		*/
		const SiniP & GetOptions() const;

		/**
		* Get the display bounds
		* @return The display bounds
		*/
		const CRectangleI & GetDisplayBounds() const;

		/**
		* Set the alive state
		* @param alive The alive state
		*/
		void SetAlive(const bool alive);

		/**
		* Set the display bounds
		* @param displayBounds The display bounds
		*/
		void SetDisplayBounds(const CRectangleI & displayBounds);

		/**
		* Initialize the window
		*/
		virtual void InitializeWindow() = 0;

		/**
		* The main loop
		*/
		virtual void MainLoop() = 0;

		/**
		* The update method
		* @param time Elapsed time
		*/
		virtual void Update(const float time) = 0;

		/**
		* Update the gamepad inputs
		*/
		virtual void UpdateGamepads() = 0;

		/**
		* The render method
		*/
		virtual void Render() = 0;

		/**
		* Resize the window
		* @param width The ne width
		* @param height The new height
		*/
		virtual void Resize(const int width, const int height) = 0;

	private:
		IApplication(const IApplication & copy);
		const IApplication & operator=(const IApplication & copy) = delete;
	};
}

#endif
