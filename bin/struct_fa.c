#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cuda_runtime.h>


typedef struct fa fa;
typedef struct state_node state_node;
typedef struct state_set state_set;

// Structure de l'automate
struct fa 
{
	size_t 	alpha_count;				// Taille de l'alphabet
	size_t 	state_count;				// Le nombre d'états

	bool* initial_states;				// Liste des noeuds état initiaux
	bool* final_states; 				// Liste des noeufs en état finaux

	state_set **transitions;	        // Contient l'ensemble des transitions de l'automate
    state_set *list;                    // Liste chainée
};

/*
 *	Liste chainée
 */

// Noeud de la liste chainée
struct state_node 
{
	size_t	state;						
	state_node* next;			        // Prochains noeud
};

// Liste chainée
struct state_set
{
	state_node* first;			        // Tête de liste
};



/*
 * Functions
 */

void print_autom( fa *self )
{
    state_node *node = self->list->first;

    while ( node->next )
    {
        printf("%d\n", node->state);
        node = node->next;
    }
}

// Créé un automate passé en paramètre
void fa_create ( fa *self, size_t alpha_count, size_t state_count )
{
    int i = -1;

    self->state_count = state_count;
    self->alpha_count = alpha_count;

    self->final_states = malloc(sizeof(bool*));
    self->initial_states = malloc(sizeof(bool*));

    state_set *list     = malloc(sizeof(state_set));
    state_node *head    = malloc(sizeof(state_node));

    head->state = 0;
    head->next = NULL;

    list->first = head;
    self->list = list;

    while ( i++, i < state_count )
    {
        state_node *new = malloc(sizeof(state_node));
        new->state = (size_t) state_count-i-1;
        new->next = self->list->first;
        self->list->first = new;
    }

}


// Rend un état initial
void fa_set_state_initial ( fa *self, size_t state )
{
    self->initial_states[state] = true;
}

// Rend un état final
void fa_set_state_final( fa *self, size_t state )
{
    self->final_states[state] = true;
}

// Ajoute une transition à l'automate
void fa_add_transition( fa *self, size_t from, char alpha, size_t to )
{
    s
}



int main ( int argc, char **argv )
{
    fa *autom = malloc(sizeof(fa));

	fa_create(autom, 5, 4);
    print_autom( autom );

    fa_set_state_final(autom, 3);

	return 0;
}