# Lego Batman: The Videogame for Archipelago
This is instructions for the alpha test of Lego Batman: The Videogame for Archipelago multiworld randomizer made by ZAPaDASH04 and Jr (jr5768).
With assistance from:
- Snolid Ice
- Spencenox

Special thanks to our dedicated play testers:
- WhirlingWinds - first to goal v0.1.
- and many more...

Special shout out to N00byKing for developing the APCpp client library to connect to the Archipelago Server and the other SM64 Devs for being a good example of its implementation.

## Important Notes
- You must use unmodified steam version of [LEGO Batman: The Videogame](https://store.steampowered.com/app/21000/LEGO_Batman_The_Videogame).
- You will be using a 3rd party tool, a mod loader called Reloaded II, to run. Installation instructions are included in the instructions below.
- Debug messages are logged in a.txt (ours) and b.txt (ap's) both of which will be located in the LEGOBatman.exe folder.
- This mod is in Alpha. If you are planning on playing it with other people, please be aware that it can break or not work properly.
- Make sure to pay attention to which locations are sent to verify that it sends the correct location (i.e. that locations aren't skipped or swapped). 

## Instructions

#### Onetime Setup

- Install [Reloaded II](https://reloaded-project.github.io/Reloaded-II/QuickStart)
- Add lego batman either through setup or by clicking the + on the left. Must use the exe found in the steam legobatman folder (LEGOBatman.exe).
- If you don't already have the mod "Reloaded II Server", click the Download Mods button in the top left and search for "Reloaded II Server" made by Sewer56 and download it.
- Navigate to the "Mods" folder in wherever you installed Reloaded II (usually desktop .\Reloaded-II\Mods)
- Move the 2 folders "legobatman1.apconnectsupport" and "legobatman1.apsupport" into the Reloaded II Mods folder.
- Reloaded should be setup now.
- Lastly move APConnect.txt from this folder to be in the Steam folder that has the LEGOBatman.exe (Steam folder, NOT Reloaded folder).
    - You can easily access this folder by right clicking the game in Steam, "Manage", then "Browse local files".
  
#### Setup AP game
- Set up an AP game using the lego_batman_1.apworld
    - If you aren't familiar with archipelago, please use the command &apworld and/or &template in the archipelago discord server.
- Open the APConnect.txt file (should be in the same folder as LEGOBatman.exe) and add the AP info. Follow instructions on the first line.
- In "Configure Mods" of Reloaded II, enable only "Reloaded II Server" (Enabled mods have a red plus and disabled mods have a grey minus). 
    - LB1 Client Connector should be a filled red box.
- With Lego Batman selected in Reloaded II, click "Launch Application".
    - If you would like to play windowed, have DXWND open at the same time as clicking "Launch Application". (Make sure that Lego Batman is setup in DXWND for it to work)
- Make a new game and **EXIT THE LEVEL**. If you already have an AP game then just load the save.
- Once in the hub go to Lego Batman in Reloaded II. In the bottom left, under Processes there should be process to select.
- Beneath the list of mods running, click the button called "Load Mod".
- Select "LB1 Archipelago" and click "Load Mod". 
- Switch back to Lego Batman and check that a message appeared in the hint box indicating that the program connected.
- Check the AP game webpage to see that you have connected.


