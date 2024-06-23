from collections import defaultdict, Counter

# Setting Up the Configuration
###########0    1    2    3    4    5    6    7    8    9    10    11
board = [[".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #0
         [".", ".", ".", ".", ".", ".", ".", ".", "2", ".", ".", "."], #1
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #2
         [".", ".", "1", ".", ".", ".", ".", ".", ".", ".", ".", "."], #3
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #4
         [".", ".", ".", ".", ".", "2", ".", ".", ".", ".", ".", "."], #5
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #6
         [".", "1", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #7
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #8
         [".", ".", ".", ".", ".", ".", ".", ".", ".", "1", ".", "."], #9
         [".", ".", ".", "1", ".", ".", ".", "1", ".", ".", ".", "."], #10
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #11
        ]

# Finding Square that contain number
setup_config = defaultdict(int)
# ADT to store movement that are no longer possible
length = len(board)
count = 0
solved = False
surrounding = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
coor_list = set((i,j) for j in range(length) for i in range(length))
traverse = []
affected = list()
temp = set()
no_guide = set()

def layout_search():
    "Initital Search for number_square location"
    for i, row in enumerate(board):
        for j, element in enumerate(row):
            if element != ".":
                setup_config[(i, j)] = int(element)
                coor_list.remove((i, j))

def is_okay(row, col):
    if (row, col) not in coor_list or (row, col) in traverse or (row, col) in no_guide:
        return False
    return True

def is_fine(row, col, reverse):
    global temp
    if reverse:
        if (row, col) in temp:
            coor_list.add((row, col))
    else:
        if (row, col) in coor_list:
            coor_list.remove((row, col))
            temp.add((row, col))
        elif (row, col) in no_guide:
            temp.add((row, col))

def number_surround(row, col, reverse):
    setup_config[(row, col)] += 1 if reverse else -1
    if setup_config[(row, col)] == 0 or reverse:
        for i, j in surrounding:
            i += row
            j += col
            is_fine(i, j, reverse)

def image(cell, reverse = False):
    global temp
    if reverse:
        temp = affected.pop()
    for i, j in surrounding:
        row = cell[0] + i
        col = cell[1] + j
        if (row, col) in setup_config:
            number_surround(row, col, reverse)
            continue
        is_fine(row, col, reverse)

    row, col = cell
    for i in range(length):
        if i == row:
            continue
        is_fine(i, col, reverse)
    for j in range(length):
        if j == col:
            continue
        is_fine(row, j, reverse)
    if not reverse:
        affected.append(temp)
    temp = set()
        
def number_check():
    """A look up to see if all the numbered square have been fullfill"""
    for key, value in setup_config.items():
            if value != 0:
                return key
    return None

def recursion(row, col):
    board[row][col] = "c"
    image((row, col))
    traverse.append((row, col))
    backtrack(coor_list)
    if len(traverse) == length:
        return
    board[row][col] = "."
    traverse.remove((row, col)) 
    image((row, col), True)

# This method is no different from simply starting at a random location instead of a specific start place
def backtrack(move_list):
    global no_guide, count, solved
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
            no_guide = set()
    else:
        for i, j in move_list.copy():
            if is_okay(i, j):
                if len(setup_config) > 0:
                    no_guide.add((i, j))
                recursion(i, j)
                
layout_search()
backtrack(coor_list)
if not solved:
    print(f"Steps: {count}, Unsolvable")
else:
    print(f"Steps: {count}", traverse)
    for row in board:
        print(row)
                
    