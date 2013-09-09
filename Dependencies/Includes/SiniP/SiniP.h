/*
==============================================================================================================================================================

www.sourceforge.net/projects/sinip
Copyright Stephane Baudoux (www.stephane-baudoux.com)

This file is part of SiniP.

SiniP is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SiniP is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SiniP.  If not, see <http://www.gnu.org/licenses/>.

==============================================================================================================================================================
*/

#ifndef __SINIP_H
#define __SINIP_H

#ifdef SINIP_DLL_EXPORTS
#define SINIP_DLL_API __declspec(dllexport) 
#else
#define SINIP_DLL_API 
#endif

#include <utility>
#include <string>
#include <vector>
#include <memory>
#include "SiniPElement.h"
#include "SiniPSection.h"

class SiniPProperty;

/**
* The Simple ini Parser main class
* Contains all the functions to open, read, modify and save the content of an ini file
*/
class SiniP
{
protected:
	std::string m_filename;
	std::vector<SiniPSection*> m_data;

public:
	SINIP_DLL_API SiniP();
	SINIP_DLL_API SiniP(const SiniP & sinip);
	SINIP_DLL_API SiniP(const std::string & filename);
	SINIP_DLL_API ~SiniP();
	SINIP_DLL_API void Parse(const std::string & filename);
	SINIP_DLL_API void SetValueBool(const std::string & section, const std::string & key, const bool value);
	SINIP_DLL_API void SetValueInt(const std::string & section, const std::string & key, const int value);
	SINIP_DLL_API void SetValueFloat(const std::string & section, const std::string & key, const float value);
	SINIP_DLL_API void SetValueDouble(const std::string & section, const std::string & key, const double value);
	SINIP_DLL_API void SetValueString(const std::string & section, const std::string & key, const std::string & value);
	SINIP_DLL_API void Clear();
	SINIP_DLL_API void RemoveSection(const std::string & section);
	SINIP_DLL_API void RemoveAllComments();
	SINIP_DLL_API void Copy(const SiniP & sinip);
	SINIP_DLL_API void Save();
	SINIP_DLL_API void SaveToFile(const std::string & filename);
	SINIP_DLL_API const SiniP & operator=(const SiniP & sinip);
	SINIP_DLL_API const bool GetValueBool(const std::string & section, const std::string & key, const bool defaultValue) const;
	SINIP_DLL_API const int GetValueInt(const std::string & section, const std::string & key, const int defaultValue) const;
	SINIP_DLL_API const float GetValueFloat(const std::string & section, const std::string & key, const float defaultValue) const;
	SINIP_DLL_API const double GetValueDouble(const std::string & section, const std::string & key, const double defaultValue) const;
	SINIP_DLL_API const std::string GetValueString(const std::string & section, const std::string & key, const std::string & defaultValue) const;

protected:
	const std::string Trim(const std::string & text) const;
	const std::string GetValue(const std::string & section, const std::string & key) const;
	SiniPSection * GetSection(const std::string & section) const;
	SiniPProperty * GetProperty(SiniPSection * section, const std::string & key) const;
};

#endif
