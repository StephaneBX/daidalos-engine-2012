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

#ifndef __SINIPCOMMENT_H
#define __SINIPCOMMENT_H

#include <string>
#include "SiniPElement.h"

/**
* The comment class.
* Handle a comment element
*/
class SiniPComment : public SiniPElement
{
protected:
	std::string m_comment;

public:
	SiniPComment(const std::string & comment);
	void SetComment(const std::string & comment);
	const std::string ToString() const;
};

#endif
