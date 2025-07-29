#include "LB1AP.h"

bool lb1AP_locations[400]; //array with the total number of locations. Currently not all sequential in the apworld - currently larger than we need because not all sequential
bool lb1AP_items[200]; //array with the in game items - currently larger than we need because not all sequential
int minikits = 0;
int gold_bricks = 0;


void LB1AP_Init(const char* serverIP, const char* playerName, const char* password){
    AP_Init(serverIP, GAME_NAME, playerName, password);
    AP_SetItemClearCallback(&LB1AP_reset); //used to clear the state of the game. Called when connecting to server, not sure why but implemented like SM64 did
    AP_SetItemRecvCallback([&](int64_t itemID, bool notify){ //I believe this is what to do when items are received WARNING: AP when initially connecting is sending an unrecognized item ID? 
        itemID -= LB1AP_ITEM_ID_OFFSET;
        if(itemID == 0){
            minikits++;
        } else if(itemID > 0){
            lb1AP_items[itemID] = true;
        }

        if(notify){ //if the player has notify turned out, how we want to out put it
            printf("Item ID was received: %011d\n", itemID); //TODO: notify via in game hint text
        }
    });
    AP_SetLocationCheckedCallback(&LB1AP_CheckLocation); //What to do when a location is checked
    AP_Start();
}

void LB1AP_CheckLocation(int64_t location_id){
    lb1AP_locations[location_id - LB1AP_LOCATION_ID_OFFSET] = true;
}

void LB1AP_send_item(int64_t location_id){
    AP_SendItem(location_id);
}

bool LB1AP_location_checked(int64_t location_id){
    return lb1AP_locations[location_id - LB1AP_LOCATION_ID_OFFSET];
}

void LB1AP_reset(){
    for(int i = 0; i < LB1AP_NUM_LOCS; i++){
        lb1AP_locations[i] = false;
    }
    for(int i = 0; i < LB1AP_NUM_ITEMS; i++){
        lb1AP_items[i] = false;
    }
    minikits = 0;
    gold_bricks = 0;
}