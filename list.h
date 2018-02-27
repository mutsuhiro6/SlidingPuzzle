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


/*空リストの作成*/
List *make_list();

/*新規cellの作成*/
Cell *make_cell(int state[SIZE], int cost);

/*ソートしつつデータの挿入*/
void insert(List *lst, Cell *cell);

/*先頭要素の削除*/
void delete_first(List *lst);

/*openの先頭nodeの展開、openへの追加*/
void extract_node(list_t *lst);
