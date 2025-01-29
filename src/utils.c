#include "utils.h"

double **create_matrix(unsigned int n_rows, unsigned int n_cols) {
  double *entries = (double *)malloc(sizeof(double) * n_rows * n_cols);
  double **matrix = (double **)malloc(sizeof(double *) * n_rows);
  for (int i = 0, j = 0; i < n_rows; i++, j += n_cols) {
    matrix[i] = entries + j;
  }
  return matrix;
}

void free_matrix(double **matrix) {
  free(matrix[0]);
  free(matrix);
}

void print_matrix(double **matrix, unsigned int n_rows, unsigned int n_cols) {
  for (int i = 0; i < n_rows; i++) {
    for (int j = 0; j < n_cols - 1; j++) {
      printf("%f, ", matrix[i][j]);
    }
    printf("%f\n", matrix[i][n_cols - 1]);
  }
}

void print_positions_list(position *list, unsigned int n_pos) {
  for (unsigned int i = 0; i < n_pos; i++) {
    printf("%f, %f, %f\n", list[i].x, list[i].y, list[i].z);
  }
}
position **create_matrix_pos(unsigned int n_rows, unsigned int n_cols) {
  position *entries = (position *)malloc(sizeof(position) * n_rows * n_cols);
  position **matrix = (position **)malloc(sizeof(position *) * n_rows);
  for (int i = 0, j = 0; i < n_rows; i++, j += n_cols) {
    matrix[i] = entries + j;
  }
  return matrix;
}

void free_matrix_pos(position **matrix) {
  free(matrix[0]);
  free(matrix);
}
/*
    Move all particles so that the lower boudaries are all at 1e-6
*/
void shift_positions(position *positions, unsigned int n_positions,
                     double x_min, double y_min, double z_min) {
  for (unsigned int i = 0; i < n_positions; i++) {
    positions[i].x += -x_min;
    positions[i].y += -y_min;
    positions[i].z += -z_min;
  }
}

double get_distance_sq(position p1, position p2) {
  return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) +
         (p1.z - p2.z) * (p1.z - p2.z);
}

position ***create_rank_3_tensor_pos(unsigned int N_x, unsigned int N_y,
                                     unsigned int N_z) {
  position ***tensor = (position ***)malloc(sizeof(position **) * N_x);
  position **tensor_planes =
      (position **)malloc(sizeof(position *) * N_x * N_y);
  position *tensor_entries =
      (position *)malloc(sizeof(position) * N_x * N_y * N_z);

  for (unsigned int i = 0; i < N_x; i++) {
    for (unsigned int j = 0; j < N_y; j++) {
      tensor_planes[i * N_x + j] = tensor_entries + j * N_z + i * N_y * N_z;
    }
    tensor[i] = tensor_planes + i * N_x;
  }
  return tensor;
}

void destroy_rank_3_tensor_pos(position ***tensor) {
  free(tensor[0][0]);
  free(tensor[0]);
  free(tensor);
}

unsigned int ***create_rank_3_tensor_uint(unsigned int N_x, unsigned int N_y,
                                          unsigned int N_z) {
  unsigned int ***tensor =
      (unsigned int ***)malloc(sizeof(unsigned int **) * N_x);
  unsigned int **tensor_planes =
      (unsigned int **)malloc(sizeof(unsigned int *) * N_x * N_y);
  unsigned int *tensor_entries =
      (unsigned int *)malloc(sizeof(unsigned int) * N_x * N_y * N_z);
  for (unsigned int i = 0; i < N_x; i++) {
    for (unsigned int j = 0; j < N_y; j++) {
      tensor_planes[i * N_x + j] = tensor_entries + j * N_z + i * N_y * N_z;
    }
    tensor[i] = tensor_planes + i * N_x;
  }
  return tensor;
}

void destroy_rank_3_tensor_uint(unsigned int ***tensor) {
  free(tensor[0][0]);
  free(tensor[0]);
  free(tensor);
}

position ****create_rank_4_tensor_pos(unsigned int N_x, unsigned N_y,
                                      unsigned int N_z, unsigned int N_l) {
  position *entries =
      (position *)malloc(sizeof(position) * N_x * N_y * N_z * N_l);
  position **columns =
      (position **)malloc(sizeof(position *) * N_x * N_y * N_z);
  position ***rows = (position ***)malloc(sizeof(position **) * N_x * N_y);
  position ****tensor = (position ****)malloc(sizeof(position ***) * N_x);

  for (int i = 0; i < N_x; i++) {
    for (int j = 0; j < N_y; j++) {
      for (int k = 0; k < N_z; k++) {
        columns[i * N_z * N_y + j * N_z + k] =
            entries + k * N_l + j * N_z * N_l + i * N_z * N_y * N_l;
      }
      rows[i * N_y + j] = columns + j * N_z + i * N_z * N_y;
    }
    tensor[i] = rows + i * N_x;
  }

  return tensor;
}

void destroy_rank4_tensor(position ****tensor) {
  free(tensor[0][0][0]);
  free(tensor[0][0]);
  free(tensor[0]);
  free(tensor);
}

short compare_index3(index3 i, index3 j) {
  return i.i == j.i && i.j == j.j && i.k == j.k;
}

unsigned int **create_matrix_ui(unsigned int n_rows, unsigned int n_cols) {
  unsigned int **out = (unsigned int **)malloc(sizeof(unsigned int *) * n_rows);
  unsigned int *entries =
      (unsigned int *)malloc(sizeof(unsigned int) * n_rows * n_cols);
  for (int i = 0; i < n_rows; i++) {
    out[i] = entries + i * n_cols;
  }
  return out;
}

void free_matrix_ui(unsigned int **matrix) {
  free(matrix[0]);
  free(matrix);
}
