import tsplib95

# Get File name
infileName = input("Enter File name: ")

# Load problem file
problem = tsplib95.load_problem('in_data/' + infileName)
