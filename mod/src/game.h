/**
 * @file game.h
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief contains all pointers, and objects that have pointers, needed for rando
 * @date 2025-07-31
 * 
 */

#pragma once

#include <windows.h>
#include "levels.h"
#include "characters.h"
#include "minikits.h"

enum ExtraName {
    Adaptive_Difficulty,

    Silhouettes,
    Beep_Beep,
    Ice_Rink,
    Disguise,
    Extra_Toggle,

    Score_x2,
    Score_x4,
    Score_x6,
    Score_x8,
    Score_x10,

    Stud_Magnet,
    Character_Studs,
    Minikit_Detector,
    Power_Brick_Detector,
    Always_Score_Multiply,

    Fast_Build,
    Immune_To_Freeze,
    Regenerate_Hearts,
    Extra_Hearts,
    Invincibility
};


enum SuitName {
    Batsuit =      0,
    Demosuit =     3,
    Glidesuit =    2,
    Sonarsuit =    4,
    Heatsuit =     1,
    Robinsuit =    5,
    Techsuit =     7,
    Magnetsuit =   8,
    Attractosuit = 9,
    Divesuit =     6
};

class Game
{
private:
    const DWORD BASE_ADDR;

public:
    Levels levels;
    Characters characters;
    Minikits minikits;
    
    // TODO: test volatile functionality
    volatile BYTE& currentLevel; // TODO: test
    
    // some of these may be 4 bytes in size but only ever use 1 byte.
    volatile BYTE& saveSlot;
    volatile BYTE& inLevelTotalKitCount;
    volatile BYTE& inLevelKitCount;
    const char* inLevelKits[10]; 
    const BYTE* inLevelKitLocations[10];
    volatile BYTE& inLevelPowerBrick; // TODO: test
    volatile BYTE& inLevelTrueStatus;
    volatile DWORD64& powerBrickPurchased;
    BYTE* extraEnabled[21];
    BYTE* suitUpgradeEnabled[15];
    volatile WORD& suitUnlocked1; // active value
    volatile WORD& suitUnlocked2; // loaded value. (when loading sets unlocked1 to unlocked2)
    volatile BYTE& inShopSubMenu;
    volatile BYTE& inFinalStatusScreen;

    // Tracking
    BYTE inLevelKitCountPrev;
    BYTE inLevelPowerBrickPrev;
    BYTE inLevelTrueStatusPrev;
    DWORD64 extraPurchasedPrev;
    BYTE inShopSubMenuPrev;
    /**
     * @brief 4 flags
     * bit 1 = Collected Location
     * bit 2 = Collected Item
     * bit 3 = Purchased Location
     * bit 4 = Purchased Item
     */
    BYTE powerBrickState[35];



    Game(DWORD BASE_ADDR);

    int checkPowerBricks();
    bool isInShop();

};
