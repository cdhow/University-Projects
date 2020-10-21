import tsplib95
import sys
import os

# Load problem file
def getProblem():
    if os.path.exists('in_data/' + str(sys.argv[1])):
        problem = tsplib95.load_problem('in_data/' + str(sys.argv[1]))
    else:
        # Handle error
        print("Could not read file: " + str(sys.argv[1]))
        sys.exit(1)
    return problem

