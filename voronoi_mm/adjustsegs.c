#include <stdlib.h>
#include <stdio.h>
#include "voronoi.h"

#define S_X0 X0
#define S_X1 X1
#define S_Y0 Y0
#define S_Y1 Y1

/* @brief -- check if the point p is located within the area
 * @param[in] p -- the point will be checked.
 * @return -- TRUE if the point is within the area, FALSE if the point is out of
 * the area 
 */
BOOL is_point_within_area(point_t *p){
    
    if(p->x >= S_X0 && p->x <= S_X1 && p->y >= S_Y0 && p->y <= S_Y1){
        return TRUE;
    }else{
        return FALSE;
    }
}

/* @brief -- check if two points are the same.
 * @param[in] a -- point a
 * @param[in] b -- point b
 * @return -- TRUE if two points are the same, otherwise return FALSE
 */

BOOL is_points_equal(point_t a, point_t b){
    if(a.x == b.x && a.y == b.y){
        return TRUE;
    }else{
        return FALSE;
    }
  
}
/* @brief - insert an intersection into list.
 * The intersection list is a list with max. 4 elements. If there is equal
 * point in the list as the point will be inserted, do nothing, otherwise add
 * the point into the list and increase the number of intersections.
 * @param[out] intersections -- a pointer of intersections-list
 * @param[out] count -- the number of intersections in the intersection-list
 * @param[in] p -- new intersection should be inserted.
 */
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

/* @brief -- put all segments into a box(workspace), this is cutting
 *  function. For every segment in the list, find all intersections with four
 *  lines of box, and reconstruct the segment according to the number of useful
 *  intersections.
 * @param[seg] -- the segments should be rebuilt.
 * @param[sl]  -- segments-list, this parameter will be used if the segment is
 *  deleted, the number of segments should be decreased.
 */

void rebuild_seg(seg_t *seg, seg_list_t* sl){
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

        /* if there is no intersection, delete the current segment from segment-list */
        if(count == 0){
           
            if(seg->prev && seg->next){
                seg->prev->next = seg->next;
                seg->next->prev = seg->prev;
                free(seg);
                sl->count--;
                return;
            }else if(seg->prev){
                seg->prev->next = NULL;
                sl->count--;
                free(seg);
                return;
            }else if(seg->next){
                sl->head = seg->next;
                sl->head->prev = NULL;
                sl->count--;
                free(seg);
                return;
            }else{
                free(seg);
                sl->head = NULL;
                sl->count--;
                return;                
            }
            
        }

        /* if there are 2 different intersections, replace the both points of
         * the segments with intersections */
        if(count == 2){
            seg->start = intersections[0];
            seg->end = intersections[1];
        }
        /* if there is only 1 intersection, replace the point not located in the
         * area with intersection */
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

/* @brief -- cutting all the segments in the segment-list, and rebuild the
 * segment if necessary  */
void rebuild_seg_list(seg_list_t * sl){
    
    seg_t *s = sl->head;
    while(s){
        rebuild_seg(s, sl);
        s = s->next;
    }
}
