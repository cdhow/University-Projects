import math
import random
import Tour as T
from copy import deepcopy
import time

class SimulatedAnnealing:

    def __init__(self, problem):
        self.problem = problem
        self.best = T.Tour(self.problem)

    # Calculate acceptance probability
    def acceptanceProbability(self, energy, newEnergy, temperature):
        # If new solution is better accept it
        if newEnergy < energy:
            return 1.0
        # If worse, calculate acceptance probability
        return math.exp((energy - newEnergy)/temperature)

    # Main process
    def main(self, time_limit):

        # Initial solution
        currentSolution = T.Tour(self.problem)
        if self.problem.dimension <= 1000:
            currentSolution.createTour()
        else:
            currentSolution.createRandomTour()

        print('Initial Greedy Tour Distance: ' + str(currentSolution.getDistance()))
        print('Applying Simulated Annealing. Please Wait...')
        # Set as current best
        self.best = currentSolution

        start = time.time()
        # Time Elapsed
        elapsed = 0
        # Time counter for counting down
        timeCounter = time_limit
        # Loop until Time limit is reach or system has cooled
        while timeCounter >= 0:
            # Generate new neighbour tour
            newSolution = deepcopy(currentSolution)

            # Get two random positions in the tour
            tourPos1 = random.randint(0, newSolution.tourSize()-1)
            # Second node cannot be same as nodePos1
            tourPos2 = random.randint(0, newSolution.tourSize()-1)
            while tourPos2 == tourPos1:
                tourPos2 = random.randint(0, newSolution.tourSize() - 1)

            # Get cities at selected positions in tour
            city1 = newSolution.getCity(tourPos1)
            city2 = newSolution.getCity(tourPos2)

            # Swap city positions
            newSolution.setCity(tourPos1, city2)
            newSolution.setCity(tourPos2, city1)

            # Get solution energy
            currentEnergy = currentSolution.getDistance()
            neighbourEnergy = newSolution.getDistance()

            # Decide if we should accept the neighbour
            if self.acceptanceProbability(currentEnergy, neighbourEnergy, timeCounter) >= random.random():
                currentSolution = deepcopy(newSolution)

            # Keep track of the best solution found
            if currentSolution.getDistance() < self.best.getDistance():
                self.best = deepcopy(currentSolution)

            # Cool System
            elapsed = time.time() - start
            timeCounter = time_limit - elapsed

        # Link home
        self.best.tour.append(self.best.tour[0])
        print('Distance after Simulated Annealing: ' + str(self.best.getDistance()))

    # Returns best Tour object
    def getBestTour(self):
        return self.best

