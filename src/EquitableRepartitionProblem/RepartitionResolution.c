#include "RepartitionResolution.h"
#include <stdlib.h>
#include <stdio.h>

bool repartition_brute_force(RepartitionGraph graph)
{
    printf("Brute Force not implemented\n");
    return false;
    /*À remplacer par votre implémentation du brute-force.
    Cette fonction doit probablement simplement appeler une fonction récursive construisant pas à pas une partition, puis appelant un vérificateur sur chaque partition complète.
    Votre vérificateur devrait être séparé de cette fonction récursive (sauf si vous faites des vérifications sur des partitions partielles, mais cela me semble peu adéquat ici). Votre vérificateur devant vérifier deux propriétés différentes, il vous est conseillé de faire une fonction pour chacune de ces propriétés.
    Les fonctions de RepartitionGraph.h utilisées dans notre solution de cette résolution sont les suivantes :
    rg_get_num_nodes, rg_get_num_players, rg_is_edge, rg_get_player_of_node_in_partition, rg_set_player_of_node_partition.
    Si vous sentez le besoin de fonctions qui ne sont pas dans cette liste, vous êtes potentiellement en train de vous planter (rg_reset_partition pouvant cependant être utile, mais on peut faire sans).
    Notre implémentation (sans ce bloc de commentaire), porte ce fichier à 94 lignes. Cette valeur n’est évidemment qu’une indication, mais si vous dépassez les 300 lignes, il est probable que vous vous compliquiez la vie.
    */
}