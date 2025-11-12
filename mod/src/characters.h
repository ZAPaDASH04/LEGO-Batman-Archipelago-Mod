/**
 * @file characters.h
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief Characters object and stuff for Characters.
 * @date 2025-07-31
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
    Joker_Henchman,         Clown_Goon,         Hush,               Ras_Al_Ghul,            Hero_1,             Hero_2,

    // landcraft
    Batmobile,      Batcycle,   Police_Car,             Police_Bike, 
    Police_Van,     Bat_Tank,   Catwomans_Motorcycle,   Two_Faces_Armoured_Truck, 
    Mr_Freezes_Kart, Harley_Quinns_Hammer_Truck, The_Jokers_Van, Garbage_Truck, 
    
    // watercraft
    Batboat,    Robins_Watercraft,  Robins_Submarine,   Police_Watercraft,  Police_Boat, 
    Penguins_Submarine, Killer_Crocs_Swamp_Rider, Penguin_Goon_Submarine, Mr_Freezes_Iceberg, Mad_Hatters_Steamboat, 

    // aircraft
    Batwing, Batcopter, Harbour_Helicopter, Police_Helicopter, Bruce_Waynes_Private_Jet, 
    The_Jokers_Helicopter, The_Scarecrows_Biplane, Goon_Helicopter, Riddlers_jet, Mad_Hatters_Glider

};


class Characters {
public: // WHY >:(
    static const DWORD characterCount = 48 + 12 + 10 + 10;
private:
    const DWORD BASE_ADDR; // may eventually make global.
    static const DWORD32 _offsets[characterCount];
public:
    BYTE* _characterBytes[characterCount];
    bool token[characterCount]; // item
    bool purchased[characterCount]; // location
    bool unlocked[characterCount]; // item
    /**
     * @brief Construct a new Characters object
     * 
     * @param BASE_ADDR includes offsets due to updates.
     */
    Characters(DWORD BASE_ADDR);

    /**
     * @brief get character pointer by character select index
     * @param i index of character. See Characters.
     * @return BYTE* 
     */
    // TODO: see if this can return volatile reference.
    BYTE* operator[](int i);

};





