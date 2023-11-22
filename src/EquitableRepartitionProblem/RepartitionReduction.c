#include "RepartitionReduction.h"
#include "Z3Tools.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @brief Returns the variable V_{@p node}^{@p player} from the subject that is true if @p node is attributed to @p player.
 *
 * @param ctx The solver context.
 * @param node A node.
 * @param player A player.
 * @return Z3_ast The variable.
 */
Z3_ast variable_node_associated_to_player(Z3_context ctx, int node, int player)
{
    char name[40];
    snprintf(name, 40, "V[%d:%d]", node, player);
    return mk_bool_var(ctx, name);
}

/**
 * @brief The varible C_{@p node1,@p node2,@p limit}^{@p player} true if there is a path from @p node1 to @p node2 with nodes between 0 and @p limit with nodes that are attributed to @p player.
 *
 * @param ctx The solver context.
 * @param node1 A node.
 * @param node2 A node.
 * @param limit A node.
 * @param player A player.
 * @return Z3_ast The variable.
 */
Z3_ast variable_floyd_warshall(Z3_context ctx, int node1, int node2, int limit, int player)
{

    char name[40];
    snprintf(name, 40, "C[%d-%d:%d;%d]", node1, node2, limit, player);
    return mk_bool_var(ctx, name);
}

/**
 * @brief The variable M_{@p node,@p position}^{@p player} from the subject, expressing that @p node is at @p position in the partition of @p player.
 *
 * @param ctx The solver context.
 * @param node A node.
 * @param position A position.
 * @param player A player.
 * @return Z3_ast The variable.
 */
Z3_ast variable_count(Z3_context ctx, int node, int position, int player)
{
    char name[40];
    snprintf(name, 40, "M[%d,%d - %d]", node, position, player);
    return mk_bool_var(ctx, name);
}

// -------------------------------- DEBUT EST PARTITION --------------------------------

// 5.1.2
Z3_ast disjunction_for_node(Z3_context ctx, int node, int num_players)
{
    Z3_ast player_clauses[num_players];

    for (int player = 0; player < num_players; ++player)
    {
        player_clauses[player] = variable_node_associated_to_player(ctx, node, player);
    }

    Z3_ast disjunction = Z3_mk_or(ctx, num_players, player_clauses);

    return disjunction;
}

// 5.1.3
Z3_ast conjunction_for_node(Z3_context ctx, int node, int num_players)
{
    Z3_ast player_clauses[num_players * (num_players - 1)];
    int clause_index = 0;
    for (int player1 = 0; player1 < num_players; ++player1)
    {
        for (int player2 = 0; player2 < num_players; ++player2)
        {
            if (player1 != player2)
            {
                Z3_ast p1 = variable_node_associated_to_player(ctx, node, player1);
                Z3_ast p2 = variable_node_associated_to_player(ctx, node, player2);
                Z3_ast p1_p2[2];
                p1_p2[0] = p1;
                p1_p2[1] = p2;

                Z3_ast both_assigned = Z3_mk_and(ctx, 2, p1_p2);

                Z3_ast not_both_assigned = Z3_mk_not(ctx, both_assigned);

                player_clauses[clause_index++] = not_both_assigned;
            }
        }
    }
    Z3_ast conjunction = Z3_mk_and(ctx, clause_index, player_clauses);
    return conjunction;
}

// 5.1.4
Z3_ast isPartition(Z3_context ctx, int num_nodes, int num_players)
{
    Z3_ast clauses[num_nodes];
    for (int node = 0; node < num_nodes; ++node)
    {
        Z3_ast disjunction = disjunction_for_node(ctx, node, num_players);
        Z3_ast conjunction = conjunction_for_node(ctx, node, num_players);

        Z3_ast tab[2];
        tab[0] = disjunction;
        tab[1] = conjunction;
        clauses[node] = Z3_mk_and(ctx, 2, tab);
    }

    Z3_ast result = Z3_mk_and(ctx, num_nodes, clauses);
    return result;
}
// -------------------------------- FIN EST PARTITION --------------------------------


// -------------------------------- DEBUT EST CONNEXE --------------------------------

Z3_ast sommetDiff(Z3_context ctx, int node, int player, int position)
{
    char name[50];
    snprintf(name, 50, "V[%d:%d:%d]", node, player, position);
    return mk_bool_var(ctx, name);
}

// 5.2.2
Z3_ast pasMemePosition(Z3_context ctx, int num_players, int num_nodes, int C)
{
    Z3_ast* clauses = (Z3_ast*)malloc(num_nodes * sizeof(Z3_ast));

    int clause_index = 0;

    for (int j = 0; j < num_players; ++j)
    {
        for (int s = 1; s <= C; ++s)
        {
            for (int s_prime = s + 1; s_prime <= C; ++s_prime)
            {
                for (int i = 0; i < num_nodes; ++i)
                {
                    clauses[clause_index++] = Z3_mk_not(ctx,
                        Z3_mk_and(ctx, 2, (Z3_ast[]){
                                            Z3_mk_eq(ctx, sommetDiff(ctx, i, j, s), Z3_mk_true(ctx)),
                                            Z3_mk_eq(ctx, sommetDiff(ctx, i, j, s_prime), Z3_mk_true(ctx))}));
                }
            }
        }
    }

    Z3_ast result = Z3_mk_and(ctx, clause_index, clauses);

    free(clauses);

    return result;
}

// 5.2.3
Z3_ast auMoinsUnSommetParPosition(Z3_context ctx, int num_players, int num_nodes, int C)
{
    Z3_ast* disjunctions = (Z3_ast*)malloc(num_nodes * sizeof(Z3_ast));

    int disjunction_index = 0;

    for (int j = 0; j < num_players; ++j)
    {
        for (int s = 1; s <= C; ++s)
        {
            Z3_ast* conjunctions = (Z3_ast*)malloc(num_nodes * sizeof(Z3_ast));

            int conjunction_index = 0;

            for (int i = 0; i < num_nodes; ++i)
            {
                conjunctions[conjunction_index++] = sommetDiff(ctx, i, j, s);
            }

            disjunctions[disjunction_index++] = Z3_mk_or(ctx, conjunction_index, conjunctions);

            free(conjunctions);
        }
    }

    Z3_ast result = Z3_mk_and(ctx, disjunction_index, disjunctions);

    free(disjunctions);

    return result;
}

// -------------------------------- FIN EST CONNEXE --------------------------------

// ---------------------- DEBUT EST EQUITABLE -------------------------------------------

// 5.3.1
Z3_ast auPlusUnSommet(Z3_context ctx, int num_players, int num_nodes, int C)
{
    Z3_ast *clauses = (Z3_ast *)malloc(C * num_players * sizeof(Z3_ast));

    int clause_index = 0;
    for (int player = 0; player < num_players; ++player)
    {
        for (int pos = 1; pos <= C; ++pos)
        {
            Z3_ast vertex_cover_clause = Z3_mk_false(ctx);

            for (int node = 0; node < num_nodes; ++node)
            {
                Z3_ast is_s_th_vertex = variable_count(ctx, node, pos, player);
                Z3_ast tab[2] = {vertex_cover_clause, is_s_th_vertex}; 
                vertex_cover_clause = Z3_mk_or(ctx, 2, tab);
            }

            clauses[clause_index++] = vertex_cover_clause;
        }
    }

    Z3_ast formula = Z3_mk_and(ctx, clause_index, clauses);

    free(clauses);

    return formula;
}

// 5.3.2
Z3_ast auMoinsUnSommet(Z3_context ctx, int C, int Vj, int player) {
    Z3_ast *inner_disjunctions = (Z3_ast *)malloc(C * sizeof(Z3_ast));

    for (int s = 1; s <= C; s++) {
        Z3_ast *inner_disjunction_args = (Z3_ast *)malloc(Vj * sizeof(Z3_ast));

        for (int i = 0; i < Vj; i++) {
            inner_disjunction_args[i] = variable_count(ctx, i, s, player);
        }

        inner_disjunctions[s - 1] = Z3_mk_or(ctx, Vj, inner_disjunction_args);

        free(inner_disjunction_args);
    }

    Z3_ast result = Z3_mk_and(ctx, C, inner_disjunctions);

    free(inner_disjunctions);

    return result;
}
// ---------------------- FIN EST EQUITABLE -------------------------------------------


Z3_ast repartition_reduction(Z3_context ctx, const RepartitionGraph graph)
{
    return Z3_mk_false(ctx);

}

void repartition_set_partition_from_model(Z3_context ctx, Z3_model model, RepartitionGraph graph)
{
    int num_nodes = rg_get_num_nodes(graph);
    int num_players = rg_get_num_players(graph);
    for (int node = 0; node < num_nodes; node++)
    {
        for (int player = 0; player < num_players; player++)
        {
            if (value_of_var_in_model(ctx, model, variable_node_associated_to_player(ctx, node, player)))
                rg_set_player_of_node_partition(graph, node, player);
        }
    }
}

void repartition_print_model(Z3_context ctx, Z3_model model, RepartitionGraph graph)
{
    int num_nodes = rg_get_num_nodes(graph);
    int num_players = rg_get_num_players(graph);
    int max_pos = (rg_get_total_weights(graph) - 1) / num_players + 1;
    for (int node = 0; node < num_nodes; node++)
        for (int player = 0; player < num_players; player++)
            for (int pos = 0; pos < max_pos; pos++)
            {
                printf("M[%d,%d - %d] = %d\n", node, pos, player, value_of_var_in_model(ctx, model, variable_count(ctx, node, pos, player)));
            }
    for (int player = 0; player < num_players; player++)
        for (int level = 0; level < num_nodes; level++)
        {
            for (int n1 = 0; n1 < num_nodes; n1++)
                for (int n2 = 0; n2 < num_nodes; n2++)
                {
                    printf("C[%d-%d:%d;%d] = %d\n", n1, n2, level, player, value_of_var_in_model(ctx, model, variable_floyd_warshall(ctx, n1, n2, level, player)));
                }
        }
    for (int node = 0; node < num_nodes; node++)
        for (int player = 0; player < num_players; player++)
            printf("V[%d:%d] = %d\n", node, player, value_of_var_in_model(ctx, model, variable_node_associated_to_player(ctx, node, player)));
}