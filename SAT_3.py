from collections import defaultdict, Counter

# Setting Up the Configuration
###########0    1    2    3    4    5    6    7    8    9    10    11
board = [[".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #0
         [".", ".", ".", ".", ".", ".", ".", ".", ".", "1", ".", "."], #0
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "1"], #2
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #3
         [".", ".", ".", ".", ".", ".", ".", "1", ".", ".", ".", "."], #4
         [".", ".", ".", "1", ".", ".", ".", ".", ".", ".", ".", "."], #5
         [".", ".", ".", ".", ".", "1", ".", ".", ".", ".", ".", "."], #6
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "1", "."], #7
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #8
         ["1", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #9
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #10
         [".", ".", "1", ".", ".", ".", ".", ".", ".", ".", ".", "."], #11
        ]

# Finding Square that contain number
setup_config = defaultdict(int)
# ADT to store movement that are no longer possible
length = len(board)
coor_list = list((i,j) for j in range(length) for i in range(length))
permitted = ["0", "1", "2"]
traverse = []
count = 0
surrounding = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
solved = False
moved = []

def layout_search():
    "Initital Search for number_square location"
    for i, row in enumerate(board):
        for j, element in enumerate(row):
            if element != ".":
                setup_config[(i, j)] = int(element)

def _okay_row_col(row, col):
    for i in range(length):
        value = board[i][col]
        if value == "c":
            return False
    for j in range(length):
        value = board[row][j]
        if value == "c":
            return False
    return True

def _okay_surrounding(row, col):
    for i, j in surrounding:
        value = (row + i, col + j)
        if value in coor_list:
            if value in setup_config:
                max = setup_config[(row+i, col +j)]
                if max == 0:
                    return False
            elif board[value[0]][value[1]] == "c":
                return False
    return True

def is_okay(row, col):
    if (row, col) not in coor_list or (row, col) in traverse:
        return False
    elif _okay_row_col(row, col) and _okay_surrounding(row, col):
        return True
    return False

def number(cell, reverse = False):
    for i, j in surrounding:
        row = cell[0] + i
        col = cell[1] + j
        if (row, col) in setup_config:
            setup_config[(row, col)] += 1 if reverse else -1
        elif (row, col) in coor_list:
            board[row][col] = "." if reverse else "-"
    row, col = cell
    for i in range(length):
        if board[i][col] not in permitted:
            board[i][col] = "." if reverse else "-"
    for j in range(length):
        if board[row][j] not in permitted:
            board[row][j] = "." if reverse else "-"

def number_check():
    """A look up to see if all the numbered square have been fullfill"""
    for key, value in setup_config.items():
            if value != 0:
                return key
    return None

def recursion(row, col):
    number((row, col))
    board[row][col] = "c"
    traverse.append((row, col))
    backtrack()
    if len(traverse) == length:
        return
    board[row][col] = "."
    traverse.remove((row, col)) 
    number((row, col), True)

# This method is no different from simply starting at a random location instead of a specific start place
def backtrack():
    global count, solved
    count += 1
    if len(traverse) == length:
        solved = True
        return
    location = number_check()
    if location:
        for i, j in surrounding:
            row = location[0] + i
            col = location[1] + j
            if is_okay(row, col):
                recursion(row, col)
    else:
        for i, j in coor_list:
            if is_okay(i, j):
                recursion(i, j)
                     
layout_search()
backtrack()
if not solved:
    print(f"Steps: {count}, Unsolvable")
else:
    print(f"Steps: {count}", traverse)
    for row in board:
        print(row)
                
    