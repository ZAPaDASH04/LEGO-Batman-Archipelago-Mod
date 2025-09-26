/**
 * @file minikits.h
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief Minikits object and stuff for Minikits.
 * @date 2025-08-01
 * 
 */

#pragma once

#include <windows.h>
#include <unordered_map>
#include <string>

struct Key {
    BYTE level;
    std::string name;

    bool operator==(const Key& other) const {
        return level == other.level && name == other.name;
    }
};

struct KeyHash {
    std::size_t operator()(const Key& k) const {
        std::hash<std::string> str_hash;
        return std::hash<BYTE>()(k.level) ^ (str_hash(k.name) << 1);
    }
};


struct Minikit {
    BYTE sublevel;
    char id[8];
};

class Minikits
{
private:
    const DWORD BASE_ADDR;
    static const DWORD tableSize = 50;

    std::unordered_map<Key, DWORD, KeyHash> kitTable;

public:
    Minikits(DWORD BASE_ADDR);
    DWORD findKitIndex(BYTE level, const char* name);

};


/**
 * minikits are saved in this way
 * 
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
 * 
 * 6 total sublevel spaces per level
 * 
 * 
 * all levels are list in order? and the level id is the offset
 * level 1 is 0x12
 * IT IS.
 * 
 */

























