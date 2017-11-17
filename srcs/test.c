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
    // suppression d'une transition
   // fa_remove_transition(fa1,0,'a',1);
   
   // suppresion d'un état
   // fa_remove_state(fa1,4);

   // afficher nombre de transitions
   printf("%zu\n",fa_count_transitions(fa1));

   // tester si fa1 est déterministe
   if (fa_is_deterministic(fa1)) printf("Automate déterministe\n");
   else printf("Automate non déterministe\n");

   // tester si fa1 est complet
   if (fa_is_complete(fa1)) printf("Automate complet\n");
   else printf("Automate non complet\n");
    // affichage de l'automate
    sortie = fopen("./files/sortie.txt","w");
    fa_pretty_print(autom,sortie);

    dot = fopen("../files/graph_test.dot", "w");
    fa_dot_print(autom, dot);

    system("dot -Tpng ../files/graph_test.dot > ../files/output.png");
    fclose(sortie);
    //Destruction de l'automate
    fa_destroy(autom);


	return 0;
}
