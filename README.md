# Adventure Game

## **Overview**
The Adventure Game is a text-based dungeon exploration game built in C. The player navigates through rooms, solves puzzles, fights monsters, and manages inventory to ultimately unlock the treasure chest in the final room. The game combines various gameplay mechanics such as combat, puzzle-solving, and item management, offering a dynamic and interactive experience.

---

## **How the Game Works**

### **1. Objectives**
- Defeat all monsters in the dungeon.
- Solve puzzles to progress through locked doors.
- Collect items and manage inventory effectively.
- Use the three keys (Bronze, Silver, Gold) to open the treasure chest in Room 5.

### **2. Rooms**
The game consists of five unique rooms:
- **Room 1**:
  - Contains a puzzle: `5 * Y = 15`. Solve it to access Room 2.
  - Features a weak monster (Goblin) with 30 health and 5 attack power.
  - Rewards the player with the **Bronze Key** after defeating the monster.
- **Room 2**:
  - A medium-strength monster (Orc) awaits, with 50 health and 10 attack power.
  - Rewards the player with the **Silver Key** upon victory.
- **Room 3**:
  - Contains various potions and a recipe for potion mixing: `Red + Blue = Purple`.
  - Players must mix potions to unlock Room 4.
- **Room 4**:
  - A strong monster (Dragon) with 100 health and 20 attack power.
  - Rewards the player with the **Gold Key** upon victory.
- **Room 5**:
  - Contains the treasure chest. The player must have all three keys to open it and win the game.

---

### **3. Gameplay Mechanics**

#### **Commands**
- **Room Interaction**:
  - `look`: Describes the current room, listing available items and any monsters.
  - `pickup <index>`: Picks up an item from the room based on its index.
  - `drop <item>`: Drops an item from the inventory into the room.
- **Combat**:
  - `fight`: Initiates a battle with the monster in the room (if present).
- **Inventory Management**:
  - `inventory`: Displays the player's inventory.
  - `use <item>`: Uses an item, such as healing items or weapons.
- **Potion Mixing**:
  - `mix <color1> <color2>`: Combines two potions to create a new one.
- **Movement**:
  - `move <room>`: Moves to a specific room by its number (1–5).
- **Game State**:
  - `save`: Saves the current game state.
  - `load`: Loads a previously saved game.
  - `exit`: Exits the game.

---

#### **Combat System**
- Combat is turn-based.
- The player attacks first, reducing the monster's health by the player's strength.
- If the monster survives, it attacks the player, reducing the player's health by its attack power.
- **Victory**:
  - The player receives rewards like keys or items upon defeating the monster.
- **Defeat**:
  - If the player's health drops to 0, the game ends.

#### **Puzzle Solving**
- Each puzzle must be solved to unlock specific doors:
  - **Room 1 Puzzle**: `5 * Y = 15`. Answer: `Y = 3`.
  - **Room 3 Puzzle**: Mix `Red Potion` and `Blue Potion` to create `Purple Potion`.

#### **Inventory System**
- Players can carry a maximum of 10 items.
- **Healing items** restore health:
  - **Bandage**: Restores 20 health.
  - **First Aid Kit**: Restores 30 health.
- **Weapons** increase attack power:
  - **Knife**: +10 attack power.
  - **Katana**: +30 attack power.
- **Keys** are essential for unlocking the treasure chest in Room 5.

---

### **4. How to Play**

#### **Setup**
1. Clone or download the project repository.
2. Compile the code using the included Makefile:
   ```bash
   make
   ```
3. Run the game:
   ```bash
   make run
   ```

#### **Example Commands**
- `look`
- `pickup 1`
- `use Knife`
- `fight`
- `mix Red Blue`
- `move 3`
- `save`
- `load`

#### **Cleaning Up**
To remove compiled files:
```bash
make clean
```

---

## **Project Structure**
- `main.c`: The source code for the game.
- `Makefile`: Automates the compilation process.
- `savegame`: File used to store saved game data.

---

## **Credits**
- Developed by [Mustafa Kesim].

Enjoy your adventure!




