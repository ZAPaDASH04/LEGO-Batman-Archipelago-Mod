/**
 * @file levels.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief 
 * @version 0.1
 * @date 2025-07-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "levels.h"

Levels::Levels(DWORD BASE_ADDR) :
    BASE_ADDR(BASE_ADDR)
{
    for (size_t i = 0; i < 32; i++) {
        // 
        levelUnlocked[i] = *((BYTE**)(BASE_ADDR + 0x006CA830)) + (-0x4C6 + i*0x0C);
        levelBeaten[i] =   *((BYTE**)(BASE_ADDR + 0x006CA830)) + (-0x4C5 + i*0x0C);
        //
        levelKitCount[i] = *((BYTE**)(BASE_ADDR + 0x006CA830)) + (-0x4C3 + i*0x0C);
        //
        levelRedBrick[i] = *((BYTE**)(BASE_ADDR + 0x006CA830)) + (-0x4C1 + i*0x0C);
    }
    hostages = *((DWORD32**)(BASE_ADDR + 0x006AF8B0)) + 0x240; // 4 bytes. at least 32bits
    

}
Levels::~Levels() = default; // TODO: why did I have to add this???