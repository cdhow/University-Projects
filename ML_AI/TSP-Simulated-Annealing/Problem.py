import math

# Contains useful function specific to a chosen problem
class Problem:
    def __init__(self, name, cities):
        self.name = name
        self.dimension = len(cities)
        # Contains city (id, x, y) tuple
        self.cities = cities

    # Returns distance from id_1 to id_2
    def wfunc(self, id_1, id_2):
        x1 = self.cities[id_1 - 1][1]
        y1 = self.cities[id_1 - 1][2]
        x2 = self.cities[id_2 - 1][1]
        y2 = self.cities[id_2 - 1][2]
        return math.sqrt(math.pow((x2-x1),2) + math.pow((y2-y1), 2))

    # Returns (x,y) at id
    def get_display(self, id):
        xyID = self.cities[id-1]
        return xyID[1], xyID[2]

    def getXList(self):
        xList = []
        for i in range(1, self.dimension+1):
            xList.append(self.get_display(i)[0])
        return xList

    def getYList(self):
        yList = []
        for i in range(1, self.dimension+1):
            yList.append(self.get_display(i)[1])
        return yList

