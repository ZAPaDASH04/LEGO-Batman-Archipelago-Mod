 /**
 * @file messagebox.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <windows.h>
#include <string>

 class HintMessageBox
 {
 private:
    DWORD BASE_ADDR;
    DWORD hintId = 0x0093D3EC;
    // hint timer float LEGOBatman.exe+6C6F34 // disapears at ~7.5 // gets set to 7 when other text shows up.
    volatile float& timer; // TODO: test
    // hint to show LEGOBatman.exe+6C6F24
    volatile DWORD& shownHint; // TODO: test
    // color of text byte LEGOBatman.exe+6C6F29
    volatile BYTE& theme; // TODO: test
    // hint text *(LEGOBatman.exe+6957F8) + 0x203
    char* text; // TODO: test

    float timerPrev;
    float messageSpacing;
    bool hold;

    // code that sets the hint id. LEGOBatman.exe+1D522D - 89 35 246FAC00        - mov [LEGOBatman.exe+6C6F24],esi
    // code that lowers timer. LEGOBatman.exe+1D5550 - D9 1D 346FAC00        - fstp dword ptr [LEGOBatman.exe+6C6F34]
    // code that resets timer to 0. LEGOBatman.exe+1D5221 - D9 15 346FAC00        - fst dword ptr [LEGOBatman.exe+6C6F34]

public:
    HintMessageBox(DWORD BASE_ADDR);
    ~HintMessageBox();

    void setText(std::string text); // TODO: test
    void tick(); // TODO: test
    void holdMessage();
    void holdMessage(std::string text);
    void releaseMessage();
 };