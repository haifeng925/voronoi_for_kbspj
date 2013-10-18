#ifndef __ENVDEFS_H
#define __ENVDEFS_H

/* This program is used to generate a Voronoi Diagram from a list of points for
 * kbs project. This version only use static memory management of C, some
 * functions like malloc, alloc, realloc... will not be used. So some
 * structures, such as arc_pool and seg_pool are created to manage the memory,
 * those provide some functions such as dispatch_seg and dispatch_arc to get
 * memory for new instance.
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
 * To use the function of the this program, just call get_voronoi_segs(point_t*, int); 
 * This two function is the same, they take a list of points and the number of
 * points as input, then return a list of segments, which represent a voronoi
 * diagram.
 *
 * Finally, this version(with static memory management) is abandoned, the
 * new version with malloc is located in the folder "voronoi_mm".
 */

typedef int BOOL;

#define TRUE 1

#define FALSE 0

#define POINT_POOL_SIZE 100

#define EVENT_QUEUE_SIZE 100

#define EVENT_LIST_SIZE 100

#define ARC_POOL_SIZE 100

#define SEG_POINT_POOL_SIZE 100

#define SEG_POOL_SIZE 100

#define VIRTUAL_NUMBER 5

/*codination*/
typedef struct point{
    int x;
    int y;
}point_t;


typedef struct intersection{
    point_t p;
    BOOL is_intersected;
}intersection_t;


typedef struct point_node{
    struct point p;
    int pos;
    struct point_node *next;
}point_node_t;



typedef struct points_pool{
    struct point_node points[POINT_POOL_SIZE];
    int points_count;
    int ipoint[POINT_POOL_SIZE];
    struct point_node *head;
    struct point_node *tail;
} points_pool_t;

typedef struct event{
    struct point p; // Center of  circle
    double x; // codination of center + radius
    struct arc *a;
    BOOL valid;
}event_t;



typedef struct event_node{
    struct event *e;
    int pos;
    struct event_node *next;
}event_node_t;

/* typedef struct event_pool{ */
/*     struct event_node events[EVENT_POOL_SIZE]; */
/*     int events_count; */
/*     int ievents[EVENT_POOL_SIZE]; */
/*     struct event_node *head; */
/*     struct event_node *tail; */
/* }event_pool_t; */

typedef struct event_queue{
    event_node_t events[EVENT_QUEUE_SIZE];
    int events_count;
    int ievents[EVENT_QUEUE_SIZE];
    event_node_t *head;
    event_node_t *tail;
}event_queue_t;


typedef struct seg{
    struct point start, end;
    BOOL done;
}seg_t;


typedef struct arc{
    struct point p;
    struct arc *prev, *next;
    struct event *e;
    struct seg *s0;
    struct seg *s1;
    int pos;
}arc_t;

/* typedef struct arc_node{ */
/*     arc_t arc; */
/*     int pos; */
/*     struct arc_node *next; */
/* }arc_node_t; */

typedef struct arc_pool{
    arc_t arcs[ARC_POOL_SIZE];
    int iarc[ARC_POOL_SIZE];
    int count;
    arc_t *head;
    arc_t *tail;
}arc_pool_t;

typedef struct event_list{
    event_t events[EVENT_LIST_SIZE];
    int i[EVENT_LIST_SIZE];
    int count;
    event_t *head;
}event_list_t;


typedef struct seg_list{
    seg_t* segs[SEG_POINT_POOL_SIZE];
    int segs_count;
}seg_list_t;


typedef struct seg_pool{
    seg_t segs[SEG_POOL_SIZE];
    BOOL i[SEG_POOL_SIZE];
    int segs_count;
}seg_pool_t;

typedef struct ness_intersections{
    intersection_t its[4];
    int count;
}ness_intersections_t;

extern struct points_pool p_pool;
//extern struct event_pool e_pool;
extern struct event_queue e_queue;
extern struct arc_pool arc_pool;
extern struct seg_list seg_list;
extern struct event_list event_list;
extern struct seg_pool seg_pool;

// eventqueue.c
void init_event_pool(void);
void push_event(struct event*);
struct event pop_event(void);
struct event *top_event(void);
void is_event_pool_empty(void);
/* void event_clone(struct event*, struct event*); */
int is_ep_empty();
event_node_t* get_partion_e(event_node_t*, event_node_t*);
void e_quicksort(event_node_t*, event_node_t*);
void events_quicksort(void);

void print_event(struct event*);
int find_free_pos_e(void);

// pointqueue.c
void init_point_list(void);
void push_point(struct point);
struct point top_point(void);
struct point pop_point(void);
void print_point(struct point*);
BOOL is_pp_empty(void);
int find_free_pos(void);
//void p_quicksort(struct point_node*, struct point_node*);
struct point_node* get_partion(struct point_node*, struct point_node*);
void points_quicksort(void);

/* arcpool.c */
void init_arc_pool(void);
int find_free_pos_arc(void);
struct arc* dispatch_arc(void);
void destory_arc(arc_t *);

void init_event_list(void);
int find_free_pos_e_list(void);
struct event* dispatch_event(void);

/* segspool.c */
void init_seg_pool(void);
int find_free_segs(void);
seg_t *dispatch_seg(void);

// seglist.c //
void init_seg_list(void);
int is_seg_list_full(void);
void push_back_seg(seg_t*);

//voronoi.c
void v_error(char *);
void debug_printf(char *);

point_t build_point(int, int);

void generate_voronoi(struct point*, int);
void process_event(void);
void process_point(void);
void front_insert(point_t);
void init_voronoi(void);
BOOL intersect(point_t, arc_t *, point_t*);
point_t intersection(point_t, point_t, double);
void check_circle_event(arc_t *,double);
BOOL circle(point_t, point_t, point_t, double *, point_t *);
void finish_edges(void);
void print_output(seg_list_t *);
void finish(seg_t*, point_t);
seg_list_t* get_voronoi_segs(point_t *, int);
void add_virtual_obstacles(void);

/* adjustsegs.c */

BOOL is_point_within_area(point_t *);
void rebuild_seg(seg_t *);
void rebuild_seg_list(seg_list_t *);

/* intersection.c */
int min(int, int);
int max(int, int);
void seg_intersection(point_t, point_t, point_t, point_t, point_t*);
BOOL get_seg_intersection(point_t, point_t, point_t, point_t, point_t *);

extern int X0;
extern int Y0;
extern int X1;
extern int Y1;

#endif
