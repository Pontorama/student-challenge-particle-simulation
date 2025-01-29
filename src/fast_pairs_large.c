#include "fast_pairs_large.h"

index3 big_box_min(index3 big_box_index, index3 n_small_boxes,
                   index3 n_big_boxes) {
  index3 out;
  if (big_box_index.i == 0) {
    out.i = 0;
  } else {
    out.i = big_box_index.i * n_small_boxes.i / n_big_boxes.i - 1;
  }
  if (big_box_index.j == 0) {
    out.j = 0;
  } else {
    out.j = big_box_index.j * n_small_boxes.j / n_big_boxes.j - 1;
  }
  if (big_box_index.k == 0) {
    out.k = 0;
  } else {
    out.k = big_box_index.k * n_small_boxes.k / n_big_boxes.k - 1;
  }
  return out;
}

index3 big_box_max(index3 big_box_index, index3 n_small_boxes,
                   index3 n_big_boxes) {
  index3 out;
  if (big_box_index.i == n_big_boxes.i) {
    out.i = n_small_boxes.i + 1;
  } else {
    // +1 at the end since we want to take care of edges of big boxes
    out.i = (big_box_index.i + 1) * n_small_boxes.i / n_big_boxes.i + 1;
  }
  if (big_box_index.j == n_big_boxes.j) {
    out.j = n_small_boxes.j + 1;
  } else {
    out.j = (big_box_index.j + 1) * n_small_boxes.j / n_big_boxes.j + 1;
  }
  if (big_box_index.k == n_big_boxes.k) {
    out.k = n_small_boxes.k + 1;
  } else {
    out.k = (big_box_index.k + 1) * n_small_boxes.k / n_big_boxes.k + 1;
  }

  return out;
}

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("This program takes exactly 3 arguments; input file, output file "
           "and # of threads to be used.\n");
    printf("Example: ./fast_pairs data/positions_large.xyz "
           "output/fast_pairs.csv 4\n");
  }
  // Take first argument as filename
  FILE *fp = fopen(argv[1], "r");
  // Find out how many particles there are
  unsigned int n_particles = get_file_length(fp);
  printf("File length: %i\n", n_particles);
  rewind(fp);
  // Allocate memory and load positions
  position *positions = (position *)malloc(sizeof(position) * n_particles);
  load_xyz_file(fp, positions, n_particles);
  fclose(fp);
  // Find bounds of volume containing particles
  bounds xbounds;
  bounds ybounds;
  bounds zbounds;
  // Take time of entire program
  find_bounds(positions, n_particles, &xbounds, &ybounds, &zbounds);
  printf("Bounds: \n");
  printf("x: [%.4f, %.4f] y: [%.4f, %.4f] z: [%.4f, %.4f]\n", xbounds.min,
         xbounds.max, ybounds.min, ybounds.max, zbounds.min, zbounds.max);
  shift_positions(positions, n_particles, xbounds.min, ybounds.min,
                  zbounds.min);
  double radius = 0.05;
  double radius_sq = radius * radius;
  // Want to sort the volume into big then small boxes
  // Calculate nr of small/big boxes
  index3 n_small_boxes = {(unsigned int)((xbounds.max - xbounds.min) / radius),
                          (unsigned int)((ybounds.max - ybounds.min) / radius),
                          (unsigned int)((zbounds.max - zbounds.min) / radius)};
  index3 n_big_boxes = {5, 5, 5};
  unsigned int n_small_box_total =
      (n_small_boxes.i + 1) * (n_small_boxes.j + 1) * (n_small_boxes.k + 1);
  printf("Number of (0.05*0.05*0.05) boxes in each direction\n");
  printf("%i %i %i\n", n_small_boxes.i, n_small_boxes.j, n_small_boxes.k);

  unsigned int max_particles_per_box = n_particles / 1000;
  // Max number of small boxes in big box
  unsigned int max_small_in_big =
      (n_small_boxes.i / n_big_boxes.i + n_small_boxes.i % n_big_boxes.i + 2) *
      (n_small_boxes.j / n_big_boxes.j + n_small_boxes.j % n_big_boxes.j + 2) *
      (n_small_boxes.k / n_big_boxes.k + n_small_boxes.k % n_big_boxes.k + 2);

  // Create memory to store output
  unsigned int **pairs =
      create_matrix_ui(n_particles, max_particles_per_box * 100);
  unsigned int *pairs_count =
      (unsigned int *)calloc(n_particles, sizeof(unsigned int));

  // Set nbr of threads to use
  unsigned int n_threads = atoi(argv[3]);
  omp_set_dynamic(0);
  omp_set_num_threads(n_threads);
  position **boxes =
      create_matrix_pos(max_small_in_big * n_threads, max_particles_per_box);

  unsigned int *box_counts = (unsigned int *)calloc(
      max_small_in_big * n_threads, sizeof(unsigned int));
  index3 *neighbour_index = (index3 *)malloc(sizeof(index3) * 13 * n_threads);
#pragma omp parallel shared(pairs, pairs_count, positions)
  {
    unsigned int thread_num = omp_get_thread_num();
    position **boxes_thread = &boxes[max_small_in_big * thread_num];
    unsigned int *box_counts_thread =
        &box_counts[max_small_in_big * thread_num];
    index3 *neighbour_index_thread = &neighbour_index[thread_num * 13];
    // Loop over big boxes
    index3 big_box_index;
    index3 min_big_box;
    index3 max_big_box;
    index3 small_box_index;
    index3 relative_index;
    unsigned int flat_idx;
    unsigned int n1;
    unsigned int n2;
    position *box1;
    position *box2;
    // Max 13 neighbours
    unsigned int n_neighbours;
    unsigned int flat_idx_neighbour;
    for (big_box_index.k = 0; big_box_index.k < n_big_boxes.k;
         big_box_index.k++) {
      for (big_box_index.j = 0; big_box_index.j < n_big_boxes.j;
           big_box_index.j++) {
        for (big_box_index.i = thread_num; big_box_index.i < n_big_boxes.i;
             big_box_index.i += n_threads) {
          // Sort particles in this big box into small boxes
          min_big_box = big_box_min(big_box_index, n_small_boxes, n_big_boxes);
          max_big_box = big_box_max(big_box_index, n_small_boxes, n_big_boxes);
          // Reset box counts
          for (unsigned int i = 0; i < (unsigned int)(max_small_in_big / 4) * 4;
               i += 4) {
            box_counts_thread[i] = 0;
            box_counts_thread[i + 1] = 0;
            box_counts_thread[i + 2] = 0;
            box_counts_thread[i + 3] = 0;
          }
          for (unsigned int i = (unsigned int)(max_small_in_big / 4) * 4;
               i < max_small_in_big; i++) {
            box_counts_thread[i] = 0;
          }
          sub_box_sort(positions, boxes_thread, box_counts_thread, n_particles,
                       min_big_box, max_big_box, radius);
          relative_index.i = max_big_box.i - min_big_box.i;
          relative_index.j = max_big_box.j - min_big_box.j;
          relative_index.k = max_big_box.k - min_big_box.k;

          // Loop over small boxes
          for (small_box_index.k = 1; small_box_index.k < relative_index.k - 1;
               small_box_index.k++) {
            for (small_box_index.j = 1;
                 small_box_index.j < relative_index.j - 1;
                 small_box_index.j++) {
              for (small_box_index.i = 1;
                   small_box_index.i < relative_index.i - 1;
                   small_box_index.i++) {
                flat_idx = flat_index(small_box_index, relative_index);
                n1 = box_counts_thread[flat_idx];
                if (n1 == 0) {
                  // Empty small box
                  continue;
                }
                // Check all particles in same box
                box1 = boxes_thread[flat_idx];
                for (unsigned int l = 0; l < n1 - 1; l++) {
                  for (unsigned int p = l + 1; p < n1; p++) {
                    if (get_distance_sq(box1[l], box1[p]) <= radius_sq) {
                      pairs[box1[l].index][pairs_count[box1[l].index]] =
                          box1[p].index;
                      pairs_count[box1[l].index]++;
                      pairs[box1[p].index][pairs_count[box1[p].index]] =
                          box1[l].index;
                      pairs_count[box1[p].index]++;
                    }
                  }
                }
                // Find neighbouring boxes
                n_neighbours = get_neighbour_indexes(
                    small_box_index, neighbour_index_thread, relative_index);
                for (unsigned int l = 0; l < n_neighbours; l++) {
                  flat_idx_neighbour =
                      flat_index(neighbour_index_thread[l], relative_index);
                  n2 = box_counts_thread[flat_idx_neighbour];
                  if (n2 == 0) {
                    // Box Empty
                    continue;
                  }
                  box2 = boxes_thread[flat_idx_neighbour];
                  for (unsigned int i = 0; i < n1; i++) {
                    for (unsigned int j = 0; j < n2; j++) {
                      if (get_distance_sq(box1[i], box2[j]) <= radius_sq) {
                        pairs[box1[i].index][pairs_count[box1[i].index]++] =
                            box2[j].index;
                        pairs[box2[j].index][pairs_count[box2[j].index]++] =
                            box1[i].index;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  // Write to file
  FILE *pairs_output = fopen(argv[2], "w");
  for (unsigned int i = 0; i < n_particles; i++) {
    if (pairs_count[i] == 0) {
      continue;
    }
    fprintf(pairs_output, "%i, ", i);
    for (unsigned int j = 0; j < pairs_count[i] - 1; j++) {
      fprintf(pairs_output, "%i, ", pairs[i][j]);
    }
    fprintf(pairs_output, "%i\n", pairs[i][pairs_count[i] - 1]);
  }

  // Free memory
  free_matrix_ui(pairs);
  free(pairs_count);
  free(positions);
  free_matrix_pos(boxes);
  free(box_counts);
  free(neighbour_index);

  return 0;
}
