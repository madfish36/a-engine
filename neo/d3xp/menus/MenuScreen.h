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
#ifndef __MENUSCREEN_H__
#define __MENUSCREEN_H__

#include "../../renderer/tr_local.h"

enum mainMenuTransition_t {
	MENU_TRANSITION_INVALID = -1,
	MENU_TRANSITION_SIMPLE,
	MENU_TRANSITION_ADVANCE,
	MENU_TRANSITION_BACK,
	MENU_TRANSITION_FORCE
};

enum cursorState_t {
	CURSOR_NONE,
	CURSOR_IN_COMBAT,
	CURSOR_TALK,
	CURSOR_GRABBER,
	CURSOR_ITEM,
};

/*
================================================
idLBRowBlock
================================================
*/
class idLBRowBlock {
public:
	idLBRowBlock() : lastTime( 0 ), startIndex( 0 ) {}

	int										lastTime;
	int										startIndex;
	idList< idLeaderboardCallback::row_t >	rows;
};

enum leaderboardFilterMode_t {
	LEADERBOARD_FILTER_OVERALL	= 0,
	LEADERBOARD_FILTER_MYSCORE	= 1,
	LEADERBOARD_FILTER_FRIENDS	= 2
};

/*
================================================
idLBCache
================================================
*/
class idLBCache {
public:
	static const int NUM_ROW_BLOCKS		= 5;
	static const leaderboardFilterMode_t DEFAULT_LEADERBOARD_FILTER = LEADERBOARD_FILTER_OVERALL;

	idLBCache() :
		def( NULL ),
		filter( DEFAULT_LEADERBOARD_FILTER ),
		pendingDef( NULL ),
		pendingFilter( DEFAULT_LEADERBOARD_FILTER ),
		requestingRows( false ),
		loadingNewLeaderboard( false ),
		numRowsInLeaderboard( 0 ),
		entryIndex( 0 ),
		rowOffset( 0 ),
		localIndex( -1 ),
		errorCode( LEADERBOARD_DISPLAY_ERROR_NONE ) {}

	void									Pump();
	void									Reset();
	void									SetLeaderboard( const leaderboardDefinition_t *	def_, leaderboardFilterMode_t filter_ = DEFAULT_LEADERBOARD_FILTER );
	void									CycleFilter();
	leaderboardFilterMode_t					GetFilter() const { return filter; }
	idStr									GetFilterStrType();
	bool									Scroll( int amount );
	bool									ScrollOffset( int amount );
	idLBRowBlock *							FindFreeRowBlock();
	void									Update( const idLeaderboardCallback * callback );
	const idLeaderboardCallback::row_t *	GetLeaderboardRow( int row );

	const leaderboardDefinition_t *			GetLeaderboard() const { return def; }
	int										GetNumRowsInLeaderboard() const { return numRowsInLeaderboard; }

	int										GetEntryIndex() const { return entryIndex; }
	int										GetRowOffset() const { return rowOffset; }
	int										GetLocalIndex() const { return localIndex; }
	leaderboardDisplayError_t				GetErrorCode() const { return errorCode; }

	bool									IsRequestingRows() const { return requestingRows; }
	bool									IsLoadingNewLeaderboard() const { return loadingNewLeaderboard; }

	void									SetEntryIndex( int value ) { entryIndex = value; }
	void									SetRowOffset( int value ) { rowOffset = value; }

	void									DisplayGamerCardUI( const idLeaderboardCallback::row_t * row );

private:
	leaderboardDisplayError_t				CallbackErrorToDisplayError( leaderboardError_t errorCode );

	idLBRowBlock					rowBlocks[NUM_ROW_BLOCKS];

	const leaderboardDefinition_t *	def;
	leaderboardFilterMode_t			filter;

	// Pending def and filter are simply used to queue up SetLeaderboard calls when the system is currently
	// busy waiting on results from a previous SetLeaderboard/GetLeaderboardRow call.
	// This is so we only have ONE request in-flight at any given time.
	const leaderboardDefinition_t *	pendingDef;
	leaderboardFilterMode_t			pendingFilter;

	bool							requestingRows;				// True while requested rows are in flight
	bool							loadingNewLeaderboard;		// True when changing to a new leaderboard (or filter type)

	int								numRowsInLeaderboard;		// Total rows in this leaderboard (they won't all be cached though)
	int								entryIndex;					// Relative row offset (from top of viewing window)
	int								rowOffset;					// Absolute row offset
	int								localIndex;					// Row for the master local user (-1 means not on leaderboard)

	leaderboardDisplayError_t		errorCode;					// Error state of the leaderboard
};

/*
================================================
idMenuScreen
================================================
*/
class idMenuScreen : public idMenuWidget {
public:

	idMenuScreen();
	virtual ~idMenuScreen();

	virtual void				Update();
	virtual void				UpdateCmds();
	virtual void				HandleMenu( const mainMenuTransition_t type );

	virtual void				ShowScreen( const mainMenuTransition_t transitionType );
	virtual void				HideScreen( const mainMenuTransition_t transitionType );

	virtual void				ObserveEvent( const idMenuWidget & widget, const idWidgetEvent & event );
	virtual void				SetScreenGui( idSWF * gui ) { menuGUI = gui; }

protected:

	idSWF *	menuGUI;
	mainMenuTransition_t	transition;

};

/*
================================================
idMenuScreen_PDA_UserData
================================================
*/
class idMenuScreen_PDA_UserData : public idMenuScreen {
public:

	idMenuScreen_PDA_UserData() {}

	virtual ~idMenuScreen_PDA_UserData() {}
	virtual void					Initialize( idMenuHandler * data );
	virtual void					Update();
	virtual void					ShowScreen( const mainMenuTransition_t transitionType );
	virtual void					HideScreen( const mainMenuTransition_t transitionType );
	virtual bool					HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled = false );
	idMenuWidget_PDA_UserData *		GetUserData() { return &pdaUserData; }
	idMenuWidget_PDA_Objective *	GetObjective() { return &pdaObjectiveSimple; }
	idMenuWidget_PDA_AudioFiles *	GetAudioFiles() { return &pdaAudioFiles; }

private:
	idMenuWidget_PDA_UserData 			pdaUserData;
	idMenuWidget_PDA_Objective 		pdaObjectiveSimple;
	idMenuWidget_PDA_AudioFiles 		pdaAudioFiles;
};

/*
================================================
idMenuScreen_PDA_UserEmails
================================================
*/
class idMenuScreen_PDA_UserEmails : public idMenuScreen {
public:
	idMenuScreen_PDA_UserEmails() :
		readingEmails( false ),
		scrollEmailInfo( false ) {
	}

	virtual ~idMenuScreen_PDA_UserEmails() {
	}

	virtual void					Update();
	virtual void					Initialize( idMenuHandler * data );
	virtual void					ShowScreen( const mainMenuTransition_t transitionType );
	virtual void					HideScreen( const mainMenuTransition_t transitionType );
	virtual bool					HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled = false );
	virtual void					ObserveEvent( const idMenuWidget & widget, const idWidgetEvent & event );
	idMenuWidget_PDA_EmailInbox &	GetInbox() { return pdaInbox; }

	bool							ScrollCorrectList( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget );
	void							ShowEmail( bool show );
	void							UpdateEmail();
private:
	idMenuWidget_PDA_EmailInbox 	pdaInbox;
	idMenuWidget_InfoBox 			emailInfo;
	idMenuWidget_ScrollBar 			emailScrollbar;
	bool							readingEmails;
	bool							scrollEmailInfo;
};

/*
================================================
idMenuScreen_PDA_UserData
================================================
*/
class idMenuScreen_PDA_VideoDisks : public idMenuScreen {
public:
	idMenuScreen_PDA_VideoDisks() :
		activeVideo( NULL ) {
	}

		virtual ~idMenuScreen_PDA_VideoDisks() {
		}

	virtual void				Initialize( idMenuHandler * data );
	virtual void				Update();
	virtual void				ShowScreen( const mainMenuTransition_t transitionType );
	virtual void				HideScreen( const mainMenuTransition_t transitionType );
	virtual bool				HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled = false );

	void						ToggleVideoDiskPlay();
	void						UpdateVideoDetails();
	void						SelectedVideoToPlay( int index );
	void						ClearActiveVideo() { activeVideo = NULL; }
	const idDeclVideo *			GetActiveVideo() { return activeVideo; }
private:
	idMenuWidget_ScrollBar		scrollbar;
	idMenuWidget_DynamicList 		pdaVideoList;
	idMenuWidget_PDA_VideoInfo 	videoDetails;
	idList< idList< idStr, TAG_IDLIB_LIST_MENU>, TAG_IDLIB_LIST_MENU >		videoItems;
	const idDeclVideo *				activeVideo;
};

/*
================================================
idMenuScreen_PDA_UserData
================================================
*/
class idMenuScreen_PDA_Inventory : public idMenuScreen {
public:
	idMenuScreen_PDA_Inventory() {
	}
	virtual void				Initialize( idMenuHandler * data );
	virtual void				Update();
	virtual void				ShowScreen( const mainMenuTransition_t transitionType );
	virtual void				HideScreen( const mainMenuTransition_t transitionType );
	virtual bool				HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled = false );

	void						EquipWeapon();
	const char *				GetWeaponName( int index );
	bool						IsVisibleWeapon( int index );

private:
	idMenuWidget_Carousel 			itemList;
	idMenuWidget_InfoBox 			infoBox;
};

//*
//================================================
//idMenuScreen_Shell_Root
//================================================
//*/
class idMenuScreen_Shell_Root : public idMenuScreen {
public:
	idMenuScreen_Shell_Root():exitBtn( NULL ){
	}
	virtual void				Initialize( idMenuHandler * data );
	virtual void				Update();
	virtual void				ShowScreen( const mainMenuTransition_t transitionType );
	virtual void				HideScreen( const mainMenuTransition_t transitionType );
	virtual bool				HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled = false );

	void						HandleExitGameBtn();
private:
	idMenuWidget_Button *		exitBtn;

};

//*
//================================================
//idMenuScreen_Shell_Pause
//================================================
//*/
class idMenuScreen_Shell_Pause : public idMenuScreen {
public:
	idMenuScreen_Shell_Pause() :
		options( NULL ),
		isMpPause( false ) {
	}
	virtual void				Initialize( idMenuHandler * data );
	virtual void				Update();
	virtual void				ShowScreen( const mainMenuTransition_t transitionType );
	virtual void				HideScreen( const mainMenuTransition_t transitionType );
	virtual bool				HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled = false );

	void						HandleExitGameBtn();
	void						HandleRestartBtn();

private:
	idMenuWidget_DynamicList *	options;
	bool						isMpPause;
};


//*
//================================================
//idMenuScreen_Shell_PressStart
//================================================
//*/
class idMenuScreen_Shell_GameSelect : public idMenuScreen {
public:
	idMenuScreen_Shell_GameSelect() :
		startButton( NULL ),
		options( NULL ),
		itemList( NULL ),
		doomCover( NULL ),
		doom2Cover( NULL ),
		doom3Cover( NULL ) {
	}
	virtual void				Initialize( idMenuHandler * data );
	virtual void				Update();
	virtual void				ShowScreen( const mainMenuTransition_t transitionType );
	virtual void				HideScreen( const mainMenuTransition_t transitionType );
	virtual bool				HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled = false );
private:
	idMenuWidget_Button *		startButton;
	idMenuWidget_DynamicList *	options;
	idMenuWidget_Carousel *		itemList;
	const idMaterial *			doomCover;
	const idMaterial *			doom2Cover;
	const idMaterial *			doom3Cover;
};


struct creditInfo_t {

	creditInfo_t() {
		type = -1;
		entry = "";
	}

	creditInfo_t( int t, const char * val ) {
		type = t;
		entry = val;
	}

	int type;
	idStr entry;
};

//*
//================================================
//idMenuScreen_HUD
//================================================
//*/
class idMenuScreen_HUD : public idMenuScreen {
public:

	idMenuScreen_HUD() :
		weaponInfo( NULL ),
		playerInfo( NULL ),
		audioLog( NULL ),
		communication( NULL ),
		oxygen( NULL ),
		stamina( NULL ),
		weaponName( NULL ),
		weaponImg( NULL ),
		weaponPills( NULL ),
		locationName( NULL ),
		downloadPda( NULL ),
		downloadVideo( NULL ),
		newPDA( NULL ),
		newVideo( NULL ),
		objective( NULL ),
		objectiveComplete( NULL ),
		tipInfo( NULL ),
		healthBorder( NULL ),
		healthPulse( NULL ),
		armorFrame( NULL ),
		security( NULL ),
		securityText( NULL ),
		newPDADownload( NULL ),
		newPDAHeading( NULL ),
		newPDAName( NULL ),
		newVideoDownload( NULL ),
		newVideoHeading( NULL ),
		audioLogPrevTime( 0 ),
		commPrevTime( 0 ),
		oxygenComm( false ),
		inVaccuum( false ),
		ammoInfo( NULL ),
		newWeapon( NULL ),
		pickupInfo( NULL ),
		cursorState( CURSOR_NONE ),
		cursorInCombat( 0 ),
		cursorTalking( 0 ),
		cursorItem( 0 ),
		cursorGrabber( 0 ),
		cursorNone( 0 ),
		talkCursor( NULL ),
		combatCursor( NULL ),
		grabberCursor( NULL ),
		bsInfo( NULL ),
		soulcubeInfo( NULL ),
		mpInfo( NULL ),
		mpHitInfo( NULL ),
		mpTime( NULL ),
		mpMessage( NULL ),
		mpChat( NULL ),
		mpWeapons( NULL ),
		newItem( NULL ),
		respawnMessage( NULL ),
		flashlight( NULL ),
		mpChatObject( NULL ),
		showSoulCubeInfoOnLoad( false ),
		mpConnection( NULL ) {
	}

	virtual void			Initialize( idMenuHandler * data );
	virtual void			Update();
	virtual void			ShowScreen( const mainMenuTransition_t transitionType );
	virtual void			HideScreen( const mainMenuTransition_t transitionType );

	void					UpdateHealthArmor( idPlayer * player );
	void					UpdateStamina( idPlayer * player );
	void					UpdateLocation( idPlayer * player );
	void					UpdateWeaponInfo( idPlayer * player );
	void					UpdateWeaponStates( idPlayer * player, bool weaponChanged );
	void					ShowTip( const char * title, const char * tip );
	void					HideTip();
	void					DownloadVideo();
	void					DownloadPDA( const idDeclPDA * pda, bool newSecurity );
	void					UpdatedSecurity();
	void					ToggleNewVideo( bool show );
	void					ClearNewPDAInfo();
	void					ToggleNewPDA( bool show );
	void					UpdateAudioLog( bool show );
	void					UpdateCommunication( bool show, idPlayer * player );
	void					UpdateOxygen( bool show, int val = 0 );
	void					SetupObjective( const idStr & title, const idStr & desc, const idMaterial * screenshot );
	void					SetupObjectiveComplete( const idStr & title );
	void					ShowObjective( bool complete );
	void					HideObjective( bool complete );
	void					GiveWeapon( idPlayer * player, int weaponIndex );
	void					UpdatePickupInfo( int index, const idStr & name );
	bool					IsPickupListReady();
	void					ShowPickups();
	void					SetCursorState( idPlayer * player, cursorState_t state, int set );
	void					SetCursorText( const idStr & action, const idStr & focus );
	void					UpdateCursorState();
	void					CombatCursorFlash();
	void					UpdateSoulCube( bool ready );
	void					ShowRespawnMessage( bool show );
	void					SetShowSoulCubeOnLoad( bool show ) { showSoulCubeInfoOnLoad = show; }

	// MULTIPLAYER

	void					ToggleMPInfo( bool show, bool showTeams, bool isCTF = false );
	void					SetFlagState( int team, int state );
	void					SetTeamScore( int team, int score );
	void					SetTeam( int team );
	void					TriggerHitTarget( bool show, const idStr & target, int color = 0 );
	void					ToggleLagged( bool show );
	void					UpdateGameTime( const char * time );
	void					UpdateMessage( bool show, const idStr & message );
	void					ShowNewItem( const char * name, const char * icon );
	void					UpdateFlashlight( idPlayer * player );
	void					UpdateChattingHud( idPlayer * player );

private:

	idSWFScriptObject *		weaponInfo;
	idSWFScriptObject *		playerInfo;
	idSWFScriptObject *		stamina;
	idSWFScriptObject *		weaponName;
	idSWFScriptObject *		weaponPills;
	idSWFScriptObject *		downloadPda;
	idSWFScriptObject *		downloadVideo;
	idSWFScriptObject *		tipInfo;
	idSWFScriptObject *		mpChat;
	idSWFScriptObject *		mpWeapons;

	idSWFSpriteInstance *	healthBorder;
	idSWFSpriteInstance *	healthPulse;
	idSWFSpriteInstance *	armorFrame;
	idSWFSpriteInstance *	security;
	idSWFSpriteInstance *	newPDADownload;
	idSWFSpriteInstance *	newVideoDownload;
	idSWFSpriteInstance *	newPDA;
	idSWFSpriteInstance *	newVideo;
	idSWFSpriteInstance *	audioLog;
	idSWFSpriteInstance *	communication;
	idSWFSpriteInstance *	oxygen;
	idSWFSpriteInstance *	objective;
	idSWFSpriteInstance *	objectiveComplete;
	idSWFSpriteInstance *	ammoInfo;
	idSWFSpriteInstance *	weaponImg;
	idSWFSpriteInstance *	newWeapon;
	idSWFSpriteInstance *	pickupInfo;
	idSWFSpriteInstance *	talkCursor;
	idSWFSpriteInstance *	combatCursor;
	idSWFSpriteInstance *	grabberCursor;
	idSWFSpriteInstance *	bsInfo;
	idSWFSpriteInstance *	soulcubeInfo;
	idSWFSpriteInstance *	newItem;
	idSWFSpriteInstance	*	respawnMessage;
	idSWFSpriteInstance *	flashlight;
	idSWFSpriteInstance *	mpChatObject;
	idSWFSpriteInstance *	mpConnection;

	idSWFSpriteInstance *	mpInfo;
	idSWFSpriteInstance *	mpHitInfo;

	idSWFTextInstance *		locationName;
	idSWFTextInstance *		securityText;
	idSWFTextInstance *		newPDAName;
	idSWFTextInstance *		newPDAHeading;
	idSWFTextInstance *		newVideoHeading;

	idSWFTextInstance *		mpMessage;
	idSWFTextInstance *		mpTime;

	int						audioLogPrevTime;
	int						commPrevTime;

	bool					oxygenComm;
	bool					inVaccuum;

	idStr					objTitle;
	idStr					objDesc;
	const idMaterial *		objScreenshot;
	idStr					objCompleteTitle;

	cursorState_t			cursorState;
	int						cursorInCombat;
	int						cursorTalking;
	int						cursorItem;
	int						cursorGrabber;
	int						cursorNone;
	idStr					cursorAction;
	idStr					cursorFocus;

	bool					showSoulCubeInfoOnLoad;
};

//*
//================================================
//idMenuScreen_Scoreboard
//================================================
//*/
class idMenuScreen_Scoreboard : public idMenuScreen {
public:

	idMenuScreen_Scoreboard() :
		playerList( NULL ) {

	}

	virtual void				Initialize( idMenuHandler * data );
	virtual void				Update();
	virtual void				ShowScreen( const mainMenuTransition_t transitionType );
	virtual bool				HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled = false );

	virtual void				SetPlayerData( idList< scoreboardInfo_t, TAG_IDLIB_LIST_MENU > data );
	virtual void				UpdateTeamScores( int r, int b );
	virtual void				UpdateGameInfo( idStr gameInfo );
	virtual void				UpdateSpectating( idStr spectating, idStr follow );
	virtual void				UpdateHighlight();

protected:
	idMenuWidget_ScoreboardList *		playerList;

};

//*
//================================================
//idMenuScreen_Scoreboard_CTF
//================================================
//*/
class idMenuScreen_Scoreboard_CTF : public idMenuScreen_Scoreboard {
public:
	virtual void				Initialize( idMenuHandler * data );
};

//*
//================================================
//idMenuScreen_Scoreboard_Team
//================================================
//*/
class idMenuScreen_Scoreboard_Team : public idMenuScreen_Scoreboard {
public:
	virtual void				Initialize( idMenuHandler * data );
};


/*
========================
InvitePartyOrFriends

Invites the master local user's party, if he's in one and the party isn't in the lobby already.
Otherwise brings up the invite friends system menu.
========================
*/
inline void InvitePartyOrFriends() {
	const idLocalUser * const user = session->GetSignInManager().GetMasterLocalUser();
	if ( user != NULL && user->IsInParty() && user->GetPartyCount() > 1 && !session->IsPlatformPartyInLobby() ) {
		session->InviteParty();
	} else {
		session->InviteFriends();
	}
}

#endif
