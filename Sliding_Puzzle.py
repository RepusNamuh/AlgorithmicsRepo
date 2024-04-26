import math
import random
from collections import defaultdict

# The Problem is a 3x3 Sliding Puzzle with 4 Red Tiles, 4 Blue Tiles and 1 Empty Tile.
# The Goal is to arrange the tiles in a alternating pattern of Red and Blue Tiles.
# The Empty Tile can only move to the adjacent tile, either up, down, left or right.
# At the end goal, it must be in the middle.
# The programme utilise a greedy but also brute force approach/
# Greedy in that it does not take arbitrary moves, but only move that
# does not break the row or column with the alternating tiles already.
# Brute force in that it does not have a heuristic to determine the best move.
# for it will take at random a move that results in a similar result in 
# close future. The programme also avoid cycles by not landing on a tile more than 3 times.
# The reason for 3 times is simply experimental, maybe there is math to it but I don't know.


# Creating the array Board
array_board = []
choice = ["B","B","B","B", "_", "R", "R", "R", "R"]
for i in range(3):
    temp_array = []
    for j in range(3):
        letter = random.choice(choice)
        temp_array.append(letter)
        choice.remove(letter)
    array_board.append(temp_array)

#Creating the desired configuration
desire_config = [["R", "B", "R"], ["B", "_", "B"], ["R", "B", "R"]]
desire_config2 = [["B", "R", "B"], ["R", "_", "R"], ["B", "R", "B"]]
print(array_board)

### rotate the array clockwise, first col become first row.
def rotate_array(array):
    return[list(row) for row in zip(*array[::-1])]

### Finding the completed rows or columns
com_row = []
com_col = []

def completed_rows(array_board):
    for i in range(len(array_board)):
        if i == 1:
            continue
        if array_board[i] == desire_config[i] or array_board[i] == desire_config2[i]:
            # Avoiding Situation where two completed rows resulting in 
            # traversal of only one row which make it impossible to solve the puzzle.
            if i not in com_row and (com_row == [] or com_col != []):
                com_row.append(i)

def completed_col(array_board):
    temp_array = rotate_array(array_board)
    for j in range(len(temp_array)):
        if j == 1:
            continue
        if temp_array[j] == desire_config[j] or temp_array[j] == desire_config2[j]:
            if j not in com_col and (com_row != [] or com_col == []):
                com_col.append(j)

### Finding where the empty space is initially
def empty_tiles(array_board):
    for i in range(3):
        for j in range(3):
            if array_board[i][j] == "_":
                return i, j
            
# Sets of function to check Possible Moves     
### Visted location, always only store a max of 3 lastest moves.
visited = []
def visiteds(move):
    if len(visited) == 3:
        visited.pop(0)
    
    visited.append(move) 
### Finding valid moves that does not break the pattern.
def possible_move(i, j):
    
    up = (-1,0) if (i-1) >= 0 and (i-1) not in com_row and (i-1, j) not in visited else False
    down = (1,0) if (i+1) <= 2 and (i+1) not in com_row and (i+1, j) not in visited else False
    right = (0,1) if j+1 <= 2 and (j+1) not in com_col and (i, j+1) not in visited else False
    left = (0,-1) if j-1 >= 0 and (j-1) not in com_col and (i, j-1) not in visited else False
    moves = []
    for move in [up, down, right, left]:
        if move!= False:
            moves.append(move)
    return moves

### location counter, a space must not be landed on more than 3 times.
location = defaultdict(tuple)

def cycle_detect(i, j, moves):
    k = 0
    for move in moves:
        if (i + move[0], j + move[1]) in location:
            location[(i + move[0], j + move[1])] += 1
            if location[(i + move[0], j + move[1])] <= 3:
                return move
            else:
                location[(i + move[0], j + move[1])] = 0
        else:
            location[(i + move[0], j + move[1])] = 1
            return move
    return moves[k]

### moving tile function
def move_tile(i, j, direction):
    row, col = direction[0], direction[1]
    array_board[i][j], array_board[i + row][col + j] = array_board[row + i][col + j], array_board[i][j]
    
### Main function
def main():
    i, j = empty_tiles(array_board)
    location[(i, j)] = 1
    visiteds((i, j))
    counter = 0
    while array_board != desire_config and array_board != desire_config2:
        completed_rows(array_board), completed_col(array_board)
        moves = possible_move(i, j)
        move = cycle_detect(i, j, moves)
        move_tile(i, j, move)
        i, j = i + move[0], j + move[1]
        visiteds((i,j))
        counter += 1
    print(counter)
main()



