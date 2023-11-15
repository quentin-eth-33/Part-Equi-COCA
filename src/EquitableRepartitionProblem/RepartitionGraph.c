#include "RepartitionGraph.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

struct RepartitionGraph_s
{
    Graph graph;      ///< The graph.
    int num_players;  ///< The number of players.
    int **weights;    ///< The weights each player associates to a node.
    int *partition;   ///< The partition of nodes between players;
    int total_weight; ///< The total of all weights for each players;
};

RepartitionGraph rg_initialize(Graph graph)
{
    RepartitionGraph result = (RepartitionGraph)malloc(sizeof(*result));
    result->graph = graph;
    int num_nodes = graph_num_nodes(graph);
    result->num_players = 0;
    char *value = parameter_list_get_value(graph.parameters[0], "xlabel");
    char *aux = (char *)malloc((strlen(value) + 1) * sizeof(char));
    strcpy(aux, value);
    char *tokens = strtok(aux, "\",");
    while (tokens != NULL)
    {
        result->num_players++;
        tokens = strtok(NULL, "\",");
    }
    free(aux);

    result->weights = (int **)malloc(num_nodes * sizeof(int *));
    for (int node = 0; node < num_nodes; node++)
    {
        result->weights[node] = (int *)malloc(result->num_players * sizeof(int));
        int player = 0;
        char *param = parameter_list_get_value(graph.parameters[node], "xlabel");
        char *aux = (char *)malloc((strlen(param) + 1) * sizeof(char));
        strcpy(aux, param);
        tokens = strtok(aux, "\",");
        while (tokens != NULL)
        {
            result->weights[node][player] = atoi(tokens);
            player++;
            tokens = strtok(NULL, "\",");
        }
        free(aux);
    }

    result->partition = (int *)malloc(num_nodes * sizeof(int));
    for (int node = 0; node < num_nodes; node++)
        result->partition[node] = -1;

    result->total_weight = 0;
    for (int node = 0; node < num_nodes; node++)
    {
        result->total_weight += result->weights[node][0];
    }

    return result;
}

void rg_print_partition(RepartitionGraph graph)
{
    int num_nodes = rg_get_num_nodes(graph);
    printf("Partition:\n");
    for (int player = 0; player < graph->num_players; player++)
    {
        printf("Player %d:\n", player);
        int count = 0;
        for (int node = 0; node < num_nodes; node++)
        {
            if (graph->partition[node] == player)
            {
                printf("%s(%d) ", rg_get_node_name(graph, node), node);
                count += rg_get_weight(graph, node, player);
            }
        }
        printf("\nTotal weight for player %d: %d\n", player, count);
    }
}

void rg_print(RepartitionGraph graph)
{
    graph_print(graph->graph);
    printf("There are %d players\n", graph->num_players);
    printf("Weights:\n");
    for (int node = 0; node < graph_num_nodes(graph->graph); node++)
    {
        printf("node %d : ", node);
        for (int player = 0; player < graph->num_players; player++)
        {
            printf(" %d ", graph->weights[node][player]);
        }
        printf("\n");
    }
    printf("Total weight for each player : %d\n", graph->total_weight);
    printf("Equitable weight is at least: %d\n", (graph->total_weight - 1) / graph->num_players + 1);
}

void rg_delete(RepartitionGraph graph)
{
    for (int node = 0; node < graph_num_nodes(graph->graph); node++)
    {
        free(graph->weights[node]);
    }
    free(graph->weights);
    free(graph->partition);
    free(graph);
}

int rg_get_num_nodes(RepartitionGraph graph)
{
    return graph_num_nodes(graph->graph);
}

int rg_get_num_edges(RepartitionGraph graph)
{
    return graph_num_edges(graph->graph);
}

bool rg_is_edge(RepartitionGraph graph, int source, int target)
{
    return (graph_is_edge(graph->graph, source, target));
}

char *rg_get_node_name(RepartitionGraph graph, int node)
{
    return graph_get_node_name(graph->graph, node);
}

int rg_get_num_players(RepartitionGraph graph)
{
    return graph->num_players;
}

int rg_get_weight(RepartitionGraph graph, int node, int player)
{
    return graph->weights[node][player];
}

int rg_get_total_weights(RepartitionGraph graph)
{
    return graph->total_weight;
}

int rg_get_player_of_node_in_partition(RepartitionGraph graph, int node)
{
    return graph->partition[node];
}

void rg_set_player_of_node_partition(RepartitionGraph graph, int node, int player)
{
    graph->partition[node] = player;
}

void rg_reinitialize_partition(RepartitionGraph graph)
{
    for (int node = 0; node < rg_get_num_nodes(graph); node++)
        graph->partition[node] = -1;
}

char **create_random_colours(int num_colours)
{
    char **colours = (char **)malloc(num_colours * sizeof(char *));
    srand(time(NULL));
    for (int col = 0; col < num_colours; col++)
    {
        colours[col] = (char *)malloc(10 * sizeof(char));
        colours[col][9] = '\0';
        colours[col][8] = '\"';
        colours[col][1] = '#';
        colours[col][0] = '\"';
        for (int digit = 2; digit < 8; digit++)
        {
            int r = rand() % 16;
            if (r < 10)
                colours[col][digit] = '0' + r;
            else
                colours[col][digit] = 'a' + r - 10;
        }
    }
    return colours;
}

int get_colour_number(char up, char down)
{
    int res = 0;
    if ('0' <= up && up <= '9')
        res = up - '0';
    else
        res = up - 'a' + 10;
    res *= 16;
    if ('0' <= down && down <= '9')
        res += down - '0';
    else
        res += down - 'a' + 10;
    return res;
}

void rg_create_dot(RepartitionGraph graph, char *name)
{
    int num_players = rg_get_num_players(graph);
    char **colours = create_random_colours(num_players);
    int num_nodes = rg_get_num_nodes(graph);

    FILE *file;

    struct stat st = {0};
    if (stat("./sol", &st) == -1)
        mkdir("./sol", 0777);

    if (name == NULL)
    {
        char nameFile[30];
        snprintf(nameFile, 30, "sol/result.dot");
        file = fopen(nameFile, "w");
        fprintf(file, "graph Sol{\n");
    }
    else
    {
        int length = strlen(name) + 12;
        char nameFile[length];
        snprintf(nameFile, length, "sol/%s.dot", name);
        file = fopen(nameFile, "w");
        fprintf(file, "graph %s{\n", name);
    }

    for (int player = 0; player < num_players; player++)
    {
        printf("Player %d is color %s : ", player, colours[player]);
        printf("\033[48;2;%d;%d;%dmThis color\033[0m\n", get_colour_number(colours[player][2], colours[player][3]), get_colour_number(colours[player][4], colours[player][5]), get_colour_number(colours[player][6], colours[player][7]));
    }

    for (int node = 0; node < num_nodes; node++)
    {
        fprintf(file, "%s", graph_get_node_name(graph->graph, node));
        int partition = rg_get_player_of_node_in_partition(graph, node);
        if (partition >= 0)
            fprintf(file, "[style=filled,fillcolor=%s]", colours[partition]);

        fprintf(file, ";\n");
    }

    graph_fill_dot_content(graph->graph, file);

    fprintf(file, "}\n");
    for (int col = 0; col < num_players; col++)
        free(colours[col]);
    free(colours);

    fclose(file);

    return;
}