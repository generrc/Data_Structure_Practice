#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define ALPHABET 26

typedef struct Edge
{
    int target;
    int weight;
    struct Edge* next;
} Edge;

typedef struct Vertex
{
    char label;
    Edge* edge_list;
} Vertex;

typedef struct Graph
{
    int num_vertices;
    char graph_type; // D is directed, U is undirected
    Vertex* vertices;
    int* label_to_index;
} Graph;

void dijkstra(Graph* graph, char source_label);
void graph_test(char *graph_name);
void print_path(int* prev, int target, Graph* graph);
void prim(Graph* graph);

void add_vertex(Graph* graph, char label, int index)
{
    if (index >= graph->num_vertices) return;
    graph->vertices[index].label = label;
    graph->vertices[index].edge_list = NULL;
    graph->label_to_index[label - 'A'] = index;
}

Graph* create_graph(int num_vertices, char graph_type)
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->graph_type = graph_type;
    graph->vertices = (Vertex*)malloc(num_vertices * sizeof(Vertex));
    graph->label_to_index = (int*)malloc(sizeof(int) * ALPHABET);
    for (int i = 0; i < ALPHABET; i++)
    {
        graph->label_to_index[i] = -1;
    }
    return graph;
}

void add_edge(Graph* graph, char source_label, char target_label, int weight)
{
    int source_index = graph->label_to_index[source_label - 'A'];
    int target_index = graph->label_to_index[target_label - 'A'];

    if (source_index < 0 || target_index < 0) return;

    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->target = target_index;
    newEdge->weight = weight;
    newEdge->next = graph->vertices[source_index].edge_list;
    graph->vertices[source_index].edge_list = newEdge;

    if (graph->graph_type == 'U')
    {
        Edge* undirected_edge = (Edge*)malloc(sizeof(Edge));
        undirected_edge->target = source_index;
        undirected_edge->weight = weight;
        undirected_edge->next = graph->vertices[target_index].edge_list;
        graph->vertices[target_index].edge_list = undirected_edge;
    }
}

void print_graph(Graph* graph)
{
    for (int i = 0; i < graph->num_vertices; i++)
    {
        Vertex v = graph->vertices[i];
        printf("Vertex %c: \n", v.label);
        Edge* edge = v.edge_list;
        while (edge != NULL)
        {
            printf("  -> %c(weight=%d) \n", graph->vertices[edge->target].label, edge->weight);
            edge = edge->next;
        }
        printf("\n");
    }
}

// Main method, basically takes in cmd line arguments and runs the code
int main(int argc, char **argv)
{
    // Not really sure how to do graphs yet, we shall see
    printf("XDDD\n");

    // Running 1 graph through the method, this is my test case until I make a makefile
    if (argc == 2)
    {
        printf("Testing 1 graph: \n");
        graph_test(argv[1]);
    }
    // This is the actual project program. This else if runs everything and asks if the user
    // wants to try separate graphs.
    else if (argc == 5)
    {
        printf("Testing 4 (or more) different graphs for the algorithms.\n"
               "Below are the results for the class assignment: \n");

        // Running the graph_test file for each graph
        for (int i = 1; i < argc; i++)
        {
            printf("-----------------------------------------------\n");
            printf("Graph %d tested below:\n", i);
            graph_test(argv[i]);
            // cp this to graphtest printf("\n------------------------------------------------------\n");
        }
    }
    else
    {
        printf("Either somehow the make file messed up, you messed up, or I messed up.\n"
               "This error can only occur if 0, 2, or 3 command line arguments are used,\n"
               "which should not be possible given the fact I am having it run using a make file \n"
               "\n"
               "Please do the following commands to run the program so I can pass: \n"
               "gcc -o main main.c \n"
               "./main graph_file1 \n"
               "./main graph_file2 \n"
               "./main graph_file3 \n"
               "./main graph_file4 \n"
               "\n"
               "Or you can email me at ajohn432@charlotte.edu and I can help :D. (I don't want to fail)");
    }
    return 0;
}

void graph_test(char *graph_name)
{
    FILE *f;
    f = fopen(graph_name, "r");

    // If file doesn't exist, gracefully exit the method. Should only happen on user-inputted files.
    if (f == NULL)
    {
        printf("Somehow, the file %s was not found. This file will be skipped.\n", graph_name);
        perror(graph_name);
    }

    // The below loop creates an adjacency matrix that stores the graph
    // It is represented using a 2d array. Size is V x V (V is # of vertices)
    //
    // First I initialize the array using the given vertices #, and edges aren't important in this.
    // I allocate memory for it, then set everything to 0, the fill it out using the loop mentioned earlier

    // This is var declaration
    char buffer[20];
    char graphinfo[20];

    int num_vertices, num_edges;
    char type_of_graph;
    int index = 0;

    int weight;
    char source_label;
    char target_label;

    if (fgets(graphinfo, sizeof(graphinfo), f) == NULL)
    {
        printf("fgets had an error\n");
        return;
    }

    sscanf(graphinfo, "%d %d %c", &num_vertices, &num_edges, &type_of_graph);

    printf("%d %d %c", num_vertices, num_edges, type_of_graph);
    Graph* graph = create_graph(num_vertices, type_of_graph);

    for (int i = 0; i < num_edges; i++)
    {
        if (fgets(buffer, sizeof(buffer), f) == NULL)
        {
            printf("Why is there numbers adding to over 20 characters bruh I did not design this for that xddd");
            return;
        }

        sscanf(buffer, "%c %c %d", &source_label, &target_label, &weight);

        // adding new vertices
        if (graph->label_to_index[source_label - 'A'] == -1)
        {
            add_vertex(graph, source_label, index);
            index++;
        }
        if (graph->label_to_index[target_label - 'A'] == -1)
        {
            add_vertex(graph, target_label, index);
            index++;
        }
        // adding edge
        add_edge(graph, source_label, target_label, weight);
    }

    fseek(f, -1, SEEK_END);
    fgets(buffer, sizeof(buffer), f);
    char o = buffer[0];
    fclose(f);
    printf("%c", o);
    print_graph(graph);

    dijkstra(graph, o);
    printf("\n\n\n\n");

    if (graph->graph_type == 'U')
    {
        printf("This graph is unweighted. Running Prim's algorithm:\n");
        prim(graph);
    }
    else
    {
        printf("This graph is weighted, Prim's algo will not be ran.\n");
    }
}

void print_path(int* prev, int target, Graph* graph)
{
    if (prev[target] == -1)
    {
        printf("%c", graph->vertices[target].label);
        return;
    }
    print_path(prev, prev[target], graph);
    printf(" -> %c", graph->vertices[target].label);
}

void dijkstra(Graph* graph, char source_label)
{
    int num_vertices = graph->num_vertices;
    int source_index = graph->label_to_index[source_label - 'A'];

    int* dist = (int *)malloc(num_vertices * sizeof(int));
    bool* visited = (bool *)malloc(num_vertices * sizeof(bool));
    int* prev = (int *)malloc(num_vertices * sizeof(int));

    for (int i = 0; i < num_vertices; i++)
    {
        dist[i] = 1000000;
        visited[i] = false;
        prev[i] = -1;
    }
    dist[source_index] = 0;

    for (int count = 0; count < num_vertices - 1; count++)
    {
        int min_dist = 1000000;
        int min_index = -1;

        for (int v = 0; v < num_vertices; v++)
        {
            if (!visited[v] && dist[v] <= min_dist)
            {
                min_dist = dist[v];
                min_index = v;
            }
        }

        if (min_index == -1) break;

        visited[min_index] = true;

        Edge* edge = graph->vertices[min_index].edge_list;
        while (edge != NULL)
        {
            int target = edge->target;
            int weight = edge->weight;

            if (!visited[target] && dist[min_index] + weight < dist[target])
            {
                dist[target] = dist[min_index] + weight;
                prev[target] = min_index;
            }
            edge = edge->next;
        }
    }

    printf("Vertex\tDistance from Source\tPath\n");
    for (int i = 0; i < num_vertices; i++)
    {
        printf("%c\t%d\t\t\t", graph->vertices[i].label, dist[i]);
        if (dist[i] < 1000000)
        {
            print_path(prev, i, graph);
        }
        else
        {
            printf("No path");
        }
        printf("\n");
    }

    // FREE THE PEASANTS
    free(dist);
    free(visited);
    free(prev);
}

void prim(Graph* graph)
{
    int num_vertices = graph->num_vertices;
    int* parent = (int*)malloc(num_vertices * sizeof(int)); 
    int* key = (int*)malloc(num_vertices * sizeof(int)); 
    bool* in_mst = (bool*)malloc(num_vertices * sizeof(bool)); 

    for (int i = 0; i < num_vertices; i++)
    {
        key[i] = 1000000;
        in_mst[i] = false;
    }

    key[0] = 0; 
    parent[0] = -1; 

    for (int count = 0; count < num_vertices - 1; count++)
    {
        int min = 1000000, min_index;

        for (int v = 0; v < num_vertices; v++)
            if (in_mst[v] == false && key[v] < min)
                min = key[v], min_index = v;
        int u = min_index;
        in_mst[u] = true;

        Edge* edge = graph->vertices[u].edge_list;
        while (edge != NULL)
        {
            int v = edge->target;
            if (in_mst[v] == false && edge->weight < key[v])
            {
                parent[v] = u;
                key[v] = edge->weight;
            }

            edge = edge->next;
        }
    }

    int total_weight = 0;
    printf("Edge \tWeight\n");
    for (int i = 1; i < num_vertices; i++)
    {
        printf("%c - %c \t%d \n", graph->vertices[parent[i]].label, graph->vertices[i].label, key[i]);
        total_weight += key[i];
    }
    printf("Total weight of MST: %d\n", total_weight);

    // FREE THE PEASANTS ONCE AGAIN
    free(parent);
    free(key);
    free(in_mst);
}
