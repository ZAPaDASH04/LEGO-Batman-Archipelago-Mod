/**
 * @file mod.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @authors jr (jradcode23@gmail.com) @jr5768
 * @brief 
 * @date 2025-07-07
 * 
 */

#include "LB1AP.h"

#include <windows.h>
#include <fstream>
#include <shlobj.h> // For SHGetFolderPath

#include "game.h"
#include "hintmessagebox.h"

#include <cctype> //TODO: remove if easter egg 1 is removed
#include <algorithm> //TODO: remove if easter egg 1 is removed

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
    std::cout << "Using Version 0.3.0-alpha" << std::endl;

    
    
    HMODULE hModule = nullptr;
    while ((hModule = GetModuleHandleA("LEGOBatman.exe")) == nullptr) {
        file << "Waiting for Module." << std::endl;
        Sleep(50);
    }
    file << "Module found." << std::endl;
    DWORD BASE_ADDR = (DWORD)hModule; 
    
    DWORD UP0 = 0x0; // initial version
    DWORD UP1 = 0x1000; // 7/9/2025 update | WARN: LEGO Batman suddenly updated and I assume this +0x1000 is the general fix for it at least when it comes to data addresses. code addresses vary.
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

    // PLAN: could be a trap.
    // disable game enabling/disabling extras. including in the extras menu.
    // LEGOBatman.exe+1CE96D - 88 44 8A 18           - mov [edx+ecx*4+18],al
    //WriteCode((BYTE*)(BASE_ADDR + 0x001CE96D),(BYTE[]){0x88, 0x44, 0x8A, 0x18},NOP,4);



    // disable default levels
    *game.levels.levelUnlocked[0] = 0; // H1-1
    *game.levels.levelUnlocked[5] = 0; // H2-1
    *game.levels.levelUnlocked[10] = 0; // H3-1

    // Send to Hub instead of YCBoB for New Game
    volatile DWORD& loadingZonePTR = *reinterpret_cast<volatile DWORD*>(BASE_ADDR + 0x6CA89C);
    volatile DWORD& hubAddress = *reinterpret_cast<volatile DWORD*>(BASE_ADDR + 0x6CA8B4);
    volatile DWORD& villainMissionAdress = *reinterpret_cast<volatile DWORD*>(BASE_ADDR + 0x5CA7B8); // WARN: What is this?
    
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
    
    
    // Easter Egg: Snolid
    volatile DWORD& harbouringAGrudgeAddress = *reinterpret_cast<volatile DWORD*>(BASE_ADDR + 0x5CA818);
    
    transform(c_player.begin(), c_player.end(), c_player.begin(), ::tolower);
    bool isSnolid = (c_player.find("snolid") != std::string::npos);
    std::cout << "isSnolid: " << isSnolid << std::endl;


    if(sublevelToLevel(game.currentLevel) != LevelName::V2_3 && isSnolid){
        while (sublevelToLevel(game.currentLevel) != LevelName::V2_3){
            std::cout << "Loading Zone PTR Value Snolid: " << loadingZonePTR << std::endl;
            loadingZonePTR = harbouringAGrudgeAddress;
            Sleep(50);
        }
        messageBox.setText("Holy Archipelago Batman!!! Successfully connected...");
        while (playerControl != 1) Sleep(100); // Wait till loaded into level
    }
    // End Easter Egg
    

    // check if need to clear log.

    std::ifstream b_filein("b.txt");
    std::string sessionId = c_player + "@" + c_host; 
    std::string line;
    if (!(getline(b_filein,line) && (line == sessionId))) {
        // different session need to clear
        std::cout << "===============clearing logs" << std::endl;
        resetLogs(sessionId);
    }




    ///////// TODO: do a loop of all memory for missed checks.







    // WARN: temporary setup for testing.



    // Easier true status
    *game.extraEnabled[ExtraName::Always_Score_Multiply] = 1;
    // Detectors on
    *game.extraEnabled[ExtraName::Minikit_Detector] = 1;
    *game.extraEnabled[ExtraName::Power_Brick_Detector] = 1;
    
    
    // unlock all characters
    // for (size_t i = 0; i < game.characters.characterCount; i++)
    // {
    //     *game.characters._characterBytes[i] = 0x03;
    // }
    *game.characters._characterBytes[0] = 0x03;
    *game.characters._characterBytes[1] = 0x03;

    // unlock all suits
    for (size_t i = 0; i < 10; i++) 
    {
        game.suitUnlocked1 |= (WORD)(1 << i);
        game.suitUnlocked2 |= (WORD)(1 << i);
    }









    /*/////////////////////
    -////  Main Loop  ////-
    ////////////////////*/

    file << "About to loop." << std::endl;

    DWORD loops = 0;
    BYTE sublevprev = game.currentLevel;
    BYTE lev = sublevelToLevel(game.currentLevel);
    BYTE levprev = lev;
    while (true) {
        // Level changed
        if (game.currentLevel != sublevprev) {
            std::cout << "Sub Level Changed to " 
                      << std::hex << (int)game.currentLevel 
                      << ". Level " << std::dec << (int)sublevelToLevel(game.currentLevel)
                      << std::endl;

            sublevprev = game.currentLevel;

            if (lev != sublevelToLevel(sublevprev)) {
                // Level changed
                lev = sublevelToLevel(sublevprev);
                std::cout << "Level Changed to " 
                      << std::dec << (int)lev
                      << std::endl;
                if (lev <= LevelName::V3_5) {
                    // entered a level
                    std::cout << "entered a level." << std::endl;
                    // TODO: can I make this level specifc. no loop? 
                    // FIXME: I am pretty sure you can just use lev as the index
                    for (size_t i = 0; i < 30; i++)
                    {
                        *game.levels.levelBeaten[i] = 0;
                    }
                    

                } else if (lev >= LevelName::Shop_Room && lev <= LevelName::Mission_Room) {
                    // entered hub or Unknown
                    std::cout << "entered hub." << std::endl;
                    // TODO: can I make this level specifc. no loop?
                    // FIXME: I am pretty sure you can just use lev as the index
                    for (size_t i = 0; i < 30; i++)
                    {
                        if (*game.levels.levelUnlocked[i] == 1 && (game.levels.levelBeatenPrev[i] == 1 || Settings::lb1_freeplayUnlocked == 1)) *game.levels.levelBeaten[i] = 1;
                    }
                }
                levprev = lev;
            }
        }
        




        loopTest(game,loops);





        /*//////////////
        *//// Shop ////*
        //////////////*/

        if (game.inShopSubMenuPrev != game.inShopSubMenu) {
            std::cout << "entered or exited shop." << std::endl;
            BYTE state;
            if (game.isInShop()) {
                // entered shop
                std::cout << "entered shop" << std::endl;

                // per level
                for (DWORD64 i = 0; i < 35; i++)
                {
                    state = game.powerBrickState[i];
                    std::cout << i << " : " << std::hex << (int)state << std::endl;

                    // Collected Item
                    if (i<30) {
                        if (state & 0b0010) *game.levels.levelRedBrick[i] = 1;
                        else *game.levels.levelRedBrick[i] = 0;
                    }

                    // Purchased Location
                    if (state & 0b0100) game.powerBrickPurchased |= (((DWORD64)1) << (DWORD64)(i+((DWORD)1)));
                    else game.powerBrickPurchased &= ~(DWORD64)(((DWORD64)1) << (DWORD64)(i+((DWORD)1)));
                    game.extraPurchasedPrev = game.powerBrickPurchased;
                }

                // per character
                for (DWORD64 i = 0; i < Characters::characterCount; i++)
                {
                    // if (game.characters.Token[i] && !game.characters.characterPurchased[i]) {
                    //     *game.characters[i] = 0x02;
                    // }
                    if (game.characters.token[i]) {
                        if (game.characters.purchased[i]){
                            *game.characters[i] = 0x03;
                            // TODO: do I need to lock purchase
                        } else {
                            *game.characters[i] = 0x02;
                            game.characters.purchaseLocks(i); // should safely ignore invalid i
                        }
                    } else {
                        *game.characters[i] = 0x00; // this is so that it's a silouhette
                        //game.characters.purchaseLocks(i);
                        // TODO: does this work as intended?
                    }
                }
                

                
            } else {
                // left shop
                std::cout << "exited shop" << std::endl;

                // per level
                for (DWORD64 i = 0; i < 35; i++)
                {
                    std::cout << i << " : " << std::hex << (int)state << std::endl;
                    state = game.powerBrickState[i];
                    
                    // Collected Location
                    
                    if (i<30) {
                        if (state & 0b0001) *game.levels.levelRedBrick[i] = 1;
                        else *game.levels.levelRedBrick[i] = 0;
                    }
                    // Purchased Item
                    if (state & 0b1000) game.powerBrickPurchased |= (((DWORD64)1) << (DWORD64)(i+((DWORD)1)));
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
                    if (game.characters.unlocked[i]) {
                        *game.characters[i] = 0x03;
                    } else {
                        *game.characters[i] = 0x00;
                    }
                }

            }
            game.inShopSubMenuPrev = game.inShopSubMenu;
        }





        /*////////////////////////////
        *//// Location Detection ////*
        ////////////////////////////*/

        // TODO: extract each of these to functions. probably in this file.
        // TODO: add autosaveing to some


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
        int hostageCheck = game.levels.checkHostages();
        if (hostageCheck != -1) { // TODO: I don't know if this is how logic works
            std::cout
                << "new hostage " << (int) hostageCheck 
                << ". hostagedata " << std::hex << (int) game.levels.hostages
                << std::endl;
            LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 400 + hostageCheck);
            game.levels.hostagesOld = game.levels.hostages;
            // TODO: Hostage count/hush wincon
        }


        // Levels
        // TODO: simplify
        if (lev >= LevelName::Shop_Room && lev <= LevelName::Mission_Room) {} // not in a level?
        else {

            if (game.levels.levelBeatenPrev[lev] != 1 && isSublevelStatus(game.currentLevel)) {
                // level beaten
                std::cout << "New Level Beaten: " << (int) lev << std::endl;
                LB1AP_SendItem(LB1AP_LOCATION_ID_OFFSET + 425 + lev);
                std::cout << "Changing Level Beaten Previous" << std:: endl;
                *game.levels.levelBeaten[lev] = 1;
                game.levels.levelBeatenPrev[lev] = *game.levels.levelBeaten[lev]; 
            }

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
                game.powerBrickState[sublevelToLevel(game.currentLevel)] |= 0b0001;
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
                    game.powerBrickState[powerBrickCheck] != 0b0100;
                    game.extraPurchasedPrev = game.powerBrickPurchased;
                    if (powerBrickCheck >= 20) *game.suitUpgradeEnabled[powerBrickCheck-20] = 0; // enable suit upgrade
                }
            }

            // Character Purchased
            // TODO: do that.

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
                // TODO: implement

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
                game.powerBrickState[i-485] |= 0b0010; // set state to item received


            } else if (i < 550) { // Red Brick Purchased
                std::cout << "Red Brick Unlocked!" << std::endl;
                game.powerBrickState[i-515] |= 0b1000; // set state to item received
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
                    *game.characters[i-550] = 0x02;
                    game.characters.purchaseLocks(i-550); // TODO: maybe need this.
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
