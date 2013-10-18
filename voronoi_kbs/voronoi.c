#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "envdefs.h"
#include "../tourenplanung/send_raw_eth.h"
//#include "find_nearestpointonline.h"

int X0,Y0,X1,Y1;
// TODO 1. sort for event queue

point_t build_point(int x, int y){
    point_t p;
    p.x = x;
    p.y = y;
    return p;
}


void v_error(char *e_info){
    printf("ERROR-VORONOI: %s \n", e_info);
}

void debug_printf(char *debug_info){
    printf("DEBUG ----> : %s \n", debug_info);
}

void init_voronoi(){
    init_point_list();
    init_event_pool();
    init_seg_list();
    init_arc_pool();
    init_event_pool();
    //   printf("----------memory init succeeded!!\n\n");
}

void print_arc_list(){
    printf("==================================================\n");
    printf("==============print arc list =====================\n");
    arc_t *arc = arc_pool.head;
    while(arc != NULL){
        print_point(&arc->p);
        arc = arc->next;
    }
    printf("==============print arc list =====================\n");
    printf("==================================================\n");    
    
}

void add_virtual_obstacles(){

    int dis_Y = Y1 - Y0;
    int dis_X = X1 - X0;
    int interval_Y = dis_Y / (VIRTUAL_NUMBER - 1);
    int interval_X = dis_X / (VIRTUAL_NUMBER - 1);
    printf("-------------------print the virtual obstacles---------------\n");
    int i=1;
    for (; i<VIRTUAL_NUMBER-1 ; i++){
        int p_x = X0 + i*interval_X;
        point_t p_with_Y0 = build_point(p_x, Y0);
        point_t p_with_Y1 = build_point(p_x, Y1);

        print_point(&p_with_Y0);
        print_point(&p_with_Y1);
        push_point(p_with_Y0);
        push_point(p_with_Y1);
    }
    i = 1;
    for(; i<VIRTUAL_NUMBER-1; i++){
        int p_y = Y0 + i*interval_Y;
        point_t p_with_X0 = build_point(X0, p_y);
        point_t p_with_X1 = build_point(X1, p_y);

        print_point(&p_with_X0);
        print_point(&p_with_X1);
        push_point(p_with_X0);
        push_point(p_with_X1);
    }

    point_t X0Y0 = build_point(X0, Y0);
    point_t X0Y1 = build_point(X0, Y1);
    point_t X1Y0 = build_point(X1, Y0);
    point_t X1Y1 = build_point(X1, Y1);
    push_point(X0Y0);
    push_point(X0Y1);
    push_point(X1Y0);
    push_point(X1Y1);

    print_point(&X0Y0);
    print_point(&X0Y1);
    print_point(&X1Y0);
    print_point(&X1Y1);
    printf("----------------virtual obstacles---------------\n");

 }

void generate_voronoi(struct point *p_array, int num){
    init_voronoi();
    //  printf("init memory succed\n");
    X0=0;
    Y0=0;
    X1=2000;
    Y1=2000;

    if(num >= POINT_POOL_SIZE){
        v_error("the number of points is out of bound");
        return;
    }

    int i;
    for (i=0; i<num; i++){
        push_point(p_array[i]);
        //   printf("%f, %f\n", p_array[i].x,  p_array[i].y);
        if(p_array[i].x < X0) X0 = p_array[i].x;
        if(p_array[i].y < Y0) Y0 = p_array[i].y;
        if(p_array[i].x > X1) X1 = p_array[i].x;
        if(p_array[i].y > Y1) Y1 = p_array[i].y;

    }
    add_virtual_obstacles();
    points_quicksort();

    /* X0=0; */
    /* Y0=0; */
    /* X1=2500; */
    /* Y1=2500; */
    //   printf("quicksort finished \n");
    //   printf("%f, %f, %f, %f\n\n\n", X0, Y0, X1,Y1);

    //add 20% margins to the bounding box
    /* double dx = (X1-X0+1)/5.0; */
    /* double dy = (Y1-Y0+1)/5.0; */
    /* X0 -= dx; */
    /* X1 += dx; */
    /* Y0 -= dy; */
    /* Y1 += dy; */
    // printf("%f, %f, %f, %f\n\n\n", X0, Y0, X1,Y1);

    while (!is_pp_empty()){
        if(!is_ep_empty()){
             event_t * top_e = top_event();
             point_t top_p = top_point();
             if(top_e->x <= top_p.x){
                 process_event();
             }else{
                 process_point();
             }

        }else{
            process_point();
        }

    }

    //   print_arc_list();
    while(!is_ep_empty()){
        process_event();
    }
    finish_edges();
    rebuild_seg_list(&seg_list);
    int k;
    char *buf=malloc(sizeof(char)*1024);
    int pcount=0;
    for(k=0;k<seg_list.segs_count;k++){
        pcount+=sprintf(&buf[pcount],"%d;%d;%d;%d",k,seg_list.segs[k]->start.x,seg_list.segs[k]->start.y,seg_list.segs[k]->end.x,seg_list.segs[k]->end.y);
    }
    send_raw_eth_chn(34,buf,603);
}


void process_point(){
    /* debug_printf("process_point_s"); */
    // Get the next point at the top of the queue (with smaller x coordinate)
    point_t p = top_point();
    pop_point();

    front_insert(p);
    /* debug_printf("process_point_end"); */
}



void process_event(){
    /* debug_printf("process_event_s"); */
    /* printf("\n>>>>>>>>>>>>>><<process event "); */
    event_t *e = top_event();
    /* printf("event-->%.3f ", e->x);//print_point(&e->p); */
    /* printf("valid? %d\n", e->valid); */
    pop_event();

    if(e->valid!=FALSE){
        seg_t * new_seg = dispatch_seg();
        new_seg->start = e->p;

         /* printf("\n\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n"); */

         //   print_arc_list();


        arc_t *a = e->a;
        if(a->prev!=NULL){
            a->prev->next = a->next;
            a->prev->s1 = new_seg;

        }

        if(a->next!=NULL){
            a->next->prev = a->prev;
            a->next->s0 = new_seg;
        }

        //  print_arc_list();
        if(a->s0!=NULL){
            if(a->s0->done == FALSE){
                 a->s0->end = e->p;
                 a->s0->done = TRUE;
            }
        }
        if(a->s1!=NULL){
            if(a->s1->done == FALSE){
                a->s1->end = e->p;
                a->s1->done = TRUE;
            }

        }
        if(a->prev!=NULL) check_circle_event(a->prev, e->x);

        if(a->next!=NULL) check_circle_event(a->next, e->x);
    }

    /* debug_printf("process_event_e"); */


}



void front_insert(point_t p){
    /* printf("process_front_insert_s points: %d, %d\n", p.x, p.y); */

    if(arc_pool.count == 0){
        arc_t *new_arc = dispatch_arc();
        arc_pool.head = new_arc;
        new_arc->p = p;
        new_arc->prev = NULL;
        new_arc->next = NULL;
        new_arc->e = NULL;
        /* printf("front inseert Head == NULL\n"); */
        return;
    }

    arc_t *i = arc_pool.head;
    for(; i != NULL; i = i->next){
        point_t z, zz;
        if(intersect(p, i, &z) == TRUE){
            if (i->next && intersect(p, i->next ,&zz)==FALSE){
                arc_t* new_arc = dispatch_arc();
                /* debug_printf("ARC--dispatch memory space for arc..."); */

                new_arc->p = i->p;
                new_arc->prev = i;
                new_arc->next = i->next;



                i->next->prev = new_arc;

                /* printf("--NEW ARC 1--"); */
                /* print_point(&i->next->prev->p); */

                i->next = i->next->prev;
                //i->next->prev = dispatch_arc();
            }else{
                arc_t* new_arc = dispatch_arc();
                /* debug_printf("ARCAAAAAAAAAAAAAAA..."); */


                new_arc->p = i->p;
                new_arc->prev = i;


                i->next = new_arc;

                /* printf("--NEW ARC 2--");  */
                /* print_point(&i->next->p); */

            }

            i->next->s1 = i->s1;

            // Add p between i and i->next;
            arc_t *new_arc = dispatch_arc();
            /* debug_printf("ARC--dispatch memory space for arc..."); */

            new_arc->p = p;
            new_arc->prev = i;
            new_arc->next = i->next;



            i->next->prev = new_arc;
            /* printf("--NEW ARC 3--"); */
            /* print_point(&i->next->prev->p); */
            i->next = i->next->prev;

            i = i->next; // i points to the new arc

            i->prev->s1 = i->s0 = dispatch_seg();
            i->prev->s1->start = z;
            /* debug_printf("-- NEW SEG..."); */


            // push_back_seg(i->s0);

            i->next->s0 = i->s1 = dispatch_seg();
            i->next->s0->start = z;
            /* debug_printf("-- NEW SEG..."); */

            check_circle_event(i, p.x);
            check_circle_event(i->prev, p.x);
            check_circle_event(i->next, p.x);
            /* debug_printf("process_front_insert_e2"); */
            //  print_arc_list();
            return;
        }
    }

    // special case:

    arc_t *new_arc;
    for(new_arc=arc_pool.head; new_arc->next!=NULL; new_arc=new_arc->next);
    /* debug_printf("process_front_insert_special case_s"); */
    arc_t *n_arc = dispatch_arc();
    n_arc->p = p;
    n_arc->prev = new_arc;
    new_arc->next = n_arc;

    point_t start;
    start.x = X0;

    start.y =(int) (((double)(new_arc->next->p.y + new_arc->p.y))/2.0);
    seg_t *new_seg = dispatch_seg();
    new_seg->start = start;
    /* new_arc->s1->start = start; */

    /* new_arc->next->s0 = new_arc->s1; */
    //  print_arc_list();
    new_arc->s1 = new_arc->next->s0 = new_seg;

}

void check_circle_event(arc_t *i, double x0){

    if((i->e != NULL) && (i->e->x != x0)){

        i->e->valid = FALSE;    }

    i->e = NULL;

    if((i->prev == NULL)||(i->next == NULL)){
        return;
    }

    double x;
    point_t o;

    BOOL rst_circle = circle(i->prev->p, i->p, i->next->p, &x,&o);
    if(rst_circle == TRUE && x > x0){
        i->e = dispatch_event();
        i->e->x = x;
        i->e->p = o;
        i->e->a = i;
        i->e->valid = TRUE;

        push_event(i->e);
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
        /* printf("p->prev"); */
        /* print_point(&i->prev->p); */
        /* print_point(&i->p); */
        /* printf("%d\n", p.x); */
        point_t pa = intersection(i->prev->p, i->p, (double)p.x);
        a = pa.y;
        /* printf("xxxxxx---> IT prev: x: %d, %d\n", pa.x, pa.y); */
    }
    if(i->next != NULL){
        /* print_point(&i->p); */
        /* print_point(&i->next->p); */
        /* printf("%d\n", p.x); */
        point_t pb = intersection(i->p, i->next->p,(double) p.x);
        /* printf("xxxxxx---> IT next: x: %d, %d\n", pb.x, pb.y); */

        b = pb.y;

    }

    if ((!i->prev || a <= p.y) && (!i->next || p.y <= b)) {
        result->y = p.y;

        result->x = (int)((double)(i->p.x*i->p.x + (i->p.y-result->y)*(i->p.y-result->y) - p.x*p.x) / ((double)(2*i->p.x - 2*p.x)));

        /* printf("-----Intersection: -> %d,  %d\n", result->x, result->y); */
        return TRUE;
    }
    /* printf("intersection false\n"); */
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
    /*    printf("original: "); */
    /* print_point(&p0); */
    /* print_point(&p1); */
    /* printf("%.3f\n",l); */
    /* print_point(&res); */

   return res;
}


void finish_edges(){
    // print_arc_list();
    double l = (double)(X1 + (X1-X0) + (Y1-Y0));
    //  printf("xxxxxxxxxxxxxxx...finish_edge  %.6f\n",l);
     arc_t *i = arc_pool.head;
     for(; i->next != NULL; i = i->next){
         if(i->s1 != NULL){
             /* printf("arc --- "); */
             /* print_point(&i->p); */
             /* printf("next: "); */
             /* print_point(&i->next->p); */
             point_t new_p = intersection(i->p,i->next->p,l*2);
             /* printf("finish edges\t"); */
             /* print_point(&new_p); */
             //i-s1->finish = new_p;
             finish(i->s1, new_p);
         }
     }

}

void print_output(seg_list_t * sl){
    // printf("Workspace: %d %d %d %d \n\n", X0, Y0, X1, Y1);
    int i=0;
    for(;i<sl->segs_count; i++){
        point_t p0 = sl->segs[i]->start;
        point_t p1 = sl->segs[i]->end;
        printf("%d, %d\n%d, %d\n\n", p0.x, p0.y, p1.x, p1.y);
    }
    /* for(;i<seg_list.segs_count; i++){ */
    /*     point_t p0 = seg_list.segs[i]->start; */
    /*     point_t p1 = seg_list.segs[i]->end; */
    /*     printf("%.3f, %.3f, %.3f, %.3f\n", p0.x, p0.y, p1.x, p1.y); */
    /* } */
}

void finish(seg_t *seg, point_t p){
    if(seg->done == TRUE){
        return;
    }
    seg->end = p;
    seg->done = TRUE;
    /* printf("---------------finish::\n\n"); */
    /* print_point(&seg->start); */
    /* print_point(&seg->end); */
    /* printf("\n"); */
}

seg_list_t* get_voronoi_segs(struct point* ps, int count){
    generate_voronoi(ps, count);

    return &seg_list;
}


