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


class Levels
{
private:
    DWORD BASE_ADDR;
    // + 12 = + 0x0C
    //BYTE* levelBeatenH1_1 = *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + -0x4C5; // you can bank on batman beaten
    // + 12 = + 0x0C
    //BYTE* levelUnlockedH1_1 = *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + -0x4C6; 
    
    // uncertain if the special levels work properly
    // first 15 hero. next 1 manor?. next 15 villain. next 1 asylum
    BYTE* _levelUnlocked[32];
    BYTE* _levelBeaten[32];
    BYTE* _levelKitCount[32];
    BYTE* _levelRedBrick[32];
    DWORD32* _hostages;
    
public:
    /**
     * @brief Construct a new Levels object
     * 
     * @param BASE_ADDR includes offsets due to updates.
     */
    Levels(DWORD BASE_ADDR);
    ~Levels();

    
};






