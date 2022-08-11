#include"header.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
int main()
{
    printf("\t\tDSA MINI PROJECT : STATIC IP ROUTING USING DIJKSTRA ALGORITHM\n\n");
    if(login()==0)
        printf("...................Access Denied!!!.................\n");

    else
    {
        printf("...................Access Granted.................\n\n");
        system("cls");
        printf("\t\tDSA MINI PROJECT : STATIC IP ROUTING USING DIJKSTRA ALGORITHM\n\n");
        printf("Enter no. of routers : \n");
        int no;
        scanf("%d",&no);
        graph* g=create_graph(no);
        printf("..............Graph is successfully created!!!.............\n\n");
        printf("Nodes available in network are from 0 to %d\n\n",no-1);
        
        while(1)
        {
            printf("1. Add Edge\n");
            printf("2. Delete Edge\n");
            printf("3. Add Devices\n");
            printf("4. Delete Devices\n");
            printf("5. Print Routing Table\n");
            printf("6. Short Paths to routers\n");
            printf("7. Find Next Hop\n");
            printf("8. Find Next Hop(LPM)\n");
            printf("9. Print Network\n");
            printf("10. Exit\n\n");
            int choice;
            printf("Enter Choice : ");
            scanf("%d",&choice);
            switch(choice)
        {
        case 1:
        {
            int src,des,cost;
            printf("Enter Source Vertex : \n");
            scanf("%d",&src);
            while(src<0 || src>=g->total_vertices)
            {
                printf("Enter Valid Source Vertex : \n");
                scanf("%d",&src);
            }

            printf("Enter Destination Vertex : \n");
            scanf("%d",&des);
            while(des<0 || des>=g->total_vertices || des==src)
            {
                printf("Enter Valid Destination Vertex : \n");
                scanf("%d",&des);
            }

            printf("Enter Weight : \n");
            scanf("%d",&cost);
            while(cost<0 || cost>=INT_MAX)
            {
                printf("Enter Valid Weight : \n");
                scanf("%d",&des);
            }
            addedge(g,src,des,cost);
            break;
        }
        case 2:
        {
            int src,des;
            printf("Enter Source Vertex : \n");
            scanf("%d",&src);
            while(src<0 || src>=g->total_vertices)
            {
                printf("Enter Valid Source Vertex : \n");
                scanf("%d",&src);
            }

            printf("Enter Destination Vertex : \n");
            scanf("%d",&des);
            while(des<0 || des>=g->total_vertices)
            {
                printf("Enter Valid Destination Vertex : \n");
                scanf("%d",&des);
            }
            system("cls");
            delete_edge(g,src,des);
            break;
        }
        case 3:
        {
            int vertex;
            printf("Enter router vertex :\n");
            scanf("%d",&vertex);
            while(vertex<0 || vertex>=g->total_vertices)
            {
                printf("Enter Valid Source Vertex : \n");
                scanf("%d",&vertex);
            }
            if(add_device(g,vertex)==-1)
                printf("Devices cannot be added to disconnected router\n");
            break;
        }
        case 4:
        {
            char*str=(char*)malloc(sizeof(char)*32);
            printf("Enter Device IP to be removed from Network\n");
            scanf("%s",str);
            delete_device(g,str);
            break;
        }
        case 5:
        {
            int vertex;
            printf("Enter Router Vertex :\n");
            scanf("%d",&vertex);
            while(vertex<0 || vertex>=g->total_vertices)
            {
                printf("Enter Valid Source Vertex : \n");
                scanf("%d",&vertex);
                system("cls");

            }
            routing_table(g,vertex);
            break;
        }
        case 6:
        {
            int src;
            printf("Enter Source Vertex :\n");
            scanf("%d",&src);
            while(src<0 || src>=g->total_vertices)
            {
                printf("Enter Valid Source Vertex : \n");
                scanf("%d",&src);
            }
            dijkstra(g,src,src);
            break;
        }
        case 7:
        {
            char*str=(char*)malloc(sizeof(char)*20);
            printf("Enter Destination IP :\n");
            scanf("%s",str);
            char*temp=(char*)malloc(sizeof(char)*20);
            strcpy(temp,str);
            if(valid_ip(temp)==0)
            {
                printf("\nInvalid IP Address\n\n");
            }
            else
                next_short_path(g,str);
            break;
        }
        case 8:
        {
            char*str=(char*)malloc(sizeof(char)*20);
            printf("Enter Destination IP :\n");
            scanf("%s",str);
            char*temp=(char*)malloc(sizeof(char)*20);
            strcpy(temp,str);
            if(valid_ip(temp)==0)
            {
                printf("Invalid IP Address\n");
            }
            else
                next_lpm_hop(g,str);
            break;
        }
        case 9:
        {
            print_network(g);
            break;
        }
        case 10:
        {
            destroy(g);
            exit(1);
            break;
        }
        default:
            break;
    }
    }
    }

    return 0;
}