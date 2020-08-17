/*
* Copyright (C) 2008-2018 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "GitRevision.h"
#include "CompilerDefs.h"
#include "revision.h"

char const* GitRevision::GetHash()
{
	return _HASH;
}

char const* GitRevision::GetDate()
{
	return _DATE;
}


#define _PACKAGENAME "Trinity"

char const* GitRevision::GetFullVersion()
{
#if PLATFORM == PLATFORM_WINDOWS
# ifdef _WIN64
	return _PACKAGENAME " rev. " VER_PRODUCTVERSION_STR " (Win64, " _BUILD_DIRECTIVE ")";
# else
	return _PACKAGENAME " rev. " VER_PRODUCTVERSION_STR " (Win32, " _BUILD_DIRECTIVE ")";
# endif
#else
	return _PACKAGENAME " rev. " VER_PRODUCTVERSION_STR " (Unix, " _BUILD_DIRECTIVE ")";
#endif
}

char const* GitRevision::GetCompanyNameStr()
{
	return VER_COMPANYNAME_STR;
}

char const* GitRevision::GetLegalCopyrightStr()
{
	return VER_LEGALCOPYRIGHT_STR;
}

char const* GitRevision::GetFileVersionStr()
{
	return VER_FILEVERSION_STR;
}

char const* GitRevision::GetProductVersionStr()
{
	return VER_PRODUCTVERSION_STR;
}