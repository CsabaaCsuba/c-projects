//wrote it cuz why not;

struct ListNode;

unsigned int hash(char* key);

unsigned int get_table_placement(unsigned int hash);

void init_node(struct ListNode* node);

void init_table(struct ListNode* hashtable, int table_row_count);

void print_table(struct ListNode* hashtable, int table_row_count);

void add_to_table(struct ListNode* table, char* key, char* value);

void* clone(void* ptr, unsigned int length_in_bytes);

char* get_from_table(struct ListNode* table, char* key);

void remove_from_table(struct ListNode* table, char* key);