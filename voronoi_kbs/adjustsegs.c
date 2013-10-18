#include <stdlib.h>
#include <stdio.h>
#include "envdefs.h"

#define S_X0 0
#define S_X1 2000
#define S_Y0 0
#define S_Y1 2000
 
BOOL is_point_within_area(point_t *p){
    
    if(p->x >= S_X0 && p->x <= S_X1 && p->y >= S_Y0 && p->y <= S_Y1){
        return TRUE;
    }else{
        return FALSE;
    }
}


BOOL is_points_equal(point_t a, point_t b){
    if(a.x == b.x && a.y == b.y){
        return TRUE;
    }else{
        return FALSE;
    }
  
}

void add_intersections(point_t * intersections, int *count, point_t p){
    //  printf("############################## count %d\n", *count);
    if(*count == 0){
        intersections[*count] = p;
        (*count)++;
        return;
    }

    int i = 0;
    while(i < *count){
        if(is_points_equal( intersections[i++], p)) return;
    }

    intersections[*count] = p;
    (*count)++;
    
}

void rebuild_seg(seg_t *seg){
    //  printf("seg........\n"); print_point(&(seg->start));print_point(&(seg->end));
    if(is_point_within_area(&(seg->start)) == TRUE && is_point_within_area(&(seg->end)) == TRUE){
        //  printf("seg in the area\n");
        return;
    }else{

        point_t x0_y0 = build_point(S_X0, S_Y0);
        point_t x1_y0 = build_point(S_X1, S_Y0);
        point_t x1_y1 = build_point(S_X1, S_Y1);
        point_t x0_y1 = build_point(S_X0, S_Y1);
        point_t intersections[4];
        int count = 0;
        /* intersection_t intersections[4]; */
  

        /* intersections[0] = get_intersection(&x0_y0, &x1_y0, &seg->start, &seg->end); */
        /* intersections[1] = get_intersection(&x0_y1, &x1_y1, &seg->start, &seg->end); */

        /* intersections[2] = get_intersection(&x0_y0, &x0_y1, &seg->start, &seg->end); */
        /* intersections[3] = get_intersection(&x1_y0, &x1_y1, &seg->start, &seg->end); */
        point_t its; 
        BOOL is_its = get_seg_intersection(x0_y0, x1_y0, seg->start, seg->end, &its);
        if(is_its == TRUE){
            add_intersections(intersections, &count, its);
        }

        is_its =  get_seg_intersection(x0_y1, x1_y1, seg->start, seg->end, &its);
        if(is_its == TRUE){
            add_intersections(intersections, &count, its);
        }

        is_its = get_seg_intersection(x0_y0, x0_y1, seg->start, seg->end, &its);
        if(is_its == TRUE){
            add_intersections(intersections, &count, its);
        }

        is_its = get_seg_intersection(x1_y0, x1_y1, seg->start, seg->end, &its); 
        if(is_its == TRUE){
            add_intersections(intersections, &count, its);
        }

        if(count == 0){
            //TODO delete the seg without intersections
            // set the seg as (0,0) (0,0)
            seg->start.x = 0;
            seg->start.y = 0;
            seg->end.x = 0;
            seg->end.y = 0;
            return;
        }
        if(count == 2){
            seg->start = intersections[0];
            seg->end = intersections[1];
        }

        if(count == 1){
            if(is_point_within_area(&(seg->start)) == FALSE){
                seg->start = intersections[0];
            }
            if(is_point_within_area(&(seg->end)) == FALSE){
                seg->end = intersections[0];
            }
        }

    }
}

void rebuild_seg_list(seg_list_t * sl){

    int i = 0;
    
    for(;i<sl->segs_count; i++){
        rebuild_seg(sl->segs[i]);
    }

}
