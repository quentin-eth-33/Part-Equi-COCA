/**
 * @file RepartitionResolution.h
 * @author Vincent Penelle (vincent.penelle@u-bordeaux.fr)
 * @brief  Algorithms to solve directly the Equitable Repartition problem
 * @version 1
 * @date 2023-09-22
 * 
 * @copyright Creative Commons.
 * 
 */

#ifndef COCA_REPARTITION_RESOLUTION_H
#define COCA_REPARTITION_RESOLUTION_H

#include "RepartitionGraph.h"

/**
 * @brief Brute Force Algorithm to solve the repartition problem. If it is solvable, @p graph is modified so at the return of the algorithm, the partition is stored in it. If there is no solution, @p graph has its partition sets to -1.
 * 
 * @param graph A RepartitionGraph.
 * @return true if there is a solution.
 * @return false if there is no solution.
 * 
 * @pre @p graph must be valid.
 * @post @p if returns true, the partition of @p graph is a solution to the Equitable Repartition problem.
 */
bool repartition_brute_force(RepartitionGraph graph);

#endif