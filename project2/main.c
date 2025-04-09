#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "monopoly.h"

#define STARTING_CASH_2_PLAYERS 20
#define STARTING_CASH_3_PLAYERS 18
#define STARTING_CASH_4_PLAYERS 16

int is_bankrupt[MAX_PLAYERS] = {0};
int jail_fine_pending[MAX_PLAYERS] = {0};
int bankrot=0;

PLAYER players[MAX_PLAYERS];


void print_player(PLAYER *player) {
    printf("%d. S: %d, C: %d, JP: %d, IJ: %s\n",
           player->number, player->space_number, player->cash, player->num_jail_pass,
           player->is_in_jail ? "yes" : "no");

    for (int i = 0; i < player->num_properties; i++) {
        PROPERTY *current_property = player->owned_properties[i];
        // Поиск недвижимости на игровом поле и вывод номера поля
        for (int j = 0; j < NUM_SPACES; j++) {
            if (game_board[j].type == Property && game_board[j].property == current_property) {
                printf("  %-20s\t%-2d\t%-7s  \tS%-2d\n",player->owned_properties[i]->name, current_property->price, property_colors[current_property->color], j+1);
                break; // Выходим из цикла после нахождения недвижимости
            }
        }
    }

}

// Функция для проверки, является ли игрок монополистом по цвету
int is_monopoly_owner(int player_index, int color) {
    int count = 0;
    for (int i = 0; i < players[player_index].num_properties; i++) {
        if (players[player_index].owned_properties[i]->color == color) {
            count++;
        }
    }
    return count >= 2; // Возвращает true, если игрок владеет двумя недвижимостями одного цвета
}

void print_game_state() {
    printf("Game board:\n");
    for (int i = 0; i < NUM_SPACES; i++) {
        printf("%-1d.  ", i + 1); // Выводим номер поля
        if (i + 1 < 10) {
            printf(" ");
        }
        if (game_board[i].type == Property && game_board[i].property != NULL) {
            int is_owned = 0;
            int owner_index = 0;
            // Проверяем, принадлежит ли недвижимость какому-либо игроку
            for (int j = 0; j < MAX_PLAYERS; j++) {
                for (int k = 0; k < players[j].num_properties; k++) {
                    if (players[j].owned_properties[k] == game_board[i].property) {
                        is_owned = 1;
                        owner_index = j + 1;
                        break;
                    }
                }
                if (is_owned) {
                    // Определяем, является ли игрок монополистом по цвету
                    int monopoly = is_monopoly_owner(owner_index - 1, game_board[i].property->color);
                    printf("%-20s  %-2d  %-7s  P%-2d  %s\n", game_board[i].property->name, game_board[i].property->price,
                           property_colors[game_board[i].property->color], owner_index, monopoly ? "yes" : "no");
                    break;
                }
            }
            if (!is_owned) {
                printf("%-20s  %-2d  %s\n", game_board[i].property->name, game_board[i].property->price,
                       property_colors[game_board[i].property->color]);
            }
        } else {
            printf("%-20s\n", space_types[game_board[i].type]);
        }
    }
}


int main(int argc, char *argv[]) {

    int s_rezim=0;
    int p_rezim=0;
    int g_rezim=0;
    int first_time=0;
    int num_players=2;

    for(int i=0; i<argc;i++){
        if (strcmp(argv[i], "-n") == 0 && i+1<argc) {
            sscanf(argv[i+1], "%d", &num_players);
        }
        if(strcmp(argv[i], "-s")==0){
            s_rezim=1;
        }
        if(strcmp(argv[i], "-p")==0){
            p_rezim=1;
        }
        if(strcmp(argv[i], "-g")==0){
          g_rezim=1;
        }
    }


    switch (num_players) {
        case 2:
            for (int i = 0; i < num_players; i++) {
                players[i].number = i + 1;
                players[i].cash = STARTING_CASH_2_PLAYERS;
            }
            break;
        case 3:
            for (int i = 0; i < num_players; i++) {
                players[i].number = i + 1;
                players[i].cash = STARTING_CASH_3_PLAYERS;
            }
            break;
        case 4:
            for (int i = 0; i < num_players; i++) {
                players[i].number = i + 1;
                players[i].cash = STARTING_CASH_4_PLAYERS;
            }
            break;
    }


    for (int i = 0; i < num_players; i++) {
        players[i].space_number = 1; // Všetci hráči začínajú na políčku 1
    }

    if(g_rezim!=1){
        printf("Players:\n");
        for (int i = 0; i < num_players; i++) {
            print_player(&players[i]);
        }
    }
    if(s_rezim!=1 && g_rezim!=1){
        print_game_state();
        printf("WINNER: -\n");
    }

    // Hadenie kockou
    int num_rolls;
    int turn=0;
    int players_turn=0;
    while (1) {
        int result = scanf("%d", &num_rolls);
        int current_player_index = players_turn; // Index aktuálneho hráča v poľove hráčov
        int current_space = players[current_player_index].space_number; // Číslo aktuálneho poľa hráča
        int new_space = (current_space + num_rolls - 1) % NUM_SPACES + 1; // Nové pole po ťahu hráča

        if (result == 0) {
            break;
        }

        if(s_rezim==1){
            print_game_state();
            if(!first_time){
                printf("WINNER: -\n");
                first_time=1;
            }
        }
        if(g_rezim==1){
            printf("Players:\n");
            for(int i=0; i<num_players; i++){
                print_player(&players[i]);
            }
            print_game_state();
            printf("WINNER: -\n");
        }
        printf("\n");
        turn++;
        printf("R: %d\n", num_rolls);
        printf("Turn: %d\n", turn);
        printf("Player on turn: P%d\n", players_turn + 1);
        printf("\n");


        if (game_board[new_space - 1].type == Jail_pass) {
            players[current_player_index].num_jail_pass += 1;
        }
        if (game_board[new_space - 1].type == Go_to_jail) {
            if(players[current_player_index].num_jail_pass>=1){
                players[current_player_index].num_jail_pass-=1;
            } else{
                players[current_player_index].space_number = 7; // Hráč sa presunie do väzenia
                players[current_player_index].is_in_jail = 1; // Hráč je vo väzení
            }
        }
        if (game_board[new_space - 1].type == Property) { // Проверка, если игрок ступает на недвижимость
            PROPERTY *current_property = game_board[new_space - 1].property;
            if (current_property != NULL) {
                int is_owned = 0;
                int owner_index = -1;
                // Проверяем, принадлежит ли недвижимость какому-либо игроку
                for (int i = 0; i < MAX_PLAYERS; i++) {
                    for (int j = 0; j < players[i].num_properties; j++) {
                        if (players[i].owned_properties[j] == current_property) {
                            is_owned = 1;
                            owner_index = i;
                            break;
                        }
                    }
                    if (is_owned) {
                        break;
                    }
                }
                if (!is_owned) {
                    if (players[current_player_index].cash >= current_property->price) {
                        players[current_player_index].cash -= current_property->price; // Вычитаем цену недвижимости из денег текущего игрока
                        players[current_player_index].owned_properties[players[current_player_index].num_properties++] = current_property;
                    } else {
                        bankrot=1;
                    }

                } else {
                    int fine = current_property->price;
                    int same_color_properties_count = 0;
                    for (int i = 0; i < players[owner_index].num_properties; i++) {
                        if (players[owner_index].owned_properties[i]->color == current_property->color) {
                            same_color_properties_count++;
                        }
                    }
                    if (same_color_properties_count >= 2) {
                        fine *= 2; // Удваиваем размер штрафа
                    }
                    if(players[current_player_index].cash>=fine){
                        players[current_player_index].cash -= fine; // Вычитаем штраф у текущего игрока
                        players[owner_index].cash += fine;
                    } else{
                        bankrot=1;
                    }
                }
            }
        }


        if (new_space == 1 || (current_space>new_space && new_space!=1)) {
            players[current_player_index].cash += 2;
        }


        players[current_player_index].space_number = new_space;

        if (players[current_player_index].is_in_jail && num_rolls > 0) {
            if (players[current_player_index].cash >= 1) {
                if (jail_fine_pending[current_player_index]){
                    players[current_player_index].cash -= 1;
                }
                players[current_player_index].is_in_jail = 0; // Hráč už nie je vo väzen
                players[current_player_index].space_number = 7;
            } else {
                bankrot=1;
            }
        }

        players_turn = (players_turn + 1) % num_players;

//        if (bankrot) {
//            is_bankrupt[current_player_index] = 1;
//        }
//        if (is_bankrupt[current_player_index]==1){
//            num_players--;
//        }
        if(p_rezim==1){
            printf("Players:\n");
            for(int i=0; i<num_players; i++){
                print_player(&players[i]);
            }
        }
    }

    if(p_rezim!=1){
        printf("Players:\n");
        for (int i = 0; i < num_players; i++) {
            print_player(&players[i]);
        }
    }

    print_game_state();
// Prechádzanie všetkých hráčov
    int max_total_wealth = -1;
    int richest_player_index = -1;

    for (int i = 0; i < num_players; i++) {
        int total_wealth = players[i].cash;
        if (total_wealth > max_total_wealth) {
            max_total_wealth = total_wealth;
            richest_player_index = i;
        } else if (total_wealth == max_total_wealth) {
            int total_property_value_current = 0;
            int total_property_value_richest = 0;
            for (int j = 0; j < players[i].num_properties; j++) {
                total_property_value_current += players[i].owned_properties[j]->price;
            }
            for (int j = 0; j < players[richest_player_index].num_properties; j++) {
                total_property_value_richest += players[richest_player_index].owned_properties[j]->price;
            }
            if (total_property_value_current > total_property_value_richest) {
                richest_player_index = i;
            } else if (total_property_value_current == total_property_value_richest) {
                // Если после учёта стоимости недвижимости осталось равенство,
                // устанавливаем победителем неопределённого
                richest_player_index = -1;
            }
        }
    }

// Výpis víťaza alebo '?' v prípade remízy
    if (richest_player_index != -1) {
        printf("WINNER: P%d\n", richest_player_index + 1);
    } else {
        printf("WINNER: ?\n");
    }

    if(bankrot==1){
        return 0;
    }

    return 0;
}
