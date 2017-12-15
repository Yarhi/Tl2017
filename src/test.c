#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "fa.h"

/*
    toutes les fonctions ont été testées
    les résultats importants se trouvent dans le fichier sortie
    - affichage d'un automate
    - rendre un automate complet
    - produit de 2 automates
    - transformer un automate non déterministe en un automate équivalent déterministe
    - inclusion d'un automate dans un autre
    - ainsi que d'autres résultats

*/

int main ( int argc, char **argv )
{
    fa *fa1=malloc(sizeof(fa));
    graph *g=malloc(sizeof(graph));
    FILE* sortie=NULL;
    FILE* graph=NULL;

    // création d'un automate
    fa_create(fa1,2,5);

    // initialisation des états initiaux et finaux
    fa_set_state_initial(fa1,0);
    fa_set_state_initial(fa1,1);
    fa_set_state_final(fa1,1);
    fa_set_state_final(fa1,4);

    // initialisation de la table des transitions
    fa_add_transition(fa1,0,'a',1);
    fa_add_transition(fa1,0,'a',2);
    fa_add_transition(fa1,0,'a',3);
    fa_add_transition(fa1,1,'b',3);
    fa_add_transition(fa1,2,'a',3);
    fa_add_transition(fa1,2,'b',4);
    fa_add_transition(fa1,3,'a',3);
    fa_add_transition(fa1,3,'b',4);
    fa_add_transition(fa1,4,'a',4);

    // affichage de l'automate
    sortie = fopen("sortie.txt","w");
    fprintf(sortie,"\n\nL'automate du TP (fa1) = \n\n");
    fa_pretty_print(fa1,sortie);

    // afficher nombre de transitions
    fprintf(sortie,"\nNombre de transitions de fa1 = %zu\n",fa_count_transitions(fa1));

    // tester si fa1 est déterministe
    if (fa_is_deterministic(fa1)) fprintf(sortie,"Automate fa1 déterministe = \n");
    else fprintf(sortie,"Automate fa1 non déterministe\n");

    // tester si fa1 est complet
    if (fa_is_complete(fa1)) fprintf(sortie,"Automate fa1 complet\n");
    else fprintf(sortie,"Automate fa1 non complet\n");
    if (fa_is_language_empty(fa1)) fprintf(sortie,"Langage (fa1) vide\n");
    else fprintf(sortie,"Langage (fa1) non vide\n");

    fprintf(sortie,"\nL'Automate précédent (fa1) transformé en automate complet = \n\n");
    // compléter fa1
    fa_make_complete(fa1);
    fa_pretty_print(fa1,sortie);

    // fa1 détruit
    fa_destroy(fa1);

    // fa1 recréé
    fa_create(fa1,2,5);

    // initialisation des états initiaux et finaux
    fa_set_state_initial(fa1,0);
    fa_set_state_initial(fa1,1);
    fa_set_state_final(fa1,1);
    fa_set_state_final(fa1,4);

    // initialisation de la table des transitions
    fa_add_transition(fa1,0,'a',1);
    fa_add_transition(fa1,0,'a',2);
    fa_add_transition(fa1,0,'a',3);
    fa_add_transition(fa1,1,'b',3);
    fa_add_transition(fa1,2,'a',3);
    fa_add_transition(fa1,2,'b',4);
    fa_add_transition(fa1,3,'a',3);
    fa_add_transition(fa1,3,'b',4);
    fa_add_transition(fa1,4,'a',4);

    graph = fopen("graph.dot", "w+");
    fa_dot_print(fa1, graph);

    // création du graphe
    graph_create_from_fa(g,fa1,false);
    // affichage graphe
    fprintf(sortie,"\nGraphe correspondant à l'automate (fa1) non complet précédent = \n\n");
    node *first_g;
    for (size_t i=0; i < g->node_count; i++) {
        first_g = g->A[i];
        while (first_g != NULL) {
            fprintf(sortie,"%zu   ",first_g->state);
            first_g = first_g->next;
        }
        fprintf(sortie,"\n");
    }

    // affichage en profondeur
    bool *visited;
    visited=malloc(g->node_count*sizeof(bool));
    for (size_t i=0; i < g->node_count; i++) visited[i] = false;
    graph_depth_first_search(g, 0, visited);


    // création et affichage dans sortie de lsh, rhs et produit

    fa *lhs=malloc(sizeof(fa));
    fa_create(lhs,2,2);
    fa_set_state_initial(lhs,0);
    fa_set_state_final(lhs,1);
    fa_add_transition(lhs,0,'a',1);
    fa_add_transition(lhs,1,'a',1);
    fa_add_transition(lhs,1,'b',1);
    fprintf(sortie,"\n\nL'automate lhs = \n\n");
    fa_pretty_print(lhs,sortie);
    
    fa *rhs=malloc(sizeof(fa));
    fa_create(rhs,2,2);
    fa_set_state_initial(rhs,0);
    fa_set_state_final(rhs,1);
    fa_add_transition(rhs,0,'a',0);
    fa_add_transition(rhs,0,'b',1);
    fa_add_transition(rhs,1,'a',0);
    fa_add_transition(rhs,1,'b',1);
    fprintf(sortie,"\n\nL'automate rhs = \n\n");
    fa_pretty_print(rhs,sortie);

    // le produit
    fa *prod=malloc(sizeof(fa));
    fa_create_product(prod,lhs,rhs);
    fprintf(sortie,"\n\nL'automate produit (prod) de lhs et rhs = \n\n");
    fa_pretty_print(prod,sortie);

    // intersection vide ou non
    if ( fa_has_empty_intersection(lhs,rhs) ) fprintf(sortie,"\nIntersection lhs et rhs vide\n");
    else fprintf(sortie,"\nIntersection lhs et rhs non vide\n");

    // déterminiser l'automate du cours nfa
    // création de l'automate nfa
    fa *nfa=malloc(sizeof(fa));
    fa_create(nfa,2,5);

    // initialisation des états initiaux et finaux
    fa_set_state_initial(nfa,0);
    fa_set_state_final(nfa,4);
    // initialisation de la table des transitions
    fa_add_transition(nfa,0,'a',2);
    fa_add_transition(nfa,0,'a',3);
    fa_add_transition(nfa,0,'b',1);
    fa_add_transition(nfa,0,'b',2);
    fa_add_transition(nfa,1,'b',4);
    fa_add_transition(nfa,2,'a',2);
    fa_add_transition(nfa,2,'a',3);
    fa_add_transition(nfa,2,'b',1);
    fa_add_transition(nfa,2,'b',2);
    fa_add_transition(nfa,3,'a',4);
    fa_add_transition(nfa,4,'a',4);
    fa_add_transition(nfa,4,'b',4);
    fprintf(sortie,"\n\nL'automate non déterministe du cours (nfa) = \n\n");
    fa_pretty_print(nfa,sortie);

    // dfa: résultat = automate déterministe
    fa *dfa=malloc(sizeof(fa));
    fa_create_deterministic(dfa,nfa);
    fprintf(sortie,"\n\nL'automate du cours déterminisé (dfa) = \n\n");
    fa_pretty_print(dfa,sortie);

    // pas d'Inclusion de nfa dans fa1
    if ( fa_is_included(nfa,fa1) ) fprintf(sortie,"\nInclusion de nfa dans fa1\n");
    else fprintf(sortie,"\nPas d'Inclusion de nfa dans fa1\n");

    /*
    // autre exemple d'inclusion
    // automate A
    fa *A=malloc(sizeof(fa));
    fa_create(A,2,2);
    // initialisation des états initiaux et finaux
    fa_set_state_initial(A,0);
    fa_set_state_final(A,1);
    // initialisation de la table des transitions
    fa_add_transition(A,0,'a',0);
    fa_add_transition(A,0,'b',1);
    fa_add_transition(A,1,'a',1);
    fa_add_transition(A,1,'b',1);
    fprintf(sortie,"\n\nL'automate A du cours (A) = \n\n");
    fa_pretty_print(A,sortie);

    // automate B
    fa *B=malloc(sizeof(fa));
    fa_create(B,2,3);
    // initialisation des états initiaux et finaux
    fa_set_state_initial(B,0);
    fa_set_state_final(B,1);
    fa_set_state_final(B,2);
    // initialisation de la table des transitions
    fa_add_transition(B,0,'a',0);
    fa_add_transition(B,0,'b',1);
    fa_add_transition(B,1,'a',1);
    fa_add_transition(B,1,'b',2);
    fa_add_transition(B,2,'a',2);
    fa_add_transition(B,2,'b',2);
    fprintf(sortie,"\n\nL'automate B = \n\n");
    fa_pretty_print(B,sortie);

    if ( fa_is_included(A,B) ) fprintf(sortie,"\nInclusion de A dans B\n");
    else fprintf(sortie,"\nPas d'Inclusion de A dans B\n");
    */

    fclose(sortie); // fermer fichier sortie

    // destruction de tous les automates et des graphes
    fa_destroy(fa1);
    fa_destroy(lhs);
    fa_destroy(rhs);
    fa_destroy(prod);
    fa_destroy(nfa);
    fa_destroy(nfa);

    graph_destroy(g);

}