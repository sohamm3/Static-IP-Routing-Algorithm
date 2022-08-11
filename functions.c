#include"header.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<conio.h>
#define DELIM "."

// Function to allocate memory for router node (network vertex)
router_node* create_router_node(int vertex_no)
{
    router_node*newnode=(router_node*)malloc(sizeof(router_node));
    if(!newnode)
    {
        printf("Memory allocation failed for router node");
        exit(1);
        // Program terminates on failed memory allocation process
    }

    newnode->head=NULL;
    newnode->vertex=vertex_no;
    newnode->next=NULL;
    // newnode->weight=0;

    return newnode;
}

// Function to allocate memory for graph to store array of adjacency list
graph* create_graph(int vertex)
{
    graph*newgraph=(graph*)malloc(sizeof(graph));
    if(!newgraph)
    {
        printf("Failed to create graph");
        exit(1);
    }
    newgraph->adj_list=(router_node**)malloc(sizeof(router_node*)*vertex);
    if(!newgraph->adj_list)
    {
        printf("Failed to create array of adjacency list");
        exit(1);
    }
    newgraph->total_vertices=vertex;
    for(int i=0;i<vertex;i++)
    {
        newgraph->adj_list[i]=NULL; // Initializes all pointers in array to NULL
    }

    return newgraph;
}

// Function to add device to router
device* create_device(char*ip,char*subnet,char*inter)
{
    device*newdevice=(device*)malloc(sizeof(device));
    strcpy(newdevice->interface,inter);
    strcpy(newdevice->ip_address,ip);
    strcpy(newdevice->subnet,subnet);
    newdevice->next=NULL;
    return newdevice;
}

// Function to add edge between two vertex
void addedge(graph*p,int src_vertex,int destination_vertex,int cost)
{
    int adj_matrix[p->total_vertices][p->total_vertices];

    // Initializes all entries in adjacency matrix as 0
    for(int i=0;i<p->total_vertices;i++)
    {
            for(int j=0;j<p->total_vertices;j++)
            {
                adj_matrix[i][j]=0;
            }
    }

    for(int y=0;y<p->total_vertices;y++)
    {
        router_node* temp=p->adj_list[y];
        while(temp)
        {
            // Entries are 1 where edge exists
            adj_matrix[y][temp->vertex]=1;
            temp=temp->next;
        }
    }

    // If edge already exists between two vertex
    if(adj_matrix[src_vertex][destination_vertex]==1 || adj_matrix[destination_vertex][src_vertex]==1)
    {
        system("cls");
        printf("Edge already exists\n\n");
        return;
    }

    // Adding edge means to add node in adjacency list.
    // Here we are adding node at the beginning of adjacency list.

    router_node*des_node=create_router_node(destination_vertex);
    des_node->weight=cost;
    des_node->next=p->adj_list[src_vertex];
    p->adj_list[src_vertex]=des_node;

    // Creating both way edge
    router_node*src_node=create_router_node(src_vertex);
    src_node->weight=cost;
    src_node->next=p->adj_list[destination_vertex];
    p->adj_list[destination_vertex]=src_node;

    p->adj_list[src_vertex]->init=0;
    p->adj_list[destination_vertex]->init=0;
    system("cls");
    printf("Edge added successfully\n\n");
    return;
}

// Deletes the edge between routers
void delete_edge(graph*p,int src_vertex,int des_vertex)
{
    // Delete a edge from source side
    router_node* temp = p->adj_list[src_vertex];
    router_node* tail=NULL;
    if(temp==NULL)
    {
        printf("Edge doesn't exists\n");
        return;
    }
    if(temp->vertex==des_vertex)
    {
        p->adj_list[src_vertex]=temp->next;
        free(temp);
    }
    else
    {
        while(temp!=NULL && temp->vertex!=des_vertex)
        {
            tail=temp;
            temp=temp->next;
        }
        if(temp==NULL)
        {
            printf("Edge doesn't exists\n");
            return;
        }
        tail->next=temp->next;
        free(temp);
        printf("Edge removed successfully.............\n");
    }

    // To delete a edge from destination side
    router_node*trav=p->adj_list[des_vertex];
    router_node*tailp=NULL;
    if(trav==NULL)
    {
        printf("Edge doesn't exists\n");
        return;
    }
    if(trav->vertex==src_vertex)
    {
        p->adj_list[des_vertex]=trav->next;
        free(trav);
    }
    else
    {
        while(trav!=NULL && trav->vertex!=src_vertex)
        {
            tailp=trav;
            trav=trav->next;
        }

        tailp->next=trav->next;
        free(trav);

    }

    return;
}

// Adds device to router
int add_device(graph*p,int router_vertex)
{
    if(router_vertex>=p->total_vertices)
    {
        printf("Router doesn't exists\n");
        return 0;
    }
    if(!p->adj_list[router_vertex])
        return -1;
    char*ip=(char*)malloc(sizeof(char)*20);
    char*temp=(char*)malloc(sizeof(char)*20);

    char*tempip=(char*)malloc(sizeof(char)*20);
    char*subnet=(char*)malloc(sizeof(char)*20);
    char*temp_subnet=(char*)malloc(sizeof(char)*20);
    char*inter=(char*)malloc(sizeof(char)*10);

    printf("Enter device IP :\n");
    scanf("%s",ip);
    strcpy(tempip,ip);
    strcpy(temp,ip);
    char*binary=(char*)malloc(sizeof(char)*32);
    binary=convert_ip_to_binary(temp);
    if(exact_match(p,binary)!=-1)
    {
        printf("Enter Valid IP. Current IP already registered on a network\n");
        return 0;
    }
    if(valid_ip(tempip)!=1)
    {
        printf("INVALID IP ADDRESS\n");
        return 0;
    }

    printf("Enter device Subnet ID:\n");
    scanf("%s",subnet);
    strcpy(temp_subnet,subnet);
    if(valid_ip(temp_subnet)!=1)
    {
        printf("INVALID SUBNET MASK\n");
        return 0;
    }
    printf("Enter device Interface :\n");
    scanf("%s",inter);

    device*dev=p->adj_list[router_vertex]->head;
    if(dev==NULL)
    {
        dev=create_device(ip,subnet,inter);
        p->adj_list[router_vertex]->head=dev;

    }
    else
    {
        device*newdev=create_device(ip,subnet,inter);
        newdev->next=p->adj_list[router_vertex]->head;
        p->adj_list[router_vertex]->head=newdev;
    }
    if(p->adj_list[router_vertex]->init!=1)
        {
            p->adj_list[router_vertex]->trie_root=init_root();
            p->adj_list[router_vertex]->init=1;
        }

    char*bin=(char*)malloc(sizeof(char)*32);
    bin=convert_ip_to_binary(ip);
    insert_trie(p->adj_list[router_vertex]->trie_root,bin);

    system("cls");
    printf("Device added successfully\n\n");

    return 0;
}

// Deletes device from router
void delete_device(graph*g,char*str)
{
    char*tempstr=(char*)malloc(sizeof(char)*20);
    strcpy(tempstr,str);

    char*tempstr2=(char*)malloc(sizeof(char)*20);
    strcpy(tempstr2,str);
    if(valid_ip(tempstr2)==0)
    {
        printf("Not Valid IP Address\n");
        return;
    }
    char*bin=(char*)malloc(sizeof(char)*32);
    bin=convert_ip_to_binary(str);
    int index=exact_match(g,bin);

    if(index==-1)
    {
        printf("Given IP is not available in Network\n");
        return;
    }

    device*tail=NULL;
    device*lead=g->adj_list[index]->head;

    if(strcmp(lead->ip_address,tempstr)==0)
    {

        g->adj_list[index]->head=lead->next;

        free(lead);
        remove_str(g->adj_list[index]->trie_root,bin,0);
        system("cls");
        printf("Device Removed Successfully\n\n");
        return;
    }
    else{
    while(lead && strcmp(lead->ip_address,tempstr)!=0)
    {
        tail=lead;
        lead=lead->next;
    }

    tail->next=lead->next;
    free(lead);
    remove_str(g->adj_list[index]->trie_root,bin,0);
    system("cls");
    printf("Device Removed Successfully\n\n");
    return;
    }
}

// Prints the whole network information
void print_network(graph*p)
{
    // If no routers are present
    if(p->total_vertices==0)
    {
        printf("No routers present\n");
        return;
    }
    for(int i=0;i<p->total_vertices;i++)
    {
        router_node*temp=p->adj_list[i];
        while(temp)
        {
            printf("%d router is connected to : %d and edge weight is %d\n",i,temp->vertex,temp->weight);
            temp=temp->next;
        }
        printf("\n");
    }
}

// Function to print all devices connected to router.
void routing_table(graph*p,int router_vertex)
{
    if(router_vertex>=p->total_vertices)
        return;
    if(p->adj_list[router_vertex]==NULL)
    {
            printf("No devices connected........\n\n");
            return;
    }
    device*t=p->adj_list[router_vertex]->head;

    printf("IP Address\t\tSubnet Mask\t\tInterface\n");
    while(t)
    {
        printf("%s\t\t",t->ip_address);
        printf("%s\t\t",t->subnet);
        printf("%s",t->interface);
        t=t->next;
        printf("\n");
    }
    printf("\n");
}

// Destroys whole graph network.
void destroy(graph*g)
{
    router_node*temp,*q;
    device*devtemp,*p;

    for(int i=0;i<g->total_vertices;i++)
    {
        // Destroys trie
        if(g->adj_list[i]!=NULL && g->adj_list[i]->init)
            destroy_trie(g->adj_list[i]->trie_root);

        if(g->adj_list[i]!=NULL)
        {
            temp=g->adj_list[i];
            devtemp=g->adj_list[i]->head;
        }
        else
            continue;
        // Frees all nodes in adjacency list
        while(temp)
        {
            q=temp->next;
            free(temp);
            temp=q;
        }
        // Frees all nodes in devices list
        while(devtemp)
        {
            p=devtemp->next;
            free(devtemp);
            devtemp=p;
        }
    }
    printf("Network is successfully destroyed\n\n");

    return;
}

int valid_digit(char*part)
{
    int len=strlen(part);
    for(int i=0;i<len;i++)
    {
        if(part[i]<'0' && part[i]>'9')
            return 0;
    }
    return 1;
}

// Checks whether IP is correct or not
int valid_ip(char*ip)
{
    int num=0,dots=0;
    char* part=strtok(ip,DELIM);
    if(!part)
        return 0;
    while(part)
    {
        if(!valid_digit(part))
        return 0;
        num=atoi(part);
        if(num>=0 && num<=255)
        {
            part=strtok(NULL,DELIM);
            if(part)
                dots++;
        }
        else
            return 0;
    }

    if(dots!=3)
        return 0;
    return 1;
}

// Heap is used to implement Dijkstra Algorithm
minheap_node* newheap_node(int vertex,int weight)
{
    minheap_node*newnode=(minheap_node*)malloc(sizeof(minheap_node));
    newnode->vertex=vertex;
    newnode->cost=weight;
    return newnode;
}

void swap_nodes(minheap_node**a,minheap_node**b)
{
    minheap_node* temp=*a;
    *a=*b;
    *b=temp;
    return;
}

minheap*create_minheap(int capacity)
{
    minheap*newheap=(minheap*)malloc(sizeof(minheap));
    newheap->capacity=capacity;
    newheap->size=0;
    newheap->pos=(int*)malloc(sizeof(int)*capacity);
    newheap->arr=(minheap_node**)malloc(sizeof(minheap_node*)*capacity);
    return newheap;

}

void min_heapify(minheap*newheap,int index)
{
    int smallest,left,right;
    smallest=index;
    left=2*index+1;
    right=2*index+2;
    if(left<newheap->size && newheap->arr[left]->cost < newheap->arr[smallest]->cost)
        smallest=left;
    if(right<newheap->size && newheap->arr[right]->cost < newheap->arr[smallest]->cost)
        smallest=right;

    if(smallest!=index)
    {
        minheap_node*smallestnode=newheap->arr[smallest];
        minheap_node*indexnode=newheap->arr[index];
        newheap->pos[smallestnode->vertex]=index;
        newheap->pos[indexnode->vertex]=smallest;

        swap_nodes(&newheap->arr[smallest],&newheap->arr[index]);
        min_heapify(newheap,smallest);
    }
}

int isempty_minheap(minheap*heap)
{
    return heap->size==0;
}

// Extracts root node from min heap
minheap_node* extract_min_node(minheap*heap)
{
    if(isempty_minheap(heap))
        return NULL;
    minheap_node*root=heap->arr[0];
    minheap_node*last=heap->arr[heap->size-1];
    heap->arr[0]=last;
    heap->pos[root->vertex]=heap->size-1;
    heap->pos[last->vertex]=0;
    --heap->size;
    min_heapify(heap,0);
    return root;
}

// Updates distances
void update_distance(minheap*heap,int vertex,int cost)
{
    int i=heap->pos[vertex];
    heap->arr[i]->cost=cost;
    while(i && heap->arr[i]->cost<heap->arr[(i-1)/2]->cost)
    {
        heap->pos[heap->arr[i]->vertex]=(i-1)/2;
        heap->pos[heap->arr[(i-1)/2]->vertex]=i;
        swap_nodes(&heap->arr[i],&heap->arr[(i-1)/2]);

        i=(i-1)/2;
    }
    return;
}

int ismin_heap(minheap*heap,int v)
{
    if(heap->pos[v]<heap->size)
        return 1;
    return 0;
}

// Dijkstra algorithm is used to find shortest paths to routers.
void dijkstra(graph*g,int src,int des)
{
    int total=g->total_vertices;
    int dist[total];
    int parent[total];
    minheap*heap=create_minheap(total);
    for(int v=0;v<total;v++)
    {
        parent[v]=-1;
        dist[v]=INT_MAX;
        heap->arr[v]=newheap_node(v,dist[v]);
        heap->pos[v]=v;
    }
    heap->arr[src]=newheap_node(src,dist[src]);
    heap->pos[src]=src;
    dist[src]=0;
    update_distance(heap,src,dist[src]);
    heap->size=total;
    while(!isempty_minheap(heap))
    {
        minheap_node*min_node=extract_min_node(heap);
        int u=min_node->vertex;
        router_node*temp=g->adj_list[u];
        while(temp)
        {
            int v=temp->vertex;
            if(ismin_heap(heap,v) && dist[u]!=INT_MAX && temp->weight+dist[u]<dist[v])
            {
                parent[v]=u;
                dist[v]=dist[u]+temp->weight;
                update_distance(heap,v,dist[v]);
            }
            temp=temp->next;
        }
    }
    print_short_path(dist,g->total_vertices,parent,src,des);
}

// Prints short path to routers
void print_short_path(int dist[], int total, int parent[],int src,int des)
{
    printf("Vertex\tHop Distance\t  Path");
    if(src!=des)
    {
        int i=des;
        if(dist[i]>=INT_MAX || dist[i]<=-INT_MAX+1)
        {

                printf("\n%d -> %d \t\t %d\t\t%d ", src, i, -1, src);
                printf("No Path!");
                printf("\n");
                return;
        }
        printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);

        printPath(parent, i);
        printf("\n\n");
        return;
    }

    else
    {
        for (int i = 0; i < total; i++)
        {
            if(i==src)
                continue;
            if(dist[i]>=INT_MAX || dist[i]<=-INT_MAX+1)
            {
                printf("\n%d -> %d \t\t %d\t\t%d ", src, i, -1, src);
                printf("No Path!");
                continue;
            }
            printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);

            printPath(parent, i);
        }
        printf("\n\n");
        return;
    }
}

void printPath(int parent[],int i)
{
    if (parent[i] == - 1)
        return;

    printPath(parent, parent[i]);

    printf("%d ", i);
}


// Initializing trie root
trienode* init_root(void)
{
    trienode*root=(trienode*)malloc(sizeof(trienode));
    root->word_end=0;
    for(int i=0;i<2;i++)
        root->child[i]=NULL;

    return root;
}

// This function is used to insert input string into trie
void insert_trie(trienode*root,char*str)
{
    int len=32;
    trienode*crawl=root;
    int index;
    for(int i=0;i<len;i++)
    {
        index=str[i]-'0';
        if(!crawl->child[index])
            crawl->child[index]=add_trienode();
        crawl=crawl->child[index];

    }

    crawl->word_end=1;
    return;
}


// Create trie node
trienode* add_trienode(void)
{
    trienode*newnode=(trienode*)malloc(sizeof(trienode));
    newnode->child[0]=newnode->child[1]=NULL;
    newnode->word_end=0;
    return newnode;
}

// This functions searches for input string into trie
int search(trienode*root,char*str)
{
    trienode*crawl=root;
    int len=strlen(str);

    int index;
    for(int i=0;i<len;i++)
    {
        index=str[i]-'0';
        if(!crawl->child[index])
            return 0;
        crawl=crawl->child[index];
    }
    return (crawl!=NULL && crawl->word_end);
}


// This function destroys whole trie
void destroy_trie(trienode*root)
{
    for(int i=0;i<2;i++)
        {
            if(root->child[i]!=NULL)
            {
                destroy_trie(root->child[i]);
            }
        }
    free(root);
}

// This function returns whether trie is empty or not
int isEmpty_trie(trienode*root)
{
    for(int i=0;i<2;i++)
    {
        if(root->child[i]!=NULL)
            return 0;
    }
    return 1;
}

trienode* remove_str(trienode* root,char* key, int depth)
{
    // If tree is empty
    if (!root)
        return NULL;

    // If last character of key is being processed
    if (depth == strlen(key)) {

        // This node is no more end of word after
        // Removal of given key
        if (root->word_end)
            root->word_end = 0;

        // If given is not prefix of any other word
        if (isEmpty_trie(root)) {
            free (root);
            root = NULL;
        }

        return root;
    }

    int index = key[depth] - '0';
    root->child[index] =
          remove_str(root->child[index], key, depth + 1);

    if (isEmpty_trie(root) && root->word_end == 0) {
        free(root);
        root = NULL;
    }

    return root;
}

// This function converts IP Address into 32 bits binary form
char* convert_ip_to_binary(char *ip)
{
    char*part;
    const char *s = ".";
    part=strtok(ip,s);
    int number;
    char*binary=(char*)calloc(sizeof(char),33);

    char*bin=(char*)malloc(sizeof(char)*8);

    char*temp=(char*)malloc(sizeof(char)*8);

    int len,p=0,i,counter=0,incr=0,index=0;
    while(part!=NULL)
    {
        number=atoi(part);
        itoa(number,bin,2);

        len=strlen(bin);
        for(i=0;i<8-len;i++)
            temp[i]='0';
        for(int j=i;j<8;j++)
        {
            temp[j]=bin[p];
            p++;
        }

        p=0;
        for(int q=counter+8*incr;q<(8+8*incr);q++)
        {
            binary[q]=temp[index];
            index++;
        }
        incr++;
        index=0;

        part=strtok(NULL,s);
    }

    binary[32]='\0';
    return binary;
}

// This function returns the longest matching string with input string
char* longest_match(trienode*root,char*input)
{
    trienode*crawl=root;

    char*output=(char*)calloc(sizeof(char),32);

    int len=strlen(input);
    int i=0;
    int index=0;
    while(crawl && i<len)
    {
        index=input[i]-'0';

        if(crawl->child[index])
            output[i]=index+'0';
        else
            return output;
        crawl=crawl->child[index];
        i++;
    }
    return output;
}


int login(void)
{
    char ch;int i;
    char*username=(char*)malloc(sizeof(char)*15);
    char*password=(char*)malloc(sizeof(char)*15);
    printf("Enter username :\n");
    scanf("%s",username);
    printf("Enter Password : \n");
    for(i = 0;i < 15;i++)
    {
    ch = getch();
    if(ch==13)
        break;
    password[i] = ch;
    ch = '*' ;
    printf("%c ", ch);
    }
    password[i] = '\0';

    printf("\n");

    if(strcmp(username,"soham")==0 && strcmp(password,"qwerty")==0)
    {
        return 1;
    }
    else
        return 0;
}

// This function returns the vertex whose trie contains given bianry string
int exact_match(graph*g,char*bin)
{
    for(int i=0;i<g->total_vertices;i++)
    {
        if(g->adj_list[i]!=NULL)
        {
            if(g->adj_list[i]->init)
                {
                    if(search(g->adj_list[i]->trie_root,bin))
                    {
                        return i;
                    }
                }
        }
    }
    return -1;
}

// This function is used to find shortest path to reach to destination
// which have device with exact match as that of destination IP 

void next_short_path(graph*g,char*str)
{
    int current_router;
    printf("Enter Current Router Vertex : \n");
    scanf("%d",&current_router);
    while(current_router<0 || current_router>=g->total_vertices)
    {
        printf("Enter Valid Current Vertex : \n");
        scanf("%d",&current_router);
    }
    char*bin=(char*)malloc(sizeof(char)*32);
    bin=convert_ip_to_binary(str);
    if(exact_match(g,bin)==current_router)
    {
        printf("This IP is present on the same router we are present.\n\n");
        return;
    }
    else if(exact_match(g,bin)==-1)
    {
        printf("This IP is currently not available on the network.\n\n");
        return;
    }
    else
    {
        int i=exact_match(g,bin);
        dijkstra(g,current_router,i);
        return;
    }
}

// This function is used to find shortest path to reach to destination
// which have device with longest prefix matching as that of destination IP

void next_lpm_hop(graph*g,char*str)
{
    int current_router;
    printf("Enter Current Router Vertex : \n");
    scanf("%d",&current_router);
    while(current_router<0 || current_router>=g->total_vertices)
    {
        printf("Enter Valid Current Vertex : \n");
        scanf("%d",&current_router);
    }
    char*bin=(char*)malloc(sizeof(char)*32);
    bin=convert_ip_to_binary(str);
    char*longest=(char*)calloc(sizeof(char),32);
    int index=-1;int len=0;int max=INT_MIN;
    for(int i=0;i<g->total_vertices;i++)
    {
        if(g->adj_list[i]!=NULL && g->adj_list[i]->init)
        {
            longest=longest_match(g->adj_list[i]->trie_root,bin);
            len=strlen(longest);
            if(max<len)
            {
                max=len;
                index=i;
            }
        }
    }

    if(index==-1)
    {
        printf("This IP is currently not available on the network.\n\n");
        return;
    }
    else if(index==current_router)
    {
        printf("This IP is present on the same router on which we are present.\n\n");
        return;
    }
    else
    {
        dijkstra(g,current_router,index);
        return;
    }
}