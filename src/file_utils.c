#include "file_utils.h"
#include "types.h"

/*
    Get comma separated data from xyz-file
    The matrix output is assumed to be size n_lines * 3
*/
void load_xyz_file(FILE *fp, position *output, unsigned int n_lines) {
  int fscan_return;
  for (int i = 0; i < n_lines; i++) {
    fscan_return =
        fscanf(fp, "%lf %lf %lf\n", &output[i].x, &output[i].y, &output[i].z);
    output[i].index = i;
    if (fscan_return == 0) {
      perror("failed to read file\n");
    }
  }
}

unsigned int get_file_length(FILE *fp) {
  unsigned int n = 0;
  char buffer[82];
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    n += 1;
  }
  return n;
}
