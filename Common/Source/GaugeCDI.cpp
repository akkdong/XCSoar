/*
Copyright_License {

  XCSoar Glide Computer - http://xcsoar.sourceforge.net/
  Copyright (C) 2000 - 2008

  	M Roberts (original release)
	Robin Birch <robinb@ruffnready.co.uk>
	Samuel Gisiger <samuel.gisiger@triadis.ch>
	Jeff Goodenough <jeff@enborne.f2s.com>
	Alastair Harrison <aharrison@magic.force9.co.uk>
	Scott Penrose <scottp@dd.com.au>
	John Wharington <jwharington@gmail.com>
	Lars H <lars_hn@hotmail.com>
	Rob Dunning <rob@raspberryridgesheepfarm.com>
	Russell King <rmk@arm.linux.org.uk>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

}

*/

#include "StdAfx.h"
#include "XCSoar.h"
#include "GaugeCDI.h"
#include "Math/FastMath.h"

HWND hWndCDIWindow = NULL; //CDI Window
extern HFONT CDIWindowFont;
extern HWND hWndMainWindow; // Main Windows
extern HINSTANCE hInst;      // The current instance

#include "InfoBoxLayout.h"

#include <tchar.h>

void GaugeCDI::Create() {
  // start of new code for displaying CDI window
  StartupStore(TEXT("Create CDI\n"));

  hWndCDIWindow = CreateWindow(TEXT("STATIC"),TEXT(" "),
			       WS_VISIBLE|WS_CHILD | WS_CLIPCHILDREN
			       | WS_CLIPSIBLINGS,
			       0,0,0,0,
			       hWndMainWindow,NULL,hInst,NULL);

  SendMessage(hWndCDIWindow,WM_SETFONT,
	      (WPARAM)CDIWindowFont,MAKELPARAM(TRUE,0));

  SetWindowPos(hWndCDIWindow,HWND_TOP,
	       (int)(InfoBoxLayout::ControlWidth*0.6),
	       (int)(InfoBoxLayout::ControlHeight+1),
	       (int)(InfoBoxLayout::ControlWidth*2.8),
	       (int)(InfoBoxLayout::TitleHeight*1.4),
	       SWP_SHOWWINDOW);

  // end of new code for drawing CDI window (see below for destruction of objects)

  ShowWindow(hWndCDIWindow, SW_HIDE);
}



void GaugeCDI::Destroy() {
  DestroyWindow(hWndCDIWindow);
}

void GaugeCDI::Update(double TrackBearing, double WaypointBearing)
{
  // JMW changed layout here to fit reorganised display
  // insert waypoint bearing ".<|>." into CDIScale string"

  TCHAR CDIScale[] = TEXT("330..340..350..000..010..020..030..040..050..060..070..080..090..100..110..120..130..140..150..160..170..180..190..200..210..220..230..240..250..260..270..280..290..300..310..320..330..340..350..000..010..020..030..040.");
  TCHAR CDIDisplay[25] = TEXT("");
  int j;
  int CDI_WP_Bearing = (int)WaypointBearing/2;
  CDIScale[CDI_WP_Bearing + 9] = 46;
  CDIScale[CDI_WP_Bearing + 10] = 60;
  CDIScale[CDI_WP_Bearing + 11] = 124; // "|" character
  CDIScale[CDI_WP_Bearing + 12] = 62;
  CDIScale[CDI_WP_Bearing + 13] = 46;
  for (j=0;j<24;j++) CDIDisplay[j] = CDIScale[(j + (int)(TrackBearing)/2)];
  CDIDisplay[24] = _T('\0');
  // JMW fix bug! This indicator doesn't always display correctly!

  // JMW added arrows at end of CDI to point to track if way off..
  int deltacdi = iround(WaypointBearing - TrackBearing);

  while (deltacdi>180) {
    deltacdi-= 360;
  }
  while (deltacdi<-180) {
    deltacdi+= 360;
  }
  if (deltacdi>20) {
    CDIDisplay[21]='>';
    CDIDisplay[22]='>';
    CDIDisplay[23]='>';
  }
  if (deltacdi<-20) {
    CDIDisplay[0]='<';
    CDIDisplay[1]='<';
    CDIDisplay[2]='<';
  }

  SetWindowText(hWndCDIWindow,CDIDisplay);
  // end of new code to display CDI scale
}
