// Réponse 1: les structures de données

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//TP1

typedef struct fa fa;
typedef struct state_node state_node;
typedef struct state_set state_set;
typedef struct node node;
typedef struct graph graph;

// Structure de l'automate

struct fa
{
	size_t 	alpha_count;		// Taille de l'alphabet
	size_t 	state_count;		// Le nombre d'états

	bool* initial_states;		// Liste des noeuds état initiaux
	bool* final_states; 		// Liste des noeuds en état finaux

	state_node **transitions;	// Contient l'ensemble des transitions de l'automate
	state_set *list;            // Liste chainée des états
};

// un état
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

// Réponse 4.1: Structure du graphe orienté par une liste d'adjacence
struct graph {
	size_t node_count;  // nombre de noeuds dans le graphe
	node **A; // liste d'adjacence
};

struct node {
	size_t	state; // numéro état
	node* next;	   // état suivant
};


// Réponse 2.1: fonction création d'un automate
void fa_create ( fa *self, size_t alpha_count, size_t state_count );

// Réponse 2.2: fonction de destruction d'un automate
void fa_destroy(struct fa *self);

// Réponse 2.3: fonctions pour état initial 
void fa_set_state_initial(struct fa *self, size_t state);

// Réponse 2.3: fonctions pour état final
void fa_set_state_final(struct fa *self, size_t state);

// Réponse 2.4: fonction ajout une transition
void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to);

// Réponse 2.5: fonction pour afficher un automate
void fa_pretty_print(const struct fa *self, FILE* out);

void fa_dot_print ( const fa *self, FILE *out );

//TP2

// Réponse 3.1: fonction pour supprimer une transition
void fa_remove_transition(struct fa *self, size_t from, char alpha, size_t to);

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

// TP3

// Réponse 4.2: fonction profondeur à partir d'un état
void graph_depth_first_search(const struct graph *self,
							  size_t state, bool *visited);

// Réponse 4.3: fonction qui détermine si un chemin existe entre deux états
bool graph_has_path(const struct graph *self, size_t from, size_t to);

// Réponse 4.4: construction d'un graphe à partir d'un automate
void graph_create_from_fa(struct graph *self, const struct fa *fa, bool inverted);

// Réponse 4.5: fonction qui détruit un graphe
void graph_destroy(struct graph *self);

// Réponse 4.6: ne fonction qui détermine si un automate accepte le langage vide
bool fa_is_language_empty(const struct fa *self);

// Réponse 5.1: fonction qui supprime tous les états qui ne sont pas accessibles
void fa_remove_non_accessible_states(struct fa *self);

// Réponse 5.2: fonction qui supprime tous les états qui ne sont pas co-accessibles
void fa_remove_non_co_accessible_states(struct fa *self);

// Réponse 6.1: fonction qui crée un automate produit à partir de deux automates
void fa_create_product(struct fa *self, const struct fa *lhs, const struct fa *rhs);

// Réponse 6.2: fonction qui détermine si l’intersection entre deux automates est vide ou pas
bool fa_has_empty_intersection(const struct fa *lhs, const struct fa *rhs);

// Réponse 7.1: fonction qui crée un automate déterministe à partir d’un automate non-déterministe.
void fa_create_deterministic(struct fa *self, const struct fa *nfa);

// Réponse 7.2: fonction qui détermine si un langage accepté par un
// automate est inclus dans un autre langage accepté par un autre automate. 
bool fa_is_included(const struct fa *lhs, const struct fa *rhs);