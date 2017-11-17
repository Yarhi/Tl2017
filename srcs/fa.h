#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

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

	state_node **transitions;	// Contient l'ensemble des transitions de l'automate
    state_set *list;             // Liste chainée
};

/*
 *	Liste chainée
 */

// Noeud de la liste chainée
struct state_node 
{
	size_t	state;
	char letter;						
	state_node* next;			// Prochains noeud
};

// Liste chainée
struct state_set
{
	state_node* first;			// Tête de liste
};

//Création d'un automate
void fa_create ( fa *self, size_t alpha_count, size_t state_count );
//Destruction d'un automate
void fa_destroy(fa *self);


//Rendre un état initial et final 
void fa_set_state_initial(struct fa *self, size_t state);
void fa_set_state_final(struct fa *self, size_t state);

//Ajouter un transition
void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to);
//Supprimer une transition
void fa_remove_transition(struct fa *self, size_t from, char alpha, size_t to);

//Afficher un automate
void fa_pretty_print(const struct fa *self, FILE *out);


// Afficher un automate en DOT
void fa_dot_print ( const fa *self, FILE *out );
// Réponse 3.2: fonction pour supprimer un état
void fa_remove_state(struct fa *self, size_t state);

// Réponse 3.3: fonction qui compte le nombre de transitions d'un automate
size_t fa_count_transitions(const struct fa *self);

// Réponse 3.4: fonction qui établit si un automate est déterministe
bool fa_is_deterministic(const struct fa *self);

// Réponse 3.5: fonction qui établit si l'automate est complet
bool fa_is_complete(const struct fa *self);

// Réponse 3.6: fonction qui complète un automate
void fa_make_complete(struct fa *self);







