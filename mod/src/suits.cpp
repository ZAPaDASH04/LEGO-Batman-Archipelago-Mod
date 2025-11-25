#include <iostream>

#include "suits.h"
#include "LB1AP.h" // this is for the location array

Suits::Suits(DWORD BASE_ADDR): 
    BASE_ADDR(BASE_ADDR),
    _unlocked1(*reinterpret_cast<volatile WORD*>(BASE_ADDR + 0x006C9450)),
    _unlocked2(
        *reinterpret_cast<volatile WORD*>(
            reinterpret_cast<uintptr_t>(
                *reinterpret_cast<void**>(BASE_ADDR + 0x005C49CC)
            ) + 0x7BB4
        )
    )
{
    /* signals
    *(LEGOBatman.exe+6BDF84)+8C
    *(LEGOBatman.exe+6BDF84)+8C+3D8
    *(LEGOBatman.exe+6BDF84)+8C+3D8+3D8
    *(LEGOBatman.exe+6B8BBC)+8C
    *(LEGOBatman.exe+6B8BBC)+8C+3D8
    *(LEGOBatman.exe+6B8BBC)+8C+3D8+3D8
    */
   //TODO: I dk
   lb1AP_items[80+0] = true; // batman
   lb1AP_items[80+5] = true; // robin
    for (size_t i = 0; i < 6; i++)
    {
        if (i < 3) {
            _signals[i] = reinterpret_cast<WORD*>(
                *reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6BCF84) + 0x8C + i*0x3D8
            );
        } else {
            _signals[i] = reinterpret_cast<WORD*>(
                *reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6B7BBC) + 0x8C + (i-3)*0x3D8
            );
        }
        _signalsPrev[i] = 0;
    }
    

}

void Suits::lock(size_t i)
{
    // untested
    _unlocked1 &= !((WORD)(1 << i));
    _unlocked2 &= !((WORD)(1 << i));
    //fixSignals(); // TODO: maybe??
}

void Suits::unlock(size_t i)
{
    _unlocked1 |= (WORD)(1 << i);
    _unlocked2 |= (WORD)(1 << i);
}

void Suits::reset() 
{
    for (size_t i = 0; i < 10; i++)
    {
        if (lb1AP_items[80+i]) {
            unlock(i);
        } else {
            // untested
            lock(i);
        }
    }
    
}

void Suits::clearSignals()
{
    for (size_t i = 0; i < 6; i++)
    {
        _signalsPrev[i] = 0x0000;
    }
    
}

// has a sleep in it. sleeps until suits are loaded
bool Suits::resetSignals()
{
    uintptr_t* addr1 = reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6BCF84);
    uintptr_t* addr2 = reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6B8BBC - 0x1000);
    std::cout << std::hex << addr2 << std::endl;
    while (*addr1 == 0 || *addr2 == 0) Sleep(100);
    std::cout << "resetting signals." << std::endl;
    for (size_t i = 0; i < 6; i++)
    {
        
        if (i < 3) {
            _signals[i] = reinterpret_cast<WORD*>(
                *addr1 + 0x8C + i*0x3D8
            );
        } else {
            _signals[i] = reinterpret_cast<WORD*>(
                *addr2 + 0x8C + (i-3)*0x3D8
            );
        }
        _signalsPrev[i] = *_signals[i];
        std::cout << "    " << _signalsPrev[i] << std::endl;
    }
    return true;
}

void Suits::fixSignals()
{
    uintptr_t* addr1 = reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6BCF84);
    uintptr_t* addr2 = reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6B7BBC);
    while (*addr1 == 0 || *addr2 == 0) return; // skip when signals arent loaded.
    for (size_t i = 0; i < 6; i++)
    {
        std::cout << "fix signal " << i << " " << _signalsPrev[i] << std::endl;
        if (_signalsPrev[i] == 0 && _signals[i] != 0 && *_signals[i] != 0) {
            // loaded I guess.
            _signalsPrev[i] = *_signals[i];
        }
        
        switch (_signalsPrev[i]) {
            case 0x0000:
                continue;
                break;
            // case Bat_Suit: 
            //     if (lb1AP_locations[80 + Bat_Suit]) {
            //         *_signals[i] = Blocked_Suit;
            //     }
            //     break;
            case Heat_SuitID: 
                if (lb1AP_items[80 + Heat_Suit]) {
                    std::cout << "heat" << std::endl;
                    *_signals[i] = Heat_SuitID;
                    continue;
                }
                break;
            case Glide_SuitID: 
                if (lb1AP_items[80 + Glide_Suit]) {
                    std::cout << "glide" << std::endl;
                    *_signals[i] = Glide_SuitID;
                    continue;
                }
                break;
            case Demo_SuitID: 
                if (lb1AP_items[80 + Demo_Suit]) {
                    std::cout << "Demo" << std::endl;
                    *_signals[i] = Demo_SuitID;
                    continue;
                }
                break;
            default:
                break;
            // TODO: the rest
        }
        std::cout << "   Blocking suit" << std::endl;
        *_signals[i] = Blocked_SuitID;
    }
    
}
