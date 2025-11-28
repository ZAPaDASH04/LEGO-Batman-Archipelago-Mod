/**
 * @file LB1AP.cpp
 * @author jrad (jradcode23@gmail.com) @jrad5768
 * @brief Handles the connection between Lego Batman 1 and archipelago
 * @date 2025-07-30
 * 
 * 
 */

#include "LB1AP.h"

bool lb1AP_locations[LB1AP_NUM_LOCS_AND_ITEMS] = {}; // Array with the every location.
bool lb1AP_items[LB1AP_NUM_LOCS_AND_ITEMS] = {}; // Array with evert item.

int Settings::minikits = 0; // Number of minikits received
int Settings::hostages = 0; // Number of hostages received
int Settings::lb1_endGoal = 0; // 0 = minikits which is currently default 1 = levels beaten
int Settings::lb1_minikitsToWin {}; // Number of minikits required to win. Default is 200
int Settings::lb1_levelsToWin {}; // Number of levels required to win. Default is 20
int Settings::lb1_minikitSanity {}; // 1 if minikit sanity check is enabled, 0 if not
int Settings::lb1_trueStatusSanity {}; // 1 if true status sanity check is enabled, 0 if not
int Settings::lb1_freeplayUnlocked {}; // 1 if freeplay is unlocked, 0 if story mode is unlocked
int Settings::lb1_decoupledTokens {}; //1 for decoupled from purchases, 0 for coupled
int Settings::lb1_decoupleHushandRasTokens {}; //1 to decouple hush and ras tokens from purchases, 0 to not
int Settings::lb1_shuffleHushandRas {}; //1 to shuffle hush and ras, 0 to not
int Settings::lb1_hushUnlockCondition {}; //Number of hostages required to unlock purchase option
int Settings::lb1_rasUnlockCondition {}; //Number of minikits required to unlock purchase option

std::queue<int> receiveQueue;

void LB1AP_Init(const char* serverIP, const char* playerName, const char* password){
    AP_Init(serverIP, GAME_NAME, playerName, password);
    AP_SetItemClearCallback(&LB1AP_Reset); // Used to clear the state of the game. Called when connecting to server as AP sends/receives all locations/items upon connection
    AP_SetItemRecvCallback(LB1AP_ReceiveItem); // What to do when an item is received
    AP_SetLocationCheckedCallback(&LB1AP_CheckLocation); // What to do when a location is checked
    AP_SetNotify(AP_GetPrivateServerDataPrefix() + "CompleteLevelGoal", AP_DataType::Int); // Used to make sure Level Complete win condition is properly calculated between sessions
    AP_RegisterSetReplyCallback(&LB1AP_SetReplyHanlder); // Set reply hanlder for Level complete win condition
    // Read Slot Data
    AP_RegisterSlotDataIntCallback("EndGoal", &LB1AP_SetCompletionType);
    AP_RegisterSlotDataIntCallback("MinikitsToWin", &LB1AP_SetMinikitsToWin);
    AP_RegisterSlotDataIntCallback("LevelsToWin", &LB1AP_SetLevelsToWin);
    AP_RegisterSlotDataIntCallback("MinikitSanity", &LB1AP_SetMinikitSanity);
    AP_RegisterSlotDataIntCallback("TrueStatusSanity", &LB1AP_SetTrueStatusSanity);
    AP_RegisterSlotDataIntCallback("FreeplayOrStory", &LB1AP_SetFreeplayOrStory);
    AP_RegisterSlotDataIntCallback("DecoupledTokens", &LB1AP_SetDecoupledTokens);
    AP_RegisterSlotDataIntCallback("DecoupleShuffleHushAndRasToken", &LB1AP_SetHushAndRasDecoupledTokens);
    AP_RegisterSlotDataIntCallback("ShuffleHushAndRas", &LB1AP_SetShuffleHushAndRas);
    AP_RegisterSlotDataIntCallback("HushUnlockCondition", &LB1AP_SetHushUnlockCondition);
    AP_RegisterSlotDataIntCallback("RasUnlockCondition", &LB1AP_SetRasUnlockCondition);
    AP_Start();
}

// Marks a location as checked in the location array
void LB1AP_CheckLocation(int64_t location_id){ 
    lb1AP_locations[location_id - LB1AP_LOCATION_ID_OFFSET] = true;
    std::cout << "Location " << location_id << " checked!" << std::endl;
}

// Sends an item to the server
void LB1AP_SendItem(int64_t location_id){
    int temp_location_id = location_id - LB1AP_LOCATION_ID_OFFSET;
    std::cout <<"Temp Location ID: " << temp_location_id << std::endl;
    // Added Fail-safe for collect messing up level beaten win condition
    if(temp_location_id >= 425 && temp_location_id < 455 && Settings::lb1_endGoal == 1){
        auto count = std::count_if(lb1AP_locations + 425, lb1AP_locations + 454, [](bool checked){ return checked == true; });
        if(!lb1AP_locations[temp_location_id]){
            count++; //increment count if this level hasn't been counted yet
        }
        std::cout << "Levels beaten so far per location array: " << count << std::endl;
        if(count >= Settings::lb1_levelsToWin){
            LB1AP_Complete();
        }
    }
    if(LB1AP_LocationChecked(location_id)){
        std::cout << "Location: " << temp_location_id << " already checked, not sending item." << std::endl;
        return;
    }
    // Testing if the level beaten has already been sent. If not, adding one to our level beaten count stored on the server
    if(temp_location_id >= 425 && temp_location_id < 455 && Settings::lb1_endGoal == 1){
        printf("Calling Level Complete request function\n");
        LB1AP_LevelComplete();
    }
    AP_SendItem(location_id);
}

// Handles an item received from the server
void LB1AP_ReceiveItem(int itemID, bool notify){
    itemID -= LB1AP_ITEM_ID_OFFSET;
    lb1AP_items[itemID] = true;
    printf("Item %d Received\n", itemID);
    if (itemID < 0) {
        printf("Received Unknown Item: %d\n", itemID);
    } else if (itemID < 80) { // Characters
        printf("Character Unlocked: %d\n", itemID);
        receiveQueue.push(itemID);
    } else if (itemID < 90) { // Suit
        printf("Suit Unlocked: %d\n", itemID);
        receiveQueue.push(itemID);
    } else if (itemID < 400) { // Minikits
        Settings::minikits++;
        printf("Number of Minikits: %d\n", Settings::minikits);
        LB1AP_CheckMinikitWinCon();
    } else if (itemID < 425) { // Hostages
        Settings::hostages++;
        printf("Number of Hostages: %d\n", Settings::hostages);
    } else if (itemID < 455) { // Level Unlock
        printf("Level Unlocked\n");
        receiveQueue.push(itemID);
    } else if (itemID < 485) { // True status
    } else if (itemID < 515) { // Red Brick
        printf("Red Brick Collected\n");
        receiveQueue.push(itemID);
    } else if (itemID < 550) { // Red Brick purchased
        printf("Red Brick Purchased\n");
        receiveQueue.push(itemID);
    } else if (itemID < 630) { // Tokens
        printf("Token Collected: %d\n", itemID);
        receiveQueue.push(itemID);
    } else { // Out of bounds
        printf("Received Unknown Item: %d\n", itemID);
    }
}

// Verify if a location has been checked
bool LB1AP_LocationChecked(int64_t location_id){
    return lb1AP_locations[location_id - LB1AP_LOCATION_ID_OFFSET];
}

// Resets everything
void LB1AP_Reset(){ 
    for(int i = 0; i < LB1AP_NUM_LOCS_AND_ITEMS; i++){
        lb1AP_locations[i] = false;
    }
    for(int i = 0; i < LB1AP_NUM_LOCS_AND_ITEMS; i++){
        lb1AP_items[i] = false;
    }
    Settings::minikits = 0;
    Settings::hostages = 0;
}

// Check if the minikit win condition has been met
void LB1AP_CheckMinikitWinCon(){
    if(Settings::minikits >= Settings::lb1_minikitsToWin && Settings::lb1_endGoal == 0){
        LB1AP_Complete();
    }
}

// Tells the server that the game has been completed
void LB1AP_Complete(){
    AP_StoryComplete();
    printf("You weren't just the hero this game deserved, you were the one it needed.\n");
}

void LB1AP_Connect(){
    // Get the last write time of the file
    namespace fs = std::filesystem; 
    fs::path filePath("APConnect.txt"); //WARNING: will likely crash if the file does not exist. To discuss adding a try catch?
    auto oldTime = fs::last_write_time(filePath);

    // Convert to system_clock time_point for standarization
    auto sctp = std::chrono::system_clock::now() + (oldTime - fs::file_time_type::clock::now());

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
            std::cout << serverIP << " " << playerName << " " << password << std::endl; //cout statement to catch user error
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
            if(LB1AP_GetConnectionStatus() == AP_ConnectionStatus::ConnectionRefused){
                printf("Connection Refused, please correct the connection file. \n");
            } else if(LB1AP_GetConnectionStatus() == AP_ConnectionStatus::Authenticated){
                connected = true;
                break;
            }
            Sleep(10000); // wait 10 seconds before trying again
            std::cout << "Waiting for connection about to loop" << std::endl;
        }
    }
}

// Used to read the connection file and return a c-string per library requirements
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

// Returns messages from the server
AP_Message* LB1AP_GetMessage(){
    if(LB1AP_GetConnectionStatus() == AP_ConnectionStatus::Disconnected) {return nullptr;}
    if(LB1AP_GetConnectionStatus() == AP_ConnectionStatus::Connected) {return nullptr;}
    if(LB1AP_GetConnectionStatus() == AP_ConnectionStatus::ConnectionRefused) {return nullptr;}
    if(!AP_IsMessagePending()) return nullptr;
    AP_Message* msg = AP_GetLatestMessage();
    AP_ClearLatestMessage();
    return msg;
}

// Sets the completion type based on slot data
void LB1AP_SetCompletionType(int type){
    Settings::lb1_endGoal = type;
    std::cout << "Completion type set to " << Settings::lb1_endGoal << std::endl;
}

// Sets the number of minikits required to win based on slot data
void LB1AP_SetMinikitsToWin(int num){
    if(num < 50 || num > 300){
        std::cout << "Could not read the number of minikits to win. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_minikitsToWin = 200; // Set to default
        return;
    }
    Settings::lb1_minikitsToWin = num;
    std::cout << "Minikits to win set to: " << Settings::lb1_minikitsToWin << std::endl;
}

// Sets the number of levels required to win based on slot data
void LB1AP_SetLevelsToWin(int num){
    if(num < 5 || num > 30){
        std::cout << "Could not read the number of levels to win. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_levelsToWin = 20; // Set to default
        return;
    }
    Settings::lb1_levelsToWin = num;
    std::cout << "Levels to win set to: " << Settings::lb1_levelsToWin << std::endl;
}

// Read if minikit sanity is enabled based on slot data
void LB1AP_SetMinikitSanity(int num){
    if(num < 0 || num > 1){
        std::cout << "Could not read the minikit sanity setting. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_minikitSanity = 1; // Set to default
        return;
    }
    Settings::lb1_minikitSanity = num;
    std::cout << "Minikit sanity set to: " << Settings::lb1_minikitSanity << std::endl;
}

// Read if true status sanity is enabled based on slot data
void LB1AP_SetTrueStatusSanity(int num){
    if(num < 0 || num > 1){
        std::cout << "Could not read the true status sanity setting. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_trueStatusSanity = 1; // Set to default
        return;
    }
    Settings::lb1_trueStatusSanity = num;
    std::cout << "True status sanity set to: " << Settings::lb1_trueStatusSanity << std::endl;
}

// Read if freeplay or story mode is unlocked based on slot data
void LB1AP_SetFreeplayOrStory(int num){
    if(num < 0 || num > 1){
        std::cout << "Could not read the Freeplay setting. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_freeplayUnlocked = 1; // Set to default
        return;
    }
    Settings::lb1_freeplayUnlocked = num;
    std::cout << "Freeplay setting set to: " << Settings::lb1_freeplayUnlocked << std::endl;
}

// Read Decoupled Tokens setting based on slot data
void LB1AP_SetDecoupledTokens(int num){
    if(num < 0 || num > 1){
        std::cout << "Could not read the Decoupled Tokens setting. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_decoupledTokens = 1; // Set to default
        return;
    }
    Settings::lb1_decoupledTokens = num;
    std::cout << "Decoupled Token setting set to: " << Settings::lb1_decoupledTokens<< std::endl;
}

// Read Decoupled Hush and Ras Tokens setting based on slot data
void LB1AP_SetHushAndRasDecoupledTokens(int num){
    if(num < 0 || num > 1){
        std::cout << "Could not read the Hush and Ras Decoupled Tokens setting. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_decoupleHushandRasTokens = 1; // Set to default
        return;
    }
    Settings::lb1_decoupleHushandRasTokens = num;
    std::cout << "Set Decoupled Hush and Ras Token setting set to: " << Settings::lb1_decoupleHushandRasTokens<< std::endl;
}

// Read Shuffle Hush and Ras setting based on slot data
void LB1AP_SetShuffleHushAndRas(int num){
    if(num < 0 || num > 1){
        std::cout << "Could not read the Shuffle Hush and Ras setting. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_shuffleHushandRas = 1; // Set to default
        return;
    }
    Settings::lb1_shuffleHushandRas = num;
    std::cout << "Shuffle Ras and Hush setting set to: " << Settings::lb1_shuffleHushandRas << std::endl;
}

// Read Hush Unlock Condition based on slot data
void LB1AP_SetHushUnlockCondition(int num){
    if(num < 0 || num > 25){
        std::cout << "Could not read the Hush Unlock Condition setting. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_hushUnlockCondition = 12; // Set to default
        return;
    }
    Settings::lb1_hushUnlockCondition = num;
    std::cout << "Hush Unlock Condition setting set to: " << Settings::lb1_hushUnlockCondition << std::endl;
}

// Read Ras Unlock Condition based on slot data
void LB1AP_SetRasUnlockCondition(int num){
    if(num < 0 || num > 300){
        std::cout << "Could not read the Ras Unlock Condition setting. Please report this to the devs. Setting to default" << std::endl;
        Settings::lb1_rasUnlockCondition = 150; // Set to default
        return;
    }
    Settings::lb1_rasUnlockCondition = num;
    std::cout << "Ras Unlock Condition setting set to: " << Settings::lb1_rasUnlockCondition << std::endl;
}

// Handles reply from the server. Implemented for level beaten win condition
void LB1AP_SetReplyHanlder(AP_SetReply reply){
    if(reply.key == AP_GetPrivateServerDataPrefix() + "CompleteLevelGoal"){
        switch(Settings::lb1_endGoal){
            case 0: //minikits
                break;
            case 1: //levels
                printf("Number of levels completed: %d\n", *(int*)reply.value);
                if(*(int*)reply.value >= Settings::lb1_levelsToWin) LB1AP_Complete();
                break;
        }
    }
    return;
}

// Tell the server that a level has been completed so it is stored inbetween sessions.
void LB1AP_LevelComplete(){
    AP_SetServerDataRequest req;
    req.key = AP_GetPrivateServerDataPrefix() + "CompleteLevelGoal";
    int def_val = 0;
    req.default_value = &def_val;
    req.type = AP_DataType::Int;
    req.want_reply = true;
    int flag = 1;
    req.operations = std::vector<AP_DataStorageOperation>{{{"add", &flag}}};
    AP_SetServerData(&req);
}

// Check if Connected to Servers
AP_ConnectionStatus LB1AP_GetConnectionStatus(){
    return AP_GetConnectionStatus();
}