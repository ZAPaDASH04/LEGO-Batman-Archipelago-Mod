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

const DWORD32 Characters::_offsets[characterCount] = 
{
    0x00, // Batman
    0x01, // Robin
    0x18, // Bruce Wayne
    0x13, // Alfred
    0x02, // Batgirl
    0x11, // Nightwing
    0x1B, // Commissioner Gordon
    0x40, // Police Officer
    0xCB, // Fishmonger
    0x32, // Military Policeman
    0x30, // Security Guard
    0x42, // Swat
    0x2B, // Scientist
    0x2D, // Sailor
    0x43, // Police Marksman
    0x0F, // Clayface
    0x16, // Mr. Freeze
    0x15, // Poison Ivy
    0x07, // Two-Face
    0x0D, // The Riddler
    0x19, // Bane
    0x04, // Catwoman
    0x05, // Catwoman (Classic)
    0x0E, // Killer Croc
    0x10, // Man-Bat
    0x0C, // The Penguin
    0x0B, // Harley Quinn
    0x14, // The Scarecrow
    0x1A, // Killer Moth
    0x21, // Mad Hatter
    0x09, // The Joker
    0x0A, // The Joker (Tropical)
    0x44, // Poison Ivy Goon
    0x46, // Zoo Sweeper
    0x3B, // Freeze Girl
    0x3D, // Yeti
    0x39, // Riddler Goon
    0x3A, // Riddler Henchman
    0x33, // Penguin Goon
    0x34, // Penguin Henchman
    0x29, // Penguin Minion
    0x36, // Joker Goon
    0x37, // Joker Henchman
    0x38, // Clown Goon
    0x08, // Hush
    0x12, // Rā's al Ghūl
    0x49, // Hero 1
    0x4A, // Hero 2

    // landcraft

    0x81, // batmobile
    0x82, // batcycle
    0x119,// police car
    0x11A,// police bike

    0x11F,// police van
    0x121,// bat-tank
    0x110,// catwoman's motorcycle
    0xC6, // two-face's armoured truck
    
    0xD2, // mr. freeze's kart
    0xCD, // harley quinn's hammer truck
    0x116,// the joker's van
    0x11B,// garbage truck

    // watercraft

    0x84, // Batboat
    0xBD, // Robin's Watercraft
    0x10D,// Robin's Submarine
    0xD1, // Police Watercraft
    0x9E, // Police Boat

    0xCE, // Penguin's Submarine
    0xBA, // Killer Croc's Swamp Rider
    0xC0, // Penguin Goon Submarine
    0x11D,// Mr. Freeze's Iceberg
    0x11E,// Mad Hatter's Steamboat

    // aircraft

    0x83, // Batwing
    0xC4, // Batcopter
    0x101,// Harbour Helicopter
    0x112,// Police Helicopter
    0x10F,// Bruce Wayne's Private Jet

    0xC1, // The Joker's Helicopter
    0xD0, // The Scarecrow's Biplane
    0x118,// Goon Helicopter
    0x11C,// Riddler's jet
    0x122// Mad Hatter's Glider


};




Characters::Characters(DWORD BASE_ADDR) : 
    BASE_ADDR(BASE_ADDR + 0x006CA830)
{
    for (size_t i = 0; i < 48; i++) _characterBytes[i] = *((BYTE**)(BASE_ADDR)) + _offsets[i];
};

BYTE* Characters::operator[](int i) {
    return _characterBytes[i];
};