/**
 * @file characters.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief 
 * @version 0.1
 * @date 2025-07-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "characters.h"
#include <windows.h>

const DWORD32 Characters::_offsets[48] = 
{
    0x00, // TODO: add names
    0x01,
    0x18,
    0x13,
    0x02,
    0x11,
    0x1B,
    0x40,
    0xCB,
    0x32,
    0x30,
    0x42,
    0x2B,
    0x2D,
    0x43,
    0x0F,
    0x16,
    0x15,
    0x07,
    0x0D,
    0x19,
    0x04,
    0x05,
    0x0E,
    0x10,
    0x0C,
    0x0B,
    0x14,
    0x1A,
    0x21,
    0x09,
    0x0A,
    0x44,
    0x46,
    0x3B,
    0x3D,
    0x39,
    0x3A,
    0x33,
    0x34,
    0x29,
    0x36,
    0x37,
    0x38,
    0x08,
    0x12,
    0x49,
    0x4A
};


// may be obsolete
const std::string Characters::_names[48] = 
{
    "batman",
    "robin"
};


Characters::Characters(DWORD BASE_ADDR) : 
    BASE_ADDR(BASE_ADDR + 0x006CA830)
{
    for (size_t i = 0; i < 48; i++) _characterBytes[i] = *((BYTE**)(BASE_ADDR)) + _offsets[i];
};

BYTE* Characters::operator[](int i) {
    return _characterBytes[i];
};