###############################################################
# Part 1
import random


def longestCommonSubstring(strings):
    if not strings:
        return ""
    if len(strings) == 1:
        return strings[0]

    mid = len(strings) // 2
    left = longestCommonSubstring(strings[:mid])
    right = longestCommonSubstring(strings[mid:])
    return findLongestCommonSubstring(left, right)


def findLongestCommonSubstring(left, right):
    min_len = min(len(left), len(right))
    for i in range(min_len):
        if left[i] != right[i]:
            return left[:i]
    return left[:min_len]
###############################################################

###############################################################
# Part 2.1
def maxProfitDaC(prices):
    if len(prices) < 2:
        return 0, 0, 0

    mid = len(prices) // 2
    left = prices[:mid]
    right = prices[mid:]

    leftProfit, leftBuyDay, leftSellDay = maxProfitDaC(left)

    rightProfit, rightBuyDay, rightSellDay = maxProfitDaC(right)

    crossProfit = 0
    minPrice = min(left)
    minPriceDay = left.index(minPrice)
    maxPrice = max(right)
    maxPriceDay = right.index(maxPrice) + mid
    if minPrice < maxPrice:
        crossProfit = maxPrice - minPrice

    if leftProfit >= rightProfit and leftProfit >= crossProfit:
        return leftProfit, leftBuyDay, leftSellDay
    elif rightProfit >= leftProfit and rightProfit >= crossProfit:
        return rightProfit, rightBuyDay + mid, rightSellDay + mid
    else:
        return crossProfit, minPriceDay, maxPriceDay
###############################################################

###############################################################
# Part 2.2
def maxProfitLin(prices):
    max_profit = 0
    buy_day = 0
    sell_day = 0
    min_price = prices[0]
    min_index = 0

    for i in range(1, len(prices)):
        if (prices[i] < min_price):
            min_price = prices[i]
            min_index = i
        else:
            if (prices[i] - min_price > max_profit):
                max_profit = prices[i] - min_price
                sell_day = i
                buy_day = min_index

    return max_profit, buy_day, sell_day
###############################################################

###############################################################
# Part 3
def longestIncreasingSubarray(arr):
    n = len(arr)
    memo = [1] * n

    for i in range(1, n):
        if (arr[i] > arr[i-1]):
            memo[i] = memo[i-1] + 1

    return max(memo)
###############################################################

###############################################################
# Part 4
def maxScoreDynamic(grid):
    rows = len(grid)
    cols = len(grid[0])
    maximumScore = [[0 for j in range(cols)] for i in range(rows)]
    path = [[None for j in range(cols)] for i in range(rows)]
    maximumScore[0][0] = grid[0][0]

    for i in range(rows):
        for j in range(cols):
            if i > 0:
                if (maximumScore[i][j] < maximumScore[i-1][j] + grid[i][j]):
                    maximumScore[i][j] = maximumScore[i-1][j] + grid[i][j]
                    path[i][j] = (i-1, j)
            if j > 0:
                if (maximumScore[i][j] < maximumScore[i][j-1] + grid[i][j]):
                    maximumScore[i][j] = maximumScore[i][j-1] + grid[i][j]
                    path[i][j] = (i, j-1)

    pathList = []
    pointList = []
    i, j = rows-1, cols-1
    while i != 0 or j != 0:
        pathList.append((i, j))
        pointList.append(grid[i][j])
        i, j = path[i][j]
    pathList.append((0, 0))
    pointList.append(grid[i][j])

    pathList.reverse()
    pointList.reverse()

    return maximumScore[rows-1][cols-1], pathList, pointList
###############################################################

###############################################################
# Part 4.2
def maxScoreGreedy(grid):
    n = len(grid)
    m = len(grid[0])
    i = 0
    j = 0
    score = grid[0][0]
    path = [(0, 0)]
    pointList = [grid[0][0]]
    
    while (i != n - 1 or j != m - 1):
        if(i == n - 1):
            j += 1
        elif(j == m - 1):
            i += 1
        else:
            if(grid[i+1][j] > grid[i][j+1]):
                i += 1
            else:
                j += 1
        
        score += grid[i][j]
        path.append((i,j))
        pointList.append(grid[i][j])

        

    return score, path, pointList
###############################################################



###############################################################
print("###############################################")
print("Part 1: Longest Common Substring\n")

strings =  ["programmable", "programming", "programmer", "programmatic", "programmability"]
print("strings: " + str(strings))
result = longestCommonSubstring(strings)
print("Longest common substring: " + result + "\n\n")

strings =   ["compute", "compatible", "computer", "compare", "compactness"]
print("strings: " + str(strings))
result = longestCommonSubstring(strings)
print("Longest common substring: " + result + "\n\n")

strings =   ["abcd", "bcde", "cdef", "defg", "efgh"]
print("strings: " + str(strings))
result = longestCommonSubstring(strings)
print("Longest common substring: " + result + "\n\n")

print("###############################################\n\n\n")




print("###############################################")
print("Part2.1: Maximum Profit by Divide and Conquer\n")

prices = [10, 11, 10, 9, 8, 7, 9, 11]
profit, buy_day, sell_day = maxProfitDaC(prices)
print("Prices: " + str(prices))
print(f"Buy on Day{buy_day} for ${prices[buy_day]} and sell on Day{sell_day} for ${prices[sell_day]}.\n")

prices =  [100, 110, 80, 90, 110, 70, 80, 80, 90]
profit, buy_day, sell_day = maxProfitDaC(prices)
print("Prices: " + str(prices))
print(f"Buy on Day{buy_day} for ${prices[buy_day]} and sell on Day{sell_day} for ${prices[sell_day]}.\n")

prices = [10, 7, 10, 11, 8, 10, 9, 11]
profit, buy_day, sell_day = maxProfitDaC(prices)
print("Prices: " + str(prices))
print(f"Buy on Day{buy_day} for ${prices[buy_day]} and sell on Day{sell_day} for ${prices[sell_day]}.")
print("###############################################\n\n\n")




print("###############################################")
print("Part2.2: Maximum Profit in Linear Time\n")

prices = [10, 11, 10, 9, 8, 7, 9, 11]
profit, buy_day, sell_day = maxProfitLin(prices)
print("Prices: " + str(prices))
print(f"Buy on Day{buy_day} for ${prices[buy_day]} and sell on Day{sell_day} for ${prices[sell_day]}.\n")

prices = [100, 110, 80, 90, 110, 70, 80, 80, 90]
profit, buy_day, sell_day = maxProfitLin(prices)
print("Prices: " + str(prices))
print(f"Buy on Day{buy_day} for ${prices[buy_day]} and sell on Day{sell_day} for ${prices[sell_day]}.\n")

prices = [10, 7, 10, 11, 8, 10, 9, 11]
profit, buy_day, sell_day = maxProfitLin(prices)
print("Prices: " + str(prices))
print(f"Buy on Day{buy_day} for ${prices[buy_day]} and sell on Day{sell_day} for ${prices[sell_day]}.")
print("###############################################\n\n\n")




print("###############################################")
print("Part 3: Longest Increasing Sequential Subarray\n")

array = [1, 4, 5, 2, 4, 3, 6, 7, 1, 2, 3, 4, 7]
print("Array: " + str(array))
lon = longestIncreasingSubarray(array)
print("Longest Increasing Sequential Subarray Length: " + str(lon) + "\n")

array = [1, 2, 3, 4, 1, 2, 3, 5, 2, 3, 4]
print("Array: " + str(array))
lon = longestIncreasingSubarray(array)
print("Longest Increasing Sequential Subarray Length: " + str(lon))
print("###############################################\n\n\n")




print("###############################################")
print("Part 4: Maximum Score")

grid = [[25, 30, 25],
        [45, 15, 11],
        [1, 88, 15],
        [9, 4, 23]]
for i in range(len(grid)):
    print(grid[i])

maxScore, path, pointList = maxScoreDynamic(grid)
print("Solution 4.1: Dynamic Programming")
print("Path: " + str(path))
print("Points: " + str(pointList))
print("Score: " + str(maxScore) + "\n")

maxScore, path, pointList = maxScoreGreedy(grid)
print("Solution 4.2: Greedy Algorithm")
print("Path: " + str(path))
print("Points: " + str(pointList))
print("Score: " + str(maxScore) + "\n\n")


rowcol = 10
grid = [[int(random.random() * 15) for j in range(rowcol)]
        for i in range(rowcol)]
for i in range(len(grid)):
    print(grid[i])
    
maxScore, path, pointList = maxScoreDynamic(grid)
print("Solution 4.1: Dynamic Programming")
print("Path: " + str(path))
print("Points: " + str(pointList))
print("Score: " + str(maxScore) + "\n")

maxScore, path, pointList = maxScoreGreedy(grid)
print("Solution 4.2: Greedy Algorithm")
print("Path: " + str(path))
print("Points: " + str(pointList))
print("Score: " + str(maxScore))
print("###############################################")
