/**
 * @file levels.h
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief Level Progress data object
 * @date 2025-07-31
 * 
 */

#pragma once

#include <windows.h>


enum LevelName {
    H1_1, H1_2, H1_3, H1_4, H1_5,
    H2_1, H2_2, H2_3, H2_4, H2_5,
    H3_1, H3_2, H3_3, H3_4, H3_5,
    V1_1, V1_2, V1_3, V1_4, V1_5,
    V2_1, V2_2, V2_3, V2_4, V2_5,
    V3_1, V3_2, V3_3, V3_4, V3_5,
    // The order of the Above cannot be changed
    Wayne_Manor,
    Arkham_Asylum,
    Shop_Room,
    Trophy_Room,
    Experiment_Room, //TODO: get actual name in dat files
    Mission_Room,
    UnknownLevelName = -1
};

BYTE sublevelToLevel(BYTE);


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

#pragma pack(push, 1) // restricts size

struct SubLevelKits {
    char kits[10][8];
    DWORD32 count; // seems to limit to intended ammount. ie there are more than 10 kits
};

#pragma pack(pop)




class Table {
private:
    const DWORD size;
    DWORD count;
    struct Entry {
        DWORD64 key;
        DWORD value;
    };
    Entry* table;
public:
    Table(DWORD);
    ~Table();
    void insert(Entry);
    DWORD operator[](DWORD64);
    DWORD operator[](BYTE*);
};


class Levels
{
public:
    const DWORD BASE_ADDR; // TODO: can be removed later if only used in constructor.
    BYTE* levelUnlocked[30];
    BYTE* levelBeaten  [30];
    BYTE* levelKitCount[30]; // TODO: test
    BYTE* levelRedBrick[30];
    volatile DWORD32& hostages;
    // usage: levelKitSaveData[levelid]
    SubLevelKits* levelKitSaveData; // WARN: this should not be public in future.

    BYTE levelBeatenPrev[30];
    DWORD32 hostagesOld;
    
    /**
     * @brief Construct a new Levels object
     * 
     * @param BASE_ADDR includes offsets due to updates.
     */
    Levels(DWORD BASE_ADDR);
    ~Levels();

    /**
     * @brief checks if there is a new hostage
     * 
     * @return BYTE which hostage. -1 for no new hostage.
     */
    int checkHostages(); 
    
};






