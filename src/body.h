#ifndef _BODY_H
#define _BODY_H

typedef struct {
  double mass;
  double position[2];
  double speed[2];
} body;

body init_body(double mass, double position[2], double speed[2]);
double get_distance(body b1, body b2);
void print_body(body b);

#endif
