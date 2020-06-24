This is a project made for a first year university C++ module - a roguelike with randomly generated dungeons using binary space partitioning. Corridors are made using A* to find paths between rooms, and the entire program is written as an Entity-Component-System. 
 

______   _______ __________________ _______  _______  _        _______  _______  _______ 
(  ___ \ (  ___  )\__   __/\__   __/(  ___  )(       )( \      (  ____ \(  ____ \(  ____ \
| (   ) )| (   ) |   ) (      ) (   | (   ) || () () || (      | (    \/| (    \/| (    \/
| (__/ / | |   | |   | |      | |   | |   | || || || || |      | (__    | (_____ | (_____ 
|  __ (  | |   | |   | |      | |   | |   | || |(_)| || |      |  __)   (_____  )(_____  )
| (  \ \ | |   | |   | |      | |   | |   | || |   | || |      | (            ) |      ) |
| )___) )| (___) |   | |      | |   | (___) || )   ( || (____/\| (____/\/\____) |/\____) |
|______/ (_______)   )_(      )_(   (_______)|/     \|(_______/(_______/\_______)\_______)
                                                                                          
                        _______  _______           _______ ________                         
                      (  ____ \(  ____ )|\     /|(  ____ )\__   __/                         
                      | (    \/| (    )|( \   / )| (    )|   ) (                            
                      | |      | (____)| \ (_) / | (____)|   | |                            
                      | |      |     __)  \   /  |  _____)   | |                            
                      | |      | (\ (      ) (   | (         | |                            
                      | (____/\| ) \ \__   | |   | )         | |                            
                      (_______/|/   \__/   \_/   |/          )_(                            
                                                                           
Instructions:

	- To launch the game run Debug/TOGA-Final.exe

Bottomless Crypt is a Roguelike featuring randomly generated dungeons. Each dungeon contains spikes,
chests and monsters. Run into an object to interact with it: spikes will hurt you, chests will grant
you a random piece of loot and monsters will enter a combat encounter. Reach the dungeon's exit to
descend to the next level - complete 10 levels to win!

Controls:
	- WASD  : move the player
	- I	: open/close inventory
	- W/S	: navigate inventory
	- E	: equip/consume item (when in inventory)
	- X	: drop item (when in inventory)
	- R	: restart game
	- SPACE : descend when standing on dungeon exit

Items:
	- Chests contain a random item and also the chance of dropping an amount of gold
	- Lower level dungeons will have better items
	- Use a weapon to equip it, and use a potion/food to consume it to regain health
	- You can hold up to eight items in your inventory, drop unwanted items to free up space
	- Your final score is your gold plus the total value of each of the items in your inventory

Monsters:
	- When you move onto a tile containing a monster you will attack it
	- Your damage is 1 + the effectiveness of your equipped weapon
	- When you take your turn my moving each monster will also take their turn
	- If a monster is close enough it will start following you for up to 3 turns
	- If it moves onto the tile you are currently occupying it will attack you!
	- Strong monsters will spawn in lower dungeons.


Enjoy and thank you for playing!


