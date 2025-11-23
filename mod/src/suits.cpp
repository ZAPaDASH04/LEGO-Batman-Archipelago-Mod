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
    for (size_t i = 0; i < 6; i++)
    {
        if (i < 3) {
            _signals[i] = reinterpret_cast<WORD*>(
                *reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6BCF84) + 0x8C + i*0x3D8
            );
        } else {
            _signals[i] = reinterpret_cast<WORD*>(
                *reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6B7BBC) + 0x8C + i*0x3D8
            );
        }
        _signalsPrev[i] = 0;
    }
    

}

void Suits::lock(size_t i)
{
    _unlocked1 |= (WORD)(1 << i);
    _unlocked2 |= (WORD)(1 << i);
}

void Suits::unlock(size_t i)
{
    // untested
    _unlocked1 &= !((WORD)(1 << i));
    _unlocked2 &= !((WORD)(1 << i));
    fixSignals(); // TODO: maybe??
}

void Suits::reset() 
{
    for (size_t i = 0; i < 10; i++)
    {
        if (lb1AP_locations[80+i]) {
            _unlocked1 |= (WORD)(1 << i);
            _unlocked2 |= (WORD)(1 << i);
        } else {
            // untested
            _unlocked1 &= !((WORD)(1 << i));
            _unlocked2 &= !((WORD)(1 << i));
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

void Suits::resetSignals()
{
    for (size_t i = 0; i < 6; i++)
    {
        _signalsPrev[i] = *_signals[i];
    }
    
}

void Suits::fixSignals()
{
    for (size_t i = 0; i < 6; i++)
    {
        switch (_signalsPrev[i]) {
            case 0:
                break;
            // case Bat_Suit: 
            //     if (lb1AP_locations[80 + Bat_Suit]) {
            //         *_signals[i] = Blocked_Suit;
            //     }
            //     break;
            case Heat_Suit: 
                if (lb1AP_locations[80 + Heat_Suit]) {
                    *_signals[i] = Blocked_SuitID;
                }
                break;
            case Glide_Suit: 
                if (lb1AP_locations[80 + Glide_Suit]) {
                    *_signals[i] = Blocked_SuitID;
                }
                break;
            case Demo_Suit: 
                if (lb1AP_locations[80 + Glide_Suit]) {
                    *_signals[i] = Blocked_SuitID;
                }
                break;
            // TODO: the rest
        }
    }
    
}
