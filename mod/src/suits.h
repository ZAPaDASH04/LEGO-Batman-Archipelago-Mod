/**
 * @file suits.h
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief 
 * @date 2025-11-23
 * 
 */

#pragma once

#include <windows.h>



enum SuitName {
    Bat_Suit,
    Heat_Suit,
    Glide_Suit,
    Demo_Suit,
    Sonar_Suit,

    Robin_Suit,
    Dive_Suit,
    Tech_Suit,
    Magnet_Suit,
    Attracto_Suit
};

enum SuitID {
    /*
    7608:batman suit
    7658:batman heat suit
    76A8:batman glide suit
    76F8:batman demo suit
    7748:batman sonar suit
    7798:robin suit
    77E8:robin dive suit
    7838:robin tech suit
    7888:robin magnet suit
    78D8:robin attracto suit

    D628: nothing (no wear)

    1000:batman secret invis suit
    5608:batman cowl only
    C6A8:batman cowl and cape (no wear)
    */
    Bat_Suit        = 0x7608,
    Heat_Suit       = 0x7658,
    Glide_Suit      = 0x76A8,
    Demo_Suit       = 0x76F8,
    Sonar_Suit      = 0x7748,

    Robin_Suit      = 0x7798,
    Dive_Suit       = 0x77E8,
    Tech_Suit       = 0x7838,
    Magnat_Suit     = 0x7888,
    Attracto_Suit   = 0x78D8,

    Blocked_Suit    = 0xD628
};

class Suits {
private:
    const DWORD BASE_ADDR; // may eventually make global.
    volatile WORD& _unlocked1; // active value
    volatile WORD& _unlocked2; // loaded value. (when loading sets unlocked1 to unlocked2)
    WORD* _signals[6];
    WORD _signalsPrev[6];
    
public:
    
    
    Suits(DWORD BASE_ADDR);

    void lock(size_t i);
    void unlock(size_t i);
    void reset();
    void fixSignals();

};