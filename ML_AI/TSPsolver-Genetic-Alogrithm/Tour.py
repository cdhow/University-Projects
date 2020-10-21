import random
import f_handle as fh


class City:
    def __init__(self, id):
        self. id = id

    # Distance from current node to given node
    def distance(self, node):
        return fh.problem.wfunc(self.id, node.id)

    # Returns the nearest node id, takes tour list to avoid visiting nodes more then once
    def nearestNode(self, tourIDS):

        # Get initial bestDistance that isn't self.id to self.id or self.id to already visited node
        for bestNode in range(1, fh.problem.dimension + 1):
            if bestNode != self.id and bestNode not in tourIDS:
                break

        bestDistance = fh.problem.wfunc(self.id, bestNode)

        # Check distances between nodes and update bestNode
        for i in range(1, fh.problem.dimension + 1):
            if self.id != i and i not in tourIDS:
                currentDistance = fh.problem.wfunc(self.id, i)
                if currentDistance < bestDistance:
                    bestDistance = currentDistance
                    bestNode = i
        return bestNode


class Fitness:
    def __init__(self, tour):
        self.tour = tour
        self.distance = 0
        self.fitness = 0.0

    # Distance of provided route
    def tourDistance(self):
        if self.distance == 0:
            pathDistance = 0
            for i in range(0, len(self.tour)):
                fromCity = self.tour[i]
                toCity = None

                if i + 1 < len(self.tour):
                    toCity = self.tour[i + 1]
                else:
                    toCity = self.tour[0]

                pathDistance += fromCity.distance(toCity)
            self.distance = pathDistance
            return self.distance

    # Fitness of current tour
    def tourFitness(self):
        if self.fitness == 0:
            self.fitness = 1 / float(self.tourDistance())
        return self.fitness