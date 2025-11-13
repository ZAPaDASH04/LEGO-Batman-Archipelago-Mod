/**
 * @file LB1AP.h
 * @author jrad (jradcode23@gmail.com) @jrad5768
 * @brief 
 * @version 0.1
 * @date 2025-07-30
 * 
 * 
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include "../inc/APCpp/Archipelago.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <queue>


#ifndef LB1AP_H
#define LB1AP_H

#define LB1AP_LOCATION_ID_OFFSET 400000 
#define LB1AP_ITEM_ID_OFFSET 400000 
#define LB1AP_NUM_LOCS_AND_ITEMS 600 //number of total locations and items
#define GAME_NAME "Lego Batman The Video Game"

extern std::queue<int> receiveQueue;

#endif

struct Settings {
    static int minikits; //number of minikits received
    static int hostages; // number of hostages received
    static int lb1_endGoal; //0 = minikits which is currently default
    static int lb1_minikitsToWin; //number of minikits required to win. Default is 200
    static int lb1_levelsToWin; //number of levels required to win. Default is 20
    static int lb1_minikitSanity; //1 if minikit sanity check is enabled, 0 if not
    static int lb1_trueStatusSanity; //1 if true status sanity check is enabled, 0 if not
    static int lb1_freeplayUnlocked; //0 if level unlocks send story mode, 1 if freeplay is sent
    static int lb1_decoupledTokens; //1 for decoupled from purchases, 0 for coupled
    static int lb1_shuffleHushandRas; //1 to shuffle hush and ras, 0 to not
    static int lb1_hushUnlockCondition; //Number of hostages required to unlock purchase option
    static int lb1_rasUnlockCondition; //Number of minikits required to unlock purchase option
};


void LB1AP_Init(const char* ip, const char* player_name, const char* passwd);
void LB1AP_CheckLocation(int64_t location_id);
bool LB1AP_LocationChecked(int64_t location_id);
void LB1AP_SendItem(int64_t location_id);
void LB1AP_ReceiveItem(int itemID, bool notify);
void LB1AP_Reset();
void LB1AP_CheckMinikitWinCon();
void LB1AP_Complete();
void LB1AP_Connect();
const char* readFile(std::ifstream& file);
AP_Message* LB1AP_GetMessage();
void LB1AP_SetCompletionType(int type);
void LB1AP_SetMinikitsToWin(int num);
void LB1AP_SetLevelsToWin(int num);
void LB1AP_SetReplyHanlder(AP_SetReply reply);
void LB1AP_LevelComplete();
void LB1AP_SetMinikitSanity(int num);
void LB1AP_SetTrueStatusSanity(int num);
void LB1AP_SetFreeplayOrStory(int num);
void LB1AP_SetDecoupledTokens(int num);
void LB1AP_SetShuffleHushAndRas(int num);
void LB1AP_SetHushUnlockCondition(int num);
void LB1AP_SetRasUnlockCondition(int num);
AP_ConnectionStatus LB1AP_GetConnectionStatus();