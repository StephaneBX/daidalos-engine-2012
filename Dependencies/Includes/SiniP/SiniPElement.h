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

#ifndef __SINIPELEMENT_H
#define __SINIPELEMENT_H

#include <string>

enum SINIP_element
{
	SINIP_ELEMENT_COMMENT,
	SINIP_ELEMENT_SECTION,
	SINIP_ELEMENT_PROPERTY
};

/**
* The basic class for all elements
*/
class SiniPElement
{
protected:
	SINIP_element m_type;

public:
	virtual ~SiniPElement();
	SINIP_element GetType() const;
	virtual const std::string ToString() const = 0;
};

#endif
