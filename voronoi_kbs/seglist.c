#include <stdlib.h>
#include <stdio.h>
#include "envdefs.h"


struct seg_list seg_list;

void init_seg_list(void){
    seg_list.segs_count = 0;
    //  printf("init seg list successed\n");

}

int is_seg_list_full(){
    if(seg_list.segs_count >= SEG_POINT_POOL_SIZE){
        return 1;
    }else{
        return 0;
    }
}


void push_back_seg(seg_t* s){
    if(is_seg_list_full()){
        v_error("seg list is full");
        return;
    }
    
    if(seg_list.segs_count == 0){
        seg_list.segs[0] = s;
        seg_list.segs_count++;
    }else{
        seg_list.segs[seg_list.segs_count] = s;
        seg_list.segs_count++;
    }

    /* debug_printf("SEG push back into seglist"); */

}

