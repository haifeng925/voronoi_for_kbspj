#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "voronoi.h"

/* Author: Haifeng Gao */

int X0, Y0, X1, Y1;
void finish(seg_t *, point_t);
void process_point(voronoi_context_t *);
void process_event(voronoi_context_t*);
void finish_edge(voronoi_context_t *);
void front_insert(point_t, voronoi_context_t*);
BOOL intersect(point_t, arc_t*, point_t*);
point_t intersection(point_t, point_t, double);
void check_circle_event(arc_t*, double, voronoi_context_t*);
BOOL circle(point_t, point_t, point_t, double*, point_t*);
void finish_edges(voronoi_context_t*);


/* @brief -build a point from two integer.
 * @param[in] x - x coordinate
 * @param[in] y - y coordinate
 * @return a point
 */
point_t build_point(int x, int y){
    point_t p;
    p.x = x;
    p.y = y;
    return p;
}

/* @brief -- add virtural obstacles on four lines of box. The number of virtual
 * obstacles depends on the constant VIRTUAL_NUMBER, which is definded in voronoi.h.
 * @param[pq] -- point-queue, the new virtual obstacles will be inserted in this
 * point-queue.   
 */
void add_virtual_obstacles(point_queue_t *pq){

    int dis_Y = Y1 - Y0;
    int dis_X = X1 - X0;
    int interval_Y = dis_Y / (VIRTUAL_NUMBER - 1);
    int interval_X = dis_X / (VIRTUAL_NUMBER - 1);
    int i=1;
    for (; i<VIRTUAL_NUMBER-1 ; i++){
        int p_x = X0 + i*interval_X;
        point_t p_with_Y0 = build_point(p_x, Y0);
        point_t p_with_Y1 = build_point(p_x, Y1);
        /* print_point(p_with_Y0); */
        /* print_point(p_with_Y1); */
        push_point(pq, p_with_Y0);
        push_point(pq, p_with_Y1);
    }
    i = 1;
    for(; i<VIRTUAL_NUMBER-1; i++){
        int p_y = Y0 + i*interval_Y;
        point_t p_with_X0 = build_point(X0, p_y);
        point_t p_with_X1 = build_point(X1, p_y);

        /* print_point(p_with_X0); */
        /* print_point(p_with_X1); */
        push_point(pq, p_with_X0);
        push_point(pq, p_with_X1);
    }

    point_t X0Y0 = build_point(X0, Y0);
    point_t X0Y1 = build_point(X0, Y1);
    point_t X1Y0 = build_point(X1, Y0);
    point_t X1Y1 = build_point(X1, Y1);
    /* print_point(X0Y0); */
    /* print_point(X0Y1); */
    /* print_point(X1Y0); */
    /* print_point(X1Y1); */
    push_point(pq, X0Y0);
    push_point(pq, X0Y1);
    push_point(pq, X1Y0);
    push_point(pq, X1Y1);

 }

/* @brief -- generate a Voronoi Diagram
 * @param[in] p_array -- pointer to a list of points
 * @param[in] num -- number of the points in the point-list
 * @return -- a list of segments, which represent the edges of the Voronoi Diagram
 */

seg_list_t* generate_voronoi(point_t *p_array, int num){
    X0=200;
    Y0=200;
    X1=1800;
    Y1=1200;
    point_queue_t *p_queue = (point_queue_t *)malloc(sizeof(point_queue_t));
    if(p_queue == NULL){
        printf("No Memory space for point_queue!!\n");
        return NULL;
    }
    event_queue_t *e_queue = (event_queue_t *)malloc(sizeof(event_queue_t));
    if(e_queue == NULL){
        printf("No enough memory space for creating event_queue!\n");
        return NULL;
    }

    seg_list_t *seg_list = (seg_list_t *)malloc(sizeof(seg_list_t));
    if(seg_list == NULL){
        printf("No enough memory space for creating seg_list!\n");
    }

    arc_t *arc_head = NULL;
   
    init_point_queue(p_queue);
    init_event_queue(e_queue);
    init_seg_list(seg_list);
    voronoi_context_t *v_context = (voronoi_context_t *)malloc(sizeof(voronoi_context_t));
    v_context->pq = p_queue;
    v_context->eq = e_queue;
    v_context->sl = seg_list;
    v_context->arc_head = arc_head;
    
    int i;
    for(i=0; i<num; i++){
        push_point(p_queue, p_array[i]);
        if(p_array[i].x < X0) X0 = p_array[i].x;
        if(p_array[i].y < Y0) Y0 = p_array[i].y;
        if(p_array[i].x > X1) X1 = p_array[i].x;
        if(p_array[i].y > Y1) Y1 = p_array[i].y;
    }

    // add virtual obstancles at the edges of plane
    if(p_queue->count < MIN_OBST){
        add_virtual_obstacles(p_queue);
    }
    // add_virtual_obstacles(p_queue);
    
//    print_point_queue(p_queue);
    
    /* 
       VORONOI_DIAGRAM(P) 
       Input: A set P of point sites in the plane 
       Output: The Voronoi diagram Vor(P) given inside a bounding box in a doubly-connected edge list structure 
       { 
       Initialize the event tree Q with all point events 
       while Q is not empty do 
       { 
       Consider the event with largest y-coordinate in Q 
       if the event is a point event, occurring at site pi 
       then HANDLE_POINT_EVENT(pi) 
       else HANDLE_CIRCLE_EVENT() 
       Remove the event from Q 
       } 
       Put Voronoi diagram into a box 
       } 


 */
    while(!is_point_queue_empty(p_queue)){
        if(!is_event_queue_empty(e_queue)){
            event_t top_e = get_top_event(e_queue);
            point_t top_p = get_top_point(p_queue);
            if(top_e.x <= top_p.x){
                process_event(v_context);
            }else{
                process_point(v_context);
            }
        }else{
            process_point(v_context);
        }
    }
    
   // After all points are processed, do the remaining circle events.

    while(!is_event_queue_empty(e_queue)){
        process_event(v_context);
    }

    finish_edges(v_context);
    /* release memory space for point-queue, event-queue and arc-list. The
     * memory space for segments-list should be released out side this program */
    free_point_queue(v_context->pq);
    free_event_queue(v_context->eq);
    free_arc_list(v_context->arc_head);

    free(p_queue);
    free(e_queue);
    free(v_context);
    /* cutting function */
    rebuild_seg_list(seg_list);
    return seg_list;
}

/* @brief -- process the point event
 * @param[in] vt -- a pointer to the voronoi context
 * @return -- void
 */

void process_point(voronoi_context_t *vt){
    point_t p = pop_point(vt->pq);
    front_insert(p, vt);
}

void process_event(voronoi_context_t *vt){
    event_t e = pop_event(vt->eq);
    if(e.valid != FALSE){
        seg_t *n_seg = new_seg();
        add_seg(vt->sl, n_seg);
        n_seg->start = e.point;
        arc_t *a = e.arc;
        if(a->prev!=NULL){
            a->prev->next = a->next;
            a->prev->s1 = n_seg;

        }

        if(a->next!=NULL){
            a->next->prev = a->prev;
            a->next->s0 = n_seg;
        }

        //  print_arc_list();
        if(a->s0!=NULL){
            if(a->s0->done == FALSE){
                 a->s0->end = e.point;
                 a->s0->done = TRUE;
            }
        }
        if(a->s1!=NULL){
            if(a->s1->done == FALSE){
                a->s1->end = e.point;
                a->s1->done = TRUE;
            }

        }
        if(a->prev!=NULL) check_circle_event(a->prev, e.x, vt);

        if(a->next!=NULL) check_circle_event(a->next, e.x, vt);

        free(a);
    }

   
    /* debug_printf("process_event_e"); */

}


/* @brief -- insert a new parabolic arc into arc-list
 * @param[in] p -- point p represents a new arc.
 * @param[out] vt -- voronoi-context, get arc-list form this context.
 */
void front_insert(point_t p, voronoi_context_t *vt){
    if(vt->arc_head == NULL){
        arc_t *n_arc = new_arc();
        vt->arc_head = n_arc;
        n_arc->p = p;
        return;
    }

    arc_t *i = vt->arc_head;
    for(; i != NULL; i = i->next){
        point_t z, zz;
        if(intersect(p, i, &z) == TRUE){
            if(i->next && intersect(p, i->next, &zz) == FALSE){
                arc_t *n_arc = new_arc();

                n_arc->p = i->p;
                n_arc->prev = i;
                n_arc->next = i->next;

                i->next->prev = n_arc;
                i->next = i->next->prev;
                
            }else{
                arc_t* n_arc = new_arc();

                n_arc->p = i->p;
                n_arc->prev = i;

                i->next = n_arc;
            }

            i->next->s1 = i->s1;
            arc_t *n_arc = new_arc();
            
            n_arc->p = p;
            n_arc->prev = i;
            n_arc->next = i->next;

            i->next->prev = n_arc;
            i->next = i->next->prev;

            i = i->next;

            seg_t *n_seg = new_seg();
            add_seg(vt->sl, n_seg);
            i->prev->s1 = i->s0 = n_seg;
            n_seg->start = z;


            seg_t *n_seg1 = new_seg();
            add_seg(vt->sl, n_seg1);
            i->next->s0 = i->s1 = n_seg1;
            n_seg1->start = z;
            
            check_circle_event(i, p.x, vt);
            check_circle_event(i->prev, p.x, vt);
            check_circle_event(i->next, p.x, vt);

            return;
            
            
        }
        
    }

    /* special case */
    arc_t *n_arc;
    for(n_arc=vt->arc_head; n_arc->next!=NULL; n_arc=n_arc->next);

    arc_t *n_arc_0 = new_arc();
    n_arc_0->p = p;
    n_arc_0->prev = n_arc;
    n_arc->next = n_arc_0;
    
    point_t start;
    start.x = X0;
    start.y =(int) (((double)(n_arc->next->p.y + n_arc->p.y))/2.0);
    seg_t *n_seg_2 = new_seg();
    add_seg(vt->sl, n_seg_2);
    n_seg_2->start = start;

    n_arc->s1 = n_arc->next->s0 = n_seg_2;
        
}

void check_circle_event(arc_t *i, double x0, voronoi_context_t *vt){

    if((i->e != NULL) && (i->e->x != x0)){

        i->e->valid = FALSE;
    }

    i->e = NULL;
    if((i->prev == NULL)||(i->next == NULL)){
        return;
    }

    double x;
    point_t o;

    BOOL rst_circle = circle(i->prev->p, i->p, i->next->p, &x,&o);
    if(rst_circle == TRUE && x > x0){
        i->e = (event_t*)malloc(sizeof(event_t));
        i->e->x = x;
        i->e->point = o;
        i->e->arc = i;
        i->e->valid = TRUE;

        push_event(vt->eq, i->e);
    }
}


BOOL circle(point_t a, point_t b, point_t c, double *x, point_t *o){
    if ((b.x-a.x)*(c.y-a.y) - (c.x-a.x)*(b.y-a.y) > 0)
        return FALSE;

    // Algorithm from O'Rourke 2ed p. 189.
    double A = (double)(b.x - a.x),  B = (double) (b.y - a.y),
        C = (c.x - a.x),  D =(double) (c.y - a.y),
        E = (double)(A*(a.x+b.x) + B*(a.y+b.y)),
        F = (double)(C*(a.x+c.x) + D*(a.y+c.y)),
        G = (double)(2.0*(A*(c.y-b.y) - B*(c.x-b.x)));

    if (G == 0) return FALSE;  // Points are co-linear.

    // Point o is the center of the circle.
    o->x = (int) ((D*E-B*F)/G);
    o->y = (int) ((A*F-C*E)/G);

    // o.x plus radius equals max x coordinate.
    *x = o->x + sqrt( pow(a.x - o->x, 2) + pow(a.y - o->y, 2) );

    return TRUE;
}



BOOL intersect(point_t p, arc_t *i, point_t *result){
    if(i->p.x == p.x) return FALSE;

    double a,b;

    if(i->prev != NULL){
     
        point_t pa = intersection(i->prev->p, i->p, (double)p.x);
        a = pa.y;

    }
    if(i->next != NULL){
   
        point_t pb = intersection(i->p, i->next->p,(double) p.x);
        b = pb.y;

    }

    if ((!i->prev || a <= p.y) && (!i->next || p.y <= b)) {
        result->y = p.y;

        result->x = (int)((double)(i->p.x*i->p.x + (i->p.y-result->y)*(i->p.y-result->y) - p.x*p.x) / ((double)(2*i->p.x - 2*p.x)));
        return TRUE;
    }
    return FALSE;


}



point_t intersection(point_t p0, point_t p1, double l)
{

   point_t res, p = p0;

   double z0 = 2*((double)p0.x - l);
   double z1 = 2*((double)p1.x - l);

   if (p0.x == p1.x)
       res.y = (int)((p0.y + p1.y) / 2);
   else if (p1.x == l)
      res.y = p1.y;
   else if (p0.x == l) {
      res.y = p0.y;
      p = p1;
   } else {
      // Use the quadratic formula.
      double a = 1/z0 - 1/z1;
      double b = -2*(p0.y/z0 - p1.y/z1);
      double c = (p0.y*p0.y + p0.x*p0.x - l*l)/z0
               - (p1.y*p1.y + p1.x*p1.x - l*l)/z1;
      //    printf("----------%.6f  %.6f %.6f\n", a, b,c);
      res.y =(int) (( -b - sqrt(b*b - 4*a*c) ) / (2*a));
      //    printf("============%d\n", res.y);
   }
   // Plug back into one of the parabola equations.
   res.x = (int)((p.x*p.x + (p.y-res.y)*(p.y-res.y) - l*l)/(2*p.x-2*l));

   return res;
}

void finish_edges(voronoi_context_t* vt){
    // print_arc_list(vt->arc_head);
    double l = (double)(X1 + (X1-X0) + (Y1-Y0));
    //  printf("xxxxxxxxxxxxxxx...finish_edge  %.6f\n",l);
     arc_t *i = vt->arc_head;
     for(; i->next ; i = i->next){
         if(i->s1){
     
             point_t new_p = intersection(i->p,i->next->p,l*2);
      
             finish(i->s1, new_p);
         }
     }
}

void finish(seg_t *seg, point_t p){
    if(seg->done){
        return;
    }
    seg->end = p;
    seg->done = TRUE;
}

/* @brief -- print the voronoi diagram in the form:
 *  p0.x p0.y
 *  p1.x p1.y
 *  p0 p1 are two points, which represents a segment in the voronoi diagram.
 *  @param[in] sl -- segment list will be printed.
   
 */
void print_voronoi(seg_list_t *sl){
    seg_t *s = sl->head;
    while(s != NULL){
        point_t p0 = s->start;
        point_t p1 = s->end;
        printf("%d, %d\n%d, %d\n\n", p0.x, p0.y, p1.x, p1.y);
        s = s->next;

    }
}


seg_list_t* get_voronoi_segs(point_t *ps, int count){
    return generate_voronoi(ps, count);
    
}
