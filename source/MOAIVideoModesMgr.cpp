// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "MOAIVideoModesMgr.h"
#include <moai-sim/MOAISim.h>

std::vector<MOAIVideoModesMgr::Mode> MOAIVideoModesMgr::s_videoModes;
u32 MOAIVideoModesMgr::s_currentMode = -1;
MOAIVideoModesMgr::SetWindowModeFuncCallback MOAIVideoModesMgr::s_setWindowModeFunc = NULL;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	singletonHello
	@text	Prints the string 'MOAIVideoModesMgr singleton foo!' to the console.

	@out	nil
*/
int MOAIVideoModesMgr::_GetNumDisplayModes ( lua_State* L ) {

	MOAILuaState state ( L );
	lua_pushnumber ( L, s_videoModes.size() );

	return 1;
}

int MOAIVideoModesMgr::_GetDisplayMode ( lua_State* L ) {

	MOAILuaState state ( L );
	u32 mode = state.GetValue < u32 >( 1, 1 ) - 1;

	if(mode >= s_videoModes.size())
		return 0;

	lua_pushnumber ( L, s_videoModes[mode].width );
	lua_pushnumber ( L, s_videoModes[mode].height );
	lua_pushnumber ( L, s_videoModes[mode].rate );

	return 3;
}

int MOAIVideoModesMgr::_SetDisplayMode( lua_State* L) {

	MOAILuaState state ( L );
	u32 mode = state.GetValue <u32> ( 1, 1 ) - 1;

	if((s_setWindowModeFunc != NULL) &&
	   (mode < s_videoModes.size())  &&
	   (s_currentMode != mode))
	{
		s_setWindowModeFunc(s_videoModes[mode]);

		MOAISim::Get().SetStep(1.0 / s_videoModes[mode].rate);

		s_currentMode = mode;
	}

	return 0;
}

//================================================================//
// MOAIVideoModesMgr
//================================================================//

//----------------------------------------------------------------//
MOAIVideoModesMgr::MOAIVideoModesMgr () {

	// register all classes MOAIVideoModesMgr derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( MOAIFooMgrBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVideoModesMgr::~MOAIVideoModesMgr () {
}

//----------------------------------------------------------------//
void MOAIVideoModesMgr::RegisterLuaClass ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaClass ( state );

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "GetNumDisplayModes", _GetNumDisplayModes },
		{ "GetDisplayMode", _GetDisplayMode },
		{ "SetDisplayMode", _SetDisplayMode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

void MOAIVideoModesMgr::ClearDisplayModes()
{
	s_videoModes.clear();
}

void MOAIVideoModesMgr::AddDisplayMode(int width, int height, int rate)
{
	Mode m = {width, height, rate};
	s_videoModes.push_back(m);
}

void MOAIVideoModesMgr::SetWindowModeFunc(SetWindowModeFuncCallback func)
{
	s_setWindowModeFunc = func;
}
