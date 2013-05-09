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

#ifndef __SINIPSECTION_H
#define __SINIPSECTION_H

#include <string>
#include <vector>
#include <memory>
#include "SiniPProperty.h"

typedef std::vector<SiniPElement*> TSectionElements;

/**
* The section class.
* A section is composed of
* - A name (except for the anonymous section)
* - A set of elements
*/
class SiniPSection : public SiniPElement
{
protected:
	std::string m_name;
	TSectionElements m_data;

public:
	SiniPSection(const SiniPSection & section);
	SiniPSection(const std::string & name);
	~SiniPSection();
	void SetName(const std::string & name);
	void AddElement(SiniPElement * element);
	void Copy(const SiniPSection & section);
	const SiniPSection & operator=(const SiniPSection & section);
	const TSectionElements & GetElements() const;
	const std::string & GetName() const;
	const std::string ToString() const;
	SiniPProperty * GetProperty(const std::string & key) const;

protected:
	void Clear();
};

#endif
