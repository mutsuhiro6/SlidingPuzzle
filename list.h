/* List API for 8 Pazzle */
#include<stdio.h>

typedef struct cell{
    int state[SIZE];
    int cost;
    struct cell *next;
}Cell;

typedef struct list{
    Cell *cell;
}List;

/*先頭要素の削除*/
void delete_first(list_t *lst);

/*ソートしながら要素を追加する。*/
void sorted_add(list_t *lst, node_t *node);

/*n番目にデータの挿入*/
void insert(list_t *lst, int n, int state[SIZE]);

/*新規nodeの作成*/
node_t *make_node(int state[SIZE], int cost);

/*openの先頭nodeの展開、openへの追加*/
void extract_node(list_t *lst);
