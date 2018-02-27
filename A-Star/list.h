/* List API for 8 Pazzle */

typedef struct cell{
    int *state;
    int cost;
    struct cell *next;
}Cell;

typedef struct list{
    Cell *cell;
}List;

/*先頭要素の削除*/
void delete_first(List *lst);

/*n番目にデータの挿入*/
void insert(List *lst, Cell *cell);

List *make_list();

/*新規nodeの作成*/
Cell *make_cell(int *state, int cost);

/*openの先頭nodeの展開、openへの追加*/
void extract_node(List *lst);
