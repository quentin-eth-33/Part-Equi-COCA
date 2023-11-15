#include <stdio.h>
#include <Graph.h>
#include <Parsing.h>
#include <RepartitionGraph.h>

void usage()
{
    printf("Usage: graphParser file\n");
    printf(" Displays information on the graph discribed by file, seen as a repartition graph. It should be provided with a .dot file\n");
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        usage();
        return 0;
    }

    Graph graph;
    graph = get_graph_from_file(argv[1]);

    RepartitionGraph rep = rg_initialize(graph);

    rg_print(rep);

    printf("detailed informations:\n");

    printf(" There are %d vertices.\n", rg_get_num_nodes(rep));
    printf(" There are %d edges.\n", rg_get_num_edges(rep));

    if (graph_is_edge(graph, 0, 1))
        printf(" There is an edge between %s and %s.\n", rg_get_node_name(rep, 0), rg_get_node_name(rep, 1));
    else
        printf("\n There is no edge between %s and %s.\n", rg_get_node_name(rep, 0), rg_get_node_name(rep, 1));

    printf("There are %d players\n",rg_get_num_players(rep));

    printf("Player 0 associates weigth %d to node %s\n",rg_get_weight(rep,0,0),rg_get_node_name(rep,0));

    printf("Sum of all weights (equal for all players) : %d\n",rg_get_total_weights(rep));

    printf("Currently, node %s is attributed to player %d\n",rg_get_node_name(rep,0),rg_get_player_of_node_in_partition(rep,0));

    printf("Changing it.\n");
    rg_set_player_of_node_partition(rep,0,0);

    printf("Now, node %s is attributed to player %d\n",rg_get_node_name(rep,0),rg_get_player_of_node_in_partition(rep,0));

    rg_set_player_of_node_partition(rep,1,1);

    rg_print_partition(rep);

    printf("Writing the graph and partition in sol/example.dot\n");
    rg_create_dot(rep,"example");

    printf("Reinitializing partition.\n");

    rg_print_partition(rep);

    rg_delete(rep);

    graph_delete(graph);
    printf("Graph successfully deleted.\n");
    return 0;
}