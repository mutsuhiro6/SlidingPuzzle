#include<stdio.h>
#include<stdlib.h>

#define OPEN_MAX 10;



struct node{
    int data;
    int cost; //親ノードからこのノードまでにかかるコスト
    int child_num; //子の数

    struct list{
        struct node *node;
        struct list *next;
    }list_t;

    list_t *child[child] //子ノードのリスト
    struct node *parent
}node_t;

int heuristic(node_t *x){
    return 0;
}

list_t *sort_open(list_t *open){
    /*
    (open[i]->cost　+ heuristic(child[i])に対して何かしらのソートアルゴリズムで書く
    */
    return sorted_open;
}

list_t *add_list(list_t *open, node_t *node){
    /*
    openのリストに要素の追加、ソートしながら。
    */
    /*リストが空だった*/
    if(open->node == NULL){
        open->node = node;
        return open;
    }
    /*以下リストは空でない*/
    list_t *p;//注目するリスト要素
    p = open;
    list_t *q;//追加するリスト要素
    q = (list_t *)malloc(sizeof(list_t));
    q->node = node;
    /*先頭要素以下であった*/
    if(q->node->cost <= p->node->cost){
        q->next = p;
        return q;
    }
    /*挿入する隙間を見つける*/
    while(p->next != NULL){
        if((q->node->cost > p->node->cost) && (q->node->cost <= p->node->cost)){
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
    list_t new_top;
    new_top = open;
    free(open);
    return new_top;
}

node_t *add_child(node_t *parent){
    node_t *added[5];
    /*parent->child[i]に前から順に子ノードを追加*/
    return added;
}

void make_child(node_t *parent, int data, int cost){
    node_t *child;
    child = (node_t *)malloc(sizeof(node_t));
    /*
    問題に対してどのような探索分岐があるか設定し、応じて子ノードを求める。whileでくくるかも
    */
    /*
    子ノードの要素初期化例
    child->data = 0;
    child->cost = parent->cost + cost;
    child->child_num = 0;
    !!!child->child = NULL;!!!
    child->parent = parent;
    */
    add_child(parent);
}

int main(void){
    node_t *root;
    list_t *open;
    open = (list_t *)malloc(sizeof(list_t));
    open->node = NULL;
    open->next = NULL;
    /*openの先頭要素をノードNと呼ぶ*/
    while(1){
        if(openが空){
            FINISH;
            return -1;
        }
        else if(ノードNがゴール){
            FINISH;
            return 0;
        }
        /*
        ノードNの子要素の展開
        */
        delete_node(open);
    }
    return 0;
}
