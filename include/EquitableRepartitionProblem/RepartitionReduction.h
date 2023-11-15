/**
 * @file RepartitionReduction.h
 * @author Vincent Penelle (vincent.penelle@u-bordeaux.fr)
 * @brief An implementation of the Coca project for 2023 year. Converts a graph g with players assigning weights to its node to a formula true if and only if it exists a partition of nodes between player so that every player has a connected component which has a high enough score.
 * Provides functions to generate the formula and the necessary variables, alongside function to decode a solution from a model of the formula.
 * @version 1
 * @date 2023-09-22
 *
 * @copyright Creative Commons
 *
 */

#ifndef REPARTITION_RED_H_
#define REPARTITION_RED_H_

#include "Graph.h"
#include "RepartitionGraph.h"
#include <z3.h>

/**
 * @brief Generates a propositional formula satisfiable if and only if there is a partition which satisfies every player and all components are connected.
 *
 * @param ctx The solver context.
 * @param graph A RepartitionGraph
 * @return Z3_ast The formula.
 * @pre @p graph must be initialized.
 */
Z3_ast repartition_reduction(Z3_context ctx, const RepartitionGraph graph);

/**
 * @brief Fills the partition in @p graph according to @p model.
 *
 * @param ctx The solver context.
 * @param model A variable assignment.
 * @param graph A RepartitionGraph.
 * @pre @p model must be a valid model which has a truth value for each variable representing the partition.
 * @pre @p graph must be the RepartitionGraph used to obtain the model.
 */
void repartition_set_partition_from_model(Z3_context ctx, Z3_model model, RepartitionGraph graph);

/**
 * @brief Prints the values of the variables in @p model. @p graph is used to determine which variables exists. @p model should have been obtained through the satisfaction of a formula obtained by repartition_reduction on @p graph.
 * 
 * @param ctx The solver context.
 * @param model A model.
 * @param graph A RepartitionGraph.
 * @pre @p model must be a valid model which has a truth value for each variable representing the partition.
 * @pre @p graph must be the RepartitionGraph used to obtain the model.
 */
void repartition_print_model(Z3_context ctx, Z3_model model, RepartitionGraph graph);
#endif