This is instructions for the alpha test of Lego Batman: The Videogame for Archipelago multiworld randomizer made by ZAPaDASH04 and Jr (jr5768).
With assistance from:
- Snolid Ice
- Spencenox

Special shout out to N00byKing for developing the client library to connect to the Archipelago Server and the other SM64 Devs for being an good example of its implementation

This alpha test is essentially a kits only test. A kit hunt if you will. All minikits are in the item pool. When you run the program it will run some setup for the sake of testing such as unlocking all levels, characters, suits, and red bricks to make the hunt faster.
## Important Notes
- You must use unmodified steam version of LEGO Batman: The Videogame
- You will be using a 3rd party tool, a mod loader called Reloaded II, to run. Installation instructions are included in the instructions below.
- Make sure to pay attention to which locations are sent to verify it sends the correct location. (check that kit locations aren't being missed or swapped)
- Debug messages are logged in a.txt (ours) and b.txt (ap's) both of which will be located in the LEGOBatman.exe folder.

### Instructions

#### Onetime Setup

- Install [Reloaded II](https://reloaded-project.github.io/Reloaded-II/QuickStart)
- Add lego batman either through setup or by clicking the + on the left. Must use the exe found in the legobatman folder of steam (LEGOBatman.exe).
- If you don't already have the mod "Reloaded II Server", click the Download Mods button in the top left and search for "Reloaded II Server" made by Sewer56 and download it.
- Navigate to the "Mods" folder in wherever you installed Reloaded II (usually desktop .\Reloaded-II\Mods)
- Move the 2 folders "legobatman1.apconnectsupport" and "legobatman1.apsupport" into the Reloaded II Mods folder.
- Reloaded should be setup now.
- Lastly move APConnect.txt from this folder to be in the same folder as LEGOBatman.exe (Steam folder, not Reloaded folder).
    - You can easily access this folder by right clicking the game in Steam, "Manage", "Browse local files"
  
#### Setup AP game
- Set up an AP game using the lego_batman_1.apworld (Alpha test is not intended for multiple players or serious games as currently there are no spheres or logic. You can play multiple games if you wish.)
- If you aren't familiar with archipelago, please use the command &apworld and/or &template in the archipelago discord server.
- Open the APConnect.txt file (should be in the same folder as LEGOBatman.exe) and add the AP info. follow instructions on the first line.
- Run lego batman (tested methods: through steam, directly through the LEGOBatman.exe, or using DXWND).
- Make a new game and **EXIT THE LEVEL**. If you already have an AP game then just load the save.
- Once in the hub go to Reloaded II and click lego batman on the left side.
- In "Configure Mods" enable only "Reloaded II Server" and "LB1 Archipelago" (Enabled mods have a red plus and disabled mods, if any, have a grey minus). LB1 Client Connector should be a filled red box.
- In the bottom left, under Processes there should be process to select.
- There should now be an inject button click it and switch back to the game.
- Check that a message appeared in the hint box indicating that the program connected.
- Check the AP game webpage to see that you have connected.
- Get those kits!


