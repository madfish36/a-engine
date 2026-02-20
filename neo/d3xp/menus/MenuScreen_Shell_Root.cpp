/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#pragma hdrstop
#include "../../idLib/precompiled.h"
#include "../Game_local.h"

extern idCVar    g_demoMode;
const static int NUM_MAIN_OPTIONS = 6;
/*
========================
idMenuScreen_Shell_Root::Initialize
========================
*/
void idMenuScreen_Shell_Root::Initialize ( idMenuHandler *data ) {
  idMenuScreen::Initialize( data );

  if ( data != NULL ) {
    menuGUI = data->GetGUI( );
  }

  SetSpritePath( "MainWindow" );


  exitBtn = new idMenuWidget_Button();
  exitBtn->SetSpritePath( GetSpritePath(  ), "DialogExit","btnOk" );
  exitBtn->Initialize( data );
  exitBtn->SetLabel( "exitBtn" );
  exitBtn->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_EXIT_GAME );
  AddChild( exitBtn );
}

/*
========================
idMenuScreen_Shell_Root::Update
========================
*/
void idMenuScreen_Shell_Root::Update ( ) {

  idMenuScreen::Update( );
  idSWFScriptObject & root = GetSWFObject()->GetRootObject();
  if (exitBtn != NULL) {
    exitBtn->BindSprite( root );

  }
}

enum rootMenuCmds_t {
  ROOT_CMD_START_DEMO,
  ROOT_CMD_START_DEMO2,
  ROOT_CMD_SETTINGS,
  ROOT_CMD_QUIT,
  ROOT_CMD_DEV,
  ROOT_CMD_CAMPAIGN,
  ROOT_CMD_MULTIPLAYER,
  ROOT_CMD_PLAYSTATION,
  ROOT_CMD_CREDITS
};

/*
========================
idMenuScreen_Shell_Root::ShowScreen
========================
*/
void idMenuScreen_Shell_Root::ShowScreen ( const mainMenuTransition_t transitionType ) {

  idMenuScreen::ShowScreen( transitionType );

}

/*
========================
idMenuScreen_Shell_Root::HideScreen
========================
*/
void idMenuScreen_Shell_Root::HideScreen ( const mainMenuTransition_t transitionType ) {
  idMenuScreen::HideScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_Root::HandleExitGameBtn
========================
*/
void idMenuScreen_Shell_Root::HandleExitGameBtn ( ) {
  common->Quit(  );
}

/*
========================
idMenuScreen_Shell_Root::HandleAction
========================
*/
bool idMenuScreen_Shell_Root::HandleAction ( idWidgetAction &     action,
                                             const idWidgetEvent &event,
                                             idMenuWidget *       widget,
                                             bool                 forceHandled ) {
  if ( menuData == NULL ) {
    return true;
  }

  if ( menuData->ActiveScreen( ) != SHELL_AREA_ROOT ) {
    return false;
  }
  if (action.GetType(  )==WIDGET_ACTION_EXIT_GAME) {
    HandleExitGameBtn();
  }

  return idMenuWidget::HandleAction( action, event, widget, forceHandled );
}
