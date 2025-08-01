/**
 * @file game.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief 
 * @version 0.1
 * @date 2025-07-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "game.h"

Game::Game(DWORD BASE_ADDR) : 
    BASE_ADDR(BASE_ADDR),
    levels(BASE_ADDR),
    characters(BASE_ADDR),
    currentLevel((BYTE*)(BASE_ADDR + 0x006C98C4)), // TODO: test
    saveSlot((BYTE*)(BASE_ADDR + 0x0056801C)), // TODO: test
    inLevelKitCount((BYTE*)(BASE_ADDR + 0x006C9424)) // TODO: test
{
    for (size_t i = 0; i < 10; i++)
    {
        inLevelKits[i] = ((char*)(BASE_ADDR + 0x006D00E0 + i*0x0C)); // TODO: test. size is uncertain
        inLevelKitLocations[i] = ((BYTE*)(BASE_ADDR + 0x006D00E8 + i*0x0C)); // TODO: test. size is uncertain
        
    }
    
};