import pymysql
import tsplib95
import os
import Problem
import Solution
import SimulatedAnnealing as SA
import datetime
import getpass

# Get server details (Quick implementation, will be updated to be more secure in future)
host_ = input('Server: ')
db_ = input('Database: ')
usr = input('Username: ')
pswd = getpass.getpass('Password:')

# Hardcoded DB connection
def connect():
    connection = pymysql.connect(
        host=host_,
        user=usr,
        password=pswd,
        db=db_
    )
    return connection


# Load problem file
def getProblem(filename):
    if os.path.exists(filename):
        problem = tsplib95.load_problem(filename)
        return problem
    else:
        # Incorrect filename return error
        return -1


# Used for pushing problems and cities to the Database, returns -1 for file dne, returns -2 for problem already in db
def add(name, filename):
    problem = getProblem("data_files/" + filename)
    # problem = -1 if file dne
    if problem == -1:
        return -1
    connection = connect()
    try:
        with connection.cursor() as cursor:
            # Check if problem record already exists
            cursor.execute("SELECT Name FROM Problem WHERE Name=%s", name)
            problem_name = cursor.fetchall()
            if not problem_name:
                # Create a new problem record
                sql = """INSERT INTO Problem(Name, Size, Comment) VALUES (%s, %s, %s)"""
                cursor.execute(sql, (name, problem.dimension, "Tsp problem."))
                # Add all cities from problem to cities database table
                for i in range(1, problem.dimension+1):
                    x = problem.get_display(i)[0]
                    y = problem.get_display(i)[1]
                    sql = """INSERT INTO Cities(Name, ID, x, y) VALUES (%s, %s, %s, %s)"""
                    cursor.execute(sql, (name, i, x, y))
                connection.commit()
            else:
                return -2
    finally:
        connection.close()


# Return -1 if problem does not exist else return a problem object
def loadProblem(name):
    connection = connect()
    # Retrieve problem dimensions and and cities in format (id, (x,y))
    try:
        with connection.cursor() as cursor:
            # Check if problem record exists
            cursor.execute("SELECT Name FROM Problem WHERE Name=%s", name)
            problem_name = cursor.fetchall()
            if problem_name:
                # Get (id, x, y)
                cursor.execute("SELECT ID, x, y FROM Cities WHERE Name=%s", name)
                # Data is in format (id, x, y)
                data = cursor.fetchall()
                # Create new problem obj and return
                problem = Problem.Problem(name, data)
                return problem
            else:
                # Problem not in db
                return -1
    finally:
        connection.close()


# Used to solve problems, returns Solution object
def solve(problem, time):
    # Run sim anneal algorithm
    sim = SA.SimulatedAnnealing(problem)
    sim.main(time)
    solution = Solution.Solution(problem.name, sim.getBestTour().getDistance(), datetime.datetime.now().date(),
                                 "Caleb Howard", "Simulated Annealing", time, sim.getBestTour().getTourIDs())
    return solution


# Used to push solutions to the database
def pushSolution(solution):
    connection = connect()
    # Change link home back to -1
    solution.tour[-1] = -1
    try:
        with connection.cursor() as cursor:
            # Insert solution in db
            sql = """INSERT INTO Solution(ProblemName, TourLength, Date, Author, Algorithm, RunningTime, Tour)
                    VALUES (%s, %s, %s, %s, %s, %s, %s)"""
            cursor.execute(sql, (solution.problemName, solution.tourLength,
                                 solution.date.strftime("%Y-%m-%d"), solution.author,
                                 solution.algorithm, solution.time,
                                 " ".join(repr(e) for e in solution.tour)))
            connection.commit()
    finally:
        connection.close()


# Takes in a solutionID returns a solution or -1 if solutionID does not exist in database
def fetchViaID(solutionID):
    connection = connect()
    try:
        with connection.cursor() as cursor:
            # Check if solution record exists
            cursor.execute("SELECT SolutionID FROM Solution WHERE SolutionID=%s", solutionID)
            solutionIDData = cursor.fetchall()
            if solutionIDData:
                # Get row with SolutionID
                sql = """ SELECT ProblemName, TourLength, Date, Author, Algorithm, RunningTime, Tour FROM Solution
                        WHERE SolutionID=%s"""
                cursor.execute(sql, (solutionID))
                data = cursor.fetchall()
                # Create Solution object
                name = data[0][0]
                tLen = data[0][1]
                date = data[0][2]
                author = data[0][3]
                alg = data[0][4]
                time = data[0][5]
                tourStrList = (data[0][6])
                # Replacing commas and brackets with spaces because some people decided to not read the assessment plan
                tourStrList = tourStrList.replace(',', ' ')
                tourStrList = tourStrList.replace('[', ' ')
                tourStrList = tourStrList.replace(']', ' ')
                tourStrList = tourStrList.split()
                tour = list(map(int, tourStrList))
                # Link tour to initial City
                if tour[-1] == -1:
                    tour[-1] = tour[0]
                elif tour[-1] != tour[0]:
                    tour.append(tour[0])
                solution = Solution.Solution(name, tLen, date, author, alg, time, tour)
                return solution
            else:
                # Solution ID does not exist in database
                return -1
    finally:
        connection.close()


# Takes in solutionName with either Rand/Best flags to determine whether to
# select solution via name + best distance, or name + random
# Returns a solution
def fetchViaFlag(solutionName=None, flag=None):
    connection = connect()
    try:
        with connection.cursor() as cursor:
            # If Best flag is chosen
            if flag == 'Best' or flag == 'best':
                # Select best distance of all solutions
                if solutionName == 'All':
                    # Select solution with name = solutionName and best distance
                    sql = """SELECT ProblemName, TourLength, Date, Author, Algorithm, RunningTime, Tour FROM Solution
                            WHERE TourLength = (SELECT MIN(TourLength) FROM Solution)"""
                    cursor.execute(sql)
                else:
                    # Select solution with name = solutionName and best distance
                    sql = """SELECT ProblemName, TourLength, Date, Author, Algorithm, RunningTime, Tour FROM Solution
                            WHERE TourLength = (SELECT MIN(TourLength) FROM Solution WHERE ProblemName=%s)
                            AND ProblemName=%s"""
                    cursor.execute(sql, (solutionName, solutionName))

            # If Random flag chosen
            elif flag == 'Rand' or flag == 'rand':
                if solutionName == 'All':
                    # Select Random solution for all solutions
                    sql = """SELECT ProblemName, TourLength, Date, Author, Algorithm, RunningTime, Tour FROM Solution
                            ORDER BY RAND() LIMIT 1;"""
                    cursor.execute(sql)
                else:
                    # Select Random solution with the solution name
                    sql = """SELECT ProblemName, TourLength, Date, Author, Algorithm, RunningTime, Tour FROM Solution
                            WHERE ProblemName=%s ORDER BY RAND() LIMIT 1;"""
                    cursor.execute(sql, (solutionName))

            # Handle data and create solution object
            data = cursor.fetchall()
            # Create Solution object
            name = data[0][0]
            tLen = data[0][1]
            date = data[0][2]
            author = data[0][3]
            alg = data[0][4]
            time = data[0][5]
            tourStrList = (data[0][6])
            # Replacing commas and brackets with spaces because some people decided to not read the assessment plan
            tourStrList = tourStrList.replace(',', ' ')
            tourStrList = tourStrList.replace('[', ' ')
            tourStrList = tourStrList.replace(']', ' ')
            tourStrList = tourStrList.split()
            tour = list(map(int, tourStrList))
            # Link tour to initial City
            if tour[-1] == -1:
                tour[-1] = tour[0]
            elif tour[-1] != tour[0]:
                tour.append(tour[0])
            solution = Solution.Solution(name, tLen, date, author, alg, time, tour)
            return solution
    finally:
        connection.close()


# Returns a list of solution names from the database
def fetchSolutionNames():
    connection = connect()
    try:
        with connection.cursor() as cursor:
            #
            cursor.execute("SELECT ProblemName FROM Solution")
            data = cursor.fetchall()
            # Extract data into a useful format of ['string','string',...]
            solutionNames = [i[0] for i in data]
            # Remove duplicates
            solutionNames = list(set(solutionNames))
            solutionNames.remove('')
            return solutionNames
    finally:
        connection.close()
