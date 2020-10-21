import random

class City:
    def __init__(self, id, problem):
        self.problem = problem
        self. id = id

    # Distance from current node to given node
    def distance(self, node):
        return self.problem.wfunc(self.id, node.id)

    # Returns the nearest node id, takes tour list to avoid visiting nodes more then once
    def nearestNode(self, tourIDS):

        # Get initial bestDistance that isn't self.id to self.id or self.id to already visited node
        for bestNode in range(1, self.problem.dimension + 1):
            if bestNode != self.id and bestNode not in tourIDS:
                break

        bestDistance = self.problem.wfunc(self.id, bestNode)

        # Check distances between nodes and update bestNode
        for i in range(1, self.problem.dimension + 1):
            if self.id != i and i not in tourIDS:
                currentDistance = self.problem.wfunc(self.id, i)
                if currentDistance < bestDistance:
                    bestDistance = currentDistance
                    bestNode = i
        return bestNode


class Tour:

    def __init__(self, problem):
        self.tour = []
        self.distance = 0
        self.problem = problem

    # Creates individual tour USING THE GREEDY ALGORITHM
    def createTour(self):
        # Keep track of used IDs so nodes aren't visited more then once
        tourIDs = []
        # Get random node
        currentCityID = random.randint(1, self.problem.dimension)
        # Append nearest city to tour list
        for i in range(1, self.problem.dimension + 1):
            city = City(currentCityID, self.problem)
            self.tour.append(city)
            tourIDs.append(currentCityID)
            # Update current city ID
            currentCityID = city.nearestNode(tourIDs)

    def createRandomTour(self):
        self.tour = []
        for i in range(1, self.problem.dimension + 1):
            city = City(i, self.problem)
            self.tour.append(city)
        random.shuffle(self.tour)

    # Get city from tour
    def getCity(self, tourPos):
        return self.tour[tourPos]

    # Sets a city at certain index in the tour
    def setCity(self, tourPos, city):
        self.tour[tourPos] = city
        # Tour has been altered reset distance
        self.distance = 0

    # Number of cities in tour
    def tourSize(self):
        return len(self.tour)

    # Returns tour list of city objects
    def getTour(self):
        return self.tour

    # Returns tour list of city ids
    def getTourIDs(self):
        ids = [o.id for o in self.tour]
        return ids

    # Get the total distance of the tour
    def getDistance(self):
        if self.distance == 0:
            pathDistance = 0
            for i in range(0, self.tourSize()):
                fromCity = self.tour[i]
                toCity = None

                if i + 1 < len(self.tour):
                    toCity = self.tour[i + 1]
                else:
                    toCity = self.tour[0]

                pathDistance += fromCity.distance(toCity)
            self.distance = pathDistance
        return self.distance
