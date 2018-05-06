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

#include "../Loaders/CMaterialsLoader.hpp"
#include "../Loaders/CImagesLoader.hpp"
#include "../Loaders/CMS3DLoader.hpp"
#include "../Loaders/CMD2Loader.hpp"
#include "../Loaders/COBJLoader.hpp"
#include "../Loaders/CSceneLoader.hpp"
#include "../Loaders/CStaticSoundLoader.hpp"
#include "../Loaders/CStreamSoundLoader.hpp"
#include "../Loaders/CFontLoader.hpp"
#include "../Loaders/CParticleEmittersTemplateLoader.hpp"
#include "../Core/IRenderer.hpp"
#include "../Core/Logger/CLoggerFile.hpp"
#include "../Core/CMediaManager.hpp"
#include "../Core/CResourceManager.hpp"
#include "../Core/CSoundContextManager.hpp"
#include "../Core/CInputDevice.hpp"
#include "../Core/Utility/CTimer.hpp"
#include "../Core/IApplication.hpp"

#define MAXIMUM_FRAME_TIME 50

namespace daidalosengine {
	IApplication::IApplication(const std::string & name) {
		// Log file
		ILogger::SetLogger(new CLoggerFile("application.log"));
		LOG_TRACE();

		// Seed initialization for the main thread
		srand((unsigned int)time(nullptr));

		this->m_name = name;
		this->m_renderer = nullptr;
		this->m_soundContext = CSoundContextManager::GetInstance();
		this->m_options = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);

		this->m_alive = true;
		this->m_width = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_RESOLUTION_X, 800);
		this->m_height = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_RESOLUTION_Y, 600);
		this->m_anisotropicFiltering = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_ANISOTROPIC, 0);
		this->m_antialiasing = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_ANTIALIASING, 0);
		this->m_fullscreen = this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_FULLSCREEN, false);
		this->m_verticalSynchro = this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_SYNCHRO, false);
		this->m_rendererName = this->m_options.GetValueString(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_RENDERER, "dx9");

		this->m_fillMode = RENDER_STATE_FILL_MODE_SOLID;
		int fillMode = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_FILL_MODE, 0);
		if(fillMode >= RENDER_STATE_FILL_MODE_POINT && fillMode <= RENDER_STATE_FILL_MODE_SOLID) {
			this->m_fillMode = (ERenderStateFillMode)fillMode;
		}
		

		// General loaders
		CMediaManager::GetInstance()->AddLoader(new CMaterialsLoader(), "mtl");
		CMediaManager::GetInstance()->AddLoader(new CImagesLoader(), "bmp dds jpg png pnm tga pcx tif");
		CMediaManager::GetInstance()->AddLoader(new COBJLoader(), "obj");
		CMediaManager::GetInstance()->AddLoader(new CMD2Loader(), "md2");
		CMediaManager::GetInstance()->AddLoader(new CMS3DLoader(), "ms3d");
		CMediaManager::GetInstance()->AddLoader(new CSceneLoader(), "des");
		CMediaManager::GetInstance()->AddLoader(new CStaticSoundLoader(), "wav ogg flac");
		CMediaManager::GetInstance()->AddLoader(new CStreamSoundLoader(), "wav ogg flac");
		CMediaManager::GetInstance()->AddLoader(new CFontLoader(), "fnt");
		CMediaManager::GetInstance()->AddLoader(new CParticleEmittersTemplateLoader(), "dept");

		// Default search path
		CMediaManager::GetInstance()->AddSearchPath("");
		CMediaManager::GetInstance()->AddSearchPath("Resources/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Textures/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Textures/Sky/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Textures/Fonts/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Models/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Materials/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Sounds/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Scenes/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Fonts/");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Particles/");
	}

	IApplication::~IApplication() {
		LOG_TRACE();

		IRenderer::Kill();
		CMediaManager::GetInstance()->RemoveLoader<IMaterialBase>();
		CMediaManager::GetInstance()->RemoveLoader<CImage>();
		CMediaManager::GetInstance()->RemoveLoader<IMeshBase>();
		CMediaManager::GetInstance()->RemoveLoader<IAnimatedMeshBase>();
		CMediaManager::GetInstance()->RemoveLoader<ISceneNode>();
		CMediaManager::GetInstance()->RemoveLoader<IStaticSoundBase>();
		CMediaManager::GetInstance()->RemoveLoader<IStreamSoundBase>();
		CMediaManager::GetInstance()->RemoveLoader<IFontBase>();
		CMediaManager::GetInstance()->RemoveLoader<IParticleEmittersTemplate>();
		CConfigurationManager::Kill();
		CInputDevice::Kill();
		CSoundContextManager::Kill();
		CMediaManager::Kill();
		CResourceManager::Kill();
		ILogger::Kill();
	}

	/**
	* Check if the application is alive
	* @return True if the application is alive
	*/
	const bool IApplication::IsAlive() const {
		return this->m_alive;
	}

	/**
	* Check if the fullscreen is enabled
	* @return True if the fullscreen is enabled
	*/
	const bool IApplication::IsFullScreen() const {
		return this->m_fullscreen;
	}

	/**
	* Check if the vertical synchro is enabled
	* @return True if the vertical synchro is enabled
	*/
	const bool IApplication::IsVerticalSynchro() const {
		return this->m_verticalSynchro;
	}

	/**
	* Get the width
	* @return The width
	*/
	const int IApplication::GetWidth() const {
		return this->m_width;
	}

	/**
	* Get the height
	* @return The height
	*/
	const int IApplication::GetHeight() const {
		return this->m_height;
	}

	/**
	* Get the anisotropic filterting level
	* @return The anisotropic filterting level
	*/
	const int IApplication::GetAnisotropicFiltering() const {
		return this->m_anisotropicFiltering;
	}

	/**
	* Get the antialiasing level
	* @return The antialiasing level
	*/
	const int IApplication::GetAntialiasing() const {
		return this->m_antialiasing;
	}

	/**
	* Get the name
	* @return The name
	*/
	const std::string & IApplication::GetName() {
		return this->m_name;
	}

	/**
	* Get the options
	* @return The options
	*/
	const SiniP & IApplication::GetOptions() const {
		return this->m_options;
	}

	/**
	* Get the display bounds
	* @return The display bounds
	*/
	const CRectangleI & IApplication::GetDisplayBounds() const {
		return this->m_displayBounds;
	}

	/**
	* Get the fill mode
	* @return The fill mode
	*/
	const ERenderStateFillMode IApplication::GetFillMode() const {
		return this->m_fillMode;
	}

	/**
	* Set the alive state
	* @param alive The alive state
	*/
	void IApplication::SetAlive(const bool alive) {
		this->m_alive = alive;
	}

	/**
	* Set the display bounds
	* @param displayBounds The display bounds
	*/
	void IApplication::SetDisplayBounds(const CRectangleI & displayBounds) {
		this->m_displayBounds = displayBounds;
	}
}

