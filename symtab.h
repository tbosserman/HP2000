#define NUM_BUCKETS     251

typedef struct node_s   node_t;

struct node_s {
    char        *name;
    symbol_t    *value;
    node_t      *next;
};

extern symbol_t *get_symbol(char *name);
extern void store_symbol(char *name, symbol_t *symbol);
