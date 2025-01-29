# Student challenge particle simulation
This is my entry for the student challange!
My approach is to divide the total volume spanned by the particles into smaller "big boxes" which are then divided further into "small boxes" that are $0.05\text{m}\times0.05\text{m}\times0.05\text{m}$ large.
The particles are then sorted into these smaller boxes, and the neighbouring boxes are searched for particles that are within a radius of $0.05$m. Doing this iteratively means I only have to check half of the neighbouring boxes.

# Building
(Disclaimer; since I built this on a linux desktop, it is not tested on any other platform)
A makefile is provided, and there are two build options:
- fast_pairs
- slow_pairs
I used no non-standard C-libraries, so simply typing
`make fast_pairs`
should work on any linux computer.

# Running
The program takes 3 arguments; input file, output file and number of threads to be used
Example usage:
`./fast_pairs data/positions_large.xyz output/large.csv 4`
The program will output a CSV file listing indexes (line numbers) of particles found in the input file as the first column, followed by the indexes of all the particles that are within a $0.05$m radius.

# Validity checks
fast_pairs is my actual solution, slow_pairs was just used to check my solution together with the (sloppy) python script `check_validity.py`. No output from this script means that `slow_pairs` and `fast_pairs` gave the same pairs.
