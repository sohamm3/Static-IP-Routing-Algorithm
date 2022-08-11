#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

// Structure to define device information connected to the router
// IP Address, Subnet mask, Interface are to be provided by user
// Stores all devices connected to particular router in linked list arrangement

typedef struct device
{
    char ip_address[20];
    char subnet[20];
    char interface[10];
    struct device* next;

}device;

// Structure for one network vertex i.e. router node
// It contains pointer to list of devices
// It contains vertex no.
// It contains pointer to another router node

typedef struct router_node
{
    struct device* head;
    struct trienode* trie_root;
    struct router_node* next;
    int weight;                     // The weight of a node is the sum of the weights of the edges connected to the node.   
    int vertex;               
    int init;

}router_node;

// Structure of graph represented/defined as an array of adjaceny list 
typedef struct graph
{
    int total_vertices;             // Total router nodes in network
    struct router_node** adj_list;
}graph;

// Min Heap Node                    
typedef struct minheap_node
{
    int vertex;
    int cost;
}minheap_node;

// Structure of Minheap used for implementing Dijkstra Algorithm
typedef struct minheap
{
    int size;
    int capacity;
    int* pos;
    struct minheap_node** arr;
}minheap;

// Defines node in trie
typedef struct trienode
{
    struct trienode* child[2];
    int word_end;
}trienode;


int valid_digit(char*part);
int valid_ip(char*ip);
char* convert_ip_to_binary(char *ip);


trienode* add_trienode(void);
void insert_trie(trienode*root,char*str);
trienode* init_root(void);
trienode* remove_str(trienode* root,char* key, int depth);
int isEmpty_trie(trienode*root);
void destroy_trie(trienode*root);
char* longest_match(trienode*root,char*input);


router_node* create_router_node(int vertex_no);
graph* create_graph(int vertex);
void addedge(graph*p,int src_vertex,int destination_vertex,int cost);
void delete_edge(graph*p,int src_vertex,int des_vertex);
void delete_device(graph*g,char*str);
void print_network(graph*p);
void routing_table(graph*p,int router_vertex);
int add_device(graph*p,int router_vertex);
int exact_match(graph*g,char*bin);
void destroy(graph*g);

minheap*create_minheap(int capacity);
void min_heapify(minheap*newheap,int index);
void swap_nodes(minheap_node**a,minheap_node**b);
minheap_node* newheap_node(int vertex,int weight);
int isempty_minheap(minheap*heap);
minheap_node* extract_min_node(minheap*heap);
void update_distance(minheap*heap,int vertex,int cost);
int ismin_heap(minheap*heap,int v);
void dijkstra(graph*g,int src,int des);
void next_lpm_hop(graph*g,char*str);
void next_short_path(graph*g,char*str);
void print_short_path(int dist[], int total, int parent[],int src,int des);
void printPath(int parent[],int i);

int login(void);

#endif        // HEADER_H_INCLUDED