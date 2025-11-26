#include <iostream>

#include "suits.h"
#include "LB1AP.h" // this is for the location array

SuitName SuitIDtoSuitName(WORD id) {
    return SuitIDtoSuitName(static_cast<SuitID>(id));
}

SuitName SuitIDtoSuitName(SuitID id)
{
    switch (id)
    {
    case Bat_SuitID:        return Bat_Suit;
    case Heat_SuitID:       return Heat_Suit;
    case Glide_SuitID:      return Glide_Suit;
    case Demo_SuitID:       return Demo_Suit;
    case Sonar_SuitID:      return Sonar_Suit;
        
    case Robin_SuitID:      return Robin_Suit;
    case Dive_SuitID:       return Dive_Suit;
    case Tech_SuitID:       return Tech_Suit;
    case Magnet_SuitID:     return Magnet_Suit;
    case Attracto_SuitID:   return Attracto_Suit;
        
    default:                return Blocked_Suit;
    }
}

SuitID SuitNametoSuitID(size_t name) {
    return SuitNametoSuitID(static_cast<SuitName>(name));
}

SuitID SuitNametoSuitID(SuitName name)
{
    switch (name)
    {
    case Bat_Suit:      return Bat_SuitID;
    case Heat_Suit:     return Heat_SuitID;
    case Glide_Suit:    return Glide_SuitID;
    case Demo_Suit:     return Demo_SuitID;
    case Sonar_Suit:    return Sonar_SuitID;
        
    case Robin_Suit:    return Robin_SuitID;
    case Dive_Suit:     return Dive_SuitID;
    case Tech_Suit:     return Tech_SuitID;
    case Magnet_Suit:   return Magnet_SuitID;
    case Attracto_Suit: return Attracto_SuitID;

    default:            return Blocked_SuitID;
    }
}


Suits::Suits(DWORD BASE_ADDR): 
    BASE_ADDR(BASE_ADDR),
    _unlocked1(*reinterpret_cast<volatile WORD*>(BASE_ADDR + 0x006C9450)),
    _unlocked2(
        *reinterpret_cast<volatile WORD*>(
            reinterpret_cast<uintptr_t>(
                *reinterpret_cast<void**>(BASE_ADDR + 0x005C49CC)
            ) + 0x7BB4
        )
    ),
    _signals(BASE_ADDR)
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
        _signalsStart[i] = 0;
        _signalsPrev[i] = 0;
    }
    

}

void Suits::lock(size_t i)
{
    // untested
    _unlocked1 &= !((WORD)(1 << i));
    _unlocked2 &= !((WORD)(1 << i));
}

void Suits::unlock(size_t i)
{
    _unlocked1 |= (WORD)(1 << i);
    _unlocked2 |= (WORD)(1 << i);
}

void Suits::updateSuits() 
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

bool Suits::checkSignals()
{
    // untested
    for (size_t i = 0; i < 6; i++)
    {
        if (!_signals[i]) return false; // TODO: start from last instead. more likely to be unloaded.
        // should I set *_signal[i] to 0? no because of level instead of hub. I think new level resets auto.
    }
    return true;
}

void Suits::clearSignals()
{
    std::cout << "clearSignals" << std::endl;
    for (size_t i = 0; i < 6; i++)
    {
        _signalsPrev[i] = 0x0000;
        _signalsStart[i] = 0x0000;
        // should I set *_signal[i] to 0? no because of level instead of hub. I think new level resets auto.
        
    }
    
}

void Suits::restoreSignals()
{
    std::cout << "restoreSignals" << std::endl;
    if (!_signals[0] || !_signals[3]) {
        std::cout << "restoreSignals failed. Signals not loaded." << std::endl;
        return;
    }
    for (size_t i = 0; i < 6; i++)
    {
        if (*_signals[i] == Blocked_SuitID) { // TODO: may be redundant.
            std::cout << "restoring Signals " << (*_signals[i]) << " <-- p" << (_signalsPrev[i]) << std::endl;
            *_signals[i] = _signalsPrev[i];
        } 
        // else if (*_signals[i] != _signalsPrev[i]) {
        //     // suit changed?
        //     std::cout << "<<<<<<<<<SUIT CHANGED??????>>>>>>>>>>>>>" << std::endl;
        //     if (!lb1AP_items[80 + SuitIDtoSuitName(*_signals[i])]) {
        //         // this prevents bad suit detection.
        //         *_signals[i] = _signalsPrev[i];
        //     }
        // }
    }
}

// has a sleep in it. sleeps until suits are loaded
// Be very careful of where this function is used.
bool Suits::resetSignals()
{
    std::cout << "resetSignals" << std::endl;
    while (!_signals[0] || !_signals[3]) {
        std::cout << "resetSignal waiting.." << std::endl;
        Sleep(100); // WARN: this might interfere with stuff. 
    }
    
   // std::cout << "resetting signals." << std::endl;
    for (size_t i = 0; i < 6; i++)
    {
        // while (!_signals[i]) {
        //     std::cout << "resetSignal waiting.." << std::endl;
        //     Sleep(100); // WARN: this might interfere with stuff. 
        // }
        std::cout << "reseting Signals " << (*_signals[i]) << " --> p" << (_signalsPrev[i]) << std::endl;
        if (_signalsPrev[i] == 0 && *_signals[i] != 0) {
            // hopefully additive for the first appearance of a new suit
            _signalsStart[i] = *_signals[i];
            std::cout << "first appearance of suit hopefully " << _signalsStart[i] << std::endl;
        }
        _signalsPrev[i] = *_signals[i];
        //std::cout << "    " << _signalsPrev[i] << std::endl;
    }
    return true;
}

void Suits::updateSignals()
{
    std::cout << "updateSignals" << std::endl;
    if (!_signals[0] || !_signals[3]) {
        std::cout << "updateSignals failed. Signals not loaded." << std::endl;
        return;
    }

    for (size_t i = 0; i < 6; i++)
    {
        //std::cout << "update signal " << i << " " << _signalsPrev[i] << std::endl;
        // if (_signalsPrev[i] == 0 && _signals[i] != 0 && *_signals[i] != 0 && *_signals[i] != Blocked_SuitID) {
        //     // loaded I guess.
        //     std::cout << std::hex << i << " " << _signalsPrev[i] << " = " << (*_signals[i]) << std::endl;
        //     _signalsPrev[i] = *_signals[i];
        // }
        std::cout << "update signal " << i << " p" << _signalsPrev[i] << std::endl;

        // if (!_signals[0] || !_signals[3]) {
        //     std::cout << "updateSignals failed. Signals not loaded." << std::endl;
        //     return;
        // }
        
        switch (static_cast<SuitID>(_signalsPrev[i])) {
            case 0x0000:
                continue;
                break;
            case Bat_SuitID: 
                if (lb1AP_items[80 + Bat_Suit]) {
                    std::cout << "Batman" << std::endl;
                    *_signals[i] = Bat_SuitID;
                    continue;
                }
                break;
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
                std::cout << "Demo_suit id. item? " << lb1AP_items[80 + Demo_Suit] << std::endl;
                if (lb1AP_items[80 + Demo_Suit]) {
                    std::cout << "Demo" << std::endl;
                    *_signals[i] = Demo_SuitID;
                    continue;
                }
                break;
            case Sonar_SuitID: 
                if (lb1AP_items[80 + Sonar_Suit]) {
                    std::cout << "Sonar" << std::endl;
                    *_signals[i] = Sonar_SuitID;
                    continue;
                }
                break;
            case Robin_SuitID: 
                if (lb1AP_items[80 + Robin_Suit]) {
                    std::cout << "Robin" << std::endl;
                    *_signals[i] = Robin_SuitID;
                    continue;
                }
                break;
            case Dive_SuitID: 
                if (lb1AP_items[80 + Dive_Suit]) {
                    std::cout << "Dive" << std::endl;
                    *_signals[i] = Dive_SuitID;
                    continue;
                }
                break;
            case Tech_SuitID: 
                if (lb1AP_items[80 + Tech_Suit]) {
                    std::cout << "Tech" << std::endl;
                    *_signals[i] = Tech_SuitID;
                    continue;
                }
                break;
            case Magnet_SuitID: 
                if (lb1AP_items[80 + Magnet_Suit]) {
                    std::cout << "Magnet" << std::endl;
                    *_signals[i] = Magnet_SuitID;
                    continue;
                }
                break;
            case Attracto_SuitID: 
                if (lb1AP_items[80 + Attracto_Suit]) {
                    std::cout << "Attracto" << std::endl;
                    *_signals[i] = Attracto_SuitID;
                    continue;
                }
                break;
            default:
                std::cout << "default case p" << _signalsPrev[i] << std::endl;
                break;
            // TODO: the rest
        }
        std::cout << "   Blocking suit" << std::endl;
        *_signals[i] = Blocked_SuitID;
    }
    
}


size_t Suits::detectWear()
{
    for (size_t i = 0; i < 6; i++)
    {
        if (!_signals[i]) break; // TODO: start from last instead. more likely to be unloaded.
        //std::cout << "Detecting wear suit " << (*_signals[i])  << " !=? p" << _signalsPrev[i] << " ==? s" << _signalsStart[i] << std::endl;

        if (
            _signalsPrev[i] != 0 && 
            *_signals[i] != 0 && 
            *_signals[i] != Blocked_SuitID && // ignore blocked.
            *_signals[i] != _signalsPrev[i] && // suit swapped
            _signalsPrev[i] == _signalsStart[i] // from starting suit
        ) {
            // loaded I guess.
            std::cout << "Detected wear suit! " << (*_signals[i])  << " != p" << _signalsPrev[i] << " == s" << _signalsStart[i] << std::endl;
            //_signalsPrev[i] = *_signals[i];
            WORD result = _signalsStart[i];
            _signalsPrev[i] = *_signals[i];

            return SuitIDtoSuitName(result);
        }
    }
    
    return -1;
}






// void extern asmFunc(uintptr_t p, int a);

// &asmFunc = BASE_ADDR + 0xDEADBEEF;

// asmFunc(obj, 7);

Suits::SignalTable::SignalTable(DWORD BASE_ADDR):
    addr1(reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6BCF84)),
    addr2(reinterpret_cast<uintptr_t*>(BASE_ADDR + 0x6B7BBC))
{
    
}

WORD *Suits::SignalTable::operator[](size_t i) const
{
    //std::cout << " [signal called " << i << "] " ;
    if (i >= 6) return nullptr;
    
    //std::cout << "    signal derefing addrs " << std::endl;
    uintptr_t base = (i<3) ? (*addr1) : (*addr2);
    //std::cout << "    signal derefed addrs " << std::hex << base << std::endl;
    if (!base) {
        return nullptr;
    }
    size_t index = i % 3;
    return reinterpret_cast<WORD*>(base + 0x8C + index * 0x3D8);
    // if (i < 3) {
    //     return reinterpret_cast<WORD*>(
    //         addr1 + 0x8C + i*0x3D8
    //     );
    // } else {
    //     return reinterpret_cast<WORD*>(
    //         addr2 + 0x8C + (i-3)*0x3D8
    //     );
    // }
};
