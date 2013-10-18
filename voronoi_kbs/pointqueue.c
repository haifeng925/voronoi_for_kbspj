#include <stdio.h>
#include <stdlib.h>

#include "envdefs.h"

struct points_pool p_pool;
void p_quicksort(struct point_node*, struct point_node*);

void init_point_list(){
    p_pool.head = NULL;
    p_pool.points_count = 0;
    int i;
    for(i=0; i<POINT_POOL_SIZE; i++){
        p_pool.ipoint[i] = FALSE;
    }
    //  printf("init Pointlist successed\n");
}

int find_free_pos(){
    int i=POINT_POOL_SIZE-1;
    while(i>=0){
        if(p_pool.ipoint[i] == FALSE){
            //     printf("freenode : i: %d\n", i);
            return i;
        }
        --i;
    }

    return -1;
}

BOOL is_pp_empty(){
    if(p_pool.points_count == 0){
        return TRUE;
    }else{
        return FALSE;
    }
}

void push_point(struct point p){

    if(p_pool.points_count >= POINT_POOL_SIZE){
        v_error("the points pool is full\n");
        return;
    }else{
        int free_pos = find_free_pos();
        if(free_pos == -1){
            v_error("the points_pool is full\n");
            return;
        }

        p_pool.points[free_pos].p.x = p.x;
        p_pool.points[free_pos].p.y = p.y;
        p_pool.points[free_pos].pos = free_pos;
        p_pool.points[free_pos].next = NULL;


        if(p_pool.head == NULL){
            p_pool.head = &(p_pool.points[free_pos]);
            p_pool.tail = &(p_pool.points[free_pos]);
            p_pool.points_count++;
            p_pool.ipoint[free_pos] = TRUE;

        }else{
            p_pool.tail->next = &(p_pool.points[free_pos]);
            p_pool.tail = &(p_pool.points[free_pos]);
            p_pool.tail->next = NULL;
            p_pool.points_count++;
            p_pool.ipoint[free_pos] = TRUE;

        }
    }
    //  debug_printf("push the point into point queue");
    

}

 struct point top_point(){
     /* printf("get the point on the top: "); */
     /* print_point(&p_pool.head->p); */
     return p_pool.head->p;
 } 


struct point pop_point(){
    struct point p_rst;
    p_rst.x = p_pool.head->p.x;
    p_rst.y = p_pool.head->p.y;
    p_pool.points_count--;
    int freenode = p_pool.head->pos;
    p_pool.ipoint[freenode] = FALSE;
    p_pool.head = p_pool.head->next;
    /* printf("pop the point: "); */
    /* print_point(&p_rst); */
    return p_rst;
}



void print_point(struct point* p){
    printf("%d %d \n", p->x, p->y);
}



struct point_node* get_partion(struct point_node *p_begin, struct point_node *p_end){
    struct point key;
    /* key.x = p_begin->p.x; */
    /* key.y = p_begin->p.y; */
    key = p_begin->p;
    struct point_node *p = p_begin;
    struct point_node *q = p->next;
    while(q!= p_end){
        if(q->p.x == key.x){

            if(q->p.y < key.y){
                p = p->next;
                struct point tmp;
                tmp.x = p->p.x;
                tmp.y = p->p.y;
                p->p.x = q->p.x;
                p->p.y = q->p.y;
                q->p.x = tmp.x;
                q->p.y = tmp.y;
            }

        }else{
            
            if(q->p.x < key.x){
                p = p->next;
                struct point tmp;
                tmp.x = p->p.x;
                tmp.y = p->p.y;
                p->p.x = q->p.x;
                p->p.y = q->p.y;
                q->p.x = tmp.x;
                q->p.y = tmp.y;
            }

            
        }
        
    
        q = q->next;

    }

    struct point tmp;
    tmp.x = p->p.x;
    tmp.y = p->p.y;
    p->p.x = p_begin->p.x;
    p->p.y = p_begin->p.y;
    p_begin->p.x = tmp.x;
    p_begin->p.y = tmp.y;
    return p;

}

void p_quicksort(struct point_node *p_begin, struct point_node *p_end){
    if(p_begin != p_end){
        struct point_node *partion = get_partion(p_begin, p_end);
        p_quicksort(p_begin, partion);
        p_quicksort(partion->next, p_end);
    }
}

void points_quicksort(){
    p_quicksort(p_pool.head, p_pool.tail->next);
}
