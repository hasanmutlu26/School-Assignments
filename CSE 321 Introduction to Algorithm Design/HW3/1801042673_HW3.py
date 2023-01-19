import math;

##################################################################################################################
# Topological Sort
class Graph:
    def __init__(self):
        self.vertices = []
        rows, cols = (0, 0)
        self.edges = [[0 for i in range(cols)] for j in range(rows)]

    def addEdge(self, src, dest):
        flag = False
        if(src not in self.vertices):
            self.vertices.append(src)
            i = self.vertices.index(src)
            self.edges.insert(i, [dest])
            flag = True
        if(dest not in self.vertices):
            self.vertices.append(dest)
            i = self.vertices.index(dest)
            self.edges.append([])
        if(flag):
            return
        i = self.vertices.index(src)
        self.edges[i].append(dest)
        
    
def topologicalSortDFSRecur(graph, v, visited, result):
    visited[v] = True

    for i in graph.edges[v]:
        j = graph.vertices.index(i)
        if visited[j] == False:
            topologicalSortDFSRecur(graph, j, visited, result)
    result.append(graph.vertices[v])
 

def topologicalSortDFS(graph):
    visited = [False]*len(graph.vertices)
    result = []

    for i in range(len(graph.vertices)):
        if visited[i] == False:
            topologicalSortDFSRecur(graph, i, visited, result)

    print(result[::-1])

def topologicalSortNonDFS(graph):
        directed = [0]*len(graph.vertices)
         
        for i in range (len(graph.edges)):
            for j in graph.edges[i]:
                directed[graph.vertices.index(j)] += 1
 
        queue = []
        for i in range(len(graph.vertices)):
            if directed[i] == 0:
                queue.append(i)
 
        count = 0
        top_order = []
 
        while queue:
            u = queue.pop(0)
            top_order.append(u)
            for i in graph.edges[u]:
                directed[graph.vertices.index(i)] -= 1
                if directed[graph.vertices.index(i)] == 0:
                    queue.append(graph.vertices.index(i))
 
            count += 1
 
        if count != len(graph.vertices):
            print ("There exists a cycle in the graph")
        else :
            output = []
            for i in range (len(top_order)):
                output.append(graph.vertices[top_order[i]])
            print (output)
##################################################################################################################


##################################################################################################################
#   Power
def power(x,y):
    global memo
    memo = [0] * math.ceil(y/2+1)
    return powFunc(x, math.ceil(y/2), memo) * powFunc(x, math.floor(y/2), memo)


def powFunc(x,y,memo):
    
    if(memo[y] != 0):
        return memo[y]

    if(y == 0):
        result = 1
    
    elif(y == 1):
        result = x
    
    else:
        result = powFunc(x, math.ceil(y/2), memo) * powFunc(x, math.floor(y/2), memo)
    
    memo[y] = result
    return result
##################################################################################################################


##################################################################################################################
#   Sudoku Solver
def checkNum(grid, row, col, num):
    for x in range(9):
        if grid[row][x] == num:
            return False
             
    for x in range(9):
        if grid[x][col] == num:
            return False
 
    startRow = row - row % 3
    startCol = col - col % 3
    for i in range(3):
        for j in range(3):
            if grid[i + startRow][j + startCol] == num:
                return False
    return True


 
def sudokuSolver(grid):
    row = 9
    col = 9
    for i in range(9):
        flag = False
        for j in range(9):
            if grid[i][j] == 0:
                row = i
                col = j
                flag = True
                break
        if(flag):
            break
    
    if(row == 9 or col == 9):
        return True
    
    for num in range(1, 10): 
        if checkNum(grid, row, col, num):
            grid[row][col] = num
            if sudokuSolver(grid):
                return True
            else:
                grid[row][col] = 0
    return False
##################################################################################################################
# Driver program
def main():
    print("Part1: Topological Sort")
    print("The same graph in the Homework PDF is provided as default.\n")
    g = Graph()
    g.addEdge(102, 241)
    g.addEdge(241, 222)
    g.addEdge(222, 321)
    g.addEdge(211, 321)
    g.addEdge(321, 422)
    print("Vertices: " + str(g.vertices))
    print("Edges: " + str(g.edges))
    print("\nDFS Topological Sort Result:")
    topologicalSortDFS(g)
    print("\nNon-DFS Topological Sort Result:")
    topologicalSortNonDFS(g)
    
    input("\nPress enter to continue: \n")

    print("------------------------")
    print("Part2: Power Algorithm\n")
    print("power(2,30) = " + str(power(2,30)))
    print("Memory Array for 2^30: " + str(memo))
    input("\nPress enter to continue: \n")

    print("------------------------")
    print("Part 3: Sudoku Solver")
    grid = [[0, 0, 0, 0, 0, 0, 0, 0, 3],
            [0, 0, 0, 0, 0, 0, 0, 6, 2],
            [0, 2, 0, 0, 0, 0, 0, 0, 0],
            [1, 0, 9, 6, 0, 0, 3, 0, 0],
            [0, 6, 0, 0, 7, 0, 0, 0, 0],
            [0, 0, 0, 0, 9, 1, 0, 0, 0],
            [8, 0, 0, 0, 0, 0, 0, 0, 0],
            [4, 0, 0, 0, 2, 0, 6, 0, 0],
            [6, 0, 0, 0, 0, 0, 0, 0, 0]]

    print("Grid: ")
    print('\n'.join([''.join(['{:4}'.format(item) for item in row])
      for row in grid]))
    print("After Solved: ")
    sudokuSolver(grid)
    print('\n'.join([''.join(['{:4}'.format(item) for item in row])
      for row in grid]))

    

main()
