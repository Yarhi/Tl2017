#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "fa.h"

/*******************************************************************
                          TP1
********************************************************************/

// Réponse 2.1: création d'au automate
void fa_create ( fa *self, size_t alpha_count, size_t state_count )
{
    self->state_count = state_count;  // initialiser le nombre d'états
    self->alpha_count = alpha_count;  // initialiser la taille de l'alphabet

    // initialisation des états initiaux et finaux à faux
    self->initial_states = malloc(state_count*sizeof(bool));
    self->final_states = malloc(state_count*sizeof(bool));
    for(size_t i=0; i < state_count; i++) {
        self->initial_states[i] = false;
        self->final_states[i] = false;
    }

    // création d'une liste des états pointée par le champ list de l'automate
    state_set *list     = malloc(sizeof(state_set));
    state_node *head    = malloc(sizeof(state_node)); // création du premier état
    head->state = 0;
    head->next = NULL;
    list->first = head;
    self->list = list;
    size_t i = 0;
    state_node *new;
    while (i++, i < state_count )
    {
        new = malloc(sizeof(state_node));  // création des états
        new->state = i;
        new->next = NULL;
        head->next = new;
        head = new;
    }

    // allocation de la table des transitions
    self->transitions = malloc(state_count*sizeof(*(self->transitions)));
    for (size_t i=0; i < state_count; i++)
        self->transitions[i] = NULL;
}

// Réponse 2.2: fonction de destruction d'un automate
void fa_destroy(struct fa *self) {

    free(self->initial_states);
    self->initial_states = NULL;
    free(self->final_states);
    self->final_states = NULL;

    // free: ensemble des états
    state_node *new,*head;
    head = self->list->first;
    for(size_t i=0; i < self->state_count; i++) {
        new = head->next;
        free(head);
        head = new;
    }

    // free: table de transitions
    for(size_t i=0; i < self->state_count; i++) {
        head = self->transitions[i];
        while (head != NULL) {
            new = head->next;
            free(head);
            head = new;
        }
    }
    free(self->transitions);
    self->transitions = NULL;

    self->alpha_count = 0;
    self->state_count = 0;
}

// Réponse 2.3: fonction pour état initial
void fa_set_state_initial(struct fa *self, size_t state) {

    self->initial_states[state] = true;

}

// Réponse 2.3: fonction pour état final
void fa_set_state_final(struct fa *self, size_t state) {

    self->final_states[state] = true;

}

// Réponse 2.4: ajouter une transtion
void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to) {

    state_node *begin,*new,*previous;

    begin = self->transitions[from];
    new = malloc(sizeof(state_node));
    new->state = to;
    new->letter = alpha;
    if (begin == NULL) {
        new->next = NULL;
        self->transitions[from] = new;
    } else { previous = begin;
        while ( begin != NULL && (alpha >= begin->letter)) {
            previous = begin;
            begin = begin->next;
        }
        if (previous == begin) { // un seul noeud
            begin->next = NULL;
            self->transitions[from] = new;
            new->next = begin;
        } else {
            new->next = begin;
            previous->next = new;
        }
    }
}

// Réponse 2.5: fonction pour afficher un automate
void fa_pretty_print(const struct fa *self, FILE *out) {
    int i;

    // affichage des états initiaux
    fprintf(out,"Initial states:\n         ");
    for(i = 0; i < self->state_count; i++) {
        if (self->initial_states[i] == true)
            fprintf(out,"%d ",i);
    }

    // affichage des états finaux
    fprintf(out,"\nFinal states:\n         ");
    for(i = 0; i < self->state_count; i++) {
        if (self->final_states[i] == true)
            fprintf(out,"%d ",i);
    }

    // affichage des transitions
    state_node *begin;
    char c;

    fprintf(out,"\nTransitions:\n");
    for (size_t i=0; i < self->state_count; i++) {
        if (self->transitions[i] != NULL){
            fprintf(out,"   For state %zu:\n", i);
            begin = self->transitions[i];
            for (int j=0; j < self->alpha_count; j++) {
                c = toascii(j+97);  // conversion d'un entier en caractère ascii
                fprintf(out,"           For Letter %c: ",c);
                while ((begin != NULL) && (c == begin->letter)) {
                    fprintf(out,"%zu ",begin->state);
                    begin = begin->next;
                }
                fprintf(out,"\n");
            }
        }
    }
}


void fa_dot_print ( const fa *self, FILE *out )
{
    state_node *transitions;
    char c;

    fprintf( out, "digraph finite_state_machine {\n" );

    fprintf( out, "\trankdir=LR;\n" );
    fprintf( out, "\tsize=\"8,5\"\n" );

    // Final state node -- START
    fprintf ( out, "\tnode [shape = doublecircle]; " );

    for(int i = 0; i < self->state_count; i++)
    {
        if (self->final_states[i] == true)
            fprintf(out,"LR_%i ",i);
    }
    fprintf(out, ";\n");
    // Final state node -- END

    fprintf( out, "\tnode [shape = circle];\n" );

    // Transitions -- START

    for ( int i = 0; i < self->state_count; i++ )
    {
        transitions = self->transitions[i];

        while ( transitions != NULL )
        {
            c = (char) toascii(transitions->letter);
            fprintf( out, "\tLR_%i -> LR_%zu [label = \"%c\" ];\n", i, transitions->state, c);
            transitions = transitions->next;
        }
    }

    // Transitions -- END

    fprintf( out, "}" );

}

/*******************************************************************
                          TP2
********************************************************************/

// Réponse 3.1: fonction pour supprimer une transition
void fa_remove_transition(struct fa *self, size_t from, char alpha, size_t to) {
    state_node *begin, *preview;

    begin = self->transitions[from];
    preview = begin;
    while (begin != NULL ) {
        if (alpha != begin->letter || to != begin->state) {
            preview = begin;
            begin = begin->next;
        } else { // suppression
            // si preview = begin alors c'est le premier de la liste
            if (preview == begin) self->transitions[from] = begin->next;
            else preview->next = begin->next;
            free(begin);
        }
    }

}

// Réponse 3.2: fonction pour supprimer un état
void fa_remove_state(struct fa *self, size_t state) {
    state_node *first,*next;
    // suppression des transitions sortantes de state
    first = self->transitions[state];
    while ( first != NULL ) {
        next = first->next;
        free(first);
        first = next;
    }
    self->transitions[state] = NULL;

    // suppression des transitions entrantes à state
    char c;
    for (size_t i=0;i < self->state_count; i++) {
        first = self->transitions[i];
        while ( first != NULL) {
            if ( first->state == state) {
                //for (int j=0; j < self->alpha_count; j++) {
                // c = toascii(j+97);
                c = first->letter;
                fa_remove_transition(self, i, c,state);
                //}
            }
            first=first->next;
        }
    }
    self->state_count--;

}

// Réponse 3.3: fonction qui compte le nombre de transitions d'un automate
size_t fa_count_transitions(const struct fa *self){

    size_t count = 0;
    state_node *first;
    for (size_t i=0; i < self->state_count; i++) {
        first = self->transitions[i];
        while ( first != NULL) {
            count = count + 1;
            first = first->next;
        }
    }
    return(count);

}

// Réponse 3.4: fonction qui établit si un automate est déterministe
bool fa_is_deterministic(const struct fa *self){

    // calculer le nombre d'états initiaux
    size_t n = 0;
    for(size_t i=0; i < self->state_count; i++)
    {
        if (self->initial_states[i] == true) n++;
    }
    if (n > 1)
        return(false); // plusieurs états initiaux

    // parcourir la table des transitions
    // pour chaque état voir s'il y a au moins 2 transitions avec la même lettre
    bool equal;
    state_node *first;
    for (size_t i=0; i < self->state_count; i++) {
        first = self->transitions[i];
        equal = false;
        while ((first != NULL) && (first->next != NULL) && !equal) {
            if (first->letter == (first->next)->letter) {
                // 2 transitions sortantes de i avec la même lettre
                equal = true;
                return(false);
            } else first = first->next;
        }
    }
    return(true);
}


// Réponse 3.5: fonction qui établit si l'automate est complet
bool fa_is_complete(const struct fa *self) {

    state_node *first;
    for (size_t i=0; i < self->state_count; i++) {
        first = self->transitions[i];
        char c = 'a';
        for (size_t j=0; j < self->alpha_count; j++){
            bool find = false;
            while (first != NULL && !find){
                if (c == first->letter) find = true;
                else first = first->next;
            }
            if (!find) return(false);
            c = c + 1;
        }
    }
    return(true);

}

// Réponse 3.6: fonction qui complète un automate
void fa_make_complete(struct fa *self){

    if (!fa_is_complete(self)){
        // compléter l'automate

        state_node *first;
        size_t puits=self->state_count;

        for (size_t i=0; i < self->state_count; i++) {
            first = self->transitions[i];
            char c = 'a';
            for (size_t j=0; j < self->alpha_count; j++){
                // chercher c
                while (first != NULL && first->letter != c) {
                    first = first->next;
                }
                if (first == NULL) { // c non trouvé alors à ajouter
                    fa_add_transition(self,i,c,puits);
                }
                first = self->transitions[i];
                c = c + 1;
            }
        }
    }

}

/*******************************************************************
                          TP3
********************************************************************/

// Réponse 4.2: fonction profondeur à partir d'un état
void graph_depth_first_search(const struct graph *self, size_t state, bool *visited) {

    node *first;
    size_t adjacent;

    first = self->A[state];
    visited[state] = true;
    while (first != NULL){
        adjacent = first->state; // prendre l'ajacent de state
        if (!visited[adjacent]) { // état déjà visité
            graph_depth_first_search(self,adjacent,visited);
        }
        first = first->next;  // passer au suivant
    }

}

// Réponse 4.3: fonction qui détermine si un chemin existe entre deux états
bool graph_has_path(const struct graph *self, size_t from, size_t to){

    // création et initialisation de visited
    bool *visited;
    visited=malloc(self->node_count*sizeof(bool));
    for (size_t i=0; i < self->node_count; i++) visited[i] = false;

    // voir si existe un chemin d from à to
    graph_depth_first_search(self, from, visited);
    if (visited[to]) return true; else return false;

}

// Réponse 4.4: construction d'un graphe à partir d'un automate
void graph_create_from_fa(struct graph *self, const struct fa *fa, bool inverted) {

    // allocation du tableau A: liste d'adjacence
    self->node_count = fa->state_count;
    self->A = malloc(self->node_count*sizeof(*(self->A)));
    for (size_t i=0; i < self->node_count; i++)
        self->A[i] = NULL; // initialisation à NULL

    // création et intialisation de la liste d'adjacence
    // en lisant l'automate état par état
    state_node *first_fa;
    node *first_g,*new, *preview;
    for (size_t i=0; i < fa->state_count; i++) {
        first_fa = fa->transitions[i];
        while (first_fa != NULL) {
            new = malloc(sizeof(node));
            new->state = first_fa->state;
            if (self->A[i] == NULL) self->A[i] = new;
            else {
                first_g = self->A[i];
                while (first_g != NULL) {
                    preview = first_g;
                    first_g = first_g->next;
                }
                preview->next = new;
            }
            new->next = NULL;
            first_fa = first_fa->next;
        }
    }

}

// Réponse 4.5: fonction qui détruit un graphe
void graph_destroy(struct graph *self) {

    node *head,*new;
    // des truction de toutes les listes associées à chaque état
    for(size_t i=0; i < self->node_count; i++) {
        head = self->A[i];
        while (head != NULL) {
            new = head->next;
            free(head);
            head = new;
        }
    }
    // destruction de la liste principale
    free(self->A);
    self->A = NULL;

}

// Réponse 4.6: fonction qui détermine si un automate accepte le langage vide
bool fa_is_language_empty(const struct fa *self) {

    // crétaion du graphe à partir de fa
    graph *g=malloc(sizeof(graph));
    graph_create_from_fa(g, self, false);

    // vérifier si existe un chemin entre un état initial et un état final
    for (size_t i=0; i < self->state_count; i++) {
        if (self->initial_states[i]) {
            for (size_t j=0; j < self->state_count; j++) {
                if (self->final_states[j])
                    if (graph_has_path(g, i, j)) return false; // chemin de i à j
            }
        }
    }
    return true;

}

// Réponse 5.1: fonction qui supprime tous les états qui ne sont pas accessibles
void fa_remove_non_accessible_states(struct fa *self) {

    // crétaion du graphe à partir de fa
    graph *g=malloc(sizeof(graph));
    graph_create_from_fa(g, self, false);
    bool accessible;

    for (size_t i=0; i < self->state_count; i++) {
        accessible=false;
        // tester si l'état i n'est pas accessible à travers tous les états initiaux
        for (size_t j=0; j < self->state_count; j++) {
            if ((i != j) && self->initial_states[j])
                if (graph_has_path(g, j, i)) {
                    accessible = true;
                    break; // il est accessible, il faut passer à un autre état
                }
        }

        if (!accessible) fa_remove_state(self,i); // suppression  de l'état i
    }

}

// Réponse 5.2: une fonction qui supprime tous les états qui ne sont pas co-accessibles
void fa_remove_non_co_accessible_states(struct fa *self) {

    graph *g=malloc(sizeof(graph));
    graph_create_from_fa(g, self, false);
    bool co_accessible;

    for (size_t i=0; i < self->state_count; i++) {
        co_accessible=false;
        // tester si l'état i n'est pas co-accessible vers tous les états finaux
        for (size_t j=0; j < self->state_count; j++) {
            if ((i != j) && self->final_states[j])
                if (graph_has_path(g, i, self->final_states[j])) {
                    co_accessible = true;
                    break; // il est co-accessible, il faut passer à un autre état
                }
        }
        if (!co_accessible) fa_remove_state(self,i); // suppression  de l'état i
    }

}

/*******************************************************************
                          TP4
********************************************************************/

// fonction qui calcule le résultat d'une transition
// elle retourne -1 si il n'y a pas de transtition
int transition(const struct fa *self, size_t state, size_t c) {

    state_node *first = self->transitions[state];

    c = c + 97;
    while (first != NULL) {
        if (first->letter == (char)c) return first->state;
        first = first->next;
    }
    if (first == NULL) return -1;
}

// Réponse 6.1: fonction qui crée un automate produit à partir de deux automates
void fa_create_product(struct fa *self, const struct fa *lhs, const struct fa *rhs) {

    // lhs: automate de gauche et rhs: automate de droite
    // matrice
    const int n1=100, n2=100;  // max de la taille de la matrice
    size_t len_lhs = lhs->state_count;
    size_t len_rhs = rhs->state_count;
    int mat[n1][n2];          // matrice qui contient les états et les transtions du produit
    size_t number[n1*n2];     // tableau qui contient les états du produit
    size_t col = lhs->alpha_count; // ou rhs->alpha_count
    int q1,q2;

    size_t len_number=1; // longueur de number
    size_t num_state;
    size_t i=0,position=1;  // position = dernière ligne de la matrice
    bool end = false;   // end = true si il n'y a plus de nouveaux états dans la matrice

    int q=0,r=0;
    mat[0][0] = 0;   // la colonne 0 de mat va contenir les nouveaux états
    number[0] = 0;

    while ( !end) {
        for (size_t j=1; j <= col; j++ ) {
            // chercher la transition (q,j-1): résultat dans q1 sinon -1
            q1 = transition(lhs,q,j-1);
            // chercher la transition (r,j-1): résultat dans q2 sinon -1
            q2 = transition(rhs,r,j-1);

            if ( (q1 != -1) && (q2 != -1) ) {
                num_state = q1*len_rhs + q2;  // numéro nouvel état: formule du TP
                mat[i][j] = num_state;
                // ajouter ce nouvel état dans le tableau number si il n'existe pas déjà
                // et l'ajouter aussi dans la colonne 0 de mat
                bool found=false;
                size_t l=0;
                while ( !found && l < len_number) {
                    if (number[l] == num_state) found = true;
                    l++;
                }
                if ( !found) {
                    number[len_number] = num_state;  // ajout
                    mat[position][0] = num_state; // la colonne 0 contient le nouvel état du produit
                    position++;
                    len_number++;
                }
            } else mat [i][j] = -1;
        }

        // nouveaux q et r
        q = mat[i+1][0] / len_rhs;  // formule du TP
        r = mat[i+1][0] % len_rhs;  // formule du TP
        i++;

        // test d'arrêt
        if ( i == position ) end = true;  // il n'y a plus de nouveaux états

    }

    // changement des numéros d'états: 0 1 2 3 ...
    for (size_t i =0; i < len_number; i++)
        for (size_t j1=0; j1 < len_number; j1++)
            for (size_t j2=0; j2 <= lhs->alpha_count; j2++)
                if (number[i] == mat[j1][j2]) mat[j1][j2] = i;

    // création de l'automate produit
    fa_create(self,lhs->alpha_count,len_number);
    fa_set_state_initial(self,0);
    fa_set_state_final(self,len_number-1);
    for (size_t i =0; i < len_number; i++) {
        for (size_t j=0; j < lhs->alpha_count; j++)
            if (mat[i][j+1] != -1) fa_add_transition(self,mat[i][0],(char)(j+97),(size_t)mat[i][j+1]);
    }

}


// Réponse 6.2: fonction qui détermine si l’intersection entre deux automates est vide ou pas
bool fa_has_empty_intersection(const struct fa *lhs, const struct fa *rhs) {

    fa *prod=malloc(sizeof(fa));
    fa_create_product(prod,lhs,rhs);
    if ( fa_is_language_empty(prod) ) return true;
    else return false;

}

// fonction qui retourne toutes les transtions dans un tableau
void toutes_transitions(const struct fa *nfa, size_t state, size_t c, int *tout) {

    state_node *first = nfa->transitions[state];

    c = c + 97;
    size_t i=0;
    while (first != NULL) {
        if (first->letter == (char)c) { tout[i] = first->state; i++; }
        first = first->next;
    }
    tout[i] = -1;

}

// fonction qui teste l'égalité de 2 tableaux
bool egaux(size_t taille, int *t1, int *t2) {
    size_t i=0;
    while (t1[i] != -1) {
        size_t j=0;
        while ( (j < taille) && (t1[i] != t2[j])) j++;
        if (j == taille) return false;
        i++;
    }
    return true;

}

// Réponse 7.1: fonction qui crée un automate déterministe à partir d’un automate non-déterministe.
void fa_create_deterministic(struct fa *self, const struct fa *nfa) {

    const size_t taille1=20;  // nombre d'état max de nfa
    size_t taille2 = nfa->alpha_count;
    size_t taille3 = nfa->state_count;
    // tableau 3 dimensions: strcture dynamique pour contenir les ensembles d'états
    int ***ptr;

    // allocation mémoire de toute la structure: 3 dimensions
    ptr = malloc(taille1*sizeof(*ptr));
    for (size_t i=0; i < taille1; i++)
        ptr[i] = malloc(taille2*sizeof(**ptr));
    for (size_t i=0; i < taille1; i++)
        for (size_t j=0; j < taille2; j++)
            ptr[i][j] = malloc(taille3*sizeof(***ptr));

    // tableau de pointeurs
    int **tab_trans;
    // tableaux intermédiaires
    int prov[taille3], prov1[taille3], tout[taille3];
    // allocation mémoire
    tab_trans = malloc(taille1*sizeof(*tab_trans));
    for (size_t i=0; i < taille1; i++)
        tab_trans[i] = malloc(taille3*sizeof(**tab_trans));

    // déterminisation
    // d'abord créer l'état initial de self à partir des états initiaux de nfa
    size_t p = 0;
    for (size_t i=0; i < nfa->state_count; i++) {
        if ( nfa->initial_states[i] == true ) {
            tab_trans[0][p] = i;
            p++;
        }
    }
    tab_trans[0][p] = -1; // marquer la fin

    bool end = false;  // indique la fin: fin des ensembles de transtions
    size_t l = 1; // nombre d'états de nfa initialisé à 1
    size_t position = 0;  // indice de la colonne des ensembles de transitions

    while ( !end ) {
        size_t ii=0;
        while (tab_trans[position][ii] != -1) ii++;

        for (size_t j=1; j <= nfa->alpha_count; j++) {
            size_t k = 0;
            size_t m = 0;
            // mettre le nouvel état dans le tableau prov
            while ( tab_trans[position][k] != -1 ) {
                toutes_transitions(nfa,tab_trans[position][k],j-1,tout);
                ii=0;
                while (tout[ii] != -1) {
                    prov[m] = tout[ii];
                    ii++;
                    m++;
                }
                k++;
            }
            prov[m] = -1;
            // supprimer les doublons de prov
            m = 0;
            size_t mm;
            while (prov[m] != -1) {
                mm = m + 1;
                while (prov[mm] != -1) {
                    if (prov[m] == prov[mm]) prov[mm] = -2; // les marquer
                    mm++;
                }
                m++;
            }
            m = 0;
            size_t nn=0;
            while (prov[m] != -1) {
                if (prov[m] != -2) { prov1[nn] = prov[m]; nn++;} // les supprimer
                m++;
            }
            prov1[nn] = -1; // ne contient pas de doublons d'états

            // mettre prov1 dans le tableau à 3 dimensions
            m = 0;
            while ( prov1[m] != -1 ) {
                ptr[position][j-1][m] = prov1[m];
                tab_trans[l][m] = prov1[m];
                m++;
            }
            ptr[position][j-1][m] = -1;
            tab_trans[l][m] = -1;
            size_t n=0;
            bool trouve = false;
            // voir si prov1 existe déjà parmi les nouveaux états
            while ( !trouve && n < l ) {
                if ( egaux(m,prov1,tab_trans[n]) ) { trouve = true;}
                n++;
            }
            if ( !trouve ) l++;
        }

        position++;
        if (position >= l) end = true;  // il n'y a plus d'entrée (état)
    }

    // changement des numéros d'états: 0 1 2 3 ...
    int mat[taille1][nfa->alpha_count];
    size_t m;
    for (size_t i =0; i < l; i++) {
        size_t l1=0;
        while (tab_trans[i][l1] != -1) l1++;
        for (size_t n=0; n < l; n++) {
            for (size_t j=0; j < nfa->alpha_count; j++) {
                size_t l2=0;
                while (ptr[n][j][l2] != -1) l2++;
                if (l1 == l2) { if (egaux(l1,tab_trans[i],ptr[n][j])) mat[n][j] = i; }
            }
        }
    }

    // création de l'automate déterministe
    fa_create(self,nfa->alpha_count,l);

    // déterminer les états initiaux de self à partir des états initiaux de nfa
    for (size_t i =0; i < l; i++) {
        size_t j=0;
        bool initial=false;
        while (tab_trans[i][j] != -1 && !initial) {
            size_t k=0;
            while (k < nfa->state_count && !initial) {
                if ( nfa->initial_states[tab_trans[i][j]] == true) initial = true;
                k++;
            }
            j++;
        }
        if (initial) fa_set_state_initial(self,i);
    }

    // déterminer les états finaux de self à partir des états finaux de nfa
    for (size_t i =0; i < l; i++) {
        size_t j=0;
        bool final=false;
        while (tab_trans[i][j] != -1 && !final) {
            size_t k=0;
            while (k < nfa->state_count && !final) {
                if ( nfa->final_states[tab_trans[i][j]] == true) final = true;
                k++;
            }
            j++;
        }
        if (final) fa_set_state_final(self,i);
    }

    // créer les transitions de self
    for (size_t i =0; i < l; i++) {
        for (size_t j=0; j < nfa->alpha_count; j++)
            fa_add_transition(self,i,(char)(j+97),(size_t)mat[i][j]);
    }

    // libérer la mémoire allouée à ptr et tab_trans
    for (size_t i =0; i < l; i++) {
        for (size_t j=0; j < nfa->alpha_count; j++) {
            free(ptr[i][j]);
        }
        free(ptr[i]);
        free(tab_trans[i]);
    }
    free(ptr);
    free(tab_trans);

}

// Réponse 7.2: fonction qui détermine si un langage accepté par un
// automate est inclus dans un autre langage accepté par un autre automate.
bool fa_is_included(const struct fa *lhs, const struct fa *rhs) {

    // d'abord il faut rendre lhs déterministe et complet
    // rhs sera déterministe et complet (cours)

    // déterminiser lhs
    fa *det=malloc(sizeof(fa));
    fa_create_deterministic(det,lhs);
    // rendre det complet
    fa_make_complete(det);

    // calculer le complément de rhs
    for (size_t i=0; i < rhs->state_count; i++) {
        if (rhs->final_states[i] == true) rhs->final_states[i] = false;
        else rhs->final_states[i] = true;
    }

    // intersection
    if ( fa_has_empty_intersection(lhs,rhs)) return true;
    else return false;

}