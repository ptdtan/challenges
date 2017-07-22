#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <math.h>
#include "string.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#define N_CHILDS 5

#define CUR(a_n, a_l, a_e) ((T*)a_n->edge[a_e])->s + a_l -1 
#define ISNEXT(a_n, a_l, a_e) (a_e != -1 && \
    ((T*)a_n->edge[a_e])->s + a_l == *((T*)a_n->edge[a_e])->e)

#define NEXT(a_n, a_l, a_e) \
        a_n = (T*)a_n->edge[a_e]; \
        a_l = 0; \
        a_e = -1;\

unsigned char seq_nt6_table[256] = { 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 4,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 0, 5, 1,  5, 5, 5, 2,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  3, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 0, 5, 1,  5, 5, 5, 2,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  3, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5, 
	5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5,  5, 5, 5, 5  
};
char *nt_rev = "ACGT$";
 
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
    int *e = malloc(4);
    *e = *ls + 1;
    //new internal node
    T *in = init_tree(p, ((T*)p->edge[a_e])->s, e, root);

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

/* walk down
 */
void walk_down(int l, int i, int* str, T **a_n, int *a_l, int *a_e)
{
	T * d = NULL;
	while(l < i + 1){
		if((*a_n)->edge[str[l]] == NULL){
			*a_l = 0;
			*a_e = -1;
			break;
		}
		d = (*a_n)->edge[str[l]];
		int ll = *d->e - d->s;
		if(ll > (i + 1) -l){
			*a_l = (i + 1) - l;
			*a_e = str[l];
			break;
		}else{
			l = l + ll;
			*a_n = d;
			if(l == i + 1){
				*a_l = 0;
				*a_e = -1;
			}
		}
	}
}

T * build_tree(int *str, int len)
{
    int i = 0, j = 1, k; 
    int *end = malloc(4);
    *end = 1;
    T * root = init_tree(NULL, -1, (int *)NULL, NULL);
    root->f = root;
    root->edge[str[i]] = (struct T*)init_tree((struct T*)root, i, end, NULL);

    T* a_n = root;
    T *last = root;
    int a_l = 0;
    int a_e = -1;
    int cur = 0;
    int l = 0;

    while(i < len - 1){
        //finish rule 1 till show stopper
        *end = *end + 1;
        last = root;
        while(j <= i + 1){
            if(ISNEXT(a_n, a_l, a_e)){
                NEXT(a_n, a_l, a_e);
                //break;
            }
            if(a_l == 0){  //being at head of one node
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
                    //walk up at most one edge
                    a_n = ((T*)a_n->edge[a_e])->f == root?a_n->f:((T*)a_n->edge[a_e])->f;

                    l = a_n == root ? j + 1: last->s;
                    //walk down
                    a_l = 0;
                    a_e = -1;
                    walk_down(l, i, str, &a_n, &a_l, &a_e);
                    j++;
                }
            }
        }
        i++;
    }
    return root;
}

/*
 * print edges of suffix tree for debugging
 *
 * */
void print_tree(T *root, int* str)
{
	for(int i = 0; i < N_CHILDS; ++i){
		if(root->edge[i] != NULL){
			T * ch = root->edge[i];
			for(int c = ch->s; c < *ch->e ; c++)
				printf("%c", nt_rev[str[c]]);
			printf("\n");
			print_tree(ch, str);
		}
	}
}

int main(int argc, char* argv[])
{
   FILE *fp = fopen(argv[1], "r");
    int len = 0;
    int *str = (int *)calloc(500000000, sizeof(int));
    int *r = str;
    int *buff = malloc(4);
    while(!feof(fp)){
        fscanf(fp, "%1c", buff);
        if(buff[0] != '\n'){
        	*r = seq_nt6_table[*buff];
			len++;
			r++;
		}
    }
    str[len - 1] = 4;
    T * tree = build_tree(str, len);
    print_tree(tree, str);
    return 0;
}
