/**
 * @file LB1AP.cpp
 * @author jrad (jradcode23@gmail.com) @jrad5768
 * @brief 
 * @version 0.1
 * @date 2025-07-30
 * 
 * 
 */

#include "LB1AP.h"
#include <fstream>
#include <cstring>

bool lb1AP_locations[LB1AP_NUM_LOCS]; //array with the total number of locations. Currently not all sequential in the apworld - currently larger than we need because not all sequential
bool lb1AP_items[LB1AP_NUM_ITEMS]; //array with the in game items - currently larger than we need because not all sequential
int minikits = 0;


void LB1AP_Init(const char* serverIP, const char* playerName, const char* password){
    AP_Init(serverIP, GAME_NAME, playerName, password);
    AP_SetItemClearCallback(&LB1AP_reset); //used to clear the state of the game. Called when connecting to server, not sure why but implemented like SM64 did
    AP_SetItemRecvCallback([&](int64_t itemID, bool notify){ //I believe this is what to do when items are received TODO: break out into own function
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

void LB1AP_CheckLocation(int64_t location_id){ //function to mark a location checked 
    lb1AP_locations[location_id - LB1AP_LOCATION_ID_OFFSET] = true;
}

void LB1AP_send_item(int64_t location_id){  //function to send an item
    AP_SendItem(location_id);
}

bool LB1AP_location_checked(int64_t location_id){ //function to verify if a location has been checked
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
}

void LB1AP_Connect(){
    std::ifstream connectionFile("APConnect.txt");
    if(!connectionFile){
        std::cout << "Failed to Open Connection File. Please ensure that APConnect.txt is in the same folder as the Lego Batman exe." << std::endl;
    }
    if(connectionFile){
        const char* header = readFile(connectionFile);
        const char* serverIP = readFile(connectionFile);
        const char* playerName = readFile(connectionFile);
        const char* password = readFile(connectionFile);
        connectionFile.close();
        std::cout << serverIP << " " << playerName << " " << password << std::endl; //cout statement for bug testing
        LB1AP_Init(serverIP, playerName, password);
        delete[] header;
        delete[] serverIP;
        delete[] playerName;
        delete[] password;
    }
}

const char* readFile(std::ifstream& file){
    if(file.eof()){
        return "";
    }
    std::string line {};
    std::getline(file, line);
    char* buffer = new char[line.length() + 1];
    strncpy(buffer, line.c_str(), line.length() + 1);
    return buffer;
}