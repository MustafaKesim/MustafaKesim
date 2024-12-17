    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    // Yapılar
    typedef struct {
        int health;
        int strength;
        char inventory[10][50];
        int inventory_count;
    } Player;

    typedef struct {
        char name[50];
        int health;
        int attack_power;
        char reward[50]; // Canavarı yendikten sonra alınacak ödül
    } Monster;

    typedef struct {
        char name[50];
        int attack_power; // Silah saldırı gücü
        int heal_amount;  // Sağlık eşyası iyileştirme gücü
    } Item;

    typedef struct {
        char description[256];
        char items[10][50];
        int item_count;
        Monster *monster;
        int puzzle_number; // Bulmaca numarası (rastgelelik için)
        char puzzle_hint[256];
        int puzzle_solved; // Bulmacanın çözülüp çözülmediği
    } Room;

    // Canavar tanımları
    Monster monsters[3] = {
        {"Goblin", 30, 5, "Bronze Key"},   // Room 1 canavarı
        {"Orc", 50, 10, "Silver Key"},     // Room 2 canavarı
        {"Dragon", 100, 20, "Gold Key"}    // Room 4 canavarı
    };

    Item items[] = {
        {"Knife", 10, 0}, 
        {"Axe", 20, 0}, 
        {"Katana", 30, 0}, 
        {"Painkiller", 0, 10}, 
        {"Bandage", 0, 20}, 
        {"First Aid Kit", 0, 30},
        {"Red Potion", 0, 0}, 
        {"Blue Potion", 0, 0}, 
        {"Purple Potion", 0, 0}
    };


    // Fonksiyon prototipleri
    void process_command(const char *command, Player *player, Room *rooms, int *current_room_index);
    void show_inventory(Player *player);
    void fight_monster(Player *player, Monster *monster);
    int solve_puzzle(int puzzle_number, const char *hint);
    void mix_potion(const char *color1, const char *color2, Player *player);
    void save_game(Player *player, Room *rooms, int current_room_index);
    void load_game(Player *player, Room *rooms, int *current_room_index);

    // Ana oyun döngüsü
    int main() {
        Player player = {100, 5, 1}; // Başlangıç sağlığı 100, saldırı gücü 5 

    Room rooms[5] = {
        {"You are in Room 1. There is a paper with a puzzle on it: '5 * Y = 15. What is Y? Solve it to proceed.'",
        {"Bandage", "Knife", "Puzzle Paper"}, 3, &monsters[0], 1, "5 * Y = 15. What is Y?", 0},
        {"You are in Room 2.",
        {"Axe", "Painkiller"}, 2, &monsters[1], 0, "", 1},
        {"You are in Room 3. There are potions here. There is also a recipe for potion mixing: 'Red + Blue = Purple'.",
        {"Red Potion", "Blue Potion", "Yellow Potion", "Potion Recipe"}, 4, NULL, 0, "", 1},
        {"You are in Room 4.",
        {"Katana"}, 1, &monsters[2], 0, "", 1},
        {"You are in Room 5. A treasure chest awaits!",
        {"Chest"}, 1, NULL, 0, "", 1}
    };


        int current_room_index = 0;
        char command[256];

        printf("Welcome to the adventure game! Your goal is to defeat monsters, solve puzzles, and find the treasure.\n");
        printf("Commands:help, look, inventory, pickup <index>, drop <item>, use <item>, fight, mix <color1> <color2>, move <room>, save, load, exit\n");

        while (1) {
            printf("\nYou are in Room %d. Command: ", current_room_index + 1);
            fgets(command, 256, stdin);
            command[strcspn(command, "\n")] = 0; // Yeni satır karakterini kaldır

            if (strcmp(command, "exit") == 0) {
                printf("Exiting the game. Goodbye!\n");
                break;
            }

            process_command(command, &player, rooms, &current_room_index);
        }

        return 0;
    }

    int solve_puzzle(int puzzle_number, const char *puzzle_hint) {
        int user_answer;
        printf("Puzzle hint: %s\n", puzzle_hint);
        printf("Enter your answer: ");
        scanf("%d", &user_answer);
        getchar(); // Yeni satır karakterini temizle

        int expected_answer = 0;

        if (puzzle_number == 1) {
            expected_answer = 3; // Örnek bulmaca: 5 * Y = 15, Y = 3
        }

        if (user_answer == expected_answer) {
            printf("Correct! You solved the puzzle.\n");
            return 1;
        } else {
            printf("Wrong answer. Try again.\n");
            return 0;
        }
    }
    void show_help() {
        printf("\n=== Available Commands ===\n");
        printf("look          : Describe the room and list items.\n");
        printf("inventory     : Show your current inventory.\n");
        printf("pickup <index>: Pick up an item from the room.\n");
        printf("drop <item>   : Drop an item from your inventory.\n");
        printf("use <item>    : Use or equip an item.\n");
        printf("fight         : Fight the monster in the room.\n");
        printf("mix <color1> <color2>: Mix two colors to create a potion.\n");
        printf("move <room>   : Move to another room.\n");
        printf("save          : Save your game progress.\n");
        printf("load          : Load a saved game.\n");
        printf("exit          : Quit the game.\n");
        printf("==========================\n\n");
    }
   void save_game(Player *player, Room *rooms, int current_room_index) {
    FILE *file = fopen("savegame.dat", "wb");
    if (file) {
        fwrite(player, sizeof(Player), 1, file);

        for (int i = 0; i < 5; i++) {
            fwrite(&rooms[i], sizeof(Room) - sizeof(Monster *), 1, file); 
            if (rooms[i].monster != NULL) {
                int monster_index = rooms[i].monster - monsters; 
                fwrite(&monster_index, sizeof(int), 1, file);
            } else {
                int monster_index = -1; // Canavar yoksa -1 yaz
                fwrite(&monster_index, sizeof(int), 1, file);
            }
        }

        fwrite(&current_room_index, sizeof(int), 1, file);
        fclose(file);
        printf("Game saved successfully!\n");
    } else {
        printf("Error saving the game.\n");
    }
}

void load_game(Player *player, Room *rooms, int *current_room_index) {
    FILE *file = fopen("savegame.dat", "rb");
    if (file) {
        // Oyuncu bilgileri
        fread(player, sizeof(Player), 1, file);

        // 5 odanın her birinin verileri
        for (int i = 0; i < 5; i++) {
            fread(&rooms[i], sizeof(Room) - sizeof(Monster *), 1, file);

            // Canavar indeksi ve canavarı odada ayarlama kısmı
            int monster_index;
            fread(&monster_index, sizeof(int), 1, file);
            if (monster_index != -1) {
                rooms[i].monster = &monsters[monster_index];
            } else {
                rooms[i].monster = NULL;
            }
        }

        // Oyuncunun bulunduğu odanın indeksi
        fread(current_room_index, sizeof(int), 1, file);

        // Oyun başarıyla yüklendi, dosyayı kapat
        fclose(file);

        // Oyun başladığında doğru odada olunduğundan emin ol
        printf("Game loaded successfully! You are in Room %d.\n", *current_room_index + 1);
    } else {
        // Eğer dosya açılamazsa, hata mesajı 
        printf("Error loading the game.\n");
    }
}





    // Commands

    void process_command(const char *command, Player *player, Room *rooms, int *current_room_index) {
      if (strcmp(command, "look") == 0) {
    printf("Room description: %s\n", rooms[*current_room_index].description);

    // Eğer odada bir bulmaca varsa ve çözülmemişse ipucunu gösterir
    if (*current_room_index == 0 && rooms[*current_room_index].puzzle_number > 0 && !rooms[*current_room_index].puzzle_solved) {
        printf("Puzzle hint: %s\n", rooms[*current_room_index].puzzle_hint);
    }

    // Eğer odada bir canavar varsa bilgilerini gösterir
    if (rooms[*current_room_index].monster != NULL) { // Monster kontrolü
        if (rooms[*current_room_index].monster->health > 0) {
            printf("There is a %s here!\n", rooms[*current_room_index].monster->name);
            printf("Monster's health: %d\n", rooms[*current_room_index].monster->health);
            printf("Monster's attack power: %d\n", rooms[*current_room_index].monster->attack_power);
        } else {
            printf("The %s has been defeated.\n", rooms[*current_room_index].monster->name);
        }
    } else {
        printf("There are no monsters here.\n");
    }

    // Odadaki eşyaları listeler
    printf("Items in the room:\n");
    if (rooms[*current_room_index].item_count > 0) { // Eğer item varsa
        for (int i = 0; i < rooms[*current_room_index].item_count; i++) {
            printf("%d. %s\n", i + 1, rooms[*current_room_index].items[i]);
        }
    } else {
        printf("No items in this room.\n");
    }

    // Oyuncuya mevcut komutları hatırlatır
    printf("\nCommands:help, look, inventory, pickup <index>, drop <item>, use <item>, fight, mix <color1> <color2>, move <room>, save, load, exit\n");
}


    else if (strcmp(command, "help") == 0) {
            show_help(); 
        }
        if (strcmp(command, "save") == 0) {
            save_game(player, rooms, *current_room_index);
        } else if (strcmp(command, "load") == 0) {
            load_game(player, rooms, current_room_index);
        }
        
    
        else if (strncmp(command, "use", 3) == 0) {
        char item[50];
        sscanf(command + 4, "%[^\n]s", item);

        int found = 0;
        for (int i = 0; i < player->inventory_count; i++) {
            if (strcmp(player->inventory[i], item) == 0) {
                found = 1;

                // Eşyanın etkilerini bulma
                for (int j = 0; j < sizeof(items) / sizeof(items[0]); j++) {
                    if (strcmp(items[j].name, item) == 0) {
                        if (items[j].heal_amount > 0) { // Sağlık eşyası ise
                            player->health += items[j].heal_amount;
                            if (player->health > 100) player->health = 100; // Maksimum sağlık
                            printf("You used %s. Your health is now %d.\n", item, player->health);
                        }
                        if (items[j].attack_power > 0) { // Silah ise saldırı gücünü artır
                            player->strength = items[j].attack_power; // Yeni saldırı gücü
                            printf("You equipped %s. Your attack power is now %d.\n", item, player->strength);
                        }
                        break;
                    }
                }

                // Eşyayı envanterden çıkartmak
                for (int k = i; k < player->inventory_count - 1; k++) {
                    strcpy(player->inventory[k], player->inventory[k + 1]);
                }
                player->inventory_count--;
                printf("%s has been removed from your inventory.\n", item);
                return;
            }
        }

        if (!found) {
            printf("You don't have %s in your inventory.\n", item);
        }
    }


    else if (strncmp(command, "drop", 4) == 0) {
        char item[50];
        sscanf(command + 5, "%[^\n]s", item);

        int found = 0;
        for (int i = 0; i < player->inventory_count; i++) {
            if (strcmp(player->inventory[i], item) == 0) {
                found = 1;

                // Odaya eşya eklemek
                if (rooms[*current_room_index].item_count < 10) {
                    strcpy(rooms[*current_room_index].items[rooms[*current_room_index].item_count], item);
                    rooms[*current_room_index].item_count++;
                } else {
                    printf("The room is full. Cannot drop the item.\n");
                    return;
                }

                // Envanterden eşya çıkarmak
                for (int j = i; j < player->inventory_count - 1; j++) {
                    strcpy(player->inventory[j], player->inventory[j + 1]);
                }
                player->inventory_count--;

                printf("You dropped %s.\n", item);
                return;
            }
        }
        if (!found) {
            printf("You don't have %s in your inventory.\n", item);
        }
    }

        
        else if (strcmp(command, "inventory") == 0) {
            show_inventory(player);
        } else if (strcmp(command, "fight") == 0) {
            if (rooms[*current_room_index].monster != NULL) {
                fight_monster(player, rooms[*current_room_index].monster);
                rooms[*current_room_index].monster = NULL; // Canavar yenildi
            } else {
                printf("There is no monster in this room to fight.\n");
            }
        } else if (strncmp(command, "move", 4) == 0) {
        int target_room = atoi(command + 5) - 1;

        // 1. odadan 2. odaya geçiş için bulmaca çözme kontrolü
        if (*current_room_index == 0 && target_room == 1) {
            if (!rooms[*current_room_index].puzzle_solved) {
                int solved = solve_puzzle(rooms[*current_room_index].puzzle_number, rooms[*current_room_index].puzzle_hint);
                if (!solved) {
                    printf("You need to solve the puzzle in this room before moving to the next room.\n");
                    return;
                } else {
                    rooms[*current_room_index].puzzle_solved = 1;
                }
            }
        }

        // 3. odadan 4. odaya geçiş için Purple Potion kontrolü
        if (*current_room_index == 2 && target_room == 3) {
            int has_purple_potion = 0;
            for (int i = 0; i < player->inventory_count; i++) {
                if (strcmp(player->inventory[i], "Purple Potion") == 0) {
                    has_purple_potion = 1;
                    break;
                }
            }
            if (!has_purple_potion) {
                printf("The door to Room 4 is locked. You need a Purple Potion to proceed.\n");
                return;
            }
        }

        // Room 5'te sandık açma kontrolü
        if (target_room == 4) {
            int has_bronze_key = 0, has_silver_key = 0, has_gold_key = 0;

            for (int i = 0; i < player->inventory_count; i++) {
                if (strcmp(player->inventory[i], "Bronze Key") == 0) has_bronze_key = 1;
                if (strcmp(player->inventory[i], "Silver Key") == 0) has_silver_key = 1;
                if (strcmp(player->inventory[i], "Gold Key") == 0) has_gold_key = 1;
            }

            if (!has_bronze_key || !has_silver_key || !has_gold_key) {
                printf("The chest is locked. You need all three keys (Bronze Key, Silver Key, Gold Key) to open it.\n");
                return;
            } else {
                printf("Congratulations! You opened the chest and won the game!\n");
                exit(0); // Oyunu başarıyla sonlandırıyoruz
            }
        }

        // Odaya geçiş
        if (target_room >= 0 && target_room < 5) {
            *current_room_index = target_room;
            printf("You moved to Room %d.\n", *current_room_index + 1);
        } else {
            printf("Invalid room number.\n");
        }
    }




        else if (strncmp(command, "pickup", 6) == 0) {
        int item_index = atoi(command + 7) - 1;
        if (item_index >= 0 && item_index < rooms[*current_room_index].item_count) {
            if (player->inventory_count < 10) {
                // Eşyayı envantere ekleme
                strcpy(player->inventory[player->inventory_count], rooms[*current_room_index].items[item_index]);
                player->inventory_count++;
                printf("Picked up: %s\n", rooms[*current_room_index].items[item_index]);

                // Eşyayı odadan kaldırma
                for (int i = item_index; i < rooms[*current_room_index].item_count - 1; i++) {
                    strcpy(rooms[*current_room_index].items[i], rooms[*current_room_index].items[i + 1]);
                }
                rooms[*current_room_index].item_count--; // Oda eşyası sayısını azalt
            } else {
                printf("Inventory full! Cannot pick up more items.\n");
            }
        } else {
            printf("Invalid item index.\n");
        }
    }else if (strncmp(command, "mix", 3) == 0) {
        char color1[20], color2[20];
        sscanf(command + 4, "%s %s", color1, color2);

        if ((strcmp(color1, "Red") == 0 && strcmp(color2, "Blue") == 0) || 
            (strcmp(color1, "Blue") == 0 && strcmp(color2, "Red") == 0)) {
            printf("You created a Purple Potion!\n");

            // Envanterden Red ve Blue Potion'ı sil
            int red_index = -1, blue_index = -1;
            for (int i = 0; i < player->inventory_count; i++) {
                if (strcmp(player->inventory[i], "Red Potion") == 0 && red_index == -1) {
                    red_index = i;
                } else if (strcmp(player->inventory[i], "Blue Potion") == 0 && blue_index == -1) {
                    blue_index = i;
                }
            }

            if (red_index != -1 && blue_index != -1) {
                // Red Potion'ı sil
                for (int i = red_index; i < player->inventory_count - 1; i++) {
                    strcpy(player->inventory[i], player->inventory[i + 1]);
                }
                player->inventory_count--;

                // Blue Potion'ı sil
                if (blue_index > red_index) blue_index--; // Red silindikten sonra index kayıyor
                for (int i = blue_index; i < player->inventory_count - 1; i++) {
                    strcpy(player->inventory[i], player->inventory[i + 1]);
                }
                player->inventory_count--;

                // Purple Potion ekle
                strcpy(player->inventory[player->inventory_count], "Purple Potion");
                player->inventory_count++;
            } else {
                printf("You don't have the required potions to mix.\n");
            }
        } else {
            printf("Invalid potion combination.\n");
        }
    }



    }




    void show_inventory(Player *player) {
        printf("Your inventory:\n");
        for (int i = 0; i < player->inventory_count; i++) {
            printf("%d. %s\n", i + 1, player->inventory[i]);
        }
        if (player->inventory_count == 0) {
            printf("Your inventory is empty.\n");
        }
    }



    void fight_monster(Player *player, Monster *monster) {
        printf("Fighting %s...\n", monster->name);
        while (player->health > 0 && monster->health > 0) {
            monster->health -= player->strength;

            // Canavarı yendikten sonra canını sıfır olarak sınırla
            if (monster->health < 0) {
                monster->health = 0;
            }

            printf("You hit the %s! Monster's health: %d\n", monster->name, monster->health);

            if (monster->health > 0) {
                player->health -= monster->attack_power;

                // Oyuncunun canı sıfırın altına düşerse
                if (player->health < 0) {
                    player->health = 0;
                }

                printf("The %s hit you! Your health: %d\n", monster->name, player->health);
            }
        }

        if (player->health <= 0) {
            printf("You have been defeated. Game Over.\n");
            exit(0);
        } else {
            printf("You defeated the %s!\n", monster->name);
            if (monster->reward[0] != '\0') {
                printf("You found a reward: %s\n", monster->reward);
                if (player->inventory_count < 10) {
                    strcpy(player->inventory[player->inventory_count], monster->reward);
                    player->inventory_count++;
                } else {
                    printf("Your inventory is full. You cannot take the reward.\n");
                }
            }
        }
    }

