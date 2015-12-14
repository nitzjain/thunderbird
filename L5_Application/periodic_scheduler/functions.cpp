#include "_can_dbc\auto_gen.inc"
#include <stdint.h>
#include "stdio.h"
#include "io.hpp"
#include "periodic_callback.h"
#include "gps_datatype.h"
#include "tasks.hpp"
#include <stdint.h>
#include "compass.hpp"
#include "can.h"
#include "math.h"
#define PI 3.14159265

float ToRadian(float degree){
    float radian = degree * PI/180;
    return radian;
}
float ToDegree(float radian){
    float degree =radian *180/PI;
    return degree;
}
float get_bearing_new(float lat1,float lat2, float lng1, float lng2){
    float dLng = ToRadian(lng2-lng1);

    float bearing= atan2(sin(dLng)*cos(ToRadian(lat2)),cos(ToRadian(lat1))*sin(ToRadian(lat2))-sin(ToRadian(lat1))*cos(ToRadian(lat2))*cos(dLng));

    bearing = ToDegree(bearing);
    bearing = fmodf((bearing +360),360);

    //if(bearing>180){
        bearing = 360-bearing;
        if(bearing>179)
            bearing = bearing - 360;
    //}
    return bearing;
}
float GetBearing(gps_data_t data_gps, gps_data_t chkp_data ){
    float lat1 = data_gps.Latitude;
    float lng1 = data_gps.Longitude;
    float lat2 = chkp_data.Latitude;
    float lng2 = chkp_data.Longitude;

    float dLng = ToRadian(lng2-lng1);

    float bearing= atan2(sin(dLng)*cos(ToRadian(lat2)),cos(ToRadian(lat1))*sin(ToRadian(lat2))-sin(ToRadian(lat1))*cos(ToRadian(lat2))*cos(dLng));

    bearing = ToDegree(bearing);
    bearing = fmodf((bearing +360),360);
    bearing = 360-bearing;
    if(bearing>179){
        bearing = bearing-360;
    }
    return bearing;
}
int calculate_sector(float heading){
    int sector;

    if(heading == 0.0)
        sector = 0;
    else if(heading<=10.0 && heading>0.0)
        sector = 0;
    else if(heading>10.0 && heading<=30.0)
        sector = 1;
    else if(heading>30.0 && heading<=50.0)
        sector = 2;
    else if(heading>50.0 && heading <=70.0)
        sector = 3;
    else if(heading>70.0 && heading <=90.0)
        sector = 4;
    else if(heading>90.0 && heading <=110.0)
        sector = 5;
    else if(heading>110.0 && heading <=130.0)
        sector = 6;
    else if(heading>130.0 && heading <=150.0)
        sector = 7;
    else if(heading>150.0 && heading <=170.0)
        sector = 8;
    else if(heading>170.0)
        sector = 9;
    else if(heading>=-180.0 && heading <=-170.0)
        sector = 9;
    else if(heading>-170.0 && heading<=-150.0)
        sector = 10;
    else if(heading>-150.0 && heading<= -130.0)
        sector = 11;
    else if(heading>-130.0 && heading<= -110.0)
        sector = 12;
    else if(heading>-110.0 && heading<= -90.0)
        sector = 13;
    else if(heading>-90.0 && heading<= -70.0)
        sector = 14;
    else if(heading>-70.0 && heading<= -50.0)
        sector = 15;
    else if(heading>-50.0 && heading<= -30.0)
        sector = 16;
    else if(heading>-30.0 && heading<-10.0)
        sector = 17;
    else if(heading>=-10.0 && heading <0.0)
        sector = 0;
    else{
        sector = -1;
        printf("ERROR: Invalid sector, heading:%f\n",heading);
    }//printf("sector# = %d",sector);
    return sector;
}
void GetDirection(direction_t *dir,int bearing_sector,int heading_sector){

    int delta_sector = bearing_sector - heading_sector;
    if(delta_sector == 0){
        dir->angle = 0;
        dir->dir = 1;
    }
    else if(delta_sector == 9 || delta_sector == -9){
        dir->angle = abs(delta_sector)*20;
        dir->dir = 0;
    }
    else if(delta_sector<9 && delta_sector > 0){
        dir->angle = delta_sector*20;
        dir->dir = 1;
    }
    else if(delta_sector>-9 && delta_sector < 0){
        dir->angle = abs(delta_sector)*20;
        dir->dir = 0;
    }
    else if(delta_sector <-9){
        dir->angle = (18+delta_sector)*20;
        dir->dir = 1;
    }
    else if(delta_sector>9){
        dir->angle = (18-delta_sector)*20;
        dir->dir = 0;
    }
    else
        dir->angle = 1000;
}
