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

#ifndef __SINIPPROPERTY_H
#define __SINIPPROPERTY_H

#include <string>
#include "SiniPElement.h"

/**
* The property class.
* A property is composed of
* - A key
* - A value
* - A comment (optional)
*/
class SiniPProperty : public SiniPElement
{
protected:
	std::string m_key;
	std::string m_value;
	std::string m_comment;

public:
	SiniPProperty(const std::string & key, const std::string & value);
	SiniPProperty(const std::string & key, const std::string & value, const std::string & comment);
	void SetValue(const std::string & value);
	void SetComment(const std::string & comment);
	const std::string & GetKey() const;
	const std::string & GetValue() const;
	const std::string & GetComment() const;
	const std::string ToString() const;
};

#endif
