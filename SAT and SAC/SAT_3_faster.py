from collections import defaultdict, Counter

# Setting Up the Configuration
###########0    1    2    3    4    5    6    7    8    9    10    11
board = [[".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #0
         [".", ".", ".", ".", ".", ".", ".", ".", "2", ".", ".", "."], #1
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #2
         [".", ".", "1", ".", ".", ".", ".", ".", ".", ".", ".", "."], #3
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #4
         [".", ".", ".", ".", ".", "2", ".", ".", ".", ".", ".", "1"], #5
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #6
         [".", "1", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #7
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."], #8
         [".", ".", ".", ".", ".", ".", ".", ".", ".", "1", ".", "."], #9
         [".", ".", ".", "1", ".", ".", ".", "1", ".", ".", ".", "."], #10
         [".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "1"], #11
        ]


setup_config = defaultdict(int)
# ADT to store movement that are no longer possible
length = len(board)
count = 0
solved = False
repeat = False
surrounding = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
coor_list = set((i,j) for j in range(length) for i in range(length))
traverse = []
all_image = {}
affected = list()
temp = set()
# A variable that store visited square that wasn't base on any number
# That is it is chosen freely from the available space of the matrix
no_guide = set()

def layout_search():
    "Initital Search for number_square location"
    for i, row in enumerate(board):
        for j, element in enumerate(row):
            if element != ".":
                setup_config[(i, j)] = int(element)
                coor_list.remove((i, j))

def is_okay(row, col):
    """The purpose is simply to check if the move is valid or exist"""
    if (row, col) not in coor_list or (row, col) in traverse or (row, col) in no_guide:
        return False
    return True

def if_fine_change(row, col, reverse):
    """Changing a square state -> affected/normal -> remove/add to coor_list"""
    global temp
    if reverse:
        if (row, col) in temp:
            coor_list.add((row, col)) #Re-adding previously eliminated square
    else:
        if (row, col) in coor_list:
            coor_list.remove((row, col)) #Removing affected square
            temp.add((row, col)) # Adding the affected square to a list of affected square
        elif (row, col) in no_guide:
            temp.add((row, col)) 

def number_surround(row, col, reverse):
    """Changing the square state around number: square -> affected/normal when reverse = False/True"""
    setup_config[(row, col)] += 1 if reverse else -1
    if setup_config[(row, col)] == 0 or reverse:
        for i, j in surrounding:
            i += row
            j += col
            if_fine_change(i, j, reverse)

def image(cell, reverse = False):
    """Create an image or go back to a previous image or the game"""
    global temp, repeat
    if reverse:
        temp = affected.pop() # Going back to the previous image
        all_image[tuple(temp)] = False
    
    # Assessing surrounding square depending on reverse = False/True
    for i, j in surrounding:
        row = cell[0] + i
        col = cell[1] + j
        if (row, col) in setup_config:
            number_surround(row, col, reverse)
            continue
        if_fine_change(row, col, reverse)

    # Assessing the square in a row
    row, col = cell
    for i in range(length):
        if i == row:
            continue
        if_fine_change(i, col, reverse)

    # Assessing the square in a column
    for j in range(length):
        if j == col:
            continue
        if_fine_change(row, j, reverse)
    
    # If not backtracking then store the current image
    temp = tuple(temp)
    if not reverse:
        if temp not in all_image:
            all_image[temp] = True
        else:
            if not all_image[temp]:
                repeat = True
        affected.append(set(temp))

    temp = set()
        
def number_check():
    """A look up to see if all the numbered square have been fullfill"""
    for key, value in setup_config.items():
            if value != 0:
                return key
    return None

def recursion(row, col):
    """The backtracking function"""
    global repeat
    board[row][col] = "c"
    image((row, col))
    traverse.append((row, col))

    backtrack(coor_list)

    repeat = False
    board[row][col] = "."
    traverse.remove((row, col)) 
    image((row, col), True)

# This method is no different from simply starting at a random location instead of a specific start place
def backtrack(move_list):
    """The main function"""
    global no_guide, count, solved, repeat
    count += 1

    # Base Case when a solution is found
    if len(traverse) == length:
        solved = True
        print(f"Steps: {count}", traverse)
        return
    
    # Avoiding image repetition
    if repeat and len(traverse) < length - 1:
        return
    
    # Number square dependency
    location = number_check()
    if location:
        for i, j in surrounding:
            row = location[0] + i
            col = location[1] + j
            if is_okay(row, col):
                recursion(row, col)
            no_guide = set()

    # Search for next square 'randomly'
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