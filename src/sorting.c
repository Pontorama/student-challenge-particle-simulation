#include "sorting.h"

unsigned int flat_index(index3 idx, index3 max_indexes) {
  return max_indexes.i * max_indexes.j * idx.k + max_indexes.i * idx.j + idx.i;
}

void box_sort(position *positions, position **buckets,
              unsigned int *bucket_counts, unsigned int n_particles,
              bounds xbounds, bounds ybounds, bounds zbounds,
              index3 bucket_max_indexes, double radius) {
  index3 idx;
  unsigned int flat_idx;
  for (unsigned int p = 0; p < n_particles; p++) {
    idx.i = (unsigned int)((positions[p].x - xbounds.min) / radius);
    idx.j = (unsigned int)((positions[p].y - ybounds.min) / radius);
    idx.k = (unsigned int)((positions[p].z - zbounds.min) / radius);
    flat_idx = flat_index(idx, bucket_max_indexes);
    buckets[flat_idx][bucket_counts[flat_idx]++] = positions[p];
  }
}

void sub_box_sort(position *positions, position **boxes,
                  unsigned int *box_counts, unsigned int n_particles,
                  index3 box_min_indexes, index3 box_max_indexes,
                  double radius) {
  // This function assumes that all positions have been shifted so that the
  // smallest position is (0,0,0), since this is going to be called multiple
  // times
  index3 idx;
  unsigned int flat_idx;
  index3 relative_max = {box_max_indexes.i - box_min_indexes.i,
                         box_max_indexes.j - box_min_indexes.j,
                         box_max_indexes.k - box_min_indexes.k};
  for (unsigned int p = 0; p < n_particles; p++) {
    idx.i = (unsigned int)(positions[p].x / radius);
    idx.j = (unsigned int)(positions[p].y / radius);
    idx.k = (unsigned int)(positions[p].z / radius);
    if (idx.i >= box_min_indexes.i && idx.j >= box_min_indexes.j &&
        idx.k >= box_min_indexes.k && idx.i < box_max_indexes.i &&
        idx.j < box_max_indexes.j && idx.k < box_max_indexes.k) {
      // Shift
      idx.i -= box_min_indexes.i;
      idx.j -= box_min_indexes.j;
      idx.k -= box_min_indexes.k;
      flat_idx = flat_index(idx, relative_max);
      boxes[flat_idx][box_counts[flat_idx]++] = positions[p];
    }
  }
}

void find_bounds(position *positions, unsigned int n_rows, bounds *xbounds,
                 bounds *ybounds, bounds *zbounds) {
  double min_x, max_x, min_y, max_y, min_z, max_z;
  min_x = max_x = positions[0].x;
  min_y = max_y = positions[0].y;
  min_z = max_z = positions[0].z;
  for (int i = 1; i < n_rows; i++) {
    if (min_x > positions[i].x) {
      min_x = positions[i].x;
    } else if (max_x < positions[i].x) {
      max_x = positions[i].x;
    }
    if (min_y > positions[i].y) {
      min_y = positions[i].y;
    } else if (max_y < positions[i].y) {
      max_y = positions[i].y;
    }
    if (min_z > positions[i].z) {
      min_z = positions[i].z;
    } else if (max_z < positions[i].z) {
      max_z = positions[i].z;
    }
  }
  xbounds->min = min_x;
  xbounds->max = max_x;
  ybounds->min = min_y;
  ybounds->max = max_y;
  zbounds->min = min_z;
  zbounds->max = max_z;
}

void find_pairs(position *box1, position *box2, unsigned int box1_count,
                unsigned int box2_count, unsigned int **pairs,
                unsigned int *pairs_count, double radius_sq) {
  for (unsigned int i = 0; i < box1_count; i++) {
    for (unsigned int j = 0; j < box2_count; j++) {
      if (get_distance_sq(box1[i], box2[j]) <= radius_sq) {
        pairs[box1[i].index][pairs_count[box1[i].index]] = box2[j].index;
        pairs_count[box1[i].index]++;
        pairs[box2[j].index][pairs_count[box2[j].index]] = box1[i].index;
        pairs_count[box2[j].index]++;
      }
    }
  }
}

unsigned int get_neighbour_indexes(index3 current, index3 *neighbours,
                                   index3 max_indexes) {
  unsigned int n = 0;
  int di;
  int dj;
  int dk;
  int nk = 0;
  int nj = 0;
  int ni = 1;
  while (nk <= 1) {
    while (nj <= 1) {
      while (ni <= 1) {
        di = current.i + ni;
        dj = current.j + nj;
        dk = current.k + nk;
        if (di >= 0 && di <= max_indexes.i && dj >= 0 && dj <= max_indexes.j &&
            dk <= max_indexes.k) {
          neighbours[n].i = di;
          neighbours[n].j = dj;
          neighbours[n].k = dk;
          n++;
        }
        ni++;
      }
      ni = -1;
      nj++;
    }
    nj = -1;
    nk++;
  }
  return n;
}
