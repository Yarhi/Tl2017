#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "fa.h"




/*void print_autom( fa *self )
{
    state_node *node = self->list->first;

    while ( node->next )
    {
        printf("%ld\n", node->state);
        node = node->next;
    }
}*/

int main ( int argc, char **argv )
{
    fa *autom = malloc(sizeof(fa));
    FILE* sortie=NULL;
    FILE* dot=NULL;


    fa_create(autom, 2, 5);
    //print_autom( autom );
    //Initialisation des états initiaux et finaux
    fa_set_state_initial(autom, 0);
    fa_set_state_initial(autom, 1);
    fa_set_state_final(autom,1);
    fa_set_state_final(autom,4);
    //Initialisation de la table des transitions
    fa_add_transition(autom,0,'a',1);
    fa_add_transition(autom,0,'a',2);
    fa_add_transition(autom,0,'a',3);
    fa_add_transition(autom,1,'b',3);
    fa_add_transition(autom,2,'a',3);
    fa_add_transition(autom,2,'b',4);
    fa_add_transition(autom,3,'a',3);
    fa_add_transition(autom,3,'b',4);
    fa_add_transition(autom,4,'a',4);
    fa_remove_transition(autom,0,'a',2);
    // affichage de l'automate
    sortie = fopen("./sortie.txt","w");
    fa_pretty_print(autom,sortie);

    dot = fopen("../graph_test.dot", "w");
    fa_dot_print(autom, dot);

    system("dot -Tpng ../graph_test.dot > ../output.png");
    fclose(sortie);
    //Destruction de l'automate
    fa_destroy(autom);


	return 0;
}
