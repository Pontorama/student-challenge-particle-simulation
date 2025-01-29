#pragma once

typedef struct {
  unsigned int index;
  double x;
  double y;
  double z;
} position;

typedef struct {
  double min;
  double max;
} bounds;

typedef struct {
  unsigned int i;
  unsigned int j;
  unsigned int k;
} index3;
