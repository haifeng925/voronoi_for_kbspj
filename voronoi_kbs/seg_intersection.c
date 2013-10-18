#include <stdlib.h>
#include <stdio.h>
#include "envdefs.h"
/* typedef int BOOL; */

/* #define TRUE 1 */

/* #define FALSE 0 */
/* typedef struct point{ */
/*     int x; */
/*     int y; */
/* }point_t; */

int min(int x, int y){
    if(x <= y){
        return x;
    }else{
        return y;
    }
}

int max(int x, int y){
    if(x >= y){
        return x;
    }else{
        return y;
    }
}

// cross product (P1 - P0)x(P2 - P0)
double cross_product(point_t p1, point_t p2, point_t p0){
    return (p1.x-p0.x)*(p2.y-p0.y) - (p2.x - p0.x)*(p1.y - p0.y);
}

// get direction of Pk according to segment Pi-Pj
double direction(point_t pi, point_t pj, point_t pk){
    return cross_product(pk, pj, pi);
}

// if point pk on segment PiPj
BOOL on_segment(point_t pi, point_t pj, point_t pk){
    int minx = min(pi.x, pj.x);
    int maxx = max(pi.x, pj.x);
    int miny = min(pi.y, pj.y);
    int maxy = max(pi.y, pj.y);
    if(pk.x >= minx && pk.x<=maxx && pk.y >= miny && pk.y <= maxy){
        return TRUE;
    }else{
        return FALSE;
    }
    
}
// get intersection points, check if there are intersection first.
void seg_intersection(point_t u1, point_t u2, point_t v1, point_t v2, point_t *rst){
    *rst = u1;
    double t=(double)((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
        /(double)((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
    
    rst->x = rst->x + ((u2.x-u1.x)*t);
    rst->y = rst->y + ((u2.y-u1.y)*t);
//    printf("u1: %d %d, u2: %d %d, v1: %d %d v2: %d %d, rst: %d %d \n", u1.x, u1.y, u2.x, u2.y, v1.x, v1.y, v2.x, v2.y, rst->x, rst->y);
}

BOOL get_seg_intersection(point_t p1, point_t p2, point_t p3, point_t p4, point_t *rst){
    double d1 = direction(p3, p4, p1);
    double d2 = direction(p3, p4, p2);
    double d3 = direction(p1, p2, p3);
    double d4 = direction(p1, p2, p4);

    if(d1*d2 < 0 && d3*d4 < 0){
        seg_intersection(p1, p2, p3, p4, rst);
        //     printf("its-1\n");
        return TRUE;
    }else if(d1 == 0 && on_segment(p3, p4, p1)){
        //      printf("its-2\n");

        seg_intersection(p1, p2, p3, p4, rst);
        return TRUE;
    }else if(d2 == 0 && on_segment(p3, p4, p2)){
        //      printf("its-3\n");

        seg_intersection(p1, p2, p3, p4, rst);
        return TRUE;
    }else if(d3 == 0 && on_segment(p1, p2, p3)){
        //       printf("its-4\n");

        seg_intersection(p1, p2, p3, p4, rst);
        return TRUE;
    }else if(d4 == 0 && on_segment(p1, p2, p4)){
        //      printf("its-5\n");

        seg_intersection(p1, p2, p3, p4, rst);
        return TRUE;
    }
    //  printf("no intersection\n");

    return FALSE;

}

/*
 test for get_intersection
 */


/* int main(void){ */
/*     point_t u1,u2,v1,v2; */
/*     point_t rst; */
/*     printf("please type two points of line1:\n"); */
/*     scanf("%d%d%d%d",&u1.x,&u1.y,&u2.x,&u2.y); */
/*     printf("please type twe points of line2:\n"); */
/*     scanf("%d%d%d%d",&v1.x,&v1.y,&v2.x,&v2.y); */

/*     BOOL is_it = get_seg_intersection(u1, u2, v1, v2, &rst); */
/*     if(is_it == FALSE) { */
/*         printf("no intersection\n"); */
/*     }else{ */
/*         printf("intersect: %d %d\n", rst.x, rst.y); */
/*     } */

/* } */
