#include "RepartitionResolution.h"
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
    bool *visited = (bool *)malloc(num_nodes * sizeof(bool));

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
    int *sommeValeursParticipants = (int *)malloc(k * sizeof(int));

    for (int i = 0; i < k; i++)
    {
        sommeValeursParticipants[i] = 0;
        for (int j = 0; allocation[i][j] != -1; j++)
        {
            sommeValeursParticipants[i] += P[allocation[i][j]];
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
    int *P = (int *)malloc(k * sizeof(int));

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
    if (node == rg_get_num_nodes(graph))
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
    /*À remplacer par votre implémentation du brute-force.
   Cette fonction doit probablement simplement appeler une fonction récursive construisant pas à pas une partition, puis appelant un vérificateur sur chaque partition complète.
   Votre vérificateur devrait être séparé de cette fonction récursive (sauf si vous faites des vérifications sur des partitions partielles, mais cela me semble peu adéquat ici). Votre vérificateur devant vérifier deux propriétés différentes, il vous est conseillé de faire une fonction pour chacune de ces propriétés.
   Les fonctions de RepartitionGraph.h utilisées dans notre solution de cette résolution sont les suivantes :
   rg_get_num_nodes, rg_get_num_players, rg_is_edge, rg_get_player_of_node_in_partition, rg_set_player_of_node_partition.
   Si vous sentez le besoin de fonctions qui ne sont pas dans cette liste, vous êtes potentiellement en train de vous planter (rg_reset_partition pouvant cependant être utile, mais on peut faire sans).
   Notre implémentation (sans ce bloc de commentaire), porte ce fichier à 94 lignes. Cette valeur n’est évidemment qu’une indication, mais si vous dépassez les 300 lignes, il est probable que vous vous compliquiez la vie.
   */
    int num_nodes = rg_get_num_nodes(graph);
    int *Vi = (int *)malloc(num_nodes * sizeof(int));

    if (generatePartitions(graph, 0, Vi))
    {
        printf("Brute Force: Found a valid partition.\n");
        free(Vi);
        return true;
    }
    else
    {
        printf("Brute Force: No valid partition found.\n");
        free(Vi);
        return false;
    }
}