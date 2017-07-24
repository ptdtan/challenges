#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#define N_CHILDS 5

#define CUR(a_n, a_l, a_e) ((T*)a_n->edge[a_e])->s + a_l -1 
#define ISNEXT(a_n, a_l, a_e) (a_e != -1 && \
    ((T*)a_n->edge[a_e])->s + a_l == *((T*)a_n->edge[a_e])->e)

#define NEXT(a_n, a_l, a_e) \
        a_n = (T*)a_n->edge[a_e]; \
        a_l = 0; \
        a_e = -1;\

uint32_t cnt = 0;
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
    uint32_t s;
    uint32_t  *e;
    struct T **edge;
    struct T *p;
    struct T *f;
} T;

T * init_tree(struct T* p, uint32_t s, uint32_t *e, struct T* f)
{
	cnt++;
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
void new_internal(T* p, int8_t a_e, uint32_t *ls, uint32_t n_l, uint32_t *end, uint8_t *str, T* root)
{
    uint32_t *e = malloc(4);
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
void new_leaf(T *p, uint32_t *end, uint32_t s, uint8_t * str)
{
    p->edge[str[s]] = init_tree(p, s, end, (T*)0);
}

/* walk down
 */
void walk_down(uint32_t l, uint32_t i, uint8_t* str, T **a_n, uint32_t *a_l, int8_t *a_e)
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

T * build_tree(uint8_t *str, uint32_t len)
{
    uint32_t i = 0, j = 1;
    uint32_t *end = malloc(4);
    *end = 1;
    T * root = init_tree(NULL, 0, (uint32_t *)0, NULL);
    root->f = root;
    root->p = root;
    root->edge[str[i]] = (struct T*)init_tree((struct T*)root, i, end, NULL);

    T* a_n = root;
    T *last = root;
    uint32_t a_l = 0;
    int8_t a_e = -1;
    uint32_t cur = 0;
    uint32_t l = 0;
    uint32_t gamma = 0;
    uint32_t k = 0;

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
                    a_n = (a_n == root)?root:a_n->f;
                    //walk down if suffix link point to root
                    if(a_n == root){
                    	walk_down(j + 1, i, str, &a_n, &a_l, &a_e);
					}
                    j++;
                    last = root;
                    continue;
                }
            } else{
                k = CUR(a_n, a_l, a_e);
                if(str[k + 1] == str[i+1]){
                    a_l++;
                    break;
                }else{
                    new_internal(a_n, a_e, &k, i + 1, end, str, root);
                    //set suffix link from previous created internal node
                    if(last != root){
                        last->f = a_n->edge[a_e];
                    }
                    //set new active node for suffix link
                    //in next extension
                    last = (T*)a_n->edge[a_e];
                    //go to the suffix link, walk up at most one edge
                    if(a_n->f == root){
                    	//increase length of have-to-walk string
                    	gamma = a_n == root ? 0 : *a_n->e - a_n->s;
                    	a_n = ((T*)(a_n->p))->f;
					}else{
						a_n = a_n->f;
						gamma = 0;
					}
                    //walk down
                    a_l = 0;
                    a_e = -1;
                    if(a_n == root){
                    	walk_down(j + 1, i, str, &a_n, &a_l, &a_e);
					}else{
						walk_down(i - (*last->e - last->s) - gamma + 1, i, str, &a_n, &a_l, &a_e);
					}
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
void print_tree(T *root, uint8_t* str)
{
	for(int i = 0; i < N_CHILDS; ++i){
		if(root->edge[i] != NULL){
			T * ch = root->edge[i];
			for(uint32_t c = ch->s; c < *ch->e ; c++)
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
    uint8_t *str = (uint8_t *)calloc(500000000, sizeof(uint8_t));
    uint8_t *r = str;
    uint8_t *buff = malloc(sizeof(uint8_t));
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
    //print_tree(tree, str);
	printf("%d\n", cnt);
    return 0;
}
