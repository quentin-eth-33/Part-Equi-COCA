#include "RepartitionGraph.h"
#include <stdlib.h>
#include <stdio.h>

bool isPartitionConnected(RepartitionGraph graph, int *allocation, int playerIndex)
{
    int numNodes = rg_get_num_nodes(graph);

    bool *visited = malloc(numNodes * sizeof(bool));
    for (int i = 0; i < numNodes; i++)
    {
        visited[i] = false;
    }

    int numNodesInPartition = 0;
    for (int i = 0; i < numNodes; i++)
    {
        if (allocation[i] == playerIndex)
        {
            numNodesInPartition++;
        }
    }

    if (numNodesInPartition == 0)
    {
        free(visited);
        return true;
    }

    int startNode;
    for (int i = 0; i < numNodes; i++)
    {
        if (allocation[i] == playerIndex)
        {
            startNode = i;
            break;
        }
    }

    int *stack = malloc(numNodes * sizeof(int));
    int top = -1;

    stack[++top] = startNode;
    visited[startNode] = true;

    while (top >= 0)
    {
        int currentNode = stack[top--];

        for (int i = 0; i < numNodes; i++)
        {
            if (rg_is_edge(graph, currentNode, i) && allocation[i] == playerIndex && !visited[i])
            {
                stack[++top] = i;
                visited[i] = true;
            }
        }
    }

    for (int i = 0; i < numNodes; i++)
    {
        if (allocation[i] == playerIndex && !visited[i])
        {
            return false;
        }
    }
    free(stack);
    free(visited);

    return true;
}

bool isPartitionFair(RepartitionGraph graph, int *allocation, int playerIndex)
{
    int numNodes = rg_get_num_nodes(graph);
    int numPlayers = rg_get_num_players(graph);

    int playerTotalValue = 0;
    for (int i = 0; i < numNodes; i++)
    {
        if (allocation[i] == playerIndex)
        {
            playerTotalValue += rg_get_weight(graph, i, playerIndex);
        }
    }
    return playerTotalValue >= rg_get_total_weights(graph) / numPlayers;
}

bool verify(RepartitionGraph graph, int *allocation, int numPlayers)
{
    bool isValid = true;

    for (int i = 0; i < numPlayers; i++)
    {
        if (!isPartitionConnected(graph, allocation, i))
        {
            isValid = false;
            break;
        }
    }

    if (isValid)
    {
        for (int i = 0; i < numPlayers; i++)
        {
            if (!isPartitionFair(graph, allocation, i))
            {
                isValid = false;
                break;
            }
        }
    }

    return isValid;
}

bool generateAllocations(RepartitionGraph graph, int *allocation, int nodeIndex)
{
    int numNodes = rg_get_num_nodes(graph);
    int numPlayers = rg_get_num_players(graph);

    if (nodeIndex == numNodes)
    {
        if (verify(graph, allocation, numPlayers))
        {
            return true;
        }
        return false;
    }

    for (int i = 0; i < numPlayers; i++)
    {
        allocation[nodeIndex] = i;
        if (generateAllocations(graph, allocation, nodeIndex + 1))
        {
            return true;
        }
    }
    return false;
}

bool repartition_brute_force(RepartitionGraph graph)
{
    int numNodes = rg_get_num_nodes(graph);
    int *allocation = malloc(numNodes * sizeof(int));

    for (int i = 0; i < numNodes; i++)
    {
        allocation[i] = -1;
    }

    bool isValid = generateAllocations(graph, allocation, 0);

    free(allocation);
    return isValid;
}