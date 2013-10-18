#include <stdlib.h>
#include <stdio.h>
#include "envdefs.h"

struct arc_pool arc_pool;
struct event_list event_list;


void init_arc_pool(){
    arc_pool.head = NULL;
    arc_pool.count = 0;
    int i;
    for(i=0; i<POINT_POOL_SIZE; i++){
        arc_pool.iarc[i] = FALSE;
    }
    //  printf("init arc pool successed\n");
}

int find_free_pos_arc(){
    int i=ARC_POOL_SIZE-1;
    while(i>0){
        if(arc_pool.iarc[i] == FALSE){
            //  printf("freenode : i: %d\n", i);
            return i;
        }
        --i;
    }

    return -1;
}


struct arc* dispatch_arc(){
    if(arc_pool.count >= ARC_POOL_SIZE){
        v_error("arc pool is full!");
        return NULL;
    }

    int free_pos = find_free_pos_arc();
    if(free_pos == -1){
        v_error("no space for arc");
        return NULL;
    }

    arc_pool.arcs[free_pos].pos = free_pos;
    arc_pool.arcs[free_pos].e = NULL;
    arc_pool.arcs[free_pos].s0 = NULL;
    arc_pool.arcs[free_pos].s1 = NULL;
    arc_pool.arcs[free_pos].next = NULL;
    arc_pool.arcs[free_pos].prev = NULL;
    arc_pool.count++;
    arc_pool.iarc[free_pos] = TRUE;

    arc_t *free_arc = &arc_pool.arcs[free_pos];
    return free_arc;
    
}




void destory_arc(arc_t *arc){
    int pos = arc->pos;
    arc_pool.iarc[pos] = FALSE;
    arc_pool.count--;
    return;
}



void init_event_list(){
    event_list.head = NULL;
    event_list.count = 0;
    int i;
    for(i=0; i<EVENT_LIST_SIZE; i++){
        event_list.i[i] = FALSE;
    }
    printf("init event list successed\n");
}

int find_free_pos_e_list(){
    int i=EVENT_LIST_SIZE-1;
    while(i>0){
        if(event_list.i[i] == FALSE){
            //  printf("freenode : i: %d\n", i);
            return i;
        }
        --i;
    }

    return -1;
}


struct event* dispatch_event(){
    if(event_list.count >= EVENT_LIST_SIZE){
        v_error("arc pool is full!");
        return NULL;
    }

    int free_pos = find_free_pos_e_list();
    if(free_pos == -1){
        v_error("no space for event");
        return NULL;
    }

    // event_list.events[free_pos].pos = free_pos;
    event_list.i[free_pos] = TRUE;
    event_list.count++;
    event_t *free_event = &event_list.events[free_pos];
    
    return free_event;
    
}


/* void destory_event(event_t *){ */
/*     int pos = arc->pos; */
/*     arc_pool.iarc[pos] = FALSE; */
/*     arc_pool.count--; */
/*     return; */
/* } */

