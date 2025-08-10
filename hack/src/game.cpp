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
    minikits(BASE_ADDR),
    currentLevel(*(volatile BYTE*)(BASE_ADDR + 0x006C98C4)),
    saveSlot(*(volatile BYTE*)(BASE_ADDR + 0x0056801C)), // TODO: test
    inLevelTotalKitCount(*(volatile BYTE*)(BASE_ADDR + 0x006C9424)),
    inLevelKitCount(*(volatile BYTE*)(BASE_ADDR + 0x006C9428)),
    extraPurchased(
        *reinterpret_cast<volatile DWORD*>(
            reinterpret_cast<uintptr_t>(
                *reinterpret_cast<void**>(BASE_ADDR + 0x006D01C8)
            ) + 0x80
        )
    ),
    suitUnlocked1(*reinterpret_cast<volatile WORD*>(BASE_ADDR + 0x006C9450)),
    //suitUnlocked3(*reinterpret_cast<volatile WORD*>(BASE_ADDR + 0x005C9294)),
    suitUnlocked2(
        *reinterpret_cast<volatile WORD*>(
            reinterpret_cast<uintptr_t>(
                *reinterpret_cast<void**>(BASE_ADDR + 0x005C49CC)
            ) + 0x7BB4
        )
    ),
    //powerBrickEnabled(*(volatile DWORD*)(BASE_ADDR + 0x00536DE0)), // TODO: test
    inLevelKitCountPrev(0)
{
    //currentLevel = ((volatile BYTE*)(BASE_ADDR + 0x006C98C4));
    for (size_t i = 0; i < 10; i++)
    {
        inLevelKits[i] = ((char*)(BASE_ADDR + 0x006D00E0 + i*0x0C)); // TODO: test. size is uncertain
        inLevelKitLocations[i] = ((BYTE*)(BASE_ADDR + 0x006D00E8 + i*0x0C)); // TODO: test. size is uncertain
        

    }

    for (size_t i = 0; i < 21; i++) {
        extraEnabled[i] = ((BYTE*)(BASE_ADDR + 0x00536DE0 + i*0x1C));
    }
    
};