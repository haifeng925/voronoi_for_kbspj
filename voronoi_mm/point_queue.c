#include <stdlib.h>
#include <stdio.h>
#include "voronoi.h"

void print_point(point_t p){
    printf("%d %d\n", p.x, p.y);
}


void print_point_queue(point_queue_t *pq){
    printf("########### print point queue ########## \n\n");
    point_node_t *p_node = pq->head;
    while(p_node != NULL){
        print_point(p_node->point);
        p_node = p_node->next;
    }
    printf("\n########### print point queue end ########## \n\n");

}
/* @brief - initialize one point-queue, 1. set the head of point-queue to NULL;
 * 2. set the number of point-queue to 0;
 */
void init_point_queue(point_queue_t *pq){
    
        pq->head = NULL;
        // pq->tail = NULL;
        pq->count = 0;
    
}

/* @ brief - release all the memory space of one point-queue, traverse all the
 * element of one point-queue and release the space. At the end, set the counter
 * of point-queue to 0;
 * @param[in] pq - point-queue
 */
void free_point_queue(point_queue_t *pq){
    point_node_t *pnode;
    while(pq->head){
        pnode = pq->head;
        pq->head = pq->head->next;
        free(pnode);
    }

    pq->count = 0;
}

/* @brief - push one point node into point queue. Start from the head of the
 * point-queue, if head is NULL, insert the point, and let head point this
 * element, else compare the point to every point in the queue, if x coordinate
 * of point is greater than the point in the queue then compare to the next point,
 * until find the point in the queue with greater x coordination than the point
 * should be inserted, or with equal x coordination but greater y
 * coordination. Then insert the point before that.
 */
void push_point(point_queue_t * pq, point_t point){
    point_node_t *pnode = (point_node_t *) malloc(sizeof(point_node_t));
    if(pnode == NULL){
        printf("No memory space for point!\n");
        return;
    }
    pnode->point = point;
    
    if(pq->head == NULL){
        pq->head = pnode;
        //    pq->tail = pnode;
        pnode->next = NULL;
        pq->count++;
    }else{
        point_node_t *current_node = pq->head;
        point_node_t *prev_node = NULL;
        while(current_node != NULL){
            if(current_node->point.x < point.x){
                prev_node = current_node;
                current_node = current_node->next;
            }else if(current_node->point.x == point.x){
                if(current_node->point.y == point.y){
                    return;
                }else if(current_node->point.y < point.y){
                    prev_node = current_node;
                    current_node = current_node->next;
                }else{
                    break;
                }
            }else{
                break;
            }
        }
        if(prev_node == NULL){
            pnode->next = pq->head;
            pq->head = pnode;
            pq->count++;
        }else{
            pnode->next = prev_node->next;
            prev_node->next = pnode;
            pq->count++;
        }

    }
    
}
/* @brief - get the first element of the point queue
 * @param[in] pq - point queue
 * @return - the first point of the point queue.
 */
point_t get_top_point(point_queue_t *pq){
    return pq->head->point;
}

/* @brief - check if the point queue is empty
 * @param[in] pq - point queue which will be checked.
 * @return - TRUE(1) if the number of points in the queue is 0, otherwise return
 * FALSE(0) 
 */
BOOL is_point_queue_empty(point_queue_t *pq){
    return pq->count == 0 ? TRUE:FALSE;
}


/* @brief - get the first point of one queue and delete this point from this
 * point queue.
 * @param[in] pq - point queue
 * @return - first point of the point-queue.
 */
point_t pop_point(point_queue_t *pq){
    
        point_t p = pq->head->point;
        point_node_t *p_node = pq->head;
        pq->head = pq->head->next;
        pq->count--;
        free(p_node);
        return p;
    
}
/* @brief - get the size of one point-queue
 */

int size_of_point_queue(point_queue_t *pq){
    return pq->count;
}
