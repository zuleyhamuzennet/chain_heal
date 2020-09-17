#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct player {
    char name[101];
    int x, y;
    int curr_PP, max_PP;
    int adjNumber;
    int visited;
    struct player **adj;
    struct player *prev;
    struct player *next;
} Player;

typedef struct data {
    int num_jumps;
    int initial_power;
    double power_reduction;
    int best_heal;
    Player **best_path;
    Player **curr_path;
    int *best_path_heal;
    int *curr_path_heal;
} Data;

void DFS(Player *curr, int hop_num, int tot_heal, Data *datas) {
    int i;
    double heal;
    
    if(!curr || !datas || curr->visited || hop_num > datas->num_jumps || hop_num <= 0) return;
    
    curr->visited = 1;
    heal = datas->initial_power;
    
    i = 1;
    while(i < hop_num) {
        heal = heal * (1.0 - datas->power_reduction);
        i++;
    }
    heal = rint(heal);
    
    if(heal > curr->max_PP - curr->curr_PP) {
        heal = curr->max_PP - curr->curr_PP;
    }
    tot_heal += heal;
    
    datas->curr_path[hop_num-1] = curr;
    datas->curr_path_heal[hop_num-1] = heal;

    if(tot_heal > datas->best_heal) {
        datas->best_heal = tot_heal;
        
        i = 0;
        while(i < datas->num_jumps) {
            datas->best_path[i] = datas->curr_path[i];
            i++;
        }

        i = 0;
        while(i < datas->num_jumps) {
            datas->best_path_heal[i] = datas->curr_path_heal[i];
            i++;
        }
    }

    i = 0;
    while(i < curr->adjNumber) {
        DFS(curr->adj[i], hop_num+1, tot_heal, datas);
        i++;
    }
    curr->visited = 0;
    datas->curr_path[hop_num-1] = NULL;
}

void push(Player **head, int X, int Y, int curr_PP, int max_PP, char *na) {
    Player *curr;
    Player *node = (Player*)malloc(sizeof(Player));
    int i;

    curr = *head;
    
    i = 0;
    while(i < 100){
        node->name[i] = na[i];
        i++;
    }

    node->visited = 0;
    node->adjNumber = 0;
    node->x = X;
    node->y = Y;
    node->curr_PP = curr_PP;
    node->max_PP = max_PP;
    node->next = NULL;

    while(curr->next){
        curr = curr->next;
    }

    curr->next = node;
    node->prev = curr;
}

int distance(Player *curr, Player *curr2) {
    return (((curr->x - curr2->x) * (curr->x - curr2->x)) + ((curr->y - curr2->y) * (curr->y - curr2->y)));
}

int main(int argc, char *argv[]) {
    Data datas;
    
    int initial_range = atoi(argv[1]);
    int jump_range = atoi(argv[2]);

    int counter, dist, i;
    int initPlayer_num;
    
    int X, Y, curr_PP, max_PP;
    char na[100];
    
    Player *head = (Player*)malloc(sizeof(Player));
    Player *lokman;
    Player *curr;
    Player *curr2;
    Player **lokmanAdj;
    Player **playerAdj;

    head->next = NULL;
    head->prev = NULL;

    sscanf(argv[5], "%lf", &datas.power_reduction);
    datas.num_jumps = atoi(argv[3]);
    datas.initial_power = atoi(argv[4]);

    counter = 0;
    while(1) {
        if((scanf("%d %d %d %d %s\n",&X, &Y, &curr_PP, &max_PP, na)) == EOF) {
            break;
        }
        push(&head, X, Y, curr_PP, max_PP, na);
        if(!(strcmp("Lokman_Hekim", na))) {
            curr = head;
            while(curr->next) curr = curr->next;
            lokman = curr;
            curr = NULL;
        }
        counter++;
    }
    
    playerAdj = (Player**) malloc(sizeof(Player*) * counter);
    lokmanAdj = (Player**) malloc(sizeof(Player*) * counter);
    lokmanAdj[0] = lokman;
    initPlayer_num = 1;
    curr = head;
    
    while(curr) {
        curr2 = head;
        while(curr2) {
            dist = distance(curr, curr2);
            if(curr != curr2) {
                if(initial_range * initial_range >= dist && curr == lokman) {
                    lokmanAdj[initPlayer_num] = curr2;
                    initPlayer_num++;
                }
                if(jump_range * jump_range >= dist) {
                    playerAdj[curr->adjNumber] = curr2;
                    curr->adjNumber++;
                }
            }
            curr2 = curr2->next;
        }
        curr->adj = (Player**) malloc(sizeof(Player*) * curr->adjNumber);
        i = 0;
        while(i < curr->adjNumber) {
            curr->adj[i] = playerAdj[i];
            i++;
        }
        curr = curr->next;
    }
    
    datas.best_heal = 0;
    datas.best_path = (Player**) malloc(sizeof(Player*) * datas.num_jumps);
    datas.curr_path = (Player**) malloc(sizeof(Player*) * datas.num_jumps);
    datas.best_path_heal = (int*) malloc(sizeof(int) * datas.num_jumps);
    datas.curr_path_heal = (int*) malloc(sizeof(int) * datas.num_jumps);

    i = 0;
    while(i < datas.num_jumps) {
        datas.best_path[i] = NULL;
        datas.curr_path[i] = NULL;
        i++;
    }

    i = 0;
    while(i < initPlayer_num) {
        DFS(lokmanAdj[i], 1, 0, &datas);
        i++;
    }

    i = 0;
    while(i < datas.num_jumps) {
        if(datas.best_path[i]) {
            printf("%s %d\n", datas.best_path[i]->name, datas.best_path_heal[i]);
        }
        i++;
    }
    printf("Toplam_İyileşme %d\n", datas.best_heal);
    
    return 0;
}
