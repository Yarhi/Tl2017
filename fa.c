#include "fa.h"

// Créer un automate passé en paramètre
void fa_create ( fa *self, size_t alpha_count, size_t state_count )
{
    //int i = -1;
    size_t i = 0;

    self->state_count = state_count;
    self->alpha_count = alpha_count;

    self->initial_states = malloc(state_count*sizeof(bool));
    self->final_states = malloc(state_count*sizeof(bool));
    for(size_t i = 0 ;i < state_count ; i++ ){
        self->initial_states[i] = false;
        self->final_states[i] = false;
    }

    state_set *list     = malloc(sizeof(state_set));
    state_node *head    = malloc(sizeof(state_node));
    state_node *new ;
    head->state = 0;
    head->next = NULL;

    list->first = head;
    self->list = list;

    while (i++,i < state_count )
    {
    new = malloc(sizeof(state_node));
    new->state = i;
    new->next = NULL;
    head->next = new;
    head = new;

    /* Risque d'écraser le début de la liste des états

        new->state = (size_t) i;
        new->next = self->list->first;
        self->list->first = new;*/
    }

    //Allocation de la table des transitions
    self->transitions = malloc(state_count*sizeof(*(self->transitions)));
    for(size_t i = 0 ;i < state_count ;i++){
        self->transitions[i] = NULL;
    }
    
    
    

}



//Fonction de destruction d'un automate
void fa_destroy(fa *self){
    state_node *head;
    state_node *new ;

    free(self->initial_states);
    self->initial_states = NULL;
    free(self->final_states);
    self->final_states = NULL;
   
    head = self->list->first;
    //Free  : ensemble des états
    for(size_t i = 0 ; i < self->state_count ; i++){
        
        new = head->next;
        free(head);
        head = new;

    }

    //Free : table des transitions

    for(size_t i = 0; i < self->state_count ; i++){
        head = self->transitions[i];

        while(head != NULL){
              new = head->next;
              free(head);
              head = new;
        }
    }
    free(self->transitions);
    self->transitions = NULL;
    

}

//Rendre un état initial et final

void fa_set_state_initial(struct fa *self, size_t state){
    self->initial_states[state] = true;
};
void fa_set_state_final(struct fa *self, size_t state){
    self->final_states[state] = true;
};

//Ajouter une transition 

void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to) {

  state_node *begin,*new,*preview = NULL;

  begin = self->transitions[from];
  if (begin == NULL) {
    new = malloc(sizeof(state_node));
    new->next = NULL;
    new->state = to;
    new->letter = alpha;
    self->transitions[from] = new;
  } else {
           while ( begin != NULL) {
             preview = begin;
             begin = begin->next;
           }
           new = malloc(sizeof(state_node));
           new->next = NULL;
           new->state = to;
           new->letter = alpha;
           preview->next = new;
         }
   
}

//  Fonction pour afficher un automate
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
   for (int i=0; i < self->state_count; i++) {
     fprintf(out,"   For state %d:\n", i);
     begin = self->transitions[i];
     for (int j=0; j < self->alpha_count; j++) {
       c = (char) toascii(j + 97);  // conversion d'un entier en caractère ascii
       fprintf(out,"           For Letter %c: ",c);
       while ((begin != NULL) && (c == begin->letter)) {
         fprintf(out,"%zu ",begin->state);
         begin = begin->next;
       }
     fprintf(out,"\n");
     }
   }
    
}


// Sortie d'un automate en DOT

void fa_dot_print ( const fa *self, FILE *out )
{
    state_node *begin;
    char c = NULL;

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
        begin = self->transitions[i];

        while ( ( begin != NULL ) && ( c == begin->letter ) )
        {
            for ( int j = 0; j < self->alpha_count; j++ )
            {
                c = (char) toascii(j + 97);
                fprintf( out, "\tLR_%i -> LR_%zu [label = \"%c\" ];", i, begin->state, c);
            }
            begin = begin->next;
        }
    }

    // Transitions -- END

    fprintf( out, "}" );

}


