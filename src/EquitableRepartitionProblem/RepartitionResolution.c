#include "RepartitionGraph.h"
#include <stdlib.h>
#include <stdio.h>

bool dfs_recursive(RepartitionGraph graph, int node, bool *visited, int *Vi);

bool dfs(RepartitionGraph graph, int node, bool *visited, int *Vi)
{
    return dfs_recursive(graph, node, visited, Vi);
}

bool dfs_recursive(RepartitionGraph graph, int node, bool *visited, int *Vi)
{
    visited[node] = true;

    for (int i = 0; i < rg_get_num_nodes(graph); i++)
    {
        if (rg_is_edge(graph, node, i) && !visited[i] && rg_get_player_of_node_in_partition(graph, i) == Vi[node])
        {
            dfs_recursive(graph, i, visited, Vi);
        }
    }

    return true;
}

bool estConnexe(RepartitionGraph graph, int *Vi)
{
    int num_nodes = rg_get_num_nodes(graph);

    if (num_nodes <= 0) {
        return false; // Handle the case of an empty graph
    }

    bool *visited = (bool *)malloc(num_nodes * sizeof(bool));
    
    if (visited == NULL) {
        return false; // Allocation failed
    }

    for (int i = 0; i < num_nodes; i++)
    {
        visited[i] = false;
    }

    int start_node = Vi[0];

    dfs(graph, start_node, visited, Vi);

    for (int i = 0; i < num_nodes; i++)
    {
        if (Vi[i] == Vi[start_node] && !visited[i])
        {
            free(visited);
            return false;
        }
    }

    free(visited);
    return true;
}

bool estAllocationEquitable(RepartitionGraph graph, int k, int *P, int **allocation)
{
    if (k <= 0 || P == NULL || allocation == NULL) {
        return false; // Handle invalid input
    }

    int *sommeValeursParticipants = (int *)malloc(k * sizeof(int));

    if (sommeValeursParticipants == NULL) {
        return false; // Allocation failed
    }

   for (int i = 0; i < k; i++)
    {
        sommeValeursParticipants[i] = 0;
        for (int j = 0; allocation[i][j] != -1; j++)
        {
            sommeValeursParticipants[i] += rg_get_weight(graph, allocation[i][j], i);
        }
    }

    int sommeTotale = rg_get_total_weights(graph);
    int fraction = sommeTotale / k;

    for (int i = 0; i < k; i++)
    {
        if (sommeValeursParticipants[i] < fraction)
        {
            free(sommeValeursParticipants);
            return false;
        }
    }

    free(sommeValeursParticipants);
    return true;
}

bool checkValidPartition(RepartitionGraph graph, int *Vi)
{
    int k = rg_get_num_players(graph);

    if (k <= 0) {
        return false; // Handle the case of an invalid number of players
    }

    int *P = (int *)malloc(k * sizeof(int));

    if (P == NULL) {
        return false; // Allocation failed
    }

    for (int i = 0; i < k; i++)
    {
        P[i] = rg_get_total_weights(graph); // Assuming this is how you obtain the weights for each player
    }

    bool isValid = estConnexe(graph, Vi) && estAllocationEquitable(graph, k, P, NULL);

    free(P);

    return isValid;
}

// Helper function for repartition_brute_force
bool generatePartitions(RepartitionGraph graph, int node, int *Vi)
{
    if (node >= rg_get_num_nodes(graph))
    {
        return checkValidPartition(graph, Vi);
    }

    for (int player = 0; player < rg_get_num_players(graph); player++)
    {
        Vi[node] = player;
        if (generatePartitions(graph, node + 1, Vi))
        {
            return true;
        }
    }
    return false;
}

bool repartition_brute_force(RepartitionGraph graph)
{
    int num_nodes = rg_get_num_nodes(graph);

    if (num_nodes <= 0) {
        return false; // Handle the case of an empty graph
    }

    int *Vi = (int *)malloc(num_nodes * sizeof(int));

    if (Vi == NULL) {
        return false; // Allocation failed
    }

    if (generatePartitions(graph, 0, Vi))
    {
        printf("Brute Force: Found a valid partition.\n");
    }
    else
    {
        printf("Brute Force: No valid partition found.\n");
    }

    free(Vi);
    return true;
}
