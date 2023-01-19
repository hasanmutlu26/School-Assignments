
##################################################################
#   Question 1
import random


def find_max_score(grid):
    n = len(grid)
    m = len(grid[0])

    max_score = 0
    score = grid[0][0]
    path = [(1, 1)]
    max_path = [(1, 1)]

    max_score, max_path = count_scores(
        grid, 0, 0, score, max_score, path, max_path)

    for i in range(len(grid)):
        print(grid[i])

    print("Route: " + str(max_path))

    points = []
    for i in max_path:
        points.append(grid[i[0]-1][i[1]-1])

    print("Points: " + str(points))
    print("Score: " + str(max_score))

    return max_score, max_path, points


def count_scores(grid, i, j, score, max_score, path, max_path):
    n = len(grid)
    m = len(grid[0])

    if i == n-1 and j == m-1:
        if (score > max_score):
            max_score = score
            max_path = path
        return max_score, max_path

    else:
        if j < m-1:
            max_score, max_path = count_scores(
                grid, i, j+1, score + grid[i][j+1], max_score, path + [(i+1, j+2)], max_path)
        if i < n-1:
            max_score, max_path = count_scores(
                grid, i+1, j, score + grid[i+1][j], max_score, path + [(i+2, j+1)], max_path)

    return max_score, max_path
##################################################################

##################################################################
# Question 2


def find_median(arr):
    kth = len(arr) // 2
    if (len(arr) % 2 != 0):
        kth += 1
    return quickSelect(arr, 0, len(arr) - 1, kth)


def partition(arr, l, r):
    x = arr[r]
    i = l

    for j in range(l, r):
        if arr[j] <= x:
            arr[i], arr[j] = arr[j], arr[i]
            i += 1

    arr[i], arr[r] = arr[r], arr[i]
    return i


def quickSelect(arr, l, r, k):
    if (k > 0 and k <= r - l + 1):

        index = partition(arr, l, r)

        if (index - l == k - 1):
            return arr[index]

        if (index - l > k - 1):
            return quickSelect(arr, l, index - 1, k)

        return quickSelect(arr, index + 1, r, k - index + l - 1)


##################################################################

##################################################################
# Question 3


class Node:
    def __init__(self, data):
        self.data = data
        self.next = None
        self.prev = None


class CircularLinkedList:
    def __init__(self):
        self.head = None
        self.size = 0

    def add(self, data):
        new_node = Node(data)
        if self.head is None:  # list is empty
            self.head = new_node
            new_node.next = new_node
            new_node.prev = new_node
        else:  # list is not empty
            curr = self.head
            while curr.next != self.head:
                curr = curr.next
            curr.next = new_node
            new_node.prev = curr
            new_node.next = self.head
            self.head.prev = new_node
        self.size += 1

    def delete(self, node):
        if self.size == 1:  # list has only one node
            self.head = None
        else:
            node.prev.next = node.next
            node.next.prev = node.prev
            if node == self.head:  # node is head of list
                self.head = node.next
        self.size -= 1


def findWinner1(list):
    curr = list.head
    while list.size > 1:
        temp = curr.next
        list.delete(temp)
        print(str(curr.data) + " eliminated " + str(temp.data))
        curr = curr.next
    print("The winner is " + str(curr.data))
    return curr.data


def findWinner2(list):
    newList = CircularLinkedList()

    if (list.size == 1):
        print("The winner is " + str(list.head.data))
        return list.head.data

    iter = list.head

    for i in range(0, list.size, 2):
        newList.add(iter.data)
        iter = iter.next
        print(str(iter.prev.data) + " eliminated " + str(iter.data))
        iter = iter.next

    if (list.size % 2 != 0):
        newList.head = newList.head.prev

    return findWinner2(newList)


##################################################################


def main():

    print("#######################################\n\nQuestion 1: \n")

    grid = [[23, 30, 25],
            [45, 15, 11],
            [1, 88, 15],
            [9, 4, 23]]
    find_max_score(grid)

    print("")

    rowcol = 3 + int(random.random() * 4)
    grid = [[int(random.random() * 10) for j in range(rowcol)]
            for i in range(rowcol)]
    find_max_score(grid)

    print("\n#######################################\n")

    print("Question 2: \n")
    arr = [random.randint(1, 100) for _ in range(random.randint(5, 8))]
    print("Array: " + str(arr))
    print("Median: " + str(find_median(arr)))

    print("")

    arr = [random.randint(1, 100) for _ in range(random.randint(5, 8))]
    print("Array: " + str(arr))
    print("Median: " + str(find_median(arr)))

    print("\n#######################################\n")
    print("Question 3.1:\n")

    players = ["P1", "P2", "P3", "P4", "P5", "P6"]
    linked_list = CircularLinkedList()
    for player in players:
        linked_list.add(player)
    print("The players: " + str(players))
    findWinner1(linked_list)

    print("")

    players = ["P1", "P2", "P3", "P4", "P5", "P6", "P7"]
    linked_list = CircularLinkedList()
    for player in players:
        linked_list.add(player)
    print("The players: " + str(players))
    findWinner1(linked_list)

    print("\n#######################################\n")
    print("Question 3.2 (Exact same outputs.):\n")

    players = ["P1", "P2", "P3", "P4", "P5", "P6"]
    linked_list = CircularLinkedList()
    for player in players:
        linked_list.add(player)
    print("The players: " + str(players))
    findWinner2(linked_list)

    print("")

    players = ["P1", "P2", "P3", "P4", "P5", "P6", "P7"]
    linked_list = CircularLinkedList()
    for player in players:
        linked_list.add(player)
    print("The players: " + str(players))
    findWinner2(linked_list)



main()
