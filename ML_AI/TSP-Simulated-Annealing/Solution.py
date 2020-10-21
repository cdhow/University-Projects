import Tour

class Solution:
    def __init__(self, problemName, tLen, date, author, algorithm, runtime, tour):
        self.problemName = problemName
        self.date = date
        self.author = author
        self. algorithm = algorithm
        self.time = runtime
        self.tourLength = tLen
        # Tour List of solution
        self.tour = tour