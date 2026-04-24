#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "geometry.h"

polygon_list* parse_file(char* file_name){
    FILE *file = fopen(file_name,"r");
    assert(file!=NULL);
    int size;
    fscanf(file,"%d ",&size);
    polygon* plist = malloc(sizeof(polygon)*size);
    for (int i =0;i<size;i++){
        fscanf(file,"%d",&(plist[i].len)); //printf("size %d \n",plist[i].len);
        plist[i].vertices = malloc(sizeof(point)*plist[i].len);
        for (int j = 0;j<plist[i].len;j++){
            fscanf(file,";(%lf,%lf,%lf)", &(plist[i].vertices[j].x) , &(plist[i].vertices[j].y) , &(plist[i].vertices[j].z) );
            //printf("%f,%f,%f\n",(plist[i].vertices[j].x),(plist[i].vertices[j].y),(plist[i].vertices[j].z));
        }
        fscanf(file,";(%d,%d,%d)",&(plist[i].col.r),&(plist[i].col.g),&(plist[i].col.b));
        plist[i].p = get_plane_passing_by(plist[i].vertices[0],plist[i].vertices[1],plist[i].vertices[2]);
    }
    fclose(file);

    polygon_list* p = create_from(plist,size);
    return p;
}