#include "stdafx.h"
/*
-----------------------------------------------------------------------------
 Class: ScreenGraphicOptions

 Desc: See header.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "ScreenGraphicOptions.h"
#include "RageTextureManager.h"
#include "RageUtil.h"
#include "RageSoundManager.h"
#include "ScreenManager.h"
#include "PrefsManager.h"
#include "GameConstantsAndTypes.h"
#include "StepMania.h"
#include "RageLog.h"
#include "ThemeManager.h"
#include "RageDisplay.h"


enum {
	GO_WINDOWED = 0,
	GO_DISPLAY_RESOLUTION,
	GO_DISPLAY_COLOR_DEPTH,
	GO_TEXTURE_COLOR_DEPTH,
	GO_REFRESH_RATE,
	GO_BGMODE,
	GO_BGBRIGHTNESS,
	GO_MOVIEDECODEMS,
	GO_BGIFNOBANNER,
	GO_VSYNC,
	NUM_GRAPHIC_OPTIONS_LINES
};
OptionRowData g_GraphicOptionsLines[NUM_GRAPHIC_OPTIONS_LINES] = {
	{ "Display\nMode",			2,  {"FULLSCREEN", "WINDOWED"} },
	{ "Display\nResolution",	7,  {"320","400","512","640","800","1024","1280"} },
	{ "Display\nColor",			2,  {"16BIT","32BIT"} },
	{ "Texture\nColor",			2,  {"16BIT","32BIT"} },
	{ "Refresh\nRate",			11, {"DEFAULT","60","70","72","75","80","85","90","100","120","150"} },
	{ "Background\nMode",		4,  {"OFF","ANIMATIONS","VISUALIZATIONS","RANDOM MOVIES"} },
	{ "Background\nBrightness",	11, {"0%","10%","20%","30%","40%","50%","60%","70%","80%","90%","100%"} },
	{ "Movie\nDecode",			4,  {"1ms","2ms","3ms","4ms"} },
	{ "BG For\nBanner",			2,  {"NO", "YES (slow)"} },
	{ "Wait For\nVsync",		2,  {"NO", "YES"} },
};

static const int HorizRes[] = {
	320, 400, 512, 640, 800, 1024, 1280
};
static const int VertRes[] = {
	240, 300, 384, 480, 600, 768, 1024
};

ScreenGraphicOptions::ScreenGraphicOptions() :
	ScreenOptions(
		THEME->GetPathTo("BGAnimations","graphic options"),
		THEME->GetPathTo("Graphics","graphic options page"),
		THEME->GetPathTo("Graphics","graphic options top edge")
		)
{
	LOG->Trace( "ScreenGraphicOptions::ScreenGraphicOptions()" );

	// fill g_InputOptionsLines with explanation text
	for( int i=0; i<NUM_GRAPHIC_OPTIONS_LINES; i++ )
	{
		CString sLineName = g_GraphicOptionsLines[i].szTitle;
		sLineName.Replace("\n","");
		sLineName.Replace(" ","");
		strcpy( g_GraphicOptionsLines[i].szExplanation, THEME->GetMetric("ScreenGraphicOptions",sLineName) );
	}

	Init(
		INPUTMODE_BOTH, 
		g_GraphicOptionsLines, 
		NUM_GRAPHIC_OPTIONS_LINES,
		false );
	UpdateRefreshRates();
	m_Menu.StopTimer();

	SOUNDMAN->PlayMusic( THEME->GetPathTo("Sounds","graphic options music") );
}


void ScreenGraphicOptions::UpdateRefreshRates()
{
	/* If we're windowed, leave all refresh rates dimmed, but don't
	 * change the actual selection. */
	if(m_iSelectedOption[0][GO_WINDOWED])
		return;

//	PositionUnderlines();
}

void ScreenGraphicOptions::OnChange()
{
	ScreenOptions::OnChange();
	UpdateRefreshRates();
}

void ScreenGraphicOptions::ImportOptions()
{
	m_iSelectedOption[0][GO_WINDOWED]				= PREFSMAN->m_bWindowed ? 1:0;

	switch( PREFSMAN->m_iDisplayWidth )
	{
	case 320:	m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 0;	break;
	case 400:	m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 1;	break;
	case 512:	m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 2;	break;
	case 640:	m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 3;	break;
	case 800:	m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 4;	break;
	case 1024:	m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 5;	break;
	case 1280:	m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 6;	break;
	default:	m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 3;	break;
	}
		
	switch( PREFSMAN->m_iDisplayColorDepth )
	{
	case 16:	m_iSelectedOption[0][GO_DISPLAY_COLOR_DEPTH] = 0;	break;
	case 32:	m_iSelectedOption[0][GO_DISPLAY_COLOR_DEPTH] = 1;	break;
	}

	switch( PREFSMAN->m_iTextureColorDepth )
	{
	case 16:	m_iSelectedOption[0][GO_TEXTURE_COLOR_DEPTH] = 0;	break;
	case 32:	m_iSelectedOption[0][GO_TEXTURE_COLOR_DEPTH] = 1;	break;
	}

	switch(PREFSMAN->m_iRefreshRate)
	{
	case REFRESH_DEFAULT:	m_iSelectedOption[0][GO_REFRESH_RATE]		= 0; break;
	default:
		for(unsigned i = 2; i < g_GraphicOptionsLines[GO_REFRESH_RATE].iNumOptions; ++i) {
			if(atoi(g_GraphicOptionsLines[GO_REFRESH_RATE].szOptionsText[i]) <= PREFSMAN->m_iRefreshRate) 
				m_iSelectedOption[0][GO_REFRESH_RATE] = i;
		}
	}

	m_iSelectedOption[0][GO_BGMODE]					= PREFSMAN->m_BackgroundMode;
	m_iSelectedOption[0][GO_BGBRIGHTNESS]			= (int)( PREFSMAN->m_fBGBrightness*10+0.5f ); 
	m_iSelectedOption[0][GO_MOVIEDECODEMS]			= PREFSMAN->m_iMovieDecodeMS-1;
	m_iSelectedOption[0][GO_BGIFNOBANNER]			= PREFSMAN->m_bUseBGIfNoBanner ? 1:0;
	m_iSelectedOption[0][GO_VSYNC]					= PREFSMAN->m_bVsync ? 1:0;	
}

void ScreenGraphicOptions::ExportOptions()
{
	PREFSMAN->m_bWindowed				= m_iSelectedOption[0][GO_WINDOWED] == 1;

	if(m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] > 6)
	{
		ASSERT(0);
		m_iSelectedOption[0][GO_DISPLAY_RESOLUTION] = 3;
	}
	PREFSMAN->m_iDisplayWidth = HorizRes[m_iSelectedOption[0][GO_DISPLAY_RESOLUTION]];
	PREFSMAN->m_iDisplayHeight = VertRes[m_iSelectedOption[0][GO_DISPLAY_RESOLUTION]];
	
	switch( m_iSelectedOption[0][GO_DISPLAY_COLOR_DEPTH] )
	{
	case 0:	PREFSMAN->m_iDisplayColorDepth = 16;	break;
	case 1:	PREFSMAN->m_iDisplayColorDepth = 32;	break;
	default:	ASSERT(0);	PREFSMAN->m_iDisplayColorDepth = 16;	break;
	}
	
	switch( m_iSelectedOption[0][GO_TEXTURE_COLOR_DEPTH] )
	{
	case 0:	PREFSMAN->m_iTextureColorDepth = 16;	break;
	case 1:	PREFSMAN->m_iTextureColorDepth = 32;	break;
	default:	ASSERT(0);	PREFSMAN->m_iTextureColorDepth = 16;	break;
	}
	
	if(m_iSelectedOption[0][GO_REFRESH_RATE] == 0)
		PREFSMAN->m_iRefreshRate = REFRESH_DEFAULT;
	else {
		int n = m_iSelectedOption[0][GO_REFRESH_RATE];
		PREFSMAN->m_iRefreshRate = atoi(g_GraphicOptionsLines[GO_REFRESH_RATE].szOptionsText[n]);
	}

	PREFSMAN->m_BackgroundMode			= PrefsManager::BackgroundMode( m_iSelectedOption[0][GO_BGMODE] );
	PREFSMAN->m_fBGBrightness			= m_iSelectedOption[0][GO_BGBRIGHTNESS] / 10.0f;
	PREFSMAN->m_iMovieDecodeMS			= m_iSelectedOption[0][GO_MOVIEDECODEMS]+1;
	PREFSMAN->m_bUseBGIfNoBanner		= m_iSelectedOption[0][GO_BGIFNOBANNER] == 1;
	PREFSMAN->m_bVsync					= m_iSelectedOption[0][GO_VSYNC] == 1;
}

void ScreenGraphicOptions::GoToPrevState()
{
	SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
	PREFSMAN->SaveGlobalPrefsToDisk();
	ApplyGraphicOptions();
}

void ScreenGraphicOptions::GoToNextState()
{
	GoToPrevState();
}

