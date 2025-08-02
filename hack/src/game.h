/**
 * @file game.h
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief 
 * @version 0.1
 * @date 2025-07-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <windows.h>
#include "levels.h"
#include "characters.h"

class Game
{
private:
    const DWORD BASE_ADDR;

public:
    Levels levels;
    Characters characters;
    
    // TODO: test volatile functionality
    volatile BYTE& currentLevel; // TODO: test
    
    // some of these may be 4 bytes in size but only ever use 1 byte.
    const BYTE* saveSlot; // TODO: test
    const BYTE* inLevelTotalKitCount; // this is saved kit count + inLevelKitCount
    const BYTE* inLevelKitCount; // TODO: test
    const char* inLevelKits[10]; // TODO: test. size is uncertain
    const BYTE* inLevelKitLocations[10]; // TODO: test. size is uncertain

    Game(DWORD BASE_ADDR);

    

};
