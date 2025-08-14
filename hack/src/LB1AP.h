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

#define LB1AP_LOCATION_ID_OFFSET 400000 
#define LB1AP_ITEM_ID_OFFSET 400000 
#define LB1AP_NUM_LOCS_AND_ITEMS 500 //number of total locations and items TODO: update once fully implemented
#define GAME_NAME "Lego Batman: The Video Game"

void LB1AP_Init(const char* ip, const char* player_name, const char* passwd);
void LB1AP_CheckLocation(int64_t location_id);
bool LB1AP_location_checked(int64_t location_id);
void LB1AP_send_item(int64_t location_id);
void LB1AP_receiveItem(int itemID, bool notify);
void LB1AP_reset();
void LB1AP_CheckWinCon();
void LB1AP_Complete();
void LB1AP_Connect();
const char* readFile(std::ifstream& file);
AP_Message* LB1AP_GetMessage();
