// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaiext-android/MOAIApp.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIApp::_setListener ( lua_State* L ) {
	USLuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		MOAIApp::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//================================================================//
// MOAIApp
//================================================================//

//----------------------------------------------------------------//
void MOAIApp::DidStartSession( ) {

	USLuaRef& callback = this->mListeners [ SESSION_START ];
	
	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
MOAIApp::MOAIApp () {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIApp::~MOAIApp () {
}

//----------------------------------------------------------------//
void MOAIApp::OnInit () {
}

//----------------------------------------------------------------//
void MOAIApp::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "SESSION_START",	    ( u32 )SESSION_START );
	state.SetField ( -1, "SESSION_END",		    ( u32 )SESSION_END );
	
	luaL_Reg regTable[] = {
		{ "setListener",						_setListener },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIApp::Reset () {
	for ( int i = 0 ; i < TOTAL; i++ ) {
		mListeners [ i ].Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIApp::WillEndSession( ) {

	USLuaRef& callback = this->mListeners [ SESSION_END ];
	
	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}
