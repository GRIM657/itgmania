#include "stdafx.h"
/*
-----------------------------------------------------------------------------
 File: ScreenSandbox.h

 Desc: Area for testing.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
	Glenn Maynard (OpenGL Code)
	Lance Gilbert (OpenGL/Usability Modifications)
-----------------------------------------------------------------------------
*/

#include "stdafx.h"

#include "ScreenSandbox.h"

#include "RageDisplay.h"
#include <math.h>

#include "SDL.h"
#include "SDL_opengl.h"
#include "ScreenSandbox.h"
#include "ScreenManager.h"
#include "RageSoundManager.h"
#include "GameConstantsAndTypes.h"
#include "PrefsManager.h"
#include "Quad.h"
#include "ThemeManager.h"


ScreenSandbox::ScreenSandbox()
{	
SOUNDMAN->music->StopPlaying();
//	m_quad.StretchTo( RectI(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM) );
//	m_quad.SetDiffuse( RageColor(1,1,1,1) );
//	this->AddChild( &m_quad );

//	m_sprite.Load( "C:\\stepmania\\stepmania\\RandomMovies\\cm301[1].avi" );
//	m_sprite.SetXY( CENTER_X, CENTER_Y );
//	this->AddChild( &m_sprite );

//	obj.SetXY(CENTER_X, CENTER_Y);
//	this->AddChild(&obj);

	this->AddChild(&HEEEEEEEEELP);

	HEEEEEEEEELP.SetXY(450, 400);
	HEEEEEEEEELP.LoadFromFont( THEME->GetPathTo("Fonts","normal") );
	HEEEEEEEEELP.SetZoom(.5);
	HEEEEEEEEELP.SetText(
		"p  Play\n"
		"s  Stop\n"
		"l  Set looping\n"
		"a  Set autostop\n"
		"c  Set continue");

	for(int i = 0; i < nsounds; ++i)
	{
		this->AddChild(&s[i].txt);
		s[i].txt.LoadFromFont( THEME->GetPathTo("Fonts","normal") );
		s[i].txt.SetZoom(.5);
	}

	s[0].txt.SetXY(150, 100);
	s[1].txt.SetXY(450, 100);
	s[2].txt.SetXY(150, 250);
	s[3].txt.SetXY(450, 250);
	s[4].txt.SetXY(150, 400);

	s[0].s.Load("Themes/default/Sounds/_common menu music.ogg");
	s[1].s.Load("Themes/default/Sounds/music scroll music.ogg");
	s[2].s.Load("Themes/default/Sounds/evaluation extra stage.mp3");
	s[3].s.Load("Themes/default/Sounds/gameplay oni die.wav");
	s[4].s.Load("Themes/default/Sounds/gameplay toasty.mp3");

//s[0].s.SetStartSeconds(45);
//s[0].s.SetPositionSeconds();
// s[4].s.SetLengthSeconds(1);
s[0].s.SetPlaybackRate(1.20);
//s[0].s.SetStopMode(RageSound::M_LOOP);
//s[0].s.Play();

	selected = 0;
}


void ScreenSandbox::UpdateText(int n)
{

	CString fn = s[n].s.GetLoadedFilePath();
	unsigned x = fn.find_last_of("/\\");
	if(x != fn.npos) fn.erase(0, x+1);

	vector<RageSound *> snds;
	SOUNDMAN->GetCopies(s[n].s, snds);

	CString pos;
	for(unsigned p = 0; p < snds.size(); ++p)
	{
		if(p) pos += ", ";
		pos += ssprintf("%.3f", snds[p]->GetPositionSeconds());
	}

	s[n].txt.SetText(ssprintf(
		"%i: %s\n"
		"%s\n"
		"%s\n"
		"(%s) of %.3f\n"
		"%s\n"
		"%s",
		n+1, fn.GetString(),
		s[n].s.IsPlaying()? "Playing":"Stopped",
		s[n].s.GetStopMode() == RageSound::M_STOP?
			"Stop when finished":
		s[n].s.GetStopMode() == RageSound::M_CONTINUE?
			"Continue until stopped":
			"Loop",
		pos.size()? pos.GetString(): "none playing",
		s[n].s.GetLengthSeconds(),
		s[n].s.IsStreaming()? "Streaming":"Preloaded",
		selected == n? "^^^^^^":""
		));
}

void ScreenSandbox::Update(float f)
{
	for(int i = 0; i < nsounds; ++i)
		UpdateText(i);
}

void ScreenSandbox::Input( const DeviceInput& DeviceI, const InputEventType type, const GameInput &GameI, const MenuInput &MenuI, const StyleInput &StyleI )
{
	if( type != IET_FIRST_PRESS )
		return;	// ignore

	switch( DeviceI.device)
	{
	case DEVICE_KEYBOARD:
		switch( DeviceI.button )
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5': selected = DeviceI.button - '0'-1; break;
		case 'p':
			s[selected].s.Play();
			break;
		case 's':
			s[selected].s.Stop();
			break;
		case 'l':
			s[selected].s.SetStopMode(RageSound::M_LOOP);
			break;
		case 'a':
			s[selected].s.SetStopMode(RageSound::M_STOP);
			break;
		case 'c':
			s[selected].s.SetStopMode(RageSound::M_CONTINUE);
			break;

/*		case SDLK_LEFT:
			obj.SetX(obj.GetX() - 10);
			break;
		case SDLK_RIGHT:
			obj.SetX(obj.GetX() + 10);
			break;
		case SDLK_UP:
			obj.SetY(obj.GetY() - 10);
			break;
		case SDLK_DOWN:
			obj.SetY(obj.GetY() + 10);
			break;
*/
		case SDLK_t: 
			{
				SDL_Event *event;
				event = (SDL_Event *) malloc(sizeof(event));
				event->type = SDL_QUIT;
				SDL_PushEvent(event);
			}
		case SDLK_ESCAPE: 
			{
			SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
			}
		}

	}

}
void ScreenSandbox::HandleScreenMessage( const ScreenMessage SM )
{
	switch( SM )
	{
	case SM_DoneClosingWipingLeft:
		break;
	case SM_DoneClosingWipingRight:
		break;
	case SM_DoneOpeningWipingLeft:
		break;
	case SM_DoneOpeningWipingRight:
		break;
	}
}
