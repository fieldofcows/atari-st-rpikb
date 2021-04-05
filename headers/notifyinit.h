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
FILE: notifyinit.h
DESCRIPTION: The window that appears while Steem is initialising.
---------------------------------------------------------------------------*/

#pragma once
#ifndef NOTIFYINIT_DECLA_H
#define NOTIFYINIT_DECLA_H

#include <easystr.h>

void CreateNotifyInitWin(char* sCaption);
void DestroyNotifyInitWin();
void SetNotifyInitText(char *NewText);

// struct TNotify for RAII use
// usage: TNotify myNotify("hello");
struct TNotify {
  TNotify(char *caption);
  ~TNotify();
  EasyStr m_sPreviousCaption;
  bool m_OurOwn;
};

#ifdef WIN32
extern HWND NotifyWin;
ONEGAME_ONLY( extern HWND NotifyWinParent; )
#endif


#endif//NOTIFYINIT_DECLA_H
