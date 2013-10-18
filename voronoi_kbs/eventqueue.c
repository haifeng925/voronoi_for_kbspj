#include <stdlib.h>
#include <stdio.h>
#include "envdefs.h"

struct event_queue e_queue;



void init_event_pool(){
    e_queue.events_count = 0;
    int i=0;
    for(; i<EVENT_QUEUE_SIZE; i++){
        e_queue.ievents[i] = FALSE;
    }
    e_queue.head = NULL;
    e_queue.tail = NULL;
    // printf("init event_pool successed\n");

}

void push_event(struct event *e){
    int free_node = find_free_pos_e();
    if(free_node == -1){
        v_error("can not find free node for the new event!");
        return;
    }
    
    // event_clone(&e, &(e_queue.events[free_node].e));
    e_queue.events[free_node].e = e;
    e_queue.events[free_node].pos = free_node;
    e_queue.events[free_node].next = NULL;
    

    if(e_queue.head == NULL){
        e_queue.head = &(e_queue.events[free_node]);
        e_queue.tail = &(e_queue.events[free_node]);
        e_queue.events_count++;
        e_queue.ievents[free_node] = TRUE;
    }else{
        e_queue.tail->next = &(e_queue.events[free_node]);
        e_queue.tail = &(e_queue.events[free_node]);
        e_queue.tail->next = NULL;
        e_queue.events_count++;
        e_queue.ievents[free_node] = TRUE;
        
    }
    events_quicksort();
    /* debug_printf("push event into eventqueue"); */
    /* printf("\n\n"); */
}

int is_ep_empty(){
    if(e_queue.events_count == 0){
        return TRUE;
    }else{
        return FALSE;
    }
}

int find_free_pos_e(){
    int i = EVENT_QUEUE_SIZE-1;
    while(i>=0){
        if(e_queue.ievents[i] == FALSE){
            // printf("feenode at : %d\n", i);
            return i;
        }
        --i;
    }
    return -1;
}


/* void event_clone(struct event *src, struct event *des){ */
/*     des->x = src->x; */
/*     des->p.x = src->p.x; */
/*     des->p.y = src->p.y; */
/*     des->a = src->a; */
/*     des->valid = src->valid; */
/*     //  des->next = src->next; */
/* } */

struct event* top_event(){
    return e_queue.head->e;
}

struct event pop_event(){
    struct event* e_rst;
    // event_clone(&(e_queue.head->e), &e_rst);
    e_rst = e_queue.head->e;
    e_queue.events_count--;
    int free_node = e_queue.head->pos;
    e_queue.ievents[free_node] = FALSE;
    e_queue.head = e_queue.head->next;
    return *e_rst;
}

event_node_t* get_partion_e(event_node_t *e_begin, event_node_t *e_end){
    double key;
    key = e_begin->e->x;
    event_node_t *p = e_begin;
    event_node_t *q = p->next;
    while(q!=e_end){
        if(q->e->x < key){
            p = p->next;
            event_t *tmp = p->e;
            p->e = q->e;
            q->e = tmp;        
        }

        q = q->next;
    }

    event_t *tmp = p->e;
    p->e = e_begin->e;
    e_begin->e = tmp;

    return p;
}

void e_quicksort(event_node_t * e_begin, event_node_t *e_end){
    if(e_begin != e_end){
        event_node_t *partion = get_partion_e(e_begin, e_end);
        e_quicksort(e_begin, partion);
        e_quicksort(partion->next, e_end);
    }
    
}



void events_quicksort(){
    /* event_node_t *h = e_queue.head; */
    /* printf("*************before event sort\n"); */

    /* while(h!=NULL){ */
    /*     printf("%.3f\t", h->e->x); */
    /*     h=h->next; */
    /* } */
    /* printf("\n"); */

    e_quicksort(e_queue.head, e_queue.tail->next);

    /* printf("**************After event sort\n"); */

    /* h = e_queue.head; */
    /* while(h!=NULL){ */
    /*     printf("%.3f\t", h->e->x); */
    /*     h=h->next; */
    /* } */
    /* printf("\n"); */
}
