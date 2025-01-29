CC ?= gcc
CFLAGS = -Iinclude \
			-O2 \
			-Wall \
			-lm \
			-fopenmp

FILES = src/file_utils.c \
				src/sorting.c \
				src/utils.c

OUTPUT_DIR = bin

fast_pairs:
	$(CC) $(CFLAGS) src/fast_pairs_large.c $(FILES) -o $(OUTPUT_DIR)/$@

fast_pair_debug:
	$(CC) $(CFLAGS) -fsanitize=address -g src/fast_pairs_large.c $(FILES) -o $(OUTPUT_DIR)/$@.debug

fast_pair_profile:
	$(CC) $(CFLAGS) -fsanitize=address -pg src/fast_pairs_large.c $(FILES) -o $(OUTPUT_DIR)/$@.debug


slow_pairs:
	$(CC) $(CFLAGS) src/slow_pairs.c $(FILES) -o $(OUTPUT_DIR)/$@

slow_pairs_debug:
	$(CC) $(CFLAGS) -fsanitize=address -g src/slow_pairs.c $(FILES) -o $(OUTPUT_DIR)/$@.debug
