#include <stdlib.h>
#include <stdio.h>
#include "voronoi.h"

void init_seg_list(seg_list_t *sl){
    sl->head = NULL;
    sl->tail = NULL;
    sl->count = 0;
}

void free_seg_list(seg_list_t *sl){
    seg_t *sg;
    while(sl->head){
        sg = sl->head;
        sl->head = sl->head->next;
        free(sg);
    }
    sl->count = 0;
}


void add_seg(seg_list_t *sl, seg_t *seg){
    if(sl->tail == NULL){
        sl->tail = seg;
        sl->tail->prev = NULL;
        sl->tail->next = NULL;
        sl->head = sl->tail;
        sl->count++;
        return;
    }
    sl->tail->next = seg;
    seg->prev = sl->tail;
    sl->tail = seg;
    sl->count++;
}

void init_seg(seg_t *s){
    s->start.x = 0;
    s->start.y = 0;
    s->end.x = 0;
    s->end.y = 0;
    s->done = FALSE;
    s->prev = NULL;
    s->next = NULL;
}

seg_t* new_seg(){
    seg_t *new_seg = (seg_t *)malloc(sizeof(seg_t));
    init_seg(new_seg);
    return new_seg;
}
