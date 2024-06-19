from collections import defaultdict, Counter

# Setting Up the Configuration
###########0    1    2    3    4    5  
board = [[".", ".", ".", ".", ".", "."], #0
         [".", ".", ".", ".", ".", "."], #1
         [".", ".", "1", ".", "1", "."], #2
         [".", ".", ".", ".", ".", "."], #3
         [".", ".", "1", ".", ".", "."], #4
         [".", ".", ".", ".", ".", "."], #5
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

def layout_search():
    "Initital Search for number_square location"
    for i, row in enumerate(board):
        for j, element in enumerate(row):
            if element != ".":
                setup_config[(i, j)] = int(element)

def _okay_row_col(row, col):
    for i in range(length):
        value = board[i][col]
        if value != "." and value not in permitted:
            return False
    for j in range(length):
        value = board[row][j]
        if value != "." and value not in permitted:
            return False
    return True

def _okay_surrounding(row, col):
    for i, j in surrounding:
        if (row +i, col + j) in coor_list:
            value = (row + i, col + j)
            if value in setup_config:
                max = setup_config[(row+i, col +j)]
                if max == 0:
                    return False
            elif board[value[0]][value[1]] != ".":
                return False
    return True

def full_affected_check():
    for row in board:
        check = Counter(row)
        if check["c"] == 0 and check["."] == 0 :
            return False
    for col in zip(*board[::1]):
        check = Counter(col)
        if check["c"] == 0 and check["."] == 0:
            return False
    return True

def is_okay(row, col):
    if (row, col) not in coor_list or (row, col) in traverse:
        return False
    elif _okay_row_col(row, col) and _okay_surrounding(row, col) and full_affected_check():
        return True
    return False

def number(cell, reverse = False):
    for i, j in surrounding:
        row = cell[0] + i
        col = cell[1] + j
        if (row, col) in setup_config:
            if not reverse:
                setup_config[(row, col)] -= 1
            else:
                setup_config[(row, col)] += 1

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
                coor_list.remove((row, col))
                recursion(row, col)
    else:
        for i, j in coor_list:
            if is_okay(i, j):
                recursion(i, j)
                     

layout_search()
backtrack()
if solved:
    print(traverse)
else:
    print("Unsolvable")
print(count)
                
    