import matplotlib.pyplot as plt

class Graph:

    def __init__(self, tour, problem):
        self.problem = problem
        self.xList = []
        self.yList = []
        # Populate axes
        for i in range(0, len(tour)):
            self.xList.append(self.problem.get_display(tour[i].id)[0])
            self.yList.append(self.problem.get_display(tour[i].id)[1])

    def display_graph(self):
        plt.plot(self.xList, self.yList, 'C3')
        plt.scatter(self.xList, self.yList, s=40)
        plt.title('TSP ')
        plt.xlabel('Node x Position')
        plt.ylabel('Node y Position')
        plt.show()
