import Tour as cf
import f_handle as fh
import random
import operator
import pandas as pd
import numpy as np
import Graph


# -- Generating Initial Population ( First Generation ) -- #

# Nearest neighbour algorithm for initial tour/population
def createGreedyTour():
    tour = []
    tourIDs = []
    # Get random node
    currentCityID = random.randint(1, fh.problem.dimension)
    for i in range(1, fh.problem.dimension+1):
        city = cf.City(currentCityID)
        tour.append(city)
        tourIDs.append(currentCityID)
        # Update current city ID
        currentCityID = city.nearestNode(tourIDs)

    return tour

# Random Tour
def createTour():
    tour = []
    # Get random node

    for i in range(1, fh.problem.dimension + 1):
        city = cf.City(i)
        tour.append(city)
    random.shuffle(tour)
    return tour


def initialPopulation(popSize):
    population = []

    population.append(createGreedyTour())
    for i in range(1, popSize):
        population.append(createTour())
    return population


# Ranking individual tours to later determine what to keep
# Returns a sorted list of tuples containing IDs and ranked fitness levels
def rankTours(population):
    fitnessResults = {}
    for i in range(0, len(population)):
        fitnessResults[i] = cf.Fitness(population[i]).tourFitness()
    return sorted(fitnessResults.items(), key=operator.itemgetter(1), reverse=True)


# -- Mating Pool & Breeding-- #

# Returns the IDs of the tours we will use in our mating pool
def selection(popRanked, eliteSize):
    selectionResults = []
    # Data frame of index, fitness, cumulative fitness, and cumulative percentage
    df = pd.DataFrame(np.array(popRanked), columns=["Index", "Fitness"])
    df['cum_sum'] = df.Fitness.cumsum()
    df['cum_perc'] = 100 * df.cum_sum / df.Fitness.sum()

    # Select the top tours ( n = eliteSize)
    for i in range(0, eliteSize):
        selectionResults.append(popRanked[i][0])

    # Now fill the rest based on cumulative percentage being less than random percentage (pick)
    for i in range(0, len(popRanked) - eliteSize):
        pick = 100 * random.random()
        for i in range(0, len(popRanked)):
            if pick <= df.iat[i, 3]:
                selectionResults.append(popRanked[i][0])
                break
    return selectionResults

# Returns a list of the selected individuals (The mating pool)
def matingPool(population, selectionResults):
    matingpool = []
    for i in range(0, len(selectionResults)):
        index = selectionResults[i]
        matingpool.append(population[index])
    return matingpool


# Breed function using ordered crossover technique
# Choose a random subset from parent1 then fill the child with the remainder in parent2
def breed(parent1, parent2):
    childP1 = []

    geneA = int(random.random() * len(parent1))
    geneB = int(random.random() * len(parent1))

    startGene = min(geneA, geneB)
    endGene = max(geneA, geneB)

    for i in range(startGene, endGene):
        childP1.append(parent1[i])

    childP2 = [item for item in parent2 if item not in childP1]

    child = childP1 + childP2
    return child


#LEARNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
def breedPopulation(matingpool, eliteSize):
    children = []
    length = len(matingpool) - eliteSize
    pool = random.sample(matingpool, len(matingpool))

    # Choose the elites from mating pool
    for i in range(0, eliteSize):
        children.append(matingpool[i])

    # Breed the non elites that were chosen in selection()
    for i in range(0, length):
        child = breed(pool[i], pool[len(matingpool) - i - 1])
        children.append(child)
    return children


# Function uses swap mutation, allowing two cities to swap places in our route
# randomly based on a specified probability (mutationRate)
# This helps with avoiding local optimums
def mutate(individual, mutationRate):
    for swapped in range(len(individual)):
        # Probability
        if (random.random() < mutationRate):
            swapWith = int(random.random() * len(individual))

            # Swapping cities
            city1 = individual[swapped]
            city2 = individual[swapWith]
            individual[swapped] = city2
            individual[swapWith] = city1

    return individual

# Returns a mutated population (population where each tour has been mutated)
# Do not mutate original greedy tour #FIGURE IT OUT
def mutatePopulation(population, mutationRate):
    mutatedPop = []

    for idx in range(0, len(population)):
        mutatedIdx = mutate(population[idx], mutationRate)
        mutatedPop.append(mutatedIdx)
    return mutatedPop

# Producing a new Generation
def nextGeneration(currentGen, eliteSize, mutationRate):
    # Rank routes in the Current Generation
    popRanked = rankTours(currentGen)

    # Determine the potential parents
    selectionResults = selection(popRanked, eliteSize)

    # Generate mating pool
    matingpool = matingPool(currentGen, selectionResults)

    # Generate the new Generation
    children = breedPopulation(matingpool, eliteSize)

    # Mutate new generation to avoid possible local optimum solutions
    nextGeneration = mutatePopulation(children, mutationRate)

    # Add greedy Algorithm to nextGeneration
    nextGeneration[len(nextGeneration) - 1] = createGreedyTour()

    return nextGeneration


# All tied together
def geneticAlgorithm(popSize, eliteSize, mutationRate, generations):
    # Generate the initial population
    pop = initialPopulation(popSize)
    print("Initial Greedy distance: " + str(1 / rankTours(pop)[0][1]))
    initialRouteIndex = rankTours(pop)[0][0]
    initialRoute = pop[initialRouteIndex]
    gGraph = Graph.Graph(initialRoute)
    gGraph.display_graph()

    # Run through the each generation a specified amount of times
    for i in range(0, generations):
        pop = nextGeneration(pop, eliteSize, mutationRate)

    print("Final distance After Applying Genetic Algorithm: " + str(1 / rankTours(pop)[0][1]))
    bestRouteIndex = rankTours(pop)[0][0]
    bestRoute = pop[bestRouteIndex]
    return bestRoute


# -- Running the GA -- #

# Pop size must be greater then elite size
solutionTour = geneticAlgorithm(popSize=100, eliteSize=20, mutationRate=0.01, generations=1000)

graph = Graph.Graph(solutionTour)

graph.display_graph()









