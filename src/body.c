#include <string.h>
#include <stdio.h>
#include "body.h"

body init_body(double mass, double position[2], double speed[2]){
  body b;
  b.mass = mass;
  memcpy(b.position,position,sizeof(double)*2);
  memcpy(b.speed,speed,sizeof(double)*2);
  return b;
}

double get_distance(body b1, body b2) {
	double res = sqrt(pow(b1.position[0] - b2.position[0],2) + pow(b1.position[1] - b2.position[1],2));
	return res;
}

void print_body(body b) {
  printf("Mass: %f, Position: %f/%f, Speed: %f/%f\n", b.mass, b.position[0], b.position[1], b.speed[0], b.speed[1]);
}
