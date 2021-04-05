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

DOMAIN: Debug
FILE: historylist.h
DESCRIPTION: Declarations for the Debugger's instruction history list.
class THistoryList
---------------------------------------------------------------------------*/


#pragma once
#ifndef HISTORY_LIST_DECLA_H
#define HISTORY_LIST_DECLA_H

#ifdef DEBUG_BUILD

#include "stemdialogs.h"

class THistoryList : public TStemDialog {
private:
  static LRESULT CALLBACK WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
  void ManageWindowClasses(bool);
public:
  THistoryList();
  ~THistoryList();
  void Show(),Hide();
  void RefreshHistoryBox(bool dump=false);
  int Width,Height;
  bool Maximized;
};

#endif//#ifdef DEBUG_BUILD

#endif//#ifndef HISTORY_LIST_DECLA_H

