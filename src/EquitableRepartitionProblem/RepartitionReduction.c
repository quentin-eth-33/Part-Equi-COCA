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


Z3_ast repartition_reduction(Z3_context ctx, const RepartitionGraph graph)
{
    printf("Reduction not implemented\n");
    return Z3_mk_false(ctx);
    /*À remplacer par votre implémentation.
    La fonction doit renvoyer une formule de la logique propositionnelle qui encode l’existence d’une partition connexe équitable.
    On ne demande pas que la formule produite soit en CNF (Z3 se débrouillera), on recommande d’ailleurs qu’elle ne le soit pas pour être plus lisible.
    Découpez votre code en petites fonctions auxiliaires générant des formules simples ou connectant des formules venant d’autres fonctions. Basez-vous sur le sujet pour le découpage (encore pour la visibilité et votre confort de débuggage).
    Faites attention aux tailles dans les fonctions Z3_mk_and et Z3_mk_or (source de bug -- si vous avez des erreurs se plaignant de code qui ne devrait pas être atteint, cela vient probablement de là). Reportez-vous à la documentation <https://z3prover.github.io/api/html/group__capi.html>.
    Un Z3_ast représente une formule de la logique propositionnel.
    Vous aurez évidemment besoin de tableaux contenant des Z3_ast.
    Utilisez les fonctions fournies en tête de ce fichier pour générer les variables propositionnelles nécessaires à votre formule.
    Dans la réalisation de cette fonction pour la solution du projet, les fonctions de Z3 utilisées sont les suivantes :
    Z3_mk_and, Z3_mk_or, Z3_mk_implies, Z3_mk_eq, Z3_mk_not
    La fonction uniqueFormula de Z3Tools.h (du présent projet) a également été utilisée.
    Les fonctions de RepartitionGraph.h utilisées sont :
    rg_get_num_nodes, rg_get_num_players, rg_is_edge, rg_get_weight, rg_get_total_weights.
    Si vous êtes tentés d’utiliser d’autres fonctions que celles listées ci-dessus, vous êtes probablement en train de vous tromper.
    Pour information, l’implémentation réalisée comme solution porte ce fichier à 180 lignes -- sans ce bloc de commentaires (ce qui est totalement indicatif, mais si vous obtenez 400 lignes, vous êtes potentiellement en train de vous compliquer la vie pour rien).

    Procédez par étape, tranquillement, en compilant régulièrement, et regardez les formules générées grâce à l’option -F du programme principal (en les renvoyant dans cette présente fonction).

    Le reste du fichier vous est fourni : les fonctions en haut du fichier sont celles qui génèrent les variables nécessaires. Celle suivantes permettent de décoder et d’afficher une valuation obtenue à partir d’une formule satisfaite. La résolution de la formule est effectuée dans la fonction main (main.c), vous n’avez pas à le faire.
    */
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