# Distributed-HighLife-MPI
Distributed and scalable implementation in the C programming language using the MPI library of a program simulating a variant of the High Life cellular automaton.

Running the project:
Compile the program using your preferred C compiler, ensuring that the MPI library is linked.
Run the program using the mpirun command with the following syntax:
mpirun --oversubscribe -np NUM_PROCS ./program_name IN_FILENAME OUT_FILENAME NUM_STEPS
NUM_PROCS: number of processes.
IN_FILENAME: path to the input file.
OUT_FILENAME: path to the output file.
NUM_STEPS: total number of steps to be performed in the simulation.

