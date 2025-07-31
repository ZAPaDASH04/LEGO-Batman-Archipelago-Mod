/**
 * @file characters.h
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
#include <string>

enum CharacterName {
    Batman,                 Robin,              Bruce_Wayne,        Alfred_Pennyworth,      Batgirl,            Nightwing,  
    Commissioner_Gordon,    Police_Officer,     Fishmonger,         Military_Policeman,     Security_Guard,     SWAT,

    Scientist,              Sailor,             Police_Marksman,    Clayface,               Mr_Freeze,          Poison_Ivy,   
    Two_Face,               The_Riddler,        Bane,               Catwoman,               Catwoman_Classic,   Killer_Croc,

    Man_Bat,                The_Penguin,        Harley_Quinn,       The_Scarecrow,          Killer_Moth,        Mad_Hatter,  
    The_Joker,              The_Joker_Tropical, Poison_Ivy_Goon,    Zoo_Sweeper,            Freeze_Girl,        Yeti,

    Riddler_Goon,           Riddler_Henchman,   Penguin_Goon,       Penguin_Henchman,       Penguin_Minion,     Joker_Goon,   
    Joker_Henchman,         Clown_Goon,         Hush,               Ras_Al_Ghul,            Hero_1,             Hero_2

    // TODO: add vehicles.

    // landcraft

    // watercraft

    // aircraft
};


class Characters {
private:
    const DWORD BASE_ADDR; // may eventually make global.
    static const DWORD32 _offsets[48];
    BYTE* _characterBytes[48]; // = {char01, char02, char03, char04, char05, char06, char07, char08, char09, char10, char11, char12, char13, char14, char15, char16, char17, char18, char19, char20, char21, char22, char23, char24, char25, char26, char27, char28, char29, char30, char31, char32, char33, char34, char35, char36, char37, char38, char39, char40, char41, char42, char43, char44, char45, char46, char47, char48};
    static const std::string _names[48];
public:
    /**
     * @brief Construct a new Characters object
     * 
     * @param BASE_ADDR 
     */
    Characters(DWORD BASE_ADDR);

    /**
     * @brief get character pointer by character select index
     * @param i index of character. See Characters.
     * @return BYTE* 
     */
    BYTE* operator[](int i);// { return _characterBytes[i]; };

};






// BYTE* char01 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x00;
// BYTE* char02 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x01;
// BYTE* char03 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x18;
// BYTE* char04 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x13;
// BYTE* char05 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x02;
// BYTE* char06 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x11;
// BYTE* char07 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x1B;
// BYTE* char08 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x40;
// BYTE* char09 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0xCB;
// BYTE* char10 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x32;
// BYTE* char11 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x30;
// BYTE* char12 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x42;
// BYTE* char13 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x2B;
// BYTE* char14 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x2D;
// BYTE* char15 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x43;
// BYTE* char16 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0F;
// BYTE* char17 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x16;
// BYTE* char18 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x15;
// BYTE* char19 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x07;
// BYTE* char20 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0D;
// BYTE* char21 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x19;
// BYTE* char22 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x04;
// BYTE* char23 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x05;
// BYTE* char24 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0E;
// BYTE* char25 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x10;
// BYTE* char26 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0C;
// BYTE* char27 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0B;
// BYTE* char28 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x14;
// BYTE* char29 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x1A;
// BYTE* char30 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x21;
// BYTE* char31 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x09;
// BYTE* char32 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0A;
// BYTE* char33 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x44;
// BYTE* char34 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x46;
// BYTE* char35 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x3B;
// BYTE* char36 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x3D;
// BYTE* char37 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x39;
// BYTE* char38 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x3A;
// BYTE* char39 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x33;
// BYTE* char40 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x34;
// BYTE* char41 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x29;
// BYTE* char42 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x36;
// BYTE* char43 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x37;
// BYTE* char44 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x38;
// BYTE* char45 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x08;
// BYTE* char46 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x12;
// BYTE* char47 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x49;
// BYTE* char48 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x4A;