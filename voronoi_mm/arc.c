#include <stdlib.h>
#include <stdio.h>
#include "voronoi.h"

void init_arc(arc_t *a){
    a->e = NULL;
    a->prev = NULL;
    a->next = NULL;
    a->s0 = NULL;
    a->s1 = NULL;
}

arc_t* new_arc(){
    arc_t *narc = (arc_t *)malloc(sizeof(arc_t));
    init_arc(narc);
    return narc;
}


void free_arc_list(arc_t *head){
    arc_t *a = head;
    while(head){
        a = head;
        
        head = head->next;
        free(a);
    }
    
}


void print_arc_list(arc_t *head){
    arc_t *a = head;
    while(a){
        a = a->next;
    }

}
