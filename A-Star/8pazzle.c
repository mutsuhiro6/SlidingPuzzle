#include<stdio.h>
#include<stdlib.h>
#include "a-star.h"
#include "list.h"


/*8 pazzle base*/
int init[SIZE] = {3, BLANK, 5, 4, 2, 6, 8, 7, 1};
int goal[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, BLANK};
Cell *move(int *state, int dir);
int find_blank(int *state);
void print_board(int *state);

/* ↓8パズルの基本動作↓ */

Cell *move(int *board, int dir){
    int state[SIZE];
    memcpy(state, board, SIZE);
    int blank_pos = find_blank(state);
    if(dir == UP){
        if(!(blank_pos > 5)){
            state[blank_pos] = state[blank_pos + 3];
            state[blank_pos + 3] = BLANK;
        }
        else{
            return NULL;
        }
    }
    else if(dir == DOWN){
        if(!(blank_pos < 3)){
            state[blank_pos] = state[blank_pos - 3];
            state[blank_pos - 3] = BLANK;
        }
        else{
            return NULL;
        }
    }
    else if(dir == LEFT){
        if(!(blank_pos == 2 || blank_pos == 5 || blank_pos == 8)){
            state[blank_pos] = state[blank_pos + 1];
            state[blank_pos + 1] = BLANK;
        }
        else{
            return NULL;
        }
    }
    else if(dir == RIGHT){
        if(!(blank_pos == 0 || blank_pos == 3 || blank_pos == 6)){
            state[blank_pos] = state[blank_pos - 1];
            state[blank_pos - 1] = BLANK;
        }
        else{
            return NULL;
        }
    }
    else{
        fprintf(stderr, "Operater Error.\n");
        exit(1);
    }
    return make_cell(state, 1);
}

int find_blank(int *state){
    for(int i = 0; i < 9; i++){
        if(state[i] == BLANK){
            return i;
        }
    }
    return -1;
}

void print_board(int *state){
    int i;
    for(i = 0; i < 9; i++){
        if(i == 2 || i == 5 || i == 8){
            printf("%d\n", state[i]);
        }
        else{
            printf("%d|", state[i]);
        }
    }
    printf("\n");
}
/* ↑8パズルの基本動作↑ */

/* ↓A* Search Algorithm↓ */

int heuristic1(int *state){
    /*
    Heuristic1
    目標状態と異なるタイルの数
    */
    int i;
    int h = 8; //h:heuristic function
    for(i = 0; i < 9; i++){
        if(state[i] != BLANK && state[i] == goal[i]){
            h--;
        }
    }
    return h;
}

int heuristic2(int *state){
    /*
    Heuristic2
    目標状態からのタイルのManhattan距離の総和
    */
    int i,j;
    int h = 0;
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            if(state[i] == goal[j]){
                h += manhattan_distance(i,j);
            }
        }
    }
    return h;
}

int manhattan_distance(int i, int j){
    struct coodinate{
        int x;//盤面のx座標
        int y;//盤面のy座標
    };
    int k,l;
    int manhattan_x, manhattan_y; //各軸のManhattan距離
    struct coodinate a;
    struct coodinate b;
    struct coodinate c;
    l = i;
    for(k = 0; k < 2; k++){
        switch(l){
            case 0:
                c.x = 0;
                c.y = 1;
                break;
            case 1:
                c.x = 1;
                c.y = 0;
                break;
            case 2:
                c.x = 2;
                c.y = 0;
                break;
            case 3:
                c.x = 0;
                c.y = 1;
                break;
            case 4:
                c.x = 1;
                c.y = 1;
                break;
            case 5:
                c.x = 2;
                c.y = 1;
                break;
            case 6:
                c.x = 0;
                c.y = 2;
                break;
            case 7:
                c.x = 1;
                c.y = 2;
                break;
            case 8:
                c.x = 2;
                c.y = 2;
                break;
        }
        if(k == 0){
            a = c;
        }
        else{
            b = c;
        }
        l = j;
    }
    manhattan_x = b.x - a.x;
    manhattan_y = b.y - b.y;
    if(manhattan_x < 0){
        manhattan_x = (-1)*manhattan_x;
    }
    if(manhattan_y < 0){
        manhattan_y = (-1)*manhattan_y;
    }
    return(manhattan_x + manhattan_y);
}

List *make_list(){
    List *lst;
    lst = (List *)malloc(sizeof(List));
    lst->cell = NULL;
    return lst;
}

Cell *make_cell(int *state, int cost){
    Cell *cell;
    cell = (Cell *)malloc(sizeof(Cell));
    cell->state = (int *)malloc(SIZE * sizeof(int));
    memcpy(cell->state, state, SIZE);
    cell->cost = cost;
    cell->next = NULL;
    return cell;
}

void insert(List *lst, Cell *cell){
    /* コストの小さい順にソートしつつリストにセルを追加する。 */
    Cell *stock;//退避用
    Cell *p;//注目するセル
    /* 空リストの時、先頭に追加 */
    if(lst->cell == NULL){
        lst->cell = (Cell *)malloc(sizeof(Cell));
        lst->cell->next = cell;
        return;
    }
    /* cellのcost<=先頭要素のコスト */
    else if(cell->cost + heuristic1(cell->state) <= lst->cell->next->cost + heuristic1(lst->cell->next->state)){
        p = lst->cell->next;
        stock = p;
        p = cell;
        cell->next = stock;
        return;
    }
    /* 先頭要素しかなく、先頭要素のコストよりもcellのそれが大きい */
    else if(lst->cell->next->next == NULL){
        p->next = cell;
        return;
    }
    else{
        while(lst->cell->next->next != NULL){
            if((lst->cell->next->cost + heuristic1(lst->cell->next->state) <= cell->cost + heuristic1(cell->state)) && (lst->cell->next->next->cost + heuristic1(lst->cell->next->state) > cell->cost + heuristic1(cell->state))){
                p = lst->cell->next;
                stock = p->next;
                p->next = cell;
                cell->next = stock;
                return;
            }
            p = p->next;
        }
        p->next = cell;
    }
}

void delete_first(List *lst){
    /* リストは空でないと仮定する。 */
    Cell *stock;
    Cell *f;//先頭のセル
    f = lst->cell->next;
    stock = f;
    f = f->next;
    free(stock);
}

void extract_node(List *lst){
    /* リストは空でないと仮定する */
    int dir;
    Cell *f;//先頭セル
    f = lst->cell->next;
    delete_first(lst);
    for(dir = UP; dir <= RIGHT; dir++){
        insert(lst, move(f->state, dir));
    }
}

int compare_board(int *state, int *goal){
    for(int i = 0; i < 9; i++){
        if(state[i] != goal[i]){
            return 0;
        }
        else if(i == 8 && state[i] == goal[i]){
            return 1;
        }
    }
    return 1;
}

/* ↑A* Search Algorithm↑ */
int main(void){
    Cell *init_node;
    List *open;
    int *initial_state = init;
    init_node = make_cell(initial_state, 0);
    open = make_list();
    insert(open, init_node);
    while(1){
        print_board(open->cell->next->state);
        if(open == NULL){
            fprintf(stderr, "Search Error.\n");
            return 1;
        }
        if(compare_board(open->cell->next->state, goal)){
            fprintf(stdout, "Search Successed.\n");
            break;
        }
        extract_node(open);
    }
    return 0;
}
