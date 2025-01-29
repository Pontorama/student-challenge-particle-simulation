#include "slow_pairs.h"

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  unsigned int n_particles = get_file_length(fp);
  rewind(fp);
  position *positions = (position *)malloc(sizeof(position) * n_particles);
  load_xyz_file(fp, positions, n_particles);
  fclose(fp);

  unsigned int max_particles_per_box = n_particles / 10;

  unsigned int **pairs =
      (unsigned int **)malloc(sizeof(unsigned int *) * n_particles);
  unsigned int *pairs_entries = (unsigned int *)malloc(
      sizeof(unsigned int) * n_particles * max_particles_per_box);
  unsigned int *pairs_count =
      (unsigned int *)calloc(n_particles, sizeof(unsigned int));
  for (int i = 0; i < n_particles; i++) {
    pairs[i] = pairs_entries + i * max_particles_per_box;
  }

  for (int i = 0; i < n_particles; i++) {
    for (int j = i + 1; j < n_particles; j++) {
      if (get_distance_sq(positions[i], positions[j]) <= 0.0025) {
        pairs[i][pairs_count[i]] = j;
        pairs_count[i]++;
        pairs[j][pairs_count[j]] = i;
        pairs_count[j]++;
      }
    }
  }
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
  fclose(pairs_output);
  free(positions);
  free(pairs[0]);
  free(pairs);
  free(pairs_count);
  return 0;
}
