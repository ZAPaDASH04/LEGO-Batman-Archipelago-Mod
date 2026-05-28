/**
 * @file mod.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @authors jr (jradcode23@gmail.com) @jr5768
 * @brief 
 * @date 2025-07-07
 * 
 */



/////////////////////////////////
/////////////////////////////////
//// IF YOU ARE READING THIS ////
//// This is mostly rapid a- ////
//// -nd experimental code.  ////
//// It will be cleaned up   ////
//// in the restructure.     ////
/////////////////////////////////
/////////////////////////////////










#include "LB1AP.h"

#include <windows.h>
#include <fstream>
#include <shlobj.h> // For SHGetFolderPath

#include "game.h"
#include "hintmessagebox.h"

// #include <cctype> //TODO: remove if easter egg 1 is removed
// #include <algorithm> //TODO: remove if easter egg 1 is removed

std::ofstream file;
std::ofstream b_file;

#define RESTART_MARKER "================Restart================"

bool WriteCode(LPVOID pAddress, void* bytesOld, void* bytes, int byteCount){
    int maxWaitMs = 20000;
    file << "Writing code." << std::endl;

    int waited = 0;
    while (waited < maxWaitMs) {
        if (memcmp(pAddress, bytesOld, byteCount) == 0) {
            break; // Pattern matched, safe to patch
        }
        file << "Doesn't Match." << std::endl;
        Sleep(200);
        waited += 200;
    }

    if (waited >= maxWaitMs) {
        file << "Failed to write. " << std::endl;
        return false;
    }
    


    file << "Memory is valid. Attempting patch..." << std::endl;


    // Change memory protection to allow writing
    DWORD oldProtect;
    if (!VirtualProtect(pAddress, byteCount, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        file << "Failed to change protections." << std::endl;
        return false;
    }
    // Write the bytes
    memcpy(pAddress, bytes, byteCount);
    file << "Written." << std::endl;

    // Restore the original protection
    VirtualProtect(pAddress, byteCount, oldProtect, &oldProtect);

    return true;

}

void HookFunc() {

}



void loopTest(Game game, DWORD loops) {
    if (loops%500 == 0) {
        std::cout << "Loop test" << std::endl 
                  //<< std::hex << game.currentLevel << std::endl
                  << "levels:" << std::endl
                  << "Hero:" << std::endl;


        std::cout << std::endl << "inlevel stuff" << std::endl 
                  << std::hex << (int) game.currentLevel << " " 
                  << std::hex << (int) game.inLevelTotalKitCount << " " 
                  << std::hex << (int) game.inLevelKitCount << std::endl;
        for (size_t i = 0; i < game.inLevelKitCount; i++)
        {
            std::cout << " " << std::hex << (int) game.inLevelKitLocations[i] << " " << game.inLevelKits[i];
        }
        std::cout << std::endl;
        
        

        // kit stuff
        std::cout << "Kit save data stuff" << std::endl;
        std::cout << std::hex << game.levels.levelKitSaveData << std::endl;
        std::cout << std::hex << &(game.levels.levelKitSaveData[0x12]) << std::endl;
        std::cout << (char*)game.levels.levelKitSaveData[0x12].kits[0] << std::endl << std::endl;

        // Level Beaten info.
        if (sublevelToLevel(game.currentLevel) >= LevelName::Shop_Room && sublevelToLevel(game.currentLevel) <= LevelName::Mission_Room) {} // not in a level?
        else {
            std::cout << "Level Beaten Debug: " << std::endl;
            std::cout << "Sublevel: " << (int) game.currentLevel << std::endl; 
            std::cout << "Level Previously Considered Beaten: " << (int) game.levels.levelBeatenPrev[sublevelToLevel(game.currentLevel)] << std::endl; // 0 not beaten, 1 beaten
            std::cout << "On Final Status Screen: " << isSublevelStatus(game.currentLevel) << std::endl;
            std::cout << std::endl;
        }
        
    }
    
    
}


void resetLog(std::string filename, std::string sessionId) {
    std::ifstream fin(filename);
    std::stringstream buffer;
    std::string line;
    bool keep = false;

    while (std::getline(fin, line)) {
        if (line == RESTART_MARKER) {
            buffer.str(""); // clear prev?
            buffer.clear(); // clear err?
            keep = true;
            //buffer << line << "\n";
        }
        if (keep) buffer << line << "\n";
    }
    fin.close();

    std::ofstream fout(filename, std::ios::trunc);
    fout << sessionId << std::endl;
    fout << buffer.str();
}

void resetLogs(std::string sessionId) {
    resetLog("a.txt", sessionId);
    resetLog("b.txt", sessionId);
}


DWORD WINAPI ThreadProc(LPVOID lpParam) {
    HMODULE hSelf = (HMODULE)lpParam;

    // Prevent the DLL from being unloaded? TODO: I don't know if this actually does anything
    HMODULE dummy;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)hSelf, &dummy);
    

    file.open("a.txt", std::ios::app);
    b_file.open("b.txt", std::ios::app);

    std::cout.rdbuf(file.rdbuf());
    std::cerr.rdbuf(file.rdbuf());
    
    freopen("b.txt", "a", stdout);
    freopen("b.txt", "a", stderr);
    setvbuf(stdout, NULL, _IONBF, 0);
    
    std::cout << "\n" RESTART_MARKER "\n";
    printf("\n" RESTART_MARKER "\n"); // I hope this works

    file << "ThreadProc started" << std::endl;
    std::cout << "Using Version 0.3.0-alpha pre-release 8" << std::endl;

    
    
    HMODULE hModule = nullptr;
    while ((hModule = GetModuleHandleA("LEGOBatman.exe")) == nullptr) {
        file << "Waiting for Module." << std::endl;
        Sleep(50);
    }
    file << "Module found." << std::endl;
    DWORD BASE_ADDR = (DWORD)hModule; 
    
    DWORD UP0 = 0x0; // initial version
    DWORD UP1 = 0x1000; // 7/9/2025 update | LEGO Batman suddenly updated and I assume this +0x1000 is the general fix for it at least when it comes to data addresses. code addresses vary.
    DWORD UP = UP0 + UP1;


    // Wait for data and code to be readable and writable
    BYTE** playerControlOnP = ((BYTE**)(BASE_ADDR + UP + 0x006B264C));

    while ((*playerControlOnP == nullptr) || (*((*playerControlOnP) + 0x258) == 0)) {
        // Waiting for game to be ready for modification
        Sleep(100);
    }
    volatile BYTE& playerControl = *(volatile BYTE*)((*playerControlOnP) + 0x258);



    
    // build the game data and message box
    std::cout << "before game" << std::endl;
    Game game(BASE_ADDR + UP);
    std::cout << "before message box" << std::endl;
    HintMessageBox messageBox(BASE_ADDR + UP);









    /*////////////////////////////////
    -////  Pre Loop Setup Begin  ////-
    ////////////////////////////////*/


    // Nops for overwriting code
    BYTE NOP[16] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};

    
    // 7 byte add function
    // subtracting offset UP1 and adding 0x20 added by update 1. this means that the new code added in update 1 is after the damage code.
    // U0 -> BASE_ADDR + 1C356D
    // U1 -> BASE_ADDR + 1C358D
    //BYTE* dmgFuncAddr = (BYTE*)(BASE_ADDR + (0x20) + (0x1C356D)); // not the damage function instead it's a pointer to an add function that adds -1 to health.
    

    // Message box overwrites

    // code that sets the hint id. LEGOBatman.exe+1D522D - 89 35 246FAC00        - mov [LEGOBatman.exe+6C6F24],esi
    WriteCode((BYTE*)(BASE_ADDR + 0x001D522D),(BYTE[]){0x89,0x35,0x24,0x6F,0xAC,0x00},NOP,6);
    // code that sets the hint id to 0 when timer > ~7.5. LEGOBatman.exe+1D5574 - C7 05 246FAC00 00000000 - mov [LEGOBatman.exe+6C6F24],00000000
    WriteCode((BYTE*)(BASE_ADDR + 0x001D5574),(BYTE[]){0xC7,0x05,0x24,0x6F,0xAC,0x00,0x00,0x00,0x00,0x00},NOP,10);
    // code that lowers timer. LEGOBatman.exe+1D5550 - D9 1D 346FAC00        - fstp dword ptr [LEGOBatman.exe+6C6F34]
    WriteCode((BYTE*)(BASE_ADDR + 0x001D5550),(BYTE[]){0xD9,0x1D,0x34,0x6F,0xAC,0x00},NOP,6);
    // code that resets timer to 0. LEGOBatman.exe+1D5221 - D9 15 346FAC00        - fst dword ptr [LEGOBatman.exe+6C6F34]
    WriteCode((BYTE*)(BASE_ADDR + 0x001D5221),(BYTE[]){0xD9,0x15,0x34,0x6F,0xAC,0x00},NOP,6);
    // code that resets time to 0 on level change? LEGOBatman.exe+1D4E3D - D9 15 346FAC00        - fst dword ptr [LEGOBatman.exe+6C6F34]
    WriteCode((BYTE*)(BASE_ADDR + 0x001D4E3D),(BYTE[]){0xD9,0x15,0x34,0x6F,0xAC,0x00},NOP,6);

    // Level unlocking overwrites

    // disable game unlocking levels
    WriteCode((BYTE*)(BASE_ADDR + 0x0024E254),(BYTE[]){0xC6,0x44,0x90,0x02,0x01},NOP,5);
    WriteCode((BYTE*)(BASE_ADDR + 0x0024E155),(BYTE[]){0xC6,0x44,0x90,0x02,0x01},NOP,5);
    // for beating episode
    WriteCode((BYTE*)(BASE_ADDR + 0x000788BF),(BYTE[]){0xC6, 0x84, 0x81, 0xDA, 0x77, 0x00, 0x00, 0x01},NOP,8);

    // suits
    // LEGOBatman.exe+A7B15 - 89 15 50A4AC00        - mov [LEGOBatman.exe+6CA450],edx { (41) }
    WriteCode((BYTE*)(BASE_ADDR + 0x000A7B15),(BYTE[]){0x89, 0x15, 0x50, 0xA4, 0xAC, 0x00},NOP,6);
    // LEGOBatman.exe+F24AA - 09 15 50A4AC00        - or [LEGOBatman.exe+6CA450],edx { (41) }
    WriteCode((BYTE*)(BASE_ADDR + 0x000F24AA),(BYTE[]){0x09, 0x15, 0x50, 0xA4, 0xAC, 0x00},NOP,6);



    // PLAN: could be a trap.
    // disable game enabling/disabling extras. including in the extras menu.
    // LEGOBatman.exe+1CE96D - 88 44 8A 18           - mov [edx+ecx*4+18],al
    //WriteCode((BYTE*)(BASE_ADDR + 0x001CE96D),(BYTE[]){0x88, 0x44, 0x8A, 0x18},NOP,4);






    /*/////////////////////
    -////  Save Load  ////-
    /////////////////////*/

    // PLAN: need to deal with player loading a save or exiting to menu then loading their save.







    // disable default levels
    *game.levels.levelUnlocked[0] = 0; // H1-1
    *game.levels.levelUnlocked[5] = 0; // H2-1
    *game.levels.levelUnlocked[10] = 0; // H3-1

    // Send to Hub instead of YCBoB for New Game
    volatile DWORD& loadingZonePTR = *reinterpret_cast<volatile DWORD*>(BASE_ADDR + 0x6CA89C);
    volatile DWORD& hubAddress = *reinterpret_cast<volatile DWORD*>(BASE_ADDR + 0x6CA8B4);
    // volatile DWORD& villainMissionAdress = *reinterpret_cast<volatile DWORD*>(BASE_ADDR + 0x5CA7B8); // WARN: What is this? - response: jrad - address that has the pointer for the villain mission loading zone. Currently unused but we wanted it added potentially for future - commenting out for now.
    
    std::cout << "Hub Pointer Address: " << hubAddress << std::endl;
    std::cout << "Loading Zone PTR Value: " << loadingZonePTR << std::endl;

    if(sublevelToLevel(game.currentLevel) != LevelName::Mission_Room){
        while (sublevelToLevel(game.currentLevel) != LevelName::Mission_Room){
                std::cout << "Loading Zone PTR Value: " << loadingZonePTR << std::endl;
                loadingZonePTR = hubAddress;
            Sleep(10);
        }
    }

    while (playerControl != 1) Sleep(100); // Wait till loaded into level

    
    messageBox.holdMessage("Holy Archipelago Batman!!! Attempting to connect...");
    //TODO: block player movement
    LB1AP_Connect();
    while(LB1AP_GetConnectionStatus() != AP_ConnectionStatus::Authenticated) {
        messageBox.tick();
        Sleep(10);
    }
    messageBox.setText("Holy Archipelago Batman!!! Successfully connected...");
    messageBox.releaseMessage();
    //TODO: restore player movement

    std::ifstream connectionFile("APConnect.txt");
    std::string c_host;
    std::string c_player;
    std::getline(connectionFile, c_host);
    std::getline(connectionFile, c_host);
    std::getline(connectionFile, c_player);
    connectionFile.close();
    
    
    // // Easter Egg 1 
    // volatile DWORD& harbouringAGrudgeAddress = *reinterpret_cast<volatile DWORD*>(BASE_ADDR + 0x5CA818);
    
    // transform(c_player.begin(), c_player.end(), c_player.begin(), ::tolower);
    // bool isEasterEgg1 = (c_player.find("snolid") != std::string::npos);
    // std::cout << "EasterEgg1: " << isEasterEgg1 << std::endl;


    // if(sublevelToLevel(game.currentLevel) != LevelName::V2_3 && isEasterEgg1){
    //     while (sublevelToLevel(game.currentLevel) != LevelName::V2_3){
    //         std::cout << "Loading Zone PTR Value EasterEgg1: " << loadingZonePTR << std::endl;
    //         loadingZonePTR = harbouringAGrudgeAddress;
    //         Sleep(50);
    //     }
    //     messageBox.setText("Holy Archipelago Batman!!! Successfully connected...");
    //     while (playerControl != 1) Sleep(100); // Wait till loaded into level
    // }
    // // End Easter Egg 1
    

    // check if need to clear log.

    std::ifstream b_filein("b.txt");
    std::string sessionId = c_player + "@" + c_host; 
    std::string line;
    if (!(getline(b_filein,line) && (line == sessionId))) {
        // different session need to clear
        std::cout << "===============clearing logs" << std::endl;
        resetLogs(sessionId);
    }




    ///////// TODO: do a loop of all memory for missed checks. Instead make every check check against the lists in lb1ap.


    /*/////////////////////////
    -////  Load Checked  ////-
    ////////////////////////*/

    // TODO: test this.
    for (size_t i = 0; i < Characters::characterCount; i++) 
    {
        game.characters.purchased[i] = lb1AP_locations[i];
    }
    for (size_t i = 0; i < 10; i++)
    {
        //if (lb1AP_locations[80+i]) game.suits.unlock(i);
        game.suits.updateSuits();
    }
    
    game.suits.resetSignals();

    for (size_t i = 0; i < 30; i++) {
        game.levels.levelBeatenPrev[i] = lb1AP_locations[425 + i];

    }
    for (size_t i = 0; i < 35; i++) { // red brick
        // Collected Location
        if (i<30) {
            if (lb1AP_locations[485+i]) *game.levels.levelRedBrick[i] = 1;
            else *game.levels.levelRedBrick[i] = 0;
        }
        // Purchased Item
        if (lb1AP_items[515+i]) {
            game.powerBrickPurchased |= (((DWORD64)1) << (DWORD64)(i+((DWORD)1)));
            if (i >= 20) *game.suitUpgradeEnabled[i-20] = 1; 
        } else {
            game.powerBrickPurchased &= ~(DWORD64)(((DWORD64)1) << (DWORD64)(i+((DWORD)1)));
            if (i >= 20) *game.suitUpgradeEnabled[i-20] = 0; 
        };

        // enable suit upgrades.
        game.extraPurchasedPrev = game.powerBrickPurchased;
    }





    // WARN: temporary setup for testing.



    // Easier true status
    *game.extraEnabled[ExtraName::Always_Score_Multiply] = 1;
    // Detectors on
    *game.extraEnabled[ExtraName::Minikit_Detector] = 1;
    *game.extraEnabled[ExtraName::Power_Brick_Detector] = 1;

    // unlock all suits
    // for (size_t i = 0; i < 10; i++) 
    // {
    //     game.suits.unlock(i);
    // }

    // batman and robin on by default
   lb1AP_items[Batman] = true; // batman
   lb1AP_items[Robin] = true; // robin
    *game.characters[0] = 0x03;
    *game.characters[1] = 0x03;
    game.characters.unlocked[0] = 1;
    game.characters.unlocked[1] = 1;

   lb1AP_items[80+Bat_Suit] = true; // batman
   lb1AP_items[80+Robin_Suit] = true; // robin
    game.suits.unlock(Bat_Suit);
    game.suits.unlock(Robin_Suit);

    // WARNING: this is to remove suit logic
    lb1AP_items[80+Demo_Suit] = true; 
    game.suits.unlock(Demo_Suit);
    lb1AP_items[80+Glide_Suit] = true; 
    game.suits.unlock(Glide_Suit);
    lb1AP_items[80+Heat_Suit] = true; 
    game.suits.unlock(Heat_Suit);
    lb1AP_items[80+Sonar_Suit] = true; 
    game.suits.unlock(Sonar_Suit);

    lb1AP_items[80+Tech_Suit] = true; 
    game.suits.unlock(Tech_Suit);
    lb1AP_items[80+Magnet_Suit] = true; 
    game.suits.unlock(Magnet_Suit);
    lb1AP_items[80+Attracto_Suit] = true; 
    game.suits.unlock(Attracto_Suit);
    lb1AP_items[80+Dive_Suit] = true; 
    game.suits.unlock(Dive_Suit);







    /*/////////////////////
    -////  Main Loop  ////-
    ////////////////////*/

    file << "About to loop." << std::endl;

    DWORD loops = 0;
    BYTE sublevprev = game.currentLevel;
    BYTE lev = sublevelToLevel(game.currentLevel);
    BYTE levprev = lev;
    uint8_t numHeroEpsisodesBeaten = 0;
    bool firstHeroEpisodeBeaten = false;
    bool firstHeroLevelBeaten = false;
    bool inLevel = false;
    bool hushSent = false;
    bool RasSent = false;
    BYTE inCharSelector = 0;
    while (true) {

        // Level changed
        if (game.currentLevel != sublevprev) {
            std::cout << "Sub Level Changed to " 
                      << std::hex << (int)game.currentLevel 
                      << ". Level " << std::dec << (int)sublevelToLevel(game.currentLevel)
                      << std::endl;

            sublevprev = game.currentLevel;
            
            // if (sublevelToLevel(sublevprev) == (BYTE)-1) {
            //     // invalid level
            //     lev = sublevelToLevel(sublevprev);
            //     std::cerr << "ERR: unknown sublevel id 0x" << std::hex << sublevprev << " previous lev 0x" << levprev << std::endl;
            // } else 
            if (lev != sublevelToLevel(sublevprev)) {
                // Level changed
                lev = sublevelToLevel(sublevprev);
                std::cout << "Level Changed to " 
                      << std::dec << (int)lev
                      << std::endl;

                //game.suits.clearSignals();
                if (lev == ((BYTE)-1)) {
                    std::cerr << "ERR: unknown sublevel id 0x" << std::hex << sublevprev << " previous lev 0x" << levprev << std::endl;
                }

                if (lev <= LevelName::V3_5) {
                    inLevel = true;
                    // entered a level
                    std::cout << "entered a level." << std::endl;
                    // TODO: test. if works apply to remove the loop below. nvm I think the loop below is needed.
                    if (game.levels.levelBeatenPrev[lev] == 0) *game.levels.levelBeaten[lev] = 0;
                    

                } else if (lev >= LevelName::Shop_Room && lev <= LevelName::Mission_Room) {
                    // entered hub or Unknown
                    inLevel = false;
                    std::cout << "entered hub." << std::endl;
                    // TODO: can I make this level specifc. no loop?
                    // FIXME: I am pretty sure you can just use lev as the index
                    for (size_t i = 0; i < 30; i++)
                    {
                        if (*game.levels.levelUnlocked[i] == 1 && (game.levels.levelBeatenPrev[i] == 1 || Settings::lb1_freeplayUnlocked == 1)) *game.levels.levelBeaten[i] = 1;
                    }

                    for (size_t i = 0; i < Characters::characterCount; i++)
                    {
                        if (game.characters.unlocked[i]){
                            *game.characters[i] = 0x03; // TODO: test if I can set this to 0x02 and it still be blocked. because in villain room what if the character starts spawning in the hub while you are in the shop.
                        } else {
                            *game.characters[i] = 0x00;
                        }
                    }
                    
                }
                levprev = lev;

                // WARN: USES WAITS
                if (lev != ((byte)-1) && !isSublevelStatus(sublevprev)) {
                    std::cout << "new level suits" << std::endl;
                    // wait for playable
                    while (playerControl != 1) Sleep(100); // Wait till loaded into level
                    game.suits.clearSignals();
                    // TODO: maybe insert clear if in mission rooms.
                    game.suits.resetSignals();
                    game.suits.updateSignals();
                }
            } else {
                // same level

                // WARN: USES WAITS
                if (lev != ((byte)-1) &&!isSublevelStatus(sublevprev)) { 
                    std::cout << "same level suits" << std::endl;
                    // wait for playable
                    while (playerControl != 1) Sleep(100); // Wait till loaded into level
                    game.suits.restoreSignals();
                    // TODO: maybe insert clear if in mission rooms.
                    game.suits.resetSignals();
                    game.suits.updateSignals();
                }
            }
        }
        
        BYTE levelType = 0;
        //fix character selector
        if (!inLevel && game.inCharacterSelectMenu != inCharSelector) {
            inCharSelector = game.inCharacterSelectMenu;
            
            if (inCharSelector) {
                if (game.player1SelectedCharacter < Characters::characterOffsets[Batmobile]) {
                    // characters
                    size_t p1 = -1; // index of hovered character
                    size_t p2 = -1; // index of hovered character
                    for (size_t i = 0; i < Characters::characterCount; i++)
                    {
                        if (p1==-1 && Characters::characterOffsets[i] == game.player1SelectedCharacter) p1 = i;
                        if (p2==-1 && Characters::characterOffsets[i] == game.player2SelectedCharacter) p2 = i;

                    }

                    // if (lb1AP_items[p1]) {
                    //     // p1 is valid so don't move it

                    // };
                    if (!lb1AP_items[p1]) p1 = -1;
                    if (!lb1AP_items[p2]) p2 = -1;
                    // needs to be moved
                    size_t v1 = p1;
                    size_t v2 = p2;
                    for (size_t i = Batman; i < Batmobile; i++)
                    {
                        if (lb1AP_items[i]) {
                            if (v1==-1) {
                                if (i != p2) v1 = i;
                            } else if (v2==-1) {
                                if (i != p1) v2 = i;
                            }
                        }
                    }

                    if (v2 == -1) {
                        // only one character unlocked
                        // this is a problem because the player could have control of player 2 while p1 is a bot wich would break this.
                        // I think I have a non existant character. 0x03
                        v2 = Noone;
                    } else if (v1 == -1) {
                        // only one character unlocked
                        // this is a problem because the player could have control of player 2 while p1 is a bot wich would break this.
                        // I think I have a non existant character. 0x03
                        v1 = Noone;
                    } else if (v1 == v2) { // both are negative right // TODO: test
                        v1 = Batman;
                        v2 = Batman;
                    }

                    game.player1SelectedCharacter = Characters::characterOffsets[v1];
                    game.player2SelectedCharacter = Characters::characterOffsets[v2];
                        
                } else if (game.player1SelectedCharacter < Characters::characterOffsets[Batboat]) {
                    // landcraft
                    levelType = 1;
                    // seems to auto correct unlike characters
                } else if (game.player1SelectedCharacter < Characters::characterOffsets[Batwing]) {
                    // watercraft
                    levelType = 2;
                    // seems to auto correct unlike characters
                } else {
                    // aircraft
                    levelType = 3;
                    
                    // seems to auto correct unlike characters
                }
            }
        }



        loopTest(game,loops);





        /*//////////////
        *//// Shop ////*
        //////////////*/

        // not repetitive. only on enter and exit.
        if (game.inShopSubMenuPrev != game.inShopSubMenu) {
            std::cout << "entered or exited shop." << std::endl;
            BYTE state;
            if (game.isInShop()) {
                // entered shop
                std::cout << "entered shop" << std::endl;

                // per level
                for (DWORD64 i = 0; i < 30; i++) {
                    *game.levels.levelBeaten[i] = (BYTE)0;
                }

                // per extra
                for (DWORD64 i = 0; i < 35; i++)
                {

                    // red bricks

                    //state = game.powerBrickState[i];
                    //std::cout << i << " : " << std::hex << (int)state << std::endl;

                    // Collected Item
                    if (i<30) {
                        if (lb1AP_items[485+i]) *game.levels.levelRedBrick[i] = 1;
                        else *game.levels.levelRedBrick[i] = 0;
                    }

                    // Purchased Location
                    // TODO: when starting new game purchases in shop aren't shown purchased but are prevented from multipurchasing. need to do the same thing as characters. third pointer.
                    if (lb1AP_locations[515+i]) game.powerBrickPurchased |= (((DWORD64)1) << (DWORD64)(i+((DWORD)1)));
                    else game.powerBrickPurchased &= ~(DWORD64)(((DWORD64)1) << (DWORD64)(i+((DWORD)1)));

                    game.extraPurchasedPrev = game.powerBrickPurchased;


                }

                // per character
                for (DWORD64 i = 0; i < Characters::characterCount; i++)
                {
                    // if (game.characters.Token[i] && !game.characters.characterPurchased[i]) {
                    //     *game.characters[i] = 0x02;
                    // }
                    // TODO: test
                    if (lb1AP_items[550+i]) { // characters.token
                        if (lb1AP_locations[i]){ // characters.purchased
                            *game.characters[i] = 0x03; // TODO: test if I can set this to 0x02 and it still be blocked. because in villain room what if the character starts spawning in the hub while you are in the shop.
                            game.characters.purchaseLocks(i); // maybe put at end of ifs
                        } else {
                            *game.characters[i] = 0x02;
                            game.characters.purchaseLocks(i); // TODO: may be unneeded. // should safely ignore invalid i
                        }
                    } else {
                        *game.characters[i] = 0x00; // this is so that it's a silouhette
                        game.characters.purchaseLocks(i); // if this is 1 and the character is 0x03 then it is locked but the bottom text is wrong.
                    }
                }
                

                
            } else {
                // left shop
                std::cout << "exited shop" << std::endl;

                // per level 
                for (DWORD64 i = 0; i < 30; i++) {
                    *game.levels.levelBeaten[i] = game.levels.levelBeatenPrev[i]; // TODO: unsure whether this works as intended.
                }

                // per extra
                for (DWORD64 i = 0; i < 35; i++)
                {
                    //std::cout << i << " : " << std::hex << (int)state << std::endl;
                    //state = game.powerBrickState[i];
                    
                    // Collected Location
                    if (i<30) {
                        if (lb1AP_locations[485+i]) *game.levels.levelRedBrick[i] = 1;
                        else *game.levels.levelRedBrick[i] = 0;
                    }
                    // Purchased Item
                    if (lb1AP_items[515+i]) game.powerBrickPurchased |= (((DWORD64)1) << (DWORD64)(i+((DWORD)1)));
                    else game.powerBrickPurchased &= ~(DWORD64)(((DWORD64)1) << (DWORD64)(i+((DWORD)1)));

                    game.extraPurchasedPrev = game.powerBrickPurchased;
                }
                
                // per character
                for (DWORD64 i = 0; i < Characters::characterCount; i++)
                {
                    // if (game.characters.token[i]) {
                    //     if (game.characters.Purchased[i]){
                    //         *game.characters[i] = 0x03;
                    //         // FIXME: lock purchase
                    //     } else {
                    //         *game.characters[i] = 0x02;
                    //         // FIXME: unlock purchase
                    //     }
                    // } else {
                    //     *game.characters[i] = 0x00; // this is so that it's a silouhette
                    // }
                    if (game.characters.unlocked[i]) { // TODO: change to use items
                        *game.characters[i] = 0x03;
                    } else {
                        *game.characters[i] = 0x00; // for when character has been purchased it would be 3 when shouldn't
                    }
                }

            }
            game.inShopSubMenuPrev = game.inShopSubMenu;
        }





        /*////////////////////////////
        *//// Location Detection ////*
        ////////////////////////////*/

        // TODO: extract each of these to functions. 
        // TODO: add autosaveing to some

        // Characters
        // had to be moved to after levels.
        // TODO: check alternatives.

        // Suits
        //if (inLevel && playerControl && game.suits.checkSignals()) {
        if (inLevel && playerControl) {
            
            //std::cout << "AAAAAAAAAAAAAA" << std::endl;
            size_t wore = game.suits.detectWear();
            if (wore!=-1) {
                std::cout 
                    << "new suit " << wore
                    << std::endl;
                // TODO: needes extensive testing
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 80 + wore);
            }

        }

        // Minikits
        if (Settings::lb1_minikitSanity) {
            //TODO: improve
            if (game.inLevelKitCount < game.inLevelKitCountPrev) {
                // left level
                game.inLevelKitCountPrev = 0;

            } else for (BYTE i = game.inLevelKitCountPrev; i < game.inLevelKitCount; i++)
            {
                // new kit picked up.
                std::cout  
                    << (int) *game.inLevelKitLocations[i] << " " << game.inLevelKits[i] << " : "
                    << (int) game.minikits.findKitIndex(*game.inLevelKitLocations[i], game.inLevelKits[i])
                    << std::endl;
                
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 100 + game.minikits.findKitIndex(*game.inLevelKitLocations[i], game.inLevelKits[i]));
                /** 
                 * TODO: auto save the kit to the file.
                 * levelKitData = game.levels.levelKitSaveData[*game.inLevelKitLocations[i]];
                 * strncpy(levelKitData.kits[levelKitData.count], game.inLevelKits[i], 8);
                 */ 
                game.inLevelKitCountPrev++;
            }
        }


        // Hostages
        bool newHostage = false; // true if a new hostage was gotten this loop.
        int hostageCheck = game.levels.checkHostages();
        if (hostageCheck != -1) { // TODO: I don't know if this is how logic works
            std::cout
                << "new hostage " << (int) hostageCheck 
                << ". hostagedata " << std::hex << (int) game.levels.hostages
                << std::endl;
            newHostage = true;
            LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 400 + hostageCheck);
            game.levels.hostagesOld = game.levels.hostages;
            // TODO: Hostage count/hush wincon
        }


        // Levels
        // TODO: simplify
        bool beatNewLevel = false;
        bool beatNewHeroLevel = false; // for batman and robin.
        int tokenOffset = 0x226;
        if (lev >= LevelName::Shop_Room && lev <= LevelName::Mission_Room) {} // not in a level?
        else {

            if (game.levels.levelBeatenPrev[lev] != 1 && isSublevelStatus(game.currentLevel)) {
                // level beaten
                std::cout << "New Level Beaten: " << (int) lev << std::endl;
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 425 + lev);
                std::cout << "Changing Level Beaten Previous" << std:: endl;
                *game.levels.levelBeaten[lev] = 1;
                game.levels.levelBeatenPrev[lev] = *game.levels.levelBeaten[lev]; 
                beatNewLevel = true;

                // character stuff
                if (lev < 15) {
                    // hero level beaten.
                    beatNewHeroLevel = true; // for batman and robin.

                    // episode beaten 
                    numHeroEpsisodesBeaten++;
                    for (size_t i = 0; i < 5; i++)
                    {
                        if (game.levels.levelBeatenPrev[(lev/5)*5 + i] != 1) {
                            numHeroEpsisodesBeaten--;
                            break;
                        };
                    }
                }

            }

        }
        
        // Character Found
        if (beatNewLevel) {
            // any H level
            // if (beatNewHeroLevel && !firstHeroLevelBeaten) { // TODO: obsolete you can use purchased bool to prevent duplicates
            //     firstHeroLevelBeaten = true; // prevent multiple
                
            //     game.characters.purchased[Batman] = 1;
            //     game.characters.purchased[Robin] = 1;
            //     LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
            //     LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);

            // }

            // any H episode beaten
            if (numHeroEpsisodesBeaten == 1 && !firstHeroEpisodeBeaten) { // TODO: obsolete you can use purchased bool to prevent duplicates
                // 1st hero episode beaten.
                firstHeroEpisodeBeaten = true; // prevent multiple triggers.

                // PLAN: these characters may get a setting to split them among the H?-5 levels
                // game.characters.purchased[Bruce_Wayne] = 1;
                // game.characters.purchased[Alfred_Pennyworth] = 1;
                // game.characters.purchased[Batgirl] = 1;
                // game.characters.purchased[Nightwing] = 1;
                // game.characters.purchased[Police_Officer] = 1;
                // game.characters.purchased[Military_Policeman] = 1;
                // game.characters.purchased[Security_Guard] = 1;
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Bruce_Wayne + tokenOffset);
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Alfred_Pennyworth + tokenOffset);
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batgirl + tokenOffset);
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Nightwing + tokenOffset);
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Police_Officer + tokenOffset);
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Military_Policeman + tokenOffset);
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Security_Guard + tokenOffset);
            
            }

            // all H episodes
            if (numHeroEpsisodesBeaten==3) { // TODO: obsolete you can use purchased bool to prevent duplicates
                numHeroEpsisodesBeaten++; // don't trigger again
                // game.characters.purchased[Bat_Tank] = 1;
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Bat_Tank + tokenOffset);

            }

            // per level
            
            switch (lev) {
                case H1_1: 
                    // game.characters.purchased[Riddler_Goon] = 1;
                    // game.characters.purchased[Riddler_Henchman] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Riddler_Goon + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Riddler_Henchman + tokenOffset);
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break;
                case H1_2:
                    // game.characters.purchased[Freeze_Girl] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Freeze_Girl + tokenOffset);
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break;
                case H1_3:
                    game.characters.purchased[Batmobile] = 1;
                    game.characters.purchased[Batcycle] = 1;
                    // game.characters.purchased[Police_Car] = 1;
                    // game.characters.purchased[Police_Bike] = 1;
                    // game.characters.purchased[Police_Van] = 1;
                    // game.characters.purchased[The_Jokers_Van] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batmobile);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batcycle);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Police_Car + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Police_Bike + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Police_Van + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Jokers_Van + tokenOffset);
                    break; 
                case H1_4:
                    // game.characters.purchased[Poison_Ivy_Goon] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Poison_Ivy_Goon + tokenOffset);
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break; 
                case H1_5:
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break;
                case H2_1: 
                    // game.characters.purchased[Fishmonger] = 1;
                    // game.characters.purchased[Penguin_Goon] = 1;
                    // game.characters.purchased[Penguin_Henchman] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Fishmonger + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Penguin_Goon + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Penguin_Henchman + tokenOffset);
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break; 
                case H2_2: 
                    game.characters.purchased[Batboat] = 1;
                    game.characters.purchased[Robins_Watercraft] = 1;
                    // game.characters.purchased[Robins_Submarine] = 1;
                    // game.characters.purchased[Penguin_Goon_Submarine] = 1;
                    // game.characters.purchased[Harbour_Helicopter] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batboat);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robins_Watercraft);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robins_Submarine + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Penguin_Goon_Submarine + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Harbour_Helicopter + tokenOffset);
                    break; 
                case H2_3: // sewers
                    break; 
                case H2_4:
                    // game.characters.purchased[Zoo_Sweeper] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Zoo_Sweeper + tokenOffset);
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break; 
                case H2_5:
                    // game.characters.purchased[Man_Bat] = 1;
                    // game.characters.purchased[Yeti] = 1;
                    // game.characters.purchased[Penguin_Minion] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Man_Bat + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Yeti + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Penguin_Minion + tokenOffset);
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break;
                case H3_1: 
                    // game.characters.purchased[Mad_Hatter] = 1;
                    // game.characters.purchased[Joker_Goon] = 1;
                    // game.characters.purchased[Joker_Henchman] = 1;
                    // game.characters.purchased[Mad_Hatters_Steamboat] = 1;
                    // game.characters.purchased[Mad_Hatters_Glider] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Mad_Hatter + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Joker_Goon + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Joker_Henchman + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Mad_Hatters_Steamboat + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Mad_Hatters_Glider + tokenOffset);
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break; 
                case H3_2:
                    // game.characters.purchased[Clown_Goon] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Clown_Goon + tokenOffset);
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break; 
                case H3_3:
                    game.characters.purchased[Batwing] = 1;
                    game.characters.purchased[Batcopter] = 1;
                    // game.characters.purchased[Bruce_Waynes_Private_Jet] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batwing);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batcopter);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Bruce_Waynes_Private_Jet + tokenOffset);
                    break; 
                case H3_4:
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break; 
                case H3_5:
                    if (!game.characters.purchased[Batman]) {
                        game.characters.purchased[Batman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Batman);
                    }
                    if (!game.characters.purchased[Robin]) {
                        game.characters.purchased[Robin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Robin);
                    }
                    break;
                case V1_1:
                    game.characters.purchased[Clayface] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Clayface);
                    
                    if (!game.characters.purchased[The_Riddler]) { // PLAN: this could be obsolete if you modify SendItem to not send duplicates.
                        game.characters.purchased[The_Riddler] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Riddler);
                    }
                    break; 
                case V1_2: 
                    game.characters.purchased[Mr_Freeze] = 1;
                    // game.characters.purchased[Mr_Freezes_Kart] = 1;
                    // game.characters.purchased[Mr_Freezes_Iceberg] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Mr_Freeze);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Mr_Freezes_Kart + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Mr_Freezes_Iceberg + tokenOffset);
                    if (!game.characters.purchased[The_Riddler]) {
                        game.characters.purchased[The_Riddler] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Riddler);
                    }
                    break; 
                case V1_3:
                    game.characters.purchased[Poison_Ivy] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Poison_Ivy);
                    if (!game.characters.purchased[The_Riddler]) {
                        game.characters.purchased[The_Riddler] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Riddler);
                    }
                    break; 
                case V1_4:
                    // game.characters.purchased[Scientist] = 1;
                    // game.characters.purchased[Two_Faces_Armoured_Truck] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Scientist + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Two_Faces_Armoured_Truck + tokenOffset);
                    if (!game.characters.purchased[The_Riddler]) {
                        game.characters.purchased[The_Riddler] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Riddler);
                    }
                    if (!game.characters.purchased[Two_Face]) {
                        game.characters.purchased[Two_Face] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Two_Face);
                    }
                    break; 
                case V1_5:
                    // game.characters.purchased[SWAT] = 1;
                    // game.characters.purchased[Riddlers_jet] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + SWAT + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Riddlers_jet + tokenOffset);
                    if (!game.characters.purchased[The_Riddler]) {
                        game.characters.purchased[The_Riddler] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Riddler);
                    }
                    
                    if (!game.characters.purchased[Two_Face]) {
                        game.characters.purchased[Two_Face] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Two_Face);
                    }
                    break;
                case V2_1: 
                    // game.characters.purchased[Sailor] = 1;
                    game.characters.purchased[Bane] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Sailor + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Bane);
                    if (!game.characters.purchased[The_Penguin]) {
                        game.characters.purchased[The_Penguin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Penguin);
                    }
                    break; 
                case V2_2: 
                    // game.characters.purchased[Catwoman_Classic] = 1;
                    // game.characters.purchased[Catwomans_Motorcycle] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Catwoman_Classic + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Catwomans_Motorcycle + tokenOffset);
                    if (!game.characters.purchased[Catwoman]) {
                        game.characters.purchased[Catwoman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Catwoman);
                    }
                    if (!game.characters.purchased[The_Penguin]) {
                        game.characters.purchased[The_Penguin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Penguin);
                    }
                    break; 
                case V2_3:
                    // game.characters.purchased[Police_Watercraft] = 1;
                    // game.characters.purchased[Police_Boat] = 1;
                    game.characters.purchased[Penguins_Submarine] = 1;
                    game.characters.purchased[Killer_Crocs_Swamp_Rider] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Police_Watercraft + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Police_Boat + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Penguins_Submarine);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Killer_Crocs_Swamp_Rider);
                    break; 
                case V2_4:
                    game.characters.purchased[Killer_Croc] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Killer_Croc);
                    if (!game.characters.purchased[The_Penguin]) {
                        game.characters.purchased[The_Penguin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Penguin);
                    }
                    break; 
                case V2_5:
                    if (!game.characters.purchased[Catwoman]) {
                        game.characters.purchased[Catwoman] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Catwoman);
                    }
                    if (!game.characters.purchased[The_Penguin]) {
                        game.characters.purchased[The_Penguin] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Penguin);
                    }
                    break;
                case V3_1: 
                    // game.characters.purchased[Commissioner_Gordon] = 1;
                    // game.characters.purchased[Harley_Quinns_Hammer_Truck] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Commissioner_Gordon + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Harley_Quinns_Hammer_Truck + tokenOffset);
                    if (!game.characters.purchased[Harley_Quinn]) {
                        game.characters.purchased[Harley_Quinn] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Harley_Quinn);
                    }
                    if (!game.characters.purchased[The_Joker]) {
                        game.characters.purchased[The_Joker] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Joker);
                    }
                    break; 
                case V3_2: 
                    game.characters.purchased[The_Jokers_Helicopter] = 1;
                    game.characters.purchased[The_Scarecrows_Biplane] = 1;
                    // game.characters.purchased[Police_Helicopter] = 1;
                    // game.characters.purchased[Goon_Helicopter] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Jokers_Helicopter);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Scarecrows_Biplane);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Police_Helicopter + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Goon_Helicopter + tokenOffset);
                    break; 
                case V3_3:
                    game.characters.purchased[The_Scarecrow] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Scarecrow);
                    if (!game.characters.purchased[The_Joker]) {
                        game.characters.purchased[The_Joker] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Joker);
                    }
                    break; 
                case V3_4:
                    game.characters.purchased[Killer_Moth] = 1;
                    // game.characters.purchased[Garbage_Truck] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Killer_Moth);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Garbage_Truck + tokenOffset);
                    if (!game.characters.purchased[The_Joker]) {
                        game.characters.purchased[The_Joker] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Joker);
                    }
                    break; 
                case V3_5:
                    // game.characters.purchased[Police_Marksman] = 1;
                    // game.characters.purchased[The_Joker_Tropical] = 1;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Police_Marksman + tokenOffset);
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Joker_Tropical + tokenOffset);
                    if (!game.characters.purchased[Harley_Quinn]) {
                        game.characters.purchased[Harley_Quinn] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Harley_Quinn);
                    }
                    if (!game.characters.purchased[The_Joker]) {
                        game.characters.purchased[The_Joker] = 1;
                        LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + The_Joker);
                    }
                    break;
            }
        }
        
        // Hard Character Found
        // ras and hush
        if (Settings::hostages >= Settings::lb1_hushUnlockCondition && !hushSent) {
            // hush
            std::cout << "New Hard Character Found: Hush (" << std::dec << Settings::hostages << "/" << Settings::lb1_hushUnlockCondition << ")" << std::endl;
            LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Hush + tokenOffset);      
            hushSent = true;      
        }
        if (Settings::minikits >= Settings::lb1_rasUnlockCondition && !RasSent) {
            // ras al ghul
            std::cout << "New Hard Character Found: Ras (" << std::dec << Settings::minikits << "/" << Settings::lb1_rasUnlockCondition << ")" << std::endl;
            LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + Ras_Al_Ghul + tokenOffset);
            RasSent = true;
        }

        // True Status
        if (Settings::lb1_trueStatusSanity) {
            if (game.inLevelTrueStatus != game.inLevelTrueStatusPrev) {
                if (game.inLevelTrueStatus < game.inLevelTrueStatusPrev) {
                    // left level
                    game.inLevelTrueStatusPrev = 0;

                } else {
                    // TrueStatus Get
                    std::cout
                        << "new True Status " << (int) lev
                        << std::endl;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 455 + sublevelToLevel(game.currentLevel));
                    game.inLevelTrueStatusPrev = game.inLevelTrueStatus;
                }

            }
        }

        // Red Brick Collected
        if (game.inLevelPowerBrick != game.inLevelPowerBrickPrev) {
            if (game.inLevelPowerBrick < 2) {
                // left level
                game.inLevelPowerBrickPrev = 0;

            } else {
                // PowerBrick Get
                std::cout
                    << "new Power Brick found " << (int) lev
                    << std::endl;
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 485 + sublevelToLevel(game.currentLevel));
                game.inLevelPowerBrickPrev = game.inLevelPowerBrick;
                //game.powerBrickState[sublevelToLevel(game.currentLevel)] |= 0b0001;
            }

        }



        // Shop locations
        if (game.isInShop()) {

            // Red Brick Purchased
            if (game.extraPurchasedPrev != game.powerBrickPurchased) {
                int powerBrickCheck = game.checkPowerBricks() - 1;
                if (powerBrickCheck != -1) { // TODO: I don't think this will ever not trigger.
                    // std::cout
                    //     << "new PowerBrick " << (int) powerBrickCheck
                    //     << ". PowerBrickdata " << std::hex << (int) game.powerBrickPurchased
                    //     << std::endl;
                    std::cout
                        << "new Power Brick purchased " << (int) lev
                            << std::endl;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 515 + powerBrickCheck);
                    //game.powerBrickState[powerBrickCheck] != 0b0100;
                    game.extraPurchasedPrev = game.powerBrickPurchased;
                    if (powerBrickCheck >= 20) *game.suitUpgradeEnabled[powerBrickCheck-20] = 0; // enable suit upgrade
                }
            }

            // Character Purchased
            for (size_t i = 0; i < Characters::characterCount; i++)
            {
                // TODO: may be bad logic
                if (lb1AP_locations[i] == false && *game.characters[i] == 0x03) {
                    std::cout
                        << "new Character purchased " << (int) i
                        << std::endl;
                    LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + i);
                    game.characters.purchased[i] = true; // WARN: needs to be stored and loaded.
                }
                
            }
            
        }




        
        /*////////////////////////
        *//// Item Receiving ////*
        ////////////////////////*/

        while (!receiveQueue.empty())
        {
            int i = receiveQueue.front();
            std::cout << "Received an Item!" << std::endl;
            if (i < 80) {
                // Characters
                std::cout << "Playable Character Unlocked!" << std::endl;
                game.characters.unlocked[i] = true;
                if (!game.isInShop()) { // enable if not in shop
                    *game.characters[i] = 0x03;
                }
                //std::cerr << "You should not be getting this: %d (report to devs)" << std::endl;
            } else if (i < 90) {
                // Suits
                std::cout << "Suit Unlocked!" << std::endl;
                // WARN: no idea if this is right
                game.suits.unlock(i-80);
                game.suits.updateSignals(); // skips if signals aren't loaded

            } else if (i < 100) {
                std::cerr << "Invalid Suit? Received." << std::endl;
            } else if (i < 400) { // Minikits
                std::cerr << "You should not be getting Minikits (report to devs)" << std::endl;
            } else if (i < 425) { // Hostages
                std::cerr << "You should not be getting Hostages (report to devs)" << std::endl;
            } else if (i < 455) { // Level Unlock
                std::cout << "Level Unlocked!" << std::endl;
                *game.levels.levelUnlocked[i-425] = 1;
                std::cout << "freeplay? " << Settings::lb1_freeplayUnlocked << std::endl;
                if (Settings::lb1_freeplayUnlocked==1) *game.levels.levelBeaten[i-425] = 1;

            } else if (i < 485) { // True status
                std::cerr << "You should not be getting true statuses (report to devs)" << std::endl;
            } else if (i < 515) { // Red Brick token
                std::cout << "Red Brick Token Unlocked!" << std::endl;
                //game.powerBrickState[i-485] |= 0b0010; // set state to item received


            } else if (i < 550) { // Red Brick Purchased
                std::cout << "Red Brick Unlocked!" << std::endl;
                //game.powerBrickState[i-515] |= 0b1000; // set state to item received
                if (!game.isInShop()) { // enable if not in shop
                    game.powerBrickPurchased |= (((DWORD64)1) << (DWORD64)(i-((DWORD64)515)+((DWORD)1)));
                    
                }
                if (i-515 >= 20) *game.suitUpgradeEnabled[i-515-20] = 1; 

                

            } else if (i < 630) {
                // Character Tokens
                /* vanilla shop tokens
                    brucewayne
                    alfred
                    batgirl
                    nightwing
                    commissionergordon
                    policeman_2
                    fish_monger
                    military_policeman
                    securityguard_1
                    swatteam
                    scientist
                    navy_sailor
                    policemarksman
                    catwoman2
                    manbat
                    madhatter
                    jokerhawaiian
                    poisonivy_goon
                    zoosweeper
                    f_goon
                    f_yeti
                    r_goon
                    r_goon_gun
                    p_goon
                    p_goon_gun
                    penguingoon
                    j_goon
                    j_goon_gun
                    Clown_Goon
                    Hush
                    Ras
                    policecarFP
                    policebikeFP
                    policevanFP
                    batTank
                    catbike
                    twofacetruck
                    mrfreezekart
                    harlequin4x4
                    jokerVanFP
                    dustbinLorry
                    robinsub
                    policejetski
                    police_boat
                    penguinsub
                    mrfreezeberg
                    madPedalo
                    harbourchopper
                    policechopperFP
                    learjet
                    gooncopter
                    riddlerjet
                    madGlider
                */
                // switch (i-550) {
                //     case Bruce_Wayne:
                //         break;
                // }
                //*game.characters[i-550] = 0x02; 
                //game.characters.Purchased
                std::cout << "Character Token Unlocked!" << std::endl;
                game.characters.token[i-550] = true;
                if (game.isInShop()) { // TODO: this is untested and I'm not sure if it does what I want.
                    std::cout << "heg" << std::endl;
                    *game.characters[i-550] = 0x02;
                    std::cout << "purchlocks" << std::endl;
                    game.characters.purchaseLocks(i-550);
                }
            } else { // out of bound
                std::cout << "Received Unknown Item: " << (int) i;
            }
            receiveQueue.pop();

        }
        



        /*////////////////////////
        *//// Post Detection ////*
        ////////////////////////*/

        messageBox.tick();

        Sleep(10);
        loops++;
    }

    // WARN: Crashed?

    file.close();
    b_file.close(); // close file before infinite loop. //temporarily moved to test sending/receiving items via archi

    FreeLibraryAndExitThread(hSelf, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    // WARNING: minimize code in this function.
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule); // Prevent thread attach/detach notifications
        HANDLE hThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)hModule, 0, NULL);
        if (hThread) CloseHandle(hThread);
    }
    return TRUE;
}
