
//{{BLOCK(game_background)

//======================================================================
//
//	game_background, 240x160@8, 
//	+ palette 256 entries, not compressed
//	+ 451 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 30x20 
//	Total size: 512 + 28864 + 1200 = 30576
//
//	Time-stamp: 2025-11-13, 00:47:44
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAME_BACKGROUND_H
#define GRIT_GAME_BACKGROUND_H

#define game_backgroundTilesLen 28864
extern const unsigned int game_backgroundTiles[7216];

#define game_backgroundMapLen 1200
extern const unsigned short game_backgroundMap[600];

#define game_backgroundPalLen 512
extern const unsigned short game_backgroundPal[256];

#endif // GRIT_GAME_BACKGROUND_H

//}}BLOCK(game_background)
