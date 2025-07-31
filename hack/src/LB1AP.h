/**
 * @file LB1AP.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-07-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include "../inc/APCpp/Archipelago.h"
#include <iostream>

#define LB1AP_LOCATION_ID_OFFSET 15868690000 //TODO: work with snolid to get this adjusted - ideally they are all sequential
#define LB1AP_ITEM_ID_OFFSET 15868690076 //TODO: minikits are currently ID 0, to adjust it to be after the array
#define LB1AP_NUM_LOCS 200 //number of total locations TODO: update once fully implemented
#define LB1AP_NUM_ITEMS 400 //number of total items
#define GAME_NAME "Manual_LegoBatmanTheVideoGame_SnolidIce"

void LB1AP_Init(const char* ip, const char* player_name, const char* passwd);
void LB1AP_CheckLocation(int64_t location_id);
bool LB1AP_location_checked(int64_t location_id);
void LB1AP_send_item(int64_t location_id);
void LB1AP_reset();
