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

#ifndef __CMEDIAMANAGER_H
#define __CMEDIAMANAGER_H

#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <set>
#include "../Core/Utility/CTokenizer.h"
#include "../Core/Utility/CFile.h"
#include "../Core/Memory/CExceptionLoad.h"
#include "../Core/Logger/ILogger.h"
#include "../Core/ILoader.h"
#include "../Core/CLoaderExtensions.h"
#include "../Core/CSingletonManager.h"

namespace DE
{
	/**
	* Media manager class
	* Handle the  process of loading and saving medias by using the correct loader
	*/
	class CMediaManager : public CSingletonManager<CMediaManager>
	{
		friend class CSingletonManager<CMediaManager>;

	private:
		std::set<std::string> m_paths;

	public:
		/**
		* Add a loader in the loaders list
		* @param loader A loader pointer
		* @param extensions A list of extensions supported by the loader, separated bu a space character
		*/
		template <class T> void AddLoader(ILoader<T> * loader, const std::string & extensions);

		/**
		* Unload a type of loader
		*/
		template <class T> void RemoveLoader();

		/**
		* Save a media into a file
		* @param object The object to save
		* @param filename The filename
		*/
		template <class T> void SaveMediaToFile(const T * object, const std::string & filename);

		/**
		* Load a media from a file
		* Find the corresponding loader and load the file if a compatible loader is found
		* @param filename The filename
		* @return The media
		*/
		template <class T> T * LoadMediaFromFile(const std::string & filename);

		/**
		* Add a search path for the medias.
		* @param path The path to add.
		*/
		void AddSearchPath(const std::string & path);

	private:
		/**
		* Find a media in the paths.
		* @param filename The file fname.
		* @return The file fullname.
		*/
		CFile FindMedia(const std::string & filename);

	private:
		/**
		* Constructor
		*/
		CMediaManager();
		CMediaManager(const CMediaManager & copy);
		const CMediaManager & operator=(const CMediaManager & copy);
	};
	
	template <class T> void CMediaManager::AddLoader(ILoader<T> * loader, const std::string & extensions)
	{
		CTokenizer tokenizer;
		//std::transform(extensions.begin(), extensions.end(), extensions.begin(), tolower);
		tokenizer.AddLine(extensions);
		tokenizer.Tokenize(" ");

		std::shared_ptr<ILoader<T>> ptr(loader);

		for(unsigned int i = 0 ; i < tokenizer.GetTokens().size() ; ++i)
			CLoaderExtensions<T>::s_loaders[tokenizer.GetTokens().at(i)] = ptr;
	}
	
	template <class T> void CMediaManager::RemoveLoader()
	{
		CLoaderExtensions<T>::s_loaders.clear();
	}

	template <class T> void CMediaManager::SaveMediaToFile(const T * object, const std::string & filename)
	{
		const std::string extension = CFile(filename).GetExtension();
		if(CLoaderExtensions<T>::IsExtensionFound(extension))
			CLoaderExtensions<T>::s_loaders[extension]->SaveToFile(object, filename);
	}

	template <class T> T * CMediaManager::LoadMediaFromFile(const std::string & filename) /* throw(CExceptionLoad) */
	{
		// Search the full path for the media
		CFile MediaPath = FindMedia(filename);

		// Find the loader to use to load the media
		const std::string extension = CFile(filename).GetExtension();
		if(CLoaderExtensions<T>::IsExtensionFound(extension))
		{
			return CLoaderExtensions<T>::s_loaders[extension]->LoadFromFile(MediaPath.GetFullname());
		}
		else
		{
			LOG_ERROR("Unable to load the file " << filename << "'");
			ILogger::Kill();

			throw new CExceptionLoad(filename);
		}
	}
}

#endif
