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
//#include <iostream>

BYTE sublevelToLevel(BYTE id) {

    switch (id)
    {
    case 0x09: return UnknownLevelName; // maybe efficient?
    case 0x12: return H1_1;
    case 0x13: return H1_1;
    case 0x1E: return H1_2;
    case 0x1F: return H1_2;
    case 0x2B: return H1_3;
    case 0x2C: return H1_3;
    case 0x2D: return H1_3;
    case 0x30: return H1_4;
    case 0x31: return H1_4;
    case 0x32: return H1_4;
    case 0x33: return H1_4;
    case 0x48: return H1_5;
    case 0x4A: return H1_5;
    case 0x4B: return H1_5;

    case 0x5B: return H2_1;
    case 0x5C: return H2_1;
    case 0x74: return H2_2;
    case 0x75: return H2_2;
    case 0x77: return H2_2;
    case 0x66: return H2_3; // diff order
    case 0x67: return H2_3;
    case 0x68: return H2_3;
    case 0x87: return H2_4;
    case 0x88: return H2_4;
    case 0x89: return H2_4;
    case 0x8E: return H2_5;
    case 0x8F: return H2_5;

    case 0xA0: return H3_1;
    case 0xA1: return H3_1;
    case 0xA2: return H3_1;
    case 0xA6: return H3_2;
    case 0xA7: return H3_2;
    case 0xA8: return H3_2;
    case 0xAC: return H3_3;
    case 0xAD: return H3_3;
    case 0xAE: return H3_3;
    case 0xB8: return H3_4;
    case 0xB9: return H3_4;
    case 0xBA: return H3_4;
    case 0xBB: return H3_4;
    case 0xC6: return H3_5;
    case 0xC7: return H3_5;
    case 0xC8: return H3_5;
    // V1
    case 0x18: return V1_1;
    case 0x19: return V1_1;
    case 0x1A: return V1_1;
    case 0x24: return V1_2;
    case 0x25: return V1_2;
    case 0x26: return V1_2;
    case 0x27: return V1_2;
    case 0x37: return V1_3;
    case 0x38: return V1_3;
    case 0x39: return V1_3;
    case 0x3A: return V1_3;
    case 0x3F: return V1_4;
    case 0x40: return V1_4;
    case 0x41: return V1_4;
    case 0x42: return V1_4;
    case 0x50: return V1_5;
    case 0x51: return V1_5;
    case 0x52: return V1_5;
    case 0x53: return V1_5;
    // V2
    case 0x81: return V2_1;
    case 0x82: return V2_1;
    case 0x83: return V2_1;
    case 0x61: return V2_2; // dif order
    case 0x62: return V2_2;
    case 0x7B: return V2_3;
    case 0x7C: return V2_3;
    case 0x7D: return V2_3;
    case 0x6D: return V2_4; // dif order
    case 0x6E: return V2_4; 
    case 0x6F: return V2_4; 
    case 0x70: return V2_4; 
    case 0x96: return V2_5; 
    case 0x97: return V2_5; 
    case 0x98: return V2_5; 
    // V3
    case 0xB2: return V3_1;
    case 0xB3: return V3_1; 
    case 0xB4: return V3_1;
    case 0xD2: return V3_2;
    case 0xD3: return V3_2;
    case 0xD4: return V3_2;
    case 0xD8: return V3_3;
    case 0xD9: return V3_3;
    case 0xDA: return V3_3;
    case 0xC0: return V3_4;
    case 0xC1: return V3_4;
    case 0xC2: return V3_4;
    case 0xCC: return V3_5;
    case 0xCD: return V3_5;
    case 0xCE: return V3_5;
    
    default: return UnknownLevelName;
    }
}


Table::Table(DWORD size):
    size(size),
    count(0)
{
    table = new Entry[size];
}

Table::~Table()
{
    free(table);
}

void Table::insert(Entry entry)
{
    table[count] = entry;
    count++;
    // if (count == 0) {
    //     table[0] = entry;
    //     count++;
    // }
    // Entry head = table[count/2];
    // if (head.key > entry.key) {

    // } else {

    // }
}

DWORD Table::operator[](DWORD64 key)
{
    DWORD low = 0;
    DWORD high = count;
    DWORD mid;

    while (low < high) {
        mid = low + (high - low) / 2;
        const Entry& entry = table[mid];

        if (key < entry.key) {
            high = mid;
        } else if (key > entry.key) {
            low = mid + 1;
        } else {
            return entry.value;
        }
    }

    return 0; // not found
}

DWORD Table::operator[](BYTE* name)
{
    DWORD64 num = 0;
    memcpy(&num, name, strnlen((char*)name, 8));
    return this->operator[](num);
}


Levels::Levels(DWORD BASE_ADDR) :
    BASE_ADDR(BASE_ADDR)
{
    for (size_t i = 0; i < 32+3; i++) {
        // 
        levelUnlocked[i] = *((BYTE**)(BASE_ADDR + 0x006CA830)) + (-0x4C6 + i*0x0C);
        levelBeaten[i] =   *((BYTE**)(BASE_ADDR + 0x006CA830)) + (-0x4C5 + i*0x0C);
        //
        levelKitCount[i] = *((BYTE**)(BASE_ADDR + 0x006CA830)) + (-0x4C3 + i*0x0C);
        //
        levelRedBrick[i] = *((BYTE**)(BASE_ADDR + 0x006CA830)) + (-0x4C1 + i*0x0C);
    }
    hostages = *((DWORD32**)(BASE_ADDR + 0x006AF8B0)) + 0x240; // 4 bytes. at least 32bits
    //levelKitSaveData = reinterpret_cast<LevelKits*>(BASE_ADDR - 0x1000 + 0x006CA8FC); // point to beginning of save data then interperet as object
    // this->a = 0x006C98FC;
    // this->b = BASE_ADDR + this->a;
    levelKitSaveData = *reinterpret_cast<SubLevelKits**>(BASE_ADDR + 0x006C98FC);
    //std::cout << std::hex;
    //std::cout << "[DEBUG] levelKitSaveData = " << reinterpret_cast<DWORD>(levelKitSaveData) << "\n";
    
}
Levels::~Levels() = default; // TODO: why did I have to add this???

