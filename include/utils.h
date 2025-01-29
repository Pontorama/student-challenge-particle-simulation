#pragma once
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

double **create_matrix(unsigned int n_rows, unsigned int n_cols);
position **create_matrix_pos(unsigned int n_rows, unsigned int n_cols);
void free_matrix(double **matrix);
void free_matrix_pos(position **matrix);
void print_matrix(double **matrix, unsigned int n_rows, unsigned int n_cols);
void print_positions_list(position *list, unsigned int n_pos);
double get_distance_sq(position p1, position p2);
void shift_positions(position *positions, unsigned int n_positions,
                     double x_min, double y_min, double z_min);
position ***create_rank_3_tensor_pos(unsigned int N_x, unsigned int N_y,
                                     unsigned int N_z);
void destroy_rank_3_tensor_pos(position ***tensor);
position ****create_rank_4_tensor_pos(unsigned int N_x, unsigned N_y,
                                      unsigned int N_z, unsigned int N_l);
void destroy_rank4_tensor(position ****tensor);

unsigned int ***create_rank_3_tensor_uint(unsigned int N_x, unsigned int N_y,
                                          unsigned int N_z);
void destroy_rank_3_tensor_uint(unsigned int ***tensor);
short compare_index3(index3 i, index3 j);

unsigned int **create_matrix_ui(unsigned int n_rows, unsigned int n_cols);
void free_matrix_ui(unsigned int **matrix);
