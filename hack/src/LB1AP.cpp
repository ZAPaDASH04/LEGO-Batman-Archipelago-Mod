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

bool lb1AP_locations[LB1AP_NUM_LOCS_AND_ITEMS]; //array with the total number of locations.
bool lb1AP_items[LB1AP_NUM_LOCS_AND_ITEMS]; //array with the in game items
int minikits = 0; //number of minikits received

void LB1AP_Init(const char* serverIP, const char* playerName, const char* password){
    AP_Init(serverIP, GAME_NAME, playerName, password);
    AP_SetItemClearCallback(&LB1AP_reset); //used to clear the state of the game. Called when connecting to server, not sure why but implemented like SM64 did
    AP_SetItemRecvCallback(LB1AP_receiveItem); //I believe this is what to do when items are received TODO: break out into own function
    AP_SetLocationCheckedCallback(&LB1AP_CheckLocation); //What to do when a location is checked
    AP_Start();
}

void LB1AP_CheckLocation(int64_t location_id){ //function to mark a location checked 
    lb1AP_locations[location_id - LB1AP_LOCATION_ID_OFFSET] = true;
}

void LB1AP_send_item(int64_t location_id){  //function to send an item
    AP_SendItem(location_id);
}

void LB1AP_receiveItem(int itemID, bool notify){
    itemID -= LB1AP_ITEM_ID_OFFSET;
    if(itemID >= 100 && itemID < 400){ //if the item is a minikit
        if(lb1AP_items[itemID] == false){ //TODO: make it so we can call location checked here
            lb1AP_items[itemID] = true;
            minikits++;
            printf("Total Minikits: %d\n", minikits);
        } else {
            printf("Minikit %d already received.\n", itemID);
        }
    }else{
        printf("Received Unknown Item: %d\n", itemID);
    }
    LB1AP_CheckWinCon();
}

bool LB1AP_location_checked(int64_t location_id){ //function to verify if a location has been checked
    return lb1AP_locations[location_id - LB1AP_LOCATION_ID_OFFSET];
}

void LB1AP_reset(){ 
    for(int i = 0; i < LB1AP_NUM_LOCS_AND_ITEMS; i++){
        lb1AP_locations[i] = false;
    }
    for(int i = 0; i < LB1AP_NUM_LOCS_AND_ITEMS; i++){
        lb1AP_items[i] = false;
    }
    minikits = 0;
}

void LB1AP_CheckWinCon(){
    if(minikits == 5){
        LB1AP_Complete();
    }
}

void LB1AP_Complete(){ //TODO: look into integrating this as part of the set reply handler
    AP_StoryComplete();
    printf("You weren't just the hero this game deserved, you were the one it needed.\n");
}

void LB1AP_Connect(){
    // Get the last write time of the file
    namespace fs = std::filesystem; //TODO: break this out into its own function WARNING: will likely crash if the file does not exist
    fs::path filePath("APConnect.txt");
    auto oldTime = fs::last_write_time(filePath);

    // Convert to system_clock time_point for standarization
    auto sctp = std::chrono::system_clock::now() +
                (oldTime - fs::file_time_type::clock::now());
    // Convert to time_t for human legibility
    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
    std::cout << "Last write time: " << std::put_time(std::localtime(&cftime), "%H:%M") << '\n';

    bool connected = false;
    while(!connected){
        std::ifstream connectionFile("APConnect.txt");
        if(!connectionFile){
            std::cout << "Failed to Open Connection File. Please ensure that APConnect.txt is in the same folder as the Lego Batman exe." << std::endl;
            Sleep(30000); //wait 30 second before trying again
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
        while(true){
            auto newFtime = fs::last_write_time(filePath);
            if(newFtime != oldTime){
                oldTime = newFtime;
                std::cout << "Connection file changed, reconnecting..." << std::endl;
                break; // file has changed, break out of loop
            }
            if(AP_GetConnectionStatus() == AP_ConnectionStatus::ConnectionRefused){
                printf("Connection Refused, please correct the connection file. \n");
            } else if(AP_GetConnectionStatus() == AP_ConnectionStatus::Authenticated){
                connected = true;
                break;
            }
            Sleep(2000); //wait 2 second before trying again
        }
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

AP_Message* LB1AP_GetMessage(){
    if(AP_GetConnectionStatus() == AP_ConnectionStatus::Disconnected) {return nullptr;}
    if(AP_GetConnectionStatus() == AP_ConnectionStatus::Connected) {return nullptr;}
    if(AP_GetConnectionStatus() == AP_ConnectionStatus::ConnectionRefused){
        std::cout << "Connection Refused, please correct the connection file and restart the game" << std::endl;
    }
    if(!AP_IsMessagePending()) return nullptr;
    AP_Message* msg = AP_GetLatestMessage();
    // if(msg->type == AP_MessageType::ItemSend){
    //     AP_ItemSendMessage* s_msg = static_cast<AP_ItemSendMessage*>(msg);
    //     std::cout << s_msg->item << " was sent to " << s_msg->recvPlayer << std::endl;
    // } else if(msg->type == AP_MessageType::ItemRecv){
    //     AP_ItemRecvMessage* s_msg = static_cast<AP_ItemRecvMessage*>(msg);
    //     std::cout << "You received " << s_msg->item << " from " << s_msg->sendPlayer << std::endl;
    // } else {
    //     std::cout << msg->text << std::endl;
    // }

    AP_ClearLatestMessage();
    return msg;
}

