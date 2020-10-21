import matplotlib.pyplot as plt
import f_handle as fh


class Graph:

    def __init__(self, tour):
        self.xList = []
        self.yList = []
        # Populate axes
        for i in range(0, len(tour)):
            self.xList.append(fh.problem.get_display(tour[i].id)[0])
            self.yList.append(fh.problem.get_display(tour[i].id)[1])
        # Add the home point
        self.xList.append(fh.problem.get_display(tour[0].id)[0])
        self.yList.append(fh.problem.get_display(tour[0].id)[1])

    def display_graph(self):
        plt.plot(self.xList, self.yList, 'C3')
        plt.scatter(self.xList, self.yList, s=120)
        plt.title('TSP ')
        plt.xlabel('Node x Position')
        plt.ylabel('Node y Position')
        plt.show()