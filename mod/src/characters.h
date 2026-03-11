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
    Joker_Henchman,         Clown_Goon,         Hush,               Ras_Al_Ghul,            Hero_1,             Hero_2, // PLAN: we could make hero 1 and 2 into the 2 secret characters in the customizer

    // landcraft
    Batmobile,      Batcycle,   Police_Car,             Police_Bike, 
    Police_Van,     Bat_Tank,   Catwomans_Motorcycle,   Two_Faces_Armoured_Truck, 
    Mr_Freezes_Kart, Harley_Quinns_Hammer_Truck, The_Jokers_Van, Garbage_Truck, 
    
    // watercraft
    Batboat,    Robins_Watercraft,  Robins_Submarine,   Police_Watercraft,  Police_Boat, 
    Penguins_Submarine, Killer_Crocs_Swamp_Rider, Penguin_Goon_Submarine, Mr_Freezes_Iceberg, Mad_Hatters_Steamboat, 

    // aircraft
    Batwing, Batcopter, Harbour_Helicopter, Police_Helicopter, Bruce_Waynes_Private_Jet, 
    The_Jokers_Helicopter, The_Scarecrows_Biplane, Goon_Helicopter, Riddlers_jet, Mad_Hatters_Glider,

    // special
    Noone
};


class Characters {
public: // WHY >:(
    static const DWORD characterCount = 48 + 12 + 10 + 10 + 1;
    static const DWORD characterShopCount = 53;
    static const DWORD32 characterOffsets[characterCount];
private:
    const DWORD BASE_ADDR; // may eventually make global.
    //BYTE* _purchaseLocks;
public:
    BYTE* _characterBytes[characterCount];
    bool token[characterCount]; // item // WARN: obsolete
    bool purchased[characterCount]; // location // WARN: obsolete Replace with lb1AP_locations[0+i]
    bool unlocked[characterCount]; // item

    static size_t vanillaShopCharacter(size_t i);


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

    BYTE characterUnlocked(size_t i);
    BYTE characterUnlocked(size_t i, BYTE value);

    // void UnlockCharacter(size_t i);
    // void UnlockToken(size_t i);
    
    void purchaseLocks(size_t i);
    void purchaseLocks(size_t i, bool lock);
};





