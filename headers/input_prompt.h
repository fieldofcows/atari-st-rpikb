/*---------------------------------------------------------------------------
PROJECT: Steem SSE
Atari ST emulator
Copyright (C) 2021 by Anthony Hayward and Russel Hayward + SSE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see https://www.gnu.org/licenses/.

DOMAIN: GUI
FILE: input_prompt.h
DESCRIPTION: Declaratio for a generic input prompt.

TODO: move to mymisc

---------------------------------------------------------------------------*/
#pragma once
#ifndef INPUTPROMPT_H
#define INPUTPROMPT_H

#ifdef WIN32
extern bool InputPrompt_Choose(HWND,char*,EasyStr &);
#endif

#endif//#ifndef INPUTPROMPT_H
