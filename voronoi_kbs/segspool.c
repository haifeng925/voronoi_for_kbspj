#include <stdlib.h>
#include <stdio.h>
#include "envdefs.h"


struct seg_pool seg_pool;

void init_seg_pool(){
    seg_pool.segs_count = 1;
    int i=0;
    for(;i<SEG_POOL_SIZE;++i){
        seg_pool.i[i] = FALSE;
        printf("init seg_pool successed\n");
            
    }
    
}


int find_free_segs(){
    int i=SEG_POOL_SIZE-1;
    while(i>=0){
        if(seg_pool.i[i] == FALSE){
            //     printf("freenode : i : %d in SEGPOOL\n",i);
            return i;
        }
        --i;
    }

    return -1;
}

seg_t * dispatch_seg(){
    if(seg_pool.segs_count >= SEG_POOL_SIZE){
        v_error("seg pool is full!");
        return NULL;
    }

    int free_pos = find_free_segs();
    if(free_pos == -1){
        v_error("no space for segs");
        return NULL;
    }

    seg_pool.i[free_pos] = TRUE;
    seg_pool.segs_count++;
    seg_pool.segs[free_pos].start.x = 0;
    seg_pool.segs[free_pos].start.y = 0;
    seg_pool.segs[free_pos].end.x = 0;
    seg_pool.segs[free_pos].end.y = 0;
    seg_pool.segs[free_pos].done = FALSE;

    seg_t *free_seg = &seg_pool.segs[free_pos];
    
    /* debug_printf("seg dispatch...."); */
    push_back_seg(free_seg);
    return free_seg;
}


