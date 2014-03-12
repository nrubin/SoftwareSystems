/* This file was automatically generated.  Do not edit! */
int main();
typedef struct
{
    enum Type {INT, STRING} type;
    union
    {
        int i;
        char *s;
    };
} Value;
typedef struct
{
    void *key;
    int (*hash) (void *);
    int (*equal) (void *, void *);
} Hashable;

typedef struct node
{
    Hashable *key;
    Value *value;
    struct node *next;
} Node;
typedef struct map
{
    int n;
    int num_items;
    Node **lists;
} Map;
void print_lookup(Value *value);
Value *map_lookup(Map *map, Hashable *key);
void print_map(Map *map);
void map_add(Map *map, Hashable *key, Value *value);
void map_resize(Map *map);
Map *make_map(int n);
Value *list_lookup(Node *list, Hashable *key);
Node *prepend(Hashable *key, Value *value, Node *rest);
void print_list(Node *node);
void print_node(Node *node);
Node *make_node(Hashable *key, Value *value, Node *next);
Hashable *make_hashable_string(char *s);
Hashable *make_hashable_int(int x);
int equal_hashable(Hashable *h1, Hashable *h2);
int equal_string(void *s1, void *s2);
int equal_int(void *ip, void *jp);
int hash_hashable(Hashable *hashable);
int hash_string(void *p);
int hash_int(void *p);
void print_hashable(Hashable *hashable);
Hashable *make_hashable(void *key, int(*hash)(void *), int(*equal)(void *, void *));
void print_value(Value *value);
Value *make_string_value(char *s);
Value *make_int_value(int i);
