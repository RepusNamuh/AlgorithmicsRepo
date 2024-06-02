from collections import defaultdict, Counter

# Setting Up the Configuration
original_config = [[None, None, None, None, None, None],
                   [None, None, None, None, None, None],
                   [None, None, None, None, None, None],
                   [2, None, None, None, None, 1],
                   [None, None, None, None, None, None],
                   [None, None, 1, None, None, None]
                   ]
# Finding Square that contain number
setup_config = defaultdict(int)
# ADT to store movement that are no longer possible
eliminated_coor = set()
length = len(original_config)
# Create a list of coordinate from the matrix of the configuration
coor_list = set((i, j) for i in range(length) for j in range(length))
coor_list_version = list()

# For recursion limit
max_recur = 500
# For tracking total coloured node of 6x6
count = 0
# Tracking last parent square e.g a number node otherwise None
last = []
# Tracking traverse node
traverse = []

None_square = []

# Searching for all move that are not possible due to the number or coloured square
def valid_affected_square(next):
    """If the square is not available or exist, then it status won't change"""
    if next not in (coor_list): 
        return False
    return True

def remove(cell, permanent = False):
    if valid_affected_square(cell):
        original_config[cell[0]][cell[1]] = "affected"
        coor_list.remove(cell)
    # Cases where the cell have been coloured before but now are empty and affected by
    # other square
    if cell in None_square:
        original_config[cell[0]][cell[1]] = "affected"

def affected_squares_radius(coloured_square, permanent = False): 
    """Calculate all affected square in the vicinity of the coloured square.
    Also search if the coloured square fullfill any number_square adj"""
    row, col = coloured_square
    for i in [-1,0,1]:
        for j in [-1,0,1]:
            if i == j == 0:
                continue
            next = (row + i, col + j)
            if next in setup_config:
                number_affected_square(next)
            remove(next)
    
def number_affected_square(number_square):
    """If the number of coloured square around the number reach maximum
    eliminate all the square around """
    setup_config[number_square] -=1
    max_adj = setup_config[number_square]
    if (max_adj) == 0:
        row, col = number_square
        for i in [-1,0,1]:
            for j in [-1,0,1]:
                if i == j == 0:
                    continue
                next = (row + i, col + j) 
                remove(next)
     
def affected_row_col(coloured_square):
    """Eliminate all square in a row and col temporarily"""
    row, col = coloured_square
    for j in range(len(original_config)):
        if j == col: # Need to be a different cell
            continue
        next = (row, j)
        remove(next)

    for i in range(len(original_config[0])):
        if i == row:
            continue
        next = (i, col)
        remove(next)

def available_move(coloured_square):
    """Calculating the list of move after a square is coloured"""
    affected_squares_radius(coloured_square)
    affected_row_col(coloured_square)

# Searching for square that contain number
def layout_search():
    "Initital Search for number_square location"
    for i, row in enumerate(original_config):
        for j, element in enumerate(row):
            if element != None and element != "affected":
                setup_config[(i, j)] = element
                eliminated_coor.add((i, j))
                coor_list.remove((i, j))
                if element == 0:
                    affected_squares_radius((i, j))

# Finding Moves.
def number_check():
    """A look up to see if all the numbered square have been fullfill"""
    for key, value in setup_config.items():
            if value != 0:
                return key
    return None
    
def next_move(number_square):
    """Search for the next possibe square"""
    # Two type of scenario, all number square are fullfill which mean we can start
    # extracting move from the coor_list/available_move
    # Other wise, Find the next possible move around number_square
    if not number_square:
            next = list(coor_list)[0] if coor_list else None
            None_square.append(next) if next else None
            return next
    row, col = number_square
    for i in [-1,0,1]:
        for j in [-1,0,1]:
            next = (row + i, col + j)
            if next != number_square and next in coor_list:
                eliminated_coor.add(next)
                return next

    return None

# Checking if things need to be reverse
def full_affected_check():
    """An optimisation search to quickly calculate if a previous move cause a 
    row or column to not be fullfill that is if move (i, j) in the previous movement cause
    row i+1 to have only affected square (this will only occur if there are number_square
    effect in conjuction)"""
    for row in original_config:
        check = Counter(row)
        if check["coloured"] == 0 and check[None] == 0 :
            return True
    for col in zip(*original_config[::1]):
        check = Counter(col)
        if check["coloured"] == 0 and check[None] == 0:
            return True
    return False

def reverse_square_state():
    """Reverting all the state of square that were change in the previous image"""
    for i, j in coor_list:
        original_config[i][j] = None

def reverse(next):
    """Checking if the solution need to be reverse to a previous state"""
    global coor_list, coor_list_version, count, None_square
    if max_recur==0:
        return "Stop"
    if (not next) or (count!= 7 and len(coor_list) ==0) or full_affected_check():
        count -= 2 if next else 1
        try:
            next ,coor_list = last.pop(), coor_list_version.pop()
            reverse_square_state()
            i, j = traverse.pop()
            original_config[i][j] = None
        except:
            return "Stop"
        if next != None:
            coor_list.update(coor_list, None_square)
            setup_config[next] +=1
            None_square = []
    return next

# Creating an image
def image(next, previous):
    """Creating a copy/image of the previous state of the solution"""
    coor_list.remove(next)
    original_config[next[0]][next[1]] = "coloured"
    temp = coor_list.copy()
    coor_list_version.append(temp) # List of coor before affected_square Calculation
    available_move(next) # List  of coor after affected_square Calculation
    last.append(previous), traverse.append(next)
    print(max_recur, traverse)


def main(previous_square):
    global count, max_recur
    max_recur -=1
    # Base Case
    if count == len(original_config):
        print("Solve:", traverse)
    else:
        next = next_move(previous_square)
        count += 1 if next else 0
        reverse_check = reverse(next)
        if reverse_check == "Stop":
            print("Unsolvable")
        elif reverse_check != next or next == None:
            print(max_recur, traverse)
            main(reverse_check)
        else:
            image(next, previous_square)
            main(number_check())
        
if __name__ == '__main__':
    layout_search()
    start = number_check()
    main(start)
