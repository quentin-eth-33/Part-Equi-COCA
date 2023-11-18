#include "RepartitionGraph.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct {
    int num_players;
    int num_nodes;
    int **allocationMatrix; // Matrix to store allocation information
} Allocation;

void dfs_check_connectivity(RepartitionGraph graph, int node, int player, bool *visited) {
    visited[node] = true;

    int num_nodes = rg_get_num_nodes(graph);
    for (int i = 0; i < num_nodes; ++i) {
        if (rg_is_edge(graph, node, i) && rg_get_player_of_node_in_partition(graph, i) == player && !visited[i]) {
            dfs_check_connectivity(graph, i, player, visited);
        }
    }
}

// Vérifie si un sous-graphe (Vi, E) est connexe pour un joueur donné
bool is_partition_connected(RepartitionGraph graph, Allocation allocation, int player) {
    int num_nodes = allocation.num_nodes;
    bool *visited = malloc(num_nodes * sizeof(bool));

    // Initialise le tableau visited à false
    for (int i = 0; i < num_nodes; ++i) {
        visited[i] = false;
    }

    // Trouve le premier nœud attribué au joueur courant
    int start_node = -1;
    for (int i = 0; i < num_nodes; ++i) {
        if (allocation.allocationMatrix[player - 1][i] == 1) {
            start_node = i;
            break;
        }
    }

    // Si aucun nœud n'est attribué au joueur, le sous-graphe est connecté par défaut
    if (start_node == -1) {
        free(visited);
        return true;
    }

    // Effectue une recherche en profondeur (DFS) pour vérifier la connectivité
    dfs_check_connectivity(graph, start_node, player, visited);

    // Vérifie si tous les nœuds attribués au joueur sont visités
    for (int i = 0; i < num_nodes; ++i) {
        if (allocation.allocationMatrix[player - 1][i] == 1 && !visited[i]) {
            free(visited);
            return false;
        }
    }

    free(visited);
    return true;
}

// Fonction pour vérifier si une allocation est valide
bool is_valid_allocation(RepartitionGraph graph, Allocation allocation) {
    int num_players = allocation.num_players;

    // Vérifie la connexité pour chaque joueur
    for (int player = 1; player <= num_players; ++player) {
        if (!is_partition_connected(graph, allocation, player)) {
            return false;
        }
    }

    return true;
}

void print_allocation(Allocation allocation) {
    printf("Allocation Matrix:\n");
    for (int i = 0; i < allocation.num_players; ++i) {
        for (int j = 0; j < allocation.num_nodes; ++j) {
            printf("%d ", allocation.allocationMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool is_allocation_equitable(RepartitionGraph graph, Allocation allocation) {
    int num_players = allocation.num_players;

    if (num_players <= 0 || allocation.allocationMatrix == NULL) {
        return false; // Handle invalid input
    }

    int *sum_values_participants = (int *)malloc(num_players * sizeof(int));

    if (sum_values_participants == NULL) {
        return false; // Allocation failed
    }

    for (int i = 0; i < num_players; i++) {
        sum_values_participants[i] = 0;
        for (int j = 0; j < allocation.num_nodes; j++) {
            sum_values_participants[i] += rg_get_weight(graph, j, i);
        }
        //printf("Valeur Joueur %d: %d \n", i+1,  sum_values_participants[i]);
    }

    int total_weight = rg_get_total_weights(graph);
    int fraction = total_weight / num_players;

    for (int i = 0; i < num_players; i++) {
        if (sum_values_participants[i] < fraction) {
            free(sum_values_participants);
            return false;
        }
    }

    free(sum_values_participants);
    return true;
}

void generate_allocations(RepartitionGraph graph, int current_node) {
    int num_nodes = rg_get_num_nodes(graph);
    int num_players = rg_get_num_players(graph);

    if (current_node == num_nodes) {
        // Allocate memory for the new allocation
        Allocation newAllocation;
        newAllocation.num_players = num_players;
        newAllocation.num_nodes = num_nodes;
        newAllocation.allocationMatrix = malloc(num_players * sizeof(int *));
        for (int i = 0; i < num_players; ++i) {
            newAllocation.allocationMatrix[i] = malloc(num_nodes * sizeof(int));
        }

        // Fill the allocation matrix
        for (int player = 1; player <= num_players; ++player) {
            for (int node = 0; node < num_nodes; ++node) {
                newAllocation.allocationMatrix[player - 1][node] = rg_get_player_of_node_in_partition(graph, node) == player;
            }
        }

        //print_allocation(newAllocation);
        //is_allocation_equitable(graph, newAllocation);
        if(is_valid_allocation(graph, newAllocation) && is_allocation_equitable(graph, newAllocation)){
            print_allocation(newAllocation);
            printf("Est Connexe: %d \n", is_valid_allocation(graph, newAllocation));
            printf("Est Equitable: %d \n", is_allocation_equitable(graph, newAllocation));
        }
        return;
    }

    // Try assigning the current node to each player
    for (int player = 1; player <= num_players; ++player) {
        // Assign the current node to the current player
        rg_set_player_of_node_partition(graph, current_node, player);

        // Recursively generate allocations for the next node
        generate_allocations(graph, current_node + 1);

        // Unassign the current node (backtrack)
        rg_set_player_of_node_partition(graph, current_node, 0);
    }
}

bool repartition_brute_force(RepartitionGraph graph)
{
    generate_allocations(graph, 0);
    return true;
}
