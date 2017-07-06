#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define N_CHILDS 4

#define CUR(a_n, a_l, a_e) ((T*)a_n->edge[a_e])->s + a_l -1 
#define ISNEXT(a_n, a_l, a_e) (a_e != -1 && \
    ((T*)a_n->edge[a_e])->s + a_l == *((T*)a_n->edge[a_e])->e)

#define NEXT(a_n, a_l, a_e) \
        a_n = (T*)a_n->edge[a_e]; \
        a_l = 0; \
        a_e = -1;\

typedef struct 
{
    int s;
    int *e;
    struct T **edge;
    struct T *p;
    struct T *f;
} T;

T * init_tree(struct T* p, int s, int *e, struct T* f)
{
    //allocate for the new node
    T *tr = (T *)malloc(sizeof(T));
    tr->edge = (struct T **)calloc(N_CHILDS, sizeof(T*));
    for(int i = 0; i < N_CHILDS; i++)
        tr->edge[i] = NULL;
    tr->e = e;
    tr->f = f;
    tr->s = s;
    tr->p = p;
    return tr;
}
/* create new node with left leaf node
 * */
void new_internal(T* p, int a_e, int *ls, int n_l, int *end, int *str, T* root)
{
    //new internal node
    T *in = init_tree(p, ((T*)p->edge[a_e])->s, ls, root);

    //new leaf
    struct T *n_leaf = (struct T*)init_tree(in, n_l, end, (T*)0);

    //assign new leaf to new edge
    in->edge[str[n_l]] = n_leaf;

    //assign new start for old leaf
    ((T*)(p->edge[a_e]))->s = *ls + 1;

    //assign old leaf to new internal node 
    in->edge[str[*ls + 1]] = p->edge[a_e];

    //assign new internal node to parent
    p->edge[a_e] = (struct T*)in;
}

/*new leaf node
 */
void new_leaf(T *p, int *end, int s, int* str)
{
    p->edge[str[s]] = init_tree(p, s, end, (T*)0);
}

T * build_tree(int *str, int len)
{
    int i = 0, j = 1, k; 
    int *end = malloc(4);
    *end = 1;
    T * root = init_tree(NULL, -1, (int *)NULL, NULL);
    root->edge[str[i]] = (struct T*)init_tree((struct T*)root, i, end, NULL);

    T* a_n = root;
    T *last = root;
    int a_l = 0;
    int a_e = -1;
    int cur = 0;

    while(i < len - 1){
        //finish rule 1 till show stopper
        *end = *end + 1;
        last = root;
        while(j <= i + 1){
            if(ISNEXT(a_n, a_l, a_e)){
                NEXT(a_n, a_l, a_e);
                break;
            }
            if(a_l == 0){
                if(a_n->edge[str[i + 1]] != NULL){
                    a_e = str[i + 1];
                    a_l++;
                    break;
                }else{
                    new_leaf(a_n, end, i + 1, str);
                    a_n = (a_n == root)?root:a_n->f; //a_n here always non-leaf 
                    j++;
                    last = root;
                    continue;
                }
            } else{
                int k = CUR(a_n, a_l, a_e);
                if(str[k + 1] == str[i+1]){
                    a_l++;
                    break;
                }else{
                    new_internal(a_n, a_e, &k, i + 1, end, str, root);
                    //go to suffix link if exits
                    if(last != root){
                        last->f = a_n->edge[a_e];
                    }
                    last = (T*)a_n->edge[a_e];
                    a_n = ((T*)a_n->edge[a_e])->f;
                    a_e = str[j + 1];
                    a_l--;
                    j++;
                }
            }
        }
        i++;
    }
    return root;
}
int main(int argc, char* argv[])
{
    FILE *fp = fopen(argv[1], "r");
    int len = 0;
    int *str = (int *)calloc(256, sizeof(int));
    int *r = str;
    while(!feof(fp)){
        fscanf(fp, "%1d", r);
        len++;
        r++;
    }
    len--;
    T * tree = build_tree(str, len);
    return 0;
}
