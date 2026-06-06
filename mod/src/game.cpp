/**
 * @file game.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief contains all pointers, and objects that have pointers, needed for rando
 * @date 2025-07-31
 * 
 */

#include "game.h"
#include <iostream>


Game::Game(DWORD BASE_ADDR) : 
    BASE_ADDR(BASE_ADDR),
    levels(BASE_ADDR),
    characters(BASE_ADDR),
    suits(BASE_ADDR),
    minikits(BASE_ADDR),
    currentLevel(*(volatile BYTE*)(BASE_ADDR + 0x006C98C4)),
    saveSlot(*(volatile BYTE*)(BASE_ADDR + 0x0056801C)), // TODO: test
    inLevelTotalKitCount(*(volatile BYTE*)(BASE_ADDR + 0x006C9424)),
    inLevelKitCount(*(volatile BYTE*)(BASE_ADDR + 0x006C9428)),
    inLevelPowerBrick(*(volatile BYTE*)(BASE_ADDR + 0x006C9420)),
    inLevelTrueStatus(*(volatile BYTE*)(BASE_ADDR + 0x006AF854)),
    powerBrickPurchased(
        *reinterpret_cast<volatile DWORD64*>(
            reinterpret_cast<uintptr_t>(
                *reinterpret_cast<void**>(BASE_ADDR + 0x006D01C8)
            ) + 0x80
        )
        
    ),
    

    inShopSubMenu(*(volatile BYTE*)(BASE_ADDR + 0x005CBC90)),
    inCharacterSelectMenu(*(volatile BYTE*)(BASE_ADDR + 0x006D5D24)),
    player1SelectedCharacter(*(volatile BYTE*)(BASE_ADDR + 0x006C9814)), // these are likely shorts
    player2SelectedCharacter(*(volatile BYTE*)(BASE_ADDR + 0x006C9816)),
    // inFinalStatusScreen(*(volatile BYTE*)(BASE_ADDR + 0x00696BA8)), //Comment out since may be faulty? Doesn't appear to constantly be 1 so mashing may skip past it?
    //powerBrickEnabled(*(volatile DWORD*)(BASE_ADDR + 0x00536DE0)), // TODO: test
    inLevelKitCountPrev(0),
    inLevelPowerBrickPrev(0),
    inLevelTrueStatusPrev(0),
    inShopSubMenuPrev(0)

{
    // in level minikits
    for (size_t i = 0; i < 10; i++)
    {
        inLevelKits[i] = ((char*)(BASE_ADDR + 0x006D00E0 + i*0x0C)); // TODO: test. size is uncertain
        inLevelKitLocations[i] = ((BYTE*)(BASE_ADDR + 0x006D00E8 + i*0x0C)); // TODO: test. size is uncertain

    }

    // extras
    for (size_t i = 0; i < 21; i++) {
        extraEnabled[i] = ((BYTE*)(BASE_ADDR + 0x00536DE0 + i*0x1C));
    }

    // suit upgrades
    for (size_t i = 0; i < 15; i++)
    {
        suitUpgradeEnabled[i] = ((BYTE*)(BASE_ADDR + 0x0053702C + i*0x1C));
    }
    

    extraPurchasedPrev = powerBrickPurchased;

    // red brick stuff
    // for (size_t i = 0; i < 35; i++)
    // {
    //     powerBrickState[i] = 0;
    // }
    
    
};

int Game::checkPowerBricks()
{

    int i = -1; // -1 is no powerbrick.
    // how many right shifts // WARN: does not account for loading a different save or losing hostage progress.
    for (DWORD64 a =  extraPurchasedPrev ^ powerBrickPurchased; a > 0; a = a >> ((DWORD64)1)) i++;
    return i; 
}

bool Game::isInShop(){
    return inShopSubMenu == 1;
}