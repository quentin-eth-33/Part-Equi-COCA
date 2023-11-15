/**
 * @file RepartitionGraph.h
 * @author Vincent Penelle (vincent.penelle@u-bordeaux.fr)
 * @brief  Structure to a graph with weights for several players and an assignation of each node to each player.
 * @version 1
 * @date 2023-09-20
 *
 * @copyright Creative Commons.
 *
 */

#ifndef COCA_REPARTITIONGRAPH_H
#define COCA_REPARTITIONGRAPH_H

#include <stdbool.h>
#include "Graph.h"

/**
 * @brief The struct containing a graph, a weight for each player and each node, and a repartition of node to each player.
 */

typedef struct RepartitionGraph_s *RepartitionGraph;

/**
 * @brief Initializes a RepartitionGraph from a Graph for use in the functions. Parses the parameters to find the number of players and the weights each player associates to each node. Initializes the partition as empty, i.e. all nodes are associated with dummy player -1.
 * The graph is NOT copied (as it is not supposed to be modified).
 *
 * @pre All nodes of graph must have parameter xlabel set, and the value of xlabel must be a list of integers separated with ',' and might be enclosed in '\"' (it is compulsory for the dot format to be analyzed correctly). All such lists must be of the same size. If not, the program will crash or have unpredictable behaviors.
 * @param graph Tha Graph that is the input of the problem.
 * @return RepartitionGraph The structure RepartitionGraph described above.
 */
RepartitionGraph rg_initialize(Graph graph);

/**
 * @brief Printer function to display the partition of @p graph.
 * 
 * @param graph A RepartitionGraph.
 */
void rg_print_partition(RepartitionGraph graph);

/**
 * @brief Printer function to display information about @p graph in input.
 *
 * @param graph A RepartitionGraph.
 */
void rg_print(RepartitionGraph graph);

/**
 * @brief Deallocates memory used by a @p graph (what is allocated by rg_initialize). Warning: does NOT deallocate the Graph inside the @p graph. The Graph must be deallocated with adequate function from Graph.h.
 *
 * @param graph A RepartitionGraph.
 */
void rg_delete(RepartitionGraph graph);

/**
 * @brief Returns the number of nodes of @p graph.
 *
 * @param graph A RepartitionGraph.
 * @return int Its number of nodes.
 */
int rg_get_num_nodes(RepartitionGraph graph);

/**
 * @brief Returns the number of edges of @p graph.
 *
 * @param graph A RepartitionGraph.
 * @return int Its number of edges.
 */
int rg_get_num_edges(RepartitionGraph graph);

/**
 * @brief Gets if (@p source,@p target) is an edge in @p graph.
 *
 * @param graph A RepartitionGraph.
 * @param source A node.
 * @param target A node.
 * @return true if (@p source,@p target) is an edge in @p graph.
 * @return false if (@p source,@p target) is not an edge in @p graph.
 */
bool rg_is_edge(RepartitionGraph graph, int source, int target);

/**
 * @brief Gets the name of @p node in @p graph. The name is what appears in the .dot file, while its number is local to this program.
 *
 * @param graph A RepartitionGraph.
 * @param node A node
 * @return char* Its name.
 */
char *rg_get_node_name(RepartitionGraph graph, int node);

/**
 * @brief Gets the number of players in the problem of Equitable Repartition on @p graph.
 *
 * @param graph A RepartitionGraph.
 * @return int The number of players.
 */
int rg_get_num_players(RepartitionGraph graph);

/**
 * @brief Gets the weight that @p player associates to @p node in @p graph.
 *
 * @param graph A RepartitionGraph.
 * @param node A node.
 * @param player A player.
 * @return int The weight associated by @p player to @p node.
 */
int rg_get_weight(RepartitionGraph graph, int node, int player);

/**
 * @brief Gets the sum of all weights of a player. This weight is supposed to be the same for every player.
 *
 * @param graph A RepartitionGraph.
 * @return int The sum of all weights fo a player.
 */
int rg_get_total_weights(RepartitionGraph graph);

/**
 * @brief Gets which player is associated with @p node in @p graph.
 *
 * @param graph A RepartitionGraph.
 * @param node A node.
 * @return int The player associated to @p node.
 */
int rg_get_player_of_node_in_partition(RepartitionGraph graph, int node);

/**
 * @brief Sets @p player as the owner of @p node in @p graph.
 *
 * @param graph A RepartitionGraph.
 * @param node A node.
 * @param player A player.
 */
void rg_set_player_of_node_partition(RepartitionGraph graph, int node, int player);

/**
 * @brief Reinitializes all nodes of @p graph to be held by dummy player -1.
 *
 * @param graph A RepartitionGraph.
 */
void rg_reinitialize_partition(RepartitionGraph graph);

/**
 * @brief Writes a dot file named <@p name>.dot representing @p graph with partition information.
 *
 * @param graph A RepartitionGraph.
 * @param name A name of file.
 */
void rg_create_dot(RepartitionGraph graph, char *name);

#endif