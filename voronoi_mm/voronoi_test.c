#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "voronoi.h"
#define POINT_QUEUE_SIZE 20
#define EVENT_QUEUE_SIZE 20
#define REPEAT_TIMES 30

int main(){
    srand((unsigned)time(0));
    /* int i=0; */
    /* int j=0; */
   
    /* printf("################ test for point queue ############\n"); */
    /* /\* test for point queue *\/ */
    /* point_queue_t *pq = (point_queue_t *) malloc(sizeof(point_queue_t)); */
    /* init_point_queue(pq); */

    /* point_t ps[POINT_QUEUE_SIZE]; */
    /* j = 0; */
    /* for(; j<REPEAT_TIMES; j++){ */
    /*     free_point_queue(pq); */
    /*     i=0; */
    /*     for(; i<POINT_QUEUE_SIZE; i++){ */
            
    /*         ps[i].x = rand()%2000; */
    /*         ps[i].y = rand()%2000; */
    /*         push_point(pq, ps[i]); */
            
    /*     } */
    /*     i=0; */
    /*     int qsize = size_of_point_queue(pq); */
    /*     for(; i<qsize; i++){ */
    /*         point_t p = pop_point(pq); */
    /*         print_point(p); */
    /*     } */
    /*     printf("----------------------\n"); */

    /* } */
    /* free(pq); */
  
    /* /\* test for event queue *\/ */
    /* printf("################ test for event queue ############ \n"); */

    
    /* event_queue_t *eq = (event_queue_t *)calloc(1, sizeof(point_queue_t)); */
    /* init_event_queue(eq); */

    /* //event_queue_t *eq  = (event_queue_t *) malloc(sizeof(point_queue_t)); */
     
    /* j=0; */
    /* for(; j<REPEAT_TIMES; j++){ */
    /*     free_event_queue(eq); */
    /*     i = 0; */
    /*     for(; i<EVENT_QUEUE_SIZE; i++){ */
          
    /*         event_t *n_event = (event_t *)malloc(sizeof(event_t)); */
    /*         n_event->x = rand()%2000; */
    /*         push_event(eq, n_event); */
    /*     } */
    
    /*     i=0; */
    /*     for(; i<EVENT_QUEUE_SIZE; i++){ */
    /*         event_t e = pop_event(eq); */
    /*         printf("popevent: %.3f\n", e.x); */
    /*     } */
    
    /*     printf("----------------------\n"); */
    /* } */
    /* free(eq); */

    printf("#################### Test for Voronoi ####################\n\n");
    point_t pstest[9];
    pstest[0].x= 200; pstest[0].y=200;
    pstest[1].x= 400; pstest[1].y=400;
    pstest[2].x= 200; pstest[2].y=800;
    pstest[3].x= 300; pstest[3].y=600;
    pstest[4].x= 400; pstest[4].y=900;
    pstest[5].x= 600; pstest[5].y=300;
    pstest[6].x= 600; pstest[6].y=500;
    pstest[7].x= 1000; pstest[7].y=0;
    pstest[8].x= 1500; pstest[8].y=500;


    seg_list_t *sl = generate_voronoi(pstest, 9);
    print_voronoi(sl);
    free_seg_list(sl);
    free(sl);
    // printf("num arc: %d\n", c_arc);
    return 1;
    
}
