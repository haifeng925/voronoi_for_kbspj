#include<stdio.h>
#include"envdefs.h"

int main(){
    point_t ps[8];
    ps[0].x= 200; ps[0].y=200;
    ps[1].x= 400; ps[1].y=400;
    ps[2].x= 200; ps[2].y=800;
    ps[3].x= 300; ps[3].y=600;
    ps[4].x= 400; ps[4].y=900;
    ps[5].x= 600; ps[5].y=300;
    ps[6].x= 600; ps[6].y=500;
    ps[7].x= 1000; ps[7].y=0;

    seg_list_t *sl = get_voronoi_segs(ps, 8);
    // print_output(sl);

    rebuild_seg_list(sl);

    print_output(sl);


    /*
      test for adjustsegs.c
      
     */

    /* init_voronoi(); */

    /* seg_t *seg = dispatch_seg(); */
    /* seg->start.x = 416; */
    /* seg->start.y = 183; */
    /* seg->end.x  = 518; */
    /* seg->end.y = -255; */

    /* push_back_seg(seg); */
    /* rebuild_seg_list(&seg_list); */
    /* print_output(&seg_list); */


    
    /* point_t p; */
    /* p.x = 400; p.y = 10; */
    /* // BOOL ipwa = is_point_within_area(&p); */
    /* printf("is p in the area: %d\n", is_point_within_area(&p)); */
     
    /* point_t p1, p2; */
    /*    p1.x = 10; */
    /*    p1.y = 10; */
    /*    p2.x = 20; */
    /*    p2.y = 10; */
    /*    //     find_nearest_point(&p1,&p2,sl); */
    /*    print_point(&p1); */
//	test_find_nearest_point();
    return 1;
    /*  int i; */
    /*  init_point_list(); */
    /*  for(i=0; i<7; i++){ */
    /*      struct point p; */
    /*      p.x = i; */
    /*      p.y = i; */
    /*      printf("%.2f\n", i); */
    /*      push_point(p); */
    /*  } */
    /*  points_quicksort(); */
    
    /*  for(i=0; i<2; i++){ */
    /*      struct point tmp = pop_point(); */
    /*      print_point(&tmp); */
    /*  } */
    /* for(i=0; i<10; i++){ */
    /*      struct point p; */
    /*      p.x = i+8; */
    /*      p.y = i+8; */
    /*      printf("%.2f\n", i); */
    /*      push_point(p); */
    /*  } */
    /* points_quicksort(); */

    /* for(i=0; i<15; i++){ */
    /*     struct point tmp = pop_point(); */
    /*     print_point(&tmp); */   /* } */
    /* return 1; */
}
