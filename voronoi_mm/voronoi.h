
#ifndef __ENVDEFS_H
#define __ENVDEFS_H

/* This program is used to generate a Voronoi Diagram from a list of points for
 * kbs project. This version use dynamic memory management, it means that,
 * malloc is used.
 *
 * This program use Fortune's sweep line Algorithm, which is
 * first publiched by Steven Fortune. http://ect.bell-labs.com/who/sjf/
 * For more details of the algorithm, see
 * https://kedras.mif.vu.lt/bastys/academic/ATE/delaunay/FortuneDemo.htm 
 *
 * special for the kbs project, "add_virtual_obstacles" is used to add virtual o
 * bstacles on the edges of the workspace. and "rebuild_seg_list" is used as
 * cutting function.
 *
 * To use the function of the this program, just call seg_list_t* generate_voronoi(point_t*, int);
 * or seg_list_t* get_voronoi_segs(point_t*, int);
 * This two function is the same, they take a list of points and the number of
 * points as input, then return a list of segments, which represent a voronoi
 * diagram.
 *
 * Author: Haifeng Gao
 * Date: 3.2013
 */


typedef int BOOL;

#define TRUE 1

#define FALSE 0

/* The number of virtual obstacles on every edges */
#define VIRTUAL_NUMBER 6
/* Minimal number of obstacles.If the total number of obstacles is less than
 * MIN_OBST, add virtural obstacles on the border of space
 */
#define MIN_OBST 4

#define V_DEBUG(msg) printf("VORONOI_DEBUG ------->: %s \n",msg); 

/*sturct for point on plane, x represents the x-coordinate, y represents the
 * y-coordinate
 */
typedef struct point{
    int x;
    int y;
}point_t;

/* point-node in point queue, next-pointer points the next node */
typedef struct point_node{
    point_t point;
    struct point_node *next;
}point_node_t;


/* a queue of point, it is basically a priority queue. All the points in the
 * queue is sorted by x and y-coordinate. The points with smaller x is in the
 * front of the queue. The points with the same x-coordinate are sorted by
 * y-coordinate.
 */
typedef struct point_queue{
    point_node_t *head;
    //   point_node_t *tail;
    int count;
}point_queue_t;


/*  event   */
typedef struct event{
    struct point point;
    struct arc *arc;
    double x;
    BOOL valid;
    struct event* prev;
    struct event* next;
}event_t;

/* event queue: sorted by the value of x in event */
typedef struct event_queue{
    event_t *head;
    int count;
}event_queue_t;


/* structure for segment, start and end points represent the segment on the
 * plane  */

typedef struct seg{
    point_t start, end;
    BOOL done;
    struct seg *prev;
    struct seg *next;
}seg_t;

/* parabolic arcs */
typedef struct arc{
    point_t p;
    struct arc *prev, *next;
    event_t *e;
    seg_t *s0;
    seg_t *s1;
}arc_t;

/* list of segments in Voronoi Diagram */
typedef struct seg_list{
    seg_t *head;
    seg_t *tail;
    int count;
}seg_list_t;

typedef struct voronoi_context{
    point_queue_t *pq;
    event_queue_t *eq;
    seg_list_t *sl;
    arc_t *arc_head;
}voronoi_context_t;



/* point_queue.c */
void print_point(point_t);
void print_point_queue(point_queue_t*);
void init_point_queue(point_queue_t*);
void free_point_queue(point_queue_t*);
void push_point(point_queue_t*, point_t);
point_t get_top_point(point_queue_t*);
BOOL is_point_queue_empty(point_queue_t*);
point_t pop_point(point_queue_t*);
int size_of_point_queue(point_queue_t*);

/* event_queue.c */
void init_event_queue(event_queue_t*);
void free_event_queue(event_queue_t*);
void push_event(event_queue_t*, event_t*);
event_t pop_event(event_queue_t *);
event_t get_top_event(event_queue_t*);
BOOL is_event_queue_empty(event_queue_t*);
int size_of_event_queue(event_queue_t *);

/* seg_list.c */
void init_seg(seg_t *);
void init_seg_list(seg_list_t*);
void free_seg_list(seg_list_t *);
seg_t* new_seg(void);
void add_seg(seg_list_t*, seg_t*);

/* arc.c */
void init_arc(arc_t*);
arc_t* new_arc();
void free_arc_list(arc_t*);
void print_arc_list(arc_t*);

/* voronoi.c */
point_t build_point(int, int);
seg_list_t* generate_voronoi(point_t*, int);
void process_point(voronoi_context_t *);
void process_event(voronoi_context_t*);
void finish_edge(voronoi_context_t *);
void front_insert(point_t, voronoi_context_t*);
BOOL intersect(point_t, arc_t*, point_t*);
point_t intersection(point_t, point_t, double);
void check_circle_event(arc_t*, double, voronoi_context_t*);
BOOL circle(point_t, point_t, point_t, double*, point_t*);
void finish_edges(voronoi_context_t*);
void print_voronoi(seg_list_t*);
void add_virtual_obstacles(point_queue_t*);
seg_list_t* get_voronoi_segs(point_t*, int);

/* seg_intersection.c */
double cross_product(point_t, point_t, point_t);
double direction(point_t, point_t, point_t);
BOOL on_segment(point_t, point_t, point_t);
void seg_intersection(point_t, point_t, point_t, point_t, point_t *);
BOOL get_seg_intersection(point_t, point_t, point_t, point_t, point_t*);


/* adjustsegs.c */

BOOL is_point_within_area(point_t*);
BOOL is_points_equal(point_t, point_t);
void add_intersections(point_t*, int*, point_t);
void rebuild_seg(seg_t *, seg_list_t*);
void rebuild_seg_list(seg_list_t *);

/* X0 the smallest x-coordinate of the area, X1: greatest x-coordinate; Y0 :
 * smallest y-coordinate, Y1: greatest Y coordinate*/
extern int X0;
extern int Y0;
extern int X1;
extern int Y1;


#endif
