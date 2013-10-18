#include <stdlib.h>
#include <stdio.h>
#include "voronoi.h"

/* all the functions for event-queue is simile as the functions in point_queue.c */
void init_event_queue(event_queue_t *eq){

    eq->head = NULL;
    eq->count = 0;
    
}



void free_event_queue(event_queue_t *eq){
    while(eq->head != NULL){
        event_t * e = eq->head;
        eq->head = eq->head->next;
        free(e);
    }

    eq->count = 0;
}

int size_of_event_queue(event_queue_t *eq){
    return eq->count;
}

BOOL is_event_queue_empty(event_queue_t *eq){
    return eq->count == 0 ? TRUE : FALSE;
}

void push_event(event_queue_t *eq, event_t *e){
    event_t *new_event = e;
    if(eq->head == NULL){
        eq->head = new_event;
        eq->head->prev = NULL;
        eq->head->next = NULL;
        eq->count++;
    }else{
        event_t *current_event = eq->head;
        event_t *prev_event = NULL;
        while(current_event != NULL){
            if(current_event->x <= e->x){
                prev_event = current_event;
                current_event = current_event->next;
            }else{
                break;               
            }
        }

        if(prev_event == NULL){
            new_event->next = eq->head;
            eq->head->prev = new_event;
            eq->head = new_event;
            eq->head->prev = NULL;
            eq->count++;
        }else{
            prev_event->next = new_event;
            new_event->prev = prev_event;
            new_event->next = current_event;
            if(current_event != NULL){
                current_event->prev = new_event;
            }

            eq->count++;
        }
        
    }
}

event_t get_top_event(event_queue_t* eq){
    return *(eq->head);
}

event_t pop_event(event_queue_t *eq){
    event_t event_tmp = *(eq->head);
    event_t *e_pointer = eq->head;
    eq->head = eq->head->next;
    eq->count--;
    free(e_pointer);
    return event_tmp;
    
}
