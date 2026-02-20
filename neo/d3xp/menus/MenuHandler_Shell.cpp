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


extern idCVar g_demoMode;

static const int PEER_UPDATE_INTERVAL = 500;
static const int MAX_MENU_OPTIONS     = 6;

void idMenuHandler_Shell::Update ( ) {
    if ( gui == NULL || !gui->IsActive( ) ) {
        return;
    }

    if ( ( common->Dialog( ).IsDialogActive( ) ) && actionRepeater.isActive ) {
        ClearWidgetActionRepeater( );
    }

    if ( nextState != state ) {
        if ( introGui != NULL && introGui->IsActive( ) ) {
            gui->StopSound( );
            showingIntro = false;
            introGui->Activate( false );
            PlaySound( GUI_SOUND_MUSIC );
        }

        if ( nextState == SHELL_STATE_PRESS_START ) {
            nextScreen = SHELL_AREA_ROOT;
            transition = MENU_TRANSITION_SIMPLE;
            state      = nextState;
        } else if ( nextState == SHELL_STATE_IDLE ) {
            if ( nextScreen == SHELL_AREA_START
                 || nextScreen == SHELL_AREA_PARTY_LOBBY
                 || nextScreen == SHELL_AREA_GAME_LOBBY
                 || nextScreen == SHELL_AREA_INVALID ) {
                nextScreen = SHELL_AREA_ROOT;
            }

            transition = MENU_TRANSITION_SIMPLE;
            state      = nextState;
        } else if ( nextState == SHELL_STATE_PAUSED ) {
            transition = MENU_TRANSITION_SIMPLE;

            if ( gameComplete ) {
                nextScreen = SHELL_AREA_CREDITS;
            } else {
                nextScreen = SHELL_AREA_ROOT;
            }

            state = nextState;
        }
    }

    if ( activeScreen != nextScreen ) {
        ClearWidgetActionRepeater( );

        if ( nextScreen == SHELL_AREA_INVALID ) {
            if ( activeScreen > SHELL_AREA_INVALID && activeScreen < SHELL_NUM_AREAS && menuScreens[ activeScreen ] !=
                 NULL ) {
                menuScreens[ activeScreen ]->HideScreen( static_cast < mainMenuTransition_t >( transition ) );
            }


            idSWFSpriteInstance *bg     = gui->GetRootObject( ).GetNestedSprite( "pause_bg" );
            idSWFSpriteInstance *edging = gui->GetRootObject( ).GetNestedSprite( "_fullscreen" );

            if ( bg != NULL ) {
                bg->PlayFrame( "rollOff" );
            }

            if ( edging != NULL ) {
                edging->PlayFrame( "rollOff" );
            }
        } else {
            if ( activeScreen > SHELL_AREA_INVALID
                 && activeScreen < SHELL_NUM_AREAS && menuScreens[ activeScreen ] != NULL ) {
                menuScreens[ activeScreen ]->HideScreen( static_cast < mainMenuTransition_t >( transition ) );
            }

            if ( nextScreen > SHELL_AREA_INVALID
                 && nextScreen < SHELL_NUM_AREAS
                 && menuScreens[ nextScreen ] != NULL ) {
                menuScreens[ nextScreen ]->UpdateCmds( );
                menuScreens[ nextScreen ]->ShowScreen( static_cast < mainMenuTransition_t >( transition ) );
            }
        }

        transition   = MENU_TRANSITION_INVALID;
        activeScreen = nextScreen;
    }


    idMenuHandler::Update( );

    if ( introGui != NULL && introGui->IsActive( ) ) {
        introGui->Render( renderSystem, Sys_Milliseconds( ) );
    }
}


/*
========================
idMenuHandler_Shell::HandleGuiEvent
========================
*/
bool idMenuHandler_Shell::HandleGuiEvent ( const sysEvent_t *sev ) {
    if ( showingIntro ) {
        return true;
    }

    return idMenuHandler::HandleGuiEvent( sev );
}

/*
========================
idMenuHandler_Shell::Initialize
========================
*/
void idMenuHandler_Shell::Initialize ( const char *swfFile, idSoundWorld *sw ) {
    idMenuHandler::Initialize( swfFile, sw );

    //---------------------
    // Initialize the menus
    //---------------------
#define BIND_SHELL_SCREEN( screenId, className, menuHandler )				\
	menuScreens[ (screenId) ] = new (TAG_SWF) className();	\
	menuScreens[ (screenId) ]->Initialize( menuHandler );	\
	menuScreens[ (screenId) ]->AddRef();

    for ( int i = 0; i < SHELL_NUM_AREAS; ++i ) {
        menuScreens[ i ] = NULL;
    }

    if ( inGame ) {
        BIND_SHELL_SCREEN( SHELL_AREA_ROOT, idMenuScreen_Shell_Pause, this );
    } else {
        BIND_SHELL_SCREEN( SHELL_AREA_ROOT, idMenuScreen_Shell_Root, this );
        typeSoundShader = declManager->FindSound( "gui/teletype/print_text", true );
    }


    // precache sounds
    // don't load gui music for the pause menu to save some memory
    const idSoundShader *soundShader = NULL;
    if ( !inGame ) {
        soundShader = declManager->FindSound( "gui/menu_music", true );
        if ( soundShader != NULL ) {
            sounds[ GUI_SOUND_MUSIC ] = soundShader->GetName( );
        }
    } else {
        idStrStatic < MAX_OSPATH > shortMapName = gameLocal.GetMapFileName( );
        shortMapName.StripFileExtension( );
        shortMapName.StripLeading( "maps/" );
        shortMapName.StripLeading( "game/" );
        if ( ( shortMapName.Icmp( "le_hell_post" ) == 0 ) || ( shortMapName.Icmp( "hellhole" ) == 0 ) || (
                 shortMapName.Icmp( "hell" ) == 0 ) ) {
            soundShader = declManager->FindSound( "hell_music_credits", true );
            if ( soundShader != NULL ) {
                sounds[ GUI_SOUND_MUSIC ] = soundShader->GetName( );
            }
        }
    }

    soundShader = declManager->FindSound( "gui/list_scroll", true );
    if ( soundShader != NULL ) {
        sounds[ GUI_SOUND_SCROLL ] = soundShader->GetName( );
    }
    soundShader = declManager->FindSound( "gui/btn_PDA_advance", true );
    if ( soundShader != NULL ) {
        sounds[ GUI_SOUND_ADVANCE ] = soundShader->GetName( );
    }
    soundShader = declManager->FindSound( "gui/btn_PDA_back", true );
    if ( soundShader != NULL ) {
        sounds[ GUI_SOUND_BACK ] = soundShader->GetName( );
    }
    soundShader = declManager->FindSound( "gui/menu_build_on", true );
    if ( soundShader != NULL ) {
        sounds[ GUI_SOUND_BUILD_ON ] = soundShader->GetName( );
    }
    soundShader = declManager->FindSound( "gui/pda_next_tab", true );
    if ( soundShader != NULL ) {
        sounds[ GUI_SOUND_BUILD_ON ] = soundShader->GetName( );
    }
    soundShader = declManager->FindSound( "gui/btn_set_focus", true );
    if ( soundShader != NULL ) {
        sounds[ GUI_SOUND_FOCUS ] = soundShader->GetName( );
    }
    soundShader = declManager->FindSound( "gui/btn_roll_over", true );
    if ( soundShader != NULL ) {
        sounds[ GUI_SOUND_ROLL_OVER ] = soundShader->GetName( );
    }
    soundShader = declManager->FindSound( "gui/btn_roll_out", true );
    if ( soundShader != NULL ) {
        sounds[ GUI_SOUND_ROLL_OUT ] = soundShader->GetName( );
    }

    class idPauseGUIClose: public idSWFScriptFunction_RefCounted {
    public:
        idSWFScriptVar Call ( idSWFScriptObject *thisObject, const idSWFParmList &parms ) {
            gameLocal.Shell_Show( false );
            return idSWFScriptVar( );
        }
    };

    if ( gui != NULL ) {
        gui->SetGlobal( "closeMenu", new idPauseGUIClose( ) );
    }
}

/*
========================
idMenuHandler_Shell::Cleanup
========================
*/
void idMenuHandler_Shell::Cleanup ( ) {
    idMenuHandler::Cleanup( );

    delete introGui;
    introGui = NULL;
}

/*
========================
idMenuHandler_Shell::ActivateMenu
========================
*/
void idMenuHandler_Shell::ActivateMenu ( bool show ) {
    if ( show && gui != NULL && gui->IsActive( ) ) {
        return;
    } else if ( !show && gui != NULL && !gui->IsActive( ) ) {
        return;
    }


    if ( inGame ) {
        idPlayer *player = gameLocal.GetLocalPlayer( );
        if ( player != NULL ) {
            if ( !show ) {
                bool isDead = false;
                if ( player->health <= 0 ) {
                    isDead = true;
                }

                if ( isDead && !common->IsMultiplayer( ) ) {
                    return;
                }
            }
        }
    }

    idMenuHandler::ActivateMenu( show );
    if ( show ) {
        if ( !inGame ) {
            PlaySound( GUI_SOUND_MUSIC );
        }
    } else {
        ClearWidgetActionRepeater( );
        nextScreen   = SHELL_AREA_INVALID;
        activeScreen = SHELL_AREA_INVALID;
        nextState    = SHELL_STATE_INVALID;
        state        = SHELL_STATE_INVALID;
        common->Dialog( ).ClearDialog( GDM_LEAVE_LOBBY_RET_NEW_PARTY );
    }
}

enum shellCommandsPC_t {
    SHELL_CMD_CAMPAIGN,
    SHELL_CMD_SETTINGS,
    SHELL_CMD_CREDITS,
    SHELL_CMD_QUIT
};


/*
========================
idMenuHandler_Shell::HandleExitGameBtn
========================
*/
void idMenuHandler_Shell::HandleExitGameBtn ( ) {
    class idSWFScriptFunction_QuitDialog: public idSWFScriptFunction_RefCounted {
    public:
        idSWFScriptFunction_QuitDialog ( gameDialogMessages_t _msg, int _accept ) {
            msg    = _msg;
            accept = _accept;
        }
        idSWFScriptVar Call ( idSWFScriptObject *thisObject, const idSWFParmList &parms ) {
            common->Dialog( ).ClearDialog( msg );
            if ( accept == 1 ) {
                common->Quit( );
            } else if ( accept == -1 ) {
                session->MoveToPressStart( );
            }
            return idSWFScriptVar( );
        }

    private:
        gameDialogMessages_t msg;
        int                  accept;
    };

    idStaticList < idSWFScriptFunction *, 4 > callbacks;
    idStaticList < idStrId, 4 >               optionText;
    callbacks.Append( new( TAG_SWF ) idSWFScriptFunction_QuitDialog( GDM_QUIT_GAME, 1 ) );
    callbacks.Append( new( TAG_SWF ) idSWFScriptFunction_QuitDialog( GDM_QUIT_GAME, 0 ) );
    callbacks.Append( new( TAG_SWF ) idSWFScriptFunction_QuitDialog( GDM_QUIT_GAME, -1 ) );
    optionText.Append( idStrId( "#STR_SWF_ACCEPT" ) );
    optionText.Append( idStrId( "#STR_SWF_CANCEL" ) );
    optionText.Append( idStrId( "#str_swf_change_game" ) );

    common->Dialog( ).AddDynamicDialog( GDM_QUIT_GAME, callbacks, optionText, true, "" );
}

/*
========================
idMenuHandler_Shell::HandleAction
========================
*/
bool idMenuHandler_Shell::HandleAction ( idWidgetAction &     action,
                                         const idWidgetEvent &event,
                                         idMenuWidget *       widget,
                                         bool                 forceHandled ) {
    if ( activeScreen == SHELL_AREA_INVALID ) {
        return true;
    }

    widgetAction_t       actionType = action.GetType( );
    const idSWFParmList &parms      = action.GetParms( );

    if ( event.type == WIDGET_EVENT_COMMAND ) {
        /*if ( activeScreen == SHELL_AREA_ROOT && navOptions.Num() > 0 ) {
            return true;
        }*/

        if ( menuScreens[ activeScreen ] != NULL && !forceHandled ) {
            if ( menuScreens[ activeScreen ]->HandleAction( action, event, widget, true ) ) {
                if ( actionType == WIDGET_ACTION_GO_BACK ) {
                    PlaySound( GUI_SOUND_BACK );
                } else {
                    PlaySound( GUI_SOUND_ADVANCE );
                }
                return true;
            }
        }
    }

    switch ( actionType ) {
        case WIDGET_ACTION_COMMAND: {
            if ( parms.Num( ) < 2 ) {
                return true;
            }

            int cmd = parms[ 0 ].ToInteger( );

            if ( ( activeScreen == SHELL_AREA_GAME_LOBBY || activeScreen == SHELL_AREA_MATCH_SETTINGS ) && cmd !=
                 SHELL_CMD_QUIT ) {
                session->Cancel( );
                session->Cancel( );
            } else if ( ( activeScreen == SHELL_AREA_PARTY_LOBBY || activeScreen == SHELL_AREA_LEADERBOARDS ||
                          activeScreen ==
                          SHELL_AREA_BROWSER || activeScreen == SHELL_AREA_MODE_SELECT ) && cmd != SHELL_CMD_QUIT ) {
                session->Cancel( );
            }

            if ( cmd != SHELL_CMD_QUIT && ( nextScreen == SHELL_AREA_STEREOSCOPICS || nextScreen ==
                                            SHELL_AREA_SYSTEM_OPTIONS
                                            || nextScreen == SHELL_AREA_GAME_OPTIONS ||
                                            nextScreen == SHELL_AREA_GAMEPAD || nextScreen ==
                                            SHELL_AREA_MATCH_SETTINGS ) ) {
                cvarSystem->SetModifiedFlags( CVAR_ARCHIVE );
            }

            switch ( cmd ) {
                case SHELL_CMD_CAMPAIGN: {
                    nextScreen = SHELL_AREA_CAMPAIGN;
                    transition = MENU_TRANSITION_SIMPLE;
                    break;
                }
                case SHELL_CMD_SETTINGS: {
                    nextScreen = SHELL_AREA_SETTINGS;
                    transition = MENU_TRANSITION_SIMPLE;
                    break;
                }
                case SHELL_CMD_CREDITS: {
                    nextScreen = SHELL_AREA_CREDITS;
                    transition = MENU_TRANSITION_SIMPLE;
                    break;
                }
                case SHELL_CMD_QUIT: {
                    HandleExitGameBtn( );
                    break;
                }
            }

            return true;
        }
    }

    return idMenuHandler::HandleAction( action, event, widget, forceHandled );
}

/*
========================
idMenuHandler_Shell::GetMenuScreen
========================
*/
idMenuScreen *idMenuHandler_Shell::GetMenuScreen ( int index ) {
    if ( index < 0 || index >= SHELL_NUM_AREAS ) {
        return NULL;
    }

    return menuScreens[ index ];
}


/*
========================
idMenuHandler_Shell::UpdateSavedGames
========================
*/
void idMenuHandler_Shell::UpdateSavedGames ( ) { }


/*
========================
idMenuHandler_Shell::StartGame
========================
*/
void idMenuHandler_Shell::StartGame ( int index ) {
    if ( index == 0 ) {
        cmdSystem->AppendCommandText( va( "map %s %d\n", "game/mars_city1", 0 ) );
    } else if ( index == 1 ) {
        cmdSystem->AppendCommandText( va( "map %s %d\n", "game/erebus1", 1 ) );
    } else if ( index == 2 ) {
        cmdSystem->AppendCommandText( va( "map %s %d\n", "game/le_enpro1", 2 ) );
    }
}
