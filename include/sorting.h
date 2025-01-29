#pragma once
#include "types.h"
#include "utils.h"

void box_sort(position *positions, position **buckets,
              unsigned int *bucket_counts, unsigned int n_particles,
              bounds xbounds, bounds ybounds, bounds zbounds,
              index3 bucket_max_indexes, double radius);

void sub_box_sort(position *positions, position **boxes,
                  unsigned int *box_counts, unsigned int n_particles,
                  index3 box_min_indexes, index3 box_max_indexes,
                  double radius);

void find_pairs(position *box1, position *box2, unsigned int box1_count,
                unsigned int box2_count, unsigned int **pairs,
                unsigned int *pairs_count, double radius_sq);

unsigned int flat_index(index3 idx, index3 max_indexes);

unsigned int get_index(unsigned int i, unsigned int j, unsigned int k);

void find_bounds(position *positions, unsigned int n_rows, bounds *xbounds,
                 bounds *ybounds, bounds *zbounds);

unsigned int get_neighbour_indexes(index3 current, index3 *neighbours,
                                   index3 max_indexes);
