#include<stdio.h>
#include<stdlib.h>

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

#define BLANK 0

#define SIZE 9;

typedef struct node{
    int board[SIZE];
    int cost; //親ノードからこのノードまでにかかるコスト
}node_t;

typedef struct list{
    struct node *node;
    struct list *next;
}list_t;

/*List API*/
/*先頭要素の削除*/
list_t *delete_first(list_t *lst);
/*ソートしながら要素を追加する。*/
list_t *sorted_add(list_t *lst, node_t *node);
/*n番目にデータの挿入*/
void insert(list_t *lst, int n, int board[SIZE]);
/*nodeの作成*/
node_t make_node(int board[SIZE], int cost);

/*8 pazzle base*/
int *move();
int find_blank();
void print_board();

/*A* Search base*/
list_t *delete_node();
list_t *extract_node();
int heuristic1();
int heuristic2();
int manhattan_distance();
list_t *add_open();

/* ↓8パズルの基本動作↓ */
/*
・盤面の表現
0|1|2
3|4|5
6|7|8
(数字は要素番号)
このような盤面を配列board[0...9]で表す。
タイルの左右の移動は要素番号を-1,+1で、上下の移動は、-3,+3で表せる。
・オペレータ
   UP:空白の下のタイルを上へ移動
 DOWN:空白の上のタイルを下へ移動
 LEFT:空白の右のタイルを左へ移動
RIGHT:空白の左のタイルを右へ移動
・用意すべき関数
タイルの移動処理をする関数(move)
(特に初期状態で)空白タイルの要素を返す関数
例えばA*探索ならその関数
*/

int *move(int *board, int dir){
    int blank_pos;
    for(int i = 0; i < 9; i++){
        if(board[i] == BLANK){
            blank_pos  = i;
            break;
        }
    }
    if(dir == UP){
        if(!(blank_pos > 5)){
            board[blank_pos] = board[blank_pos + 3];
            board[blank_pos + 3] = BLANK;
        }
        else{
            return NULL;
        }
    }
    else if(dir == DOWN){
        if(!(blank_pos < 3)){
            board[blank_pos] = board[blank_pos - 3];
            board[blank_pos - 3] = BLANK;
        }
        else{
            return NULL;
        }
    }
    else if(dir == LEFT){
        if(!(blank_pos == 2 || blank_pos == 5 || blank_pos == 8)){
            board[blank_pos] = board[blank_pos + 1];
            board[blank_pos + 1] = BLANK;
        }
        else{
            return NULL;
        }
    }
    else if(dir == RIGHT){
        if(!(blank_pos == 0 || blank_pos == 3 || blank_pos == 6)){
            board[blank_pos] = board[blank_pos - 1];
            board[blank_pos - 1] = BLANK;
        }
        else{
            return NULL;
        }
    }
    else{
        fprintf(stderr, "Operater Error.\n");
        exit(1);
    }
    return board;
}

int find_blank(int *board){
    for(int i = 0; i < 9; i++){
        if(board[i] == BLANK){
            return i;
        }
    }
    return -1;
}

void print_board(int *board){
    int i;
    for(i = 0; i < 9; i++){
        if(i == 2 || i == 5 || i == 8){
            printf("%d\n", board[i]);
        }
        else{
            printf("%d|", board[i]);
        }
    }
    printf("\n");
}
/* ↑8パズルの基本動作↑ */

/* ↓A* Search Algorithm↓ */

int heuristic1(int *board, int *goal){
    /*
    Heuristic1
    目標状態と異なるタイルの数
    */
    int i;
    int h = 8; //h:heuristic function
    for(i = 0; i < 9; i++){
        if(board[i] != BLANK && board[i] == goal[i]){
            h--;
        }
    }
    return h;
}

int heuristic2(int *board, int *goal){
    /*
    Heuristic2
    目標状態からのタイルのManhattan距離の総和
    */
    int i,j;
    int h = 0;
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            if(board[i] == goal[j]){
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

list_t *add_open(list_t *open, node_t *node, int *goal){
    /*
    openのリストに要素の追加、ソートしながら。
    ヒューリスティック関数の影響はここで追加する。
    */
    int p_cost, q_cost, p_next_cost;
    int *board;
    /*openが空だった*/
    if(open == NULL){
        open = (list_t *)malloc(sizeof(list_t));
        open->node = node;
        open->next = NULL;
        return open;
    }
    /*以下リストは空でない*/
    list_t *p;//注目するリスト要素
    p = open;
    list_t *q;//追加するリスト要素
    q = (list_t *)malloc(sizeof(list_t));
    q->node = node;
    /*先頭要素以下であった*/
    p_cost = p->node->cost + heuristic1(p->node->board, goal);
    q_cost = q->node->cost + heuristic1(q->node->board, goal);
    if(q_cost <= p_cost){
        q->next = p;
        return q;
    }
    /*挿入する隙間を見つける*/
    while(p->next != NULL){
        p_next_cost = p->next->node->cost + heuristic1(p->next->node->board, goal);
        if((q_cost > p_cost) && (q_cost <= p_next_cost)){
            list_t *r;
            r = p->next;
            p->next = q;
            q->next = r;
            return open;
        }
        p = p->next;
    }
    /*結局どの要素よりも大きかった*/
    p->next = q;
    q->next = NULL;
    return open;
}

list_t *delete_node(list_t *open){
    /*openの先頭要素を削除(free())*/
    /*結局、次の要素のポインタを返す*/
    list_t *new_top;
    if(open->next == NULL){
        free(open);
        return NULL;
    }
    else{
        new_top = open->next;
        free(open);
        return new_top;
    }
}

list_t *extract_node(list_t *open, int *goal){
    /*
    openの先頭要素firstの展開。
    UP,DOWN,LEFT,RIGHTを試して、次状態のノードを作成して、add_nodeする。
    moveがNULLを返すとそのノードは追加しない。
    */
    int i,j,DIR;//DIR方向用の整数
    int *up, *down, *left, *right, *q; //board用ポインタ変数
    list_t *first;
    node_t *upnode, *downnode, *leftnode, *rightnode;
    first = open;
    q = first->node->board;
    open = delete_node(open);
    //for(i = 0; i < 9; i++){
        //if(first->node->board[i] == BLANK){
            up = move(q, UP);
            if(up != NULL){
                //printf("move UP\n");
                //print_board(up);
                upnode = (node_t *)malloc(sizeof(node_t));
                upnode->board = up;
                upnode->cost = first->node->cost + 1;
                open = add_open(open, upnode, goal);
            }

            down = move(q, DOWN);
            if(down != NULL){
                //printf("move DOWN\n");
                //print_board(down);
                downnode = (node_t *)malloc(sizeof(node_t));
                downnode->board = up;
                downnode->cost = first->node->cost + 1;
                open = add_open(open, downnode, goal);
            }

            left = move(q, LEFT);
            if(left != NULL){
                //printf("move LEFT\n");
                //print_board(left);
                leftnode = (node_t *)malloc(sizeof(node_t));
                leftnode->board = up;
                leftnode->cost = first->node->cost + 1;
                open = add_open(open, leftnode, goal);
            }

            right = move(q, RIGHT);
            if(right != NULL){
                //printf("move RIGHT\n");
                //print_board(right);
                rightnode = (node_t *)malloc(sizeof(node_t));
                rightnode->board = up;
                rightnode->cost = first->node->cost + 1;
                open = add_open(open, rightnode, goal);
            }
        //}
    //}
    return open;
}

int compare_board(int *board, int *goal){
    for(int i = 0; i < 9; i++){
        if(board[i] != goal[i]){
            return 0;
        }
        else if(i == 8 && board[i] == goal[i]){
            return 1;
        }
    }
    return 1;
}

/* ↑A* Search Algorithm↑ */
int main(void){
    int init_board[9] = {3, BLANK, 5, 4, 2, 6, 8, 7, 1};
    int goal_board[9] = {1, 2, 3, 4, 5, 6, 7, 8, BLANK};
    node_t *init_node;
    list_t *open, *open2;
    int *board, *goal;
    board = init_board;
    goal = goal_board;
    init_node = (node_t *)malloc(sizeof(node_t));
    if(init_node == NULL){
        fprintf(stderr, "malloc error:(init_node)\n");
        return 1;
    }
    init_node->board = board;
    init_node->cost = 0;
    open = (list_t *)malloc(sizeof(list_t));
    if(open == NULL){
        fprintf(stderr, "malloc error:(open)\n");
        return 1;
    }
    open->node = init_node;
    open->next = NULL;
    while(1){
        print_board(open->node->board);
        if(open == NULL){
            fprintf(stderr, "Search Error.\n");
            return 1;
        }
        if(compare_board(open->node->board, goal)){
            fprintf(stdout, "Search Successed.\n");
            break;
        }
        open = extract_node(open, goal);
    }
    return 0;
}
