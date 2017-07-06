#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define N_CHILDS 4

#define CUR(a_n, a_l, a_e) a_n->edge[a_e]->s + a_l -1 
#define NEXT(a_n, a_l, a_e) \
    if(a_e != -1 && a_n->edge[a_e]->s + a_l == a_n->edge[a_e]->e) {\
        a_n = a_n->edge[a_e]; \
        a_l = 0; \
        a_e = -1;\
    }

typedef struct 
{
    int s;
    int *e;
    struct T **edge;
    struct T *p;
    struct T *f;
} T;

T * init_tree(T* p, int s, int *e, T* f)
{
    //allocate for the new node
    T *tr = (T *)malloc(sizeof(T));
    tr->edge = (T **)calloc(N_CHILDS, sizeof(T*));
    for(int i = 0; i < N_CHILDS; i++)
        tr->edge[i] = NULL;
    tr->e = e;
    tr->f = f;
    tr->s = s;
    tr->p = p;
    return tr;
}
/* create new node with left split
 * and right split
 * */
void new_internal(T* p, int *ls, int rs, int *end, int *str, T* root)
{
    T *in = init_tree(p, p->s, ls, root);
    T *n_leaf = init_tree(in, rs, end, root);
    in->edge[str[rs]] = n_leaf;
    in->edge[str[*ls]] = p->edge[rs];
    p->edge[str[p->s]] = in;
}

/*new leaf node
 */
void new_leaf(T *p, int *end, int s, int* str)
{
    p->edge[str[s]] = init_tree(p, s, end, (T*)0);
}

T * build_tree(int *str, int len)
{
    int i = 0, j = 1; 
    int end = 1;
    T * root = init_tree(NULL, -1, -1, NULL);
    root->edge[str[i]] = init_tree(root, i, end);

    T *a_n = root;
    T *last = root;
    int a_l = 0;
    int a_e = -1;
    int cur = 0;

    while(i < len - 1){
        //finish rule 1 till short stopper
        end+=1;
        while(j <= i + 1){
            NEXT(a_n, a_l, a_e);
            if(a_l == 0){
                if(a_n->edge[str[j]] != NULL){
                    a_e = str[j];
                    a_l++;
                    break;
                }else{
                    new_leaf(a_n, j, end);
                    j++;
                    continue;
                }
            } else{
                
            }
        }
        i++;
    }

    
    return root;
}
int main(int argc, char* argv)
{

    return 0;
}
