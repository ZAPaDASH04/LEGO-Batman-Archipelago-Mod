/**
 * @file levels.h
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


enum LevelName {
    H1_1, H1_2, H1_3, H1_4, H1_5,
    H2_1, H2_2, H2_3, H2_4, H2_5,
    H3_1, H3_2, H3_3, H3_4, H3_5,
    Wayne_Manor,
    V1_1, V1_2, V1_3, V1_4, V1_5,
    V2_1, V2_2, V2_3, V2_4, V2_5,
    V3_1, V3_2, V3_3, V3_4, V3_5,
    Arkham_Asylum
};



#pragma pack(push, 1) // restricts size

/**
 * // sublevel kit save data structure
 * 8 char[8] kit0
 * 8 char[8] kit1
 * 8 char[8] kit2
 * 8 char[8] kit3
 * 8 char[8] kit4
 * 8 char[8] kit5
 * 8 char[8] kit6
 * 8 char[8] kit7
 * 8 char[8] kit8
 * 8 char[8] kit9 - 80 dec
 * 4 DWORD32 kit count - 84 dec
 */

struct SubLevelKits {
    char kits[10][8];
    DWORD32 count; // seems to limit to intended ammount. ie there are more than 10 kits
};

// struct LevelKits
// {
//     SubLevelKits lev[6];
// };
//typedef SubLevelKits LevelKits[6];

#pragma pack(pop)



// class Level
// {
// private:
//     /* data */
// public:
//     Level(/* args */);
// };


class Levels
{
private:
    // + 12 = + 0x0C
    //BYTE* levelBeatenH1_1 = *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + -0x4C5; // you can bank on batman beaten
    // + 12 = + 0x0C
    //BYTE* levelUnlockedH1_1 = *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + -0x4C6; 
    
    // uncertain if the special levels work properly
    // first 15 hero. next 1 manor?. next 15 villain. next 1 asylum
    // BYTE* levelUnlocked[32];
    // BYTE* levelBeaten[32];
    // BYTE* levelKitCount[32];
    // BYTE* levelRedBrick[32];
    // DWORD32* hostages;
    
public:
    const DWORD BASE_ADDR;
    // TODO: maybe const
    BYTE* levelUnlocked[32];
    BYTE* levelBeaten[32];
    BYTE* levelKitCount[32];
    BYTE* levelRedBrick[32];
    //DWORD32* hostages; // messed up initializer
    // usage: levelKitSaveData[levelid]
    SubLevelKits* levelKitSaveData; // WARN: this should not be public.

    /**
     * @brief Construct a new Levels object
     * 
     * @param BASE_ADDR includes offsets due to updates.
     */
    Levels(DWORD BASE_ADDR);
    ~Levels();

    
};






