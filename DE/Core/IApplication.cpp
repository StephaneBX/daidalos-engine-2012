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

#include "../Loaders/CMaterialsLoader.h"
#include "../Loaders/CImagesLoader.h"
#include "../Loaders/CMS3DLoader.h"
#include "../Loaders/CMD2Loader.h"
#include "../Loaders/COBJLoader.h"
#include "../Loaders/CSceneLoader.h"
#include "../Loaders/CStaticSoundLoader.h"
#include "../Loaders/CStreamSoundLoader.h"
#include "../Loaders/CFontLoader.h"
#include "../Scene/CSceneManager.h"
#include "../Core/IRenderer.h"
#include "../Core/Logger/CLoggerFile.h"
#include "../Core/CMediaManager.h"
#include "../Core/CResourceManager.h"
#include "../Core/CSoundContextManager.h"
#include "../Core/CGUIManager.h"
#include "../Core/Utility/CTimer.h"
#include "../Core/IApplication.h"

#define MAXIMUM_FRAME_TIME 50

namespace DE
{
	IApplication::IApplication()
	{
		this->m_renderer = nullptr;
		this->m_input = CInputDevice::GetInstance();
		this->m_soundContext = CSoundContextManager::GetInstance();
		this->m_options = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);
		this->m_gui = CGUIManager::GetInstance();

		this->m_alive = true;
		this->m_width = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_RESOLUTION_X, 800);
		this->m_height = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_RESOLUTION_Y, 600);
		this->m_anisotropicFiltering = this->m_options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_ANISOTROPIC, 0);
		this->m_fullscreen = this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_FULLSCREEN, false);
		this->m_verticalSynchro = this->m_options.GetValueBool(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_SYNCHRO, false);
		this->m_rendererName = this->m_options.GetValueString(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_RENDERER, "dx9");

		// Log file
		ILogger::SetLogger(new CLoggerFile("log.txt"));

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
	}

	IApplication::~IApplication()
	{
		IRenderer::Kill();
		CMediaManager::GetInstance()->RemoveLoader<IMaterialBase>();
		CMediaManager::GetInstance()->RemoveLoader<CImage>();
		CMediaManager::GetInstance()->RemoveLoader<IMeshBase>();
		CMediaManager::GetInstance()->RemoveLoader<IAnimatedMeshBase>();
		CMediaManager::GetInstance()->RemoveLoader<ISceneNode>();
		CMediaManager::GetInstance()->RemoveLoader<IStaticSoundBase>();
		CMediaManager::GetInstance()->RemoveLoader<IStreamSoundBase>();
		CMediaManager::GetInstance()->RemoveLoader<IFontBase>();
		CConfigurationManager::Kill();
		CInputDevice::Kill();
		CSoundContextManager::Kill();
		CGUIManager::Kill();
		CMediaManager::Kill();
		CSceneManager::Kill();
		CResourceManager::Kill();
		ILogger::Kill();
	}
}

