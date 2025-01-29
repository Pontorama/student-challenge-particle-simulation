#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

void load_xyz_file(FILE* fp, position* output, unsigned int n_lines);
unsigned int get_file_length(FILE* fp);
