/*
-----------------------------------------------------------------------------
 File: RageTextureManager.h

 Desc: Interface for loading and releasing textures.

 Copyright (c) 2001 Chris Danford.  All rights reserved.
-----------------------------------------------------------------------------
*/

class RageTextureManager;
typedef RageTextureManager* LPRageTextureManager;




#ifndef _RAGETEXTUREMANAGER_H_
#define _RAGETEXTUREMANAGER_H_


#include "RageTexture.h"
//#include <d3dx8.h>
//#include <d3d8types.h>


//-----------------------------------------------------------------------------
// RageTextureManager Class Declarations
//-----------------------------------------------------------------------------
class RageTextureManager
{
public:
	RageTextureManager( LPRageScreen pScreen );
	~RageTextureManager();

	LPRageTexture LoadTexture( CString sTexturePath, DWORD dwHints = 0, bool bForceReload = false );
	bool IsTextureLoaded( CString sTexturePath );
	void UnloadTexture( CString sTexturePath );

protected:
	LPRageScreen m_pScreen;

	// map from file name to a texture holder
	CTypedPtrMap<CMapStringToPtr, CString, LPRageTexture> m_mapPathToTexture;
};

extern LPRageTextureManager	TM;	// global and accessable from anywhere in our program

#endif
