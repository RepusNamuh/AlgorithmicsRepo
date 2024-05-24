from collections import defaultdict

# Setting Up the Configuration
original_config = [[None, None, None, None, None, None],
                   [None, None, None, None, None, None],
                   [None, None, 0, None, None, None],
                   [None, None, None, None, None, None],
                   [None, None, None, 2, None, None],
                   [None, None, None, None, None, None]]
# Finding Square that contain number
setup_config = defaultdict(int)
# ADT to store movement that are no longer possible
eliminated_coor = set()

# Create a list of coordinate from the matrix of the configuration
coor_list = set((i, j) for i in range(6) for j in range(6))
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
    if next not in (coor_list): 
        return False
    return True

def affected_squares_radius(coloured_square, permanent = False):
    """Calculate all affected square in the vicinity of the coloured square.
    Also search if the coloured square fullfill any number_square adj"""
    try:
        row, col = coloured_square
        for i in [-1,0,1]:
            for j in [-1,0,1]:
                next = (row + i, col + j)
                if i == j == 0:
                    continue
                if next in setup_config:
                    number_affected_square(next)
                if valid_affected_square(next):
                    if permanent: # This is simply an extra in case there is a zero number square
                        eliminated_coor.add(next)
                    coor_list.remove(next)
    
    except:
        None

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
                if valid_affected_square(next):
                    coor_list.remove(next)
        
        
def affected_row_col(coloured_square):
    """Eliminate all square in a row and col temporarily"""
    if coloured_square:
        row, col = coloured_square
        for j in range(len(original_config)):
            next = (row, j)
            if valid_affected_square(next):
                coor_list.remove(next) 

        for i in range(len(original_config[0])):
            next = (i, col)
            if valid_affected_square(next):
                coor_list.remove(next)
    else:
        None

def available_move(coloured_square):
    """Producing a list of move that are available through converting list to set and back again"""
    affected_squares_radius(coloured_square)
    affected_row_col(coloured_square)

# Searching for square that contain number
def layout_search():
    for i, row in enumerate(original_config):
        for j, element in enumerate(row):
            if element != None:
                setup_config[(i, j)] = element
                eliminated_coor.add((i, j))
                coor_list.remove((i, j))
                if element == 0:
                    affected_squares_radius((i, j), True)

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

def reverse(next):
    global coor_list, coor_list_version, count, None_square
    if max_recur==0:
        return "Stop"
    if not next or (count!= 6 and len(coor_list) ==0) and len(last) != 0:
        next = last.pop()
        coor_list = coor_list_version.pop()
        if next != None:
            coor_list.update(coor_list, None_square)
            setup_config[next] +=1
            None_square = []
        traverse.pop()
        count -= 1
    return next

def image(next, previous):
    global max_recur
    coor_list.remove(next)
    temp = coor_list.copy()
    coor_list_version.append(temp) # List of coor before affected_square Calculation
    available_move(next) # List  of coor after affected_square Calculation
    last.append(previous)
    traverse.append(next)
    max_recur -=1
    print(max_recur, traverse)

def main(previous_square):
    global count, max_recur
    # Base Case
    if count == len(original_config):
        last.append(previous_square)
        print("Solve:", traverse)
    else:
        next = next_move(previous_square)
        count += 1 if next else 0
        reverse_check = reverse(next)
        if reverse_check == "Stop":
            print("Unsolvable")
        elif reverse_check != next or next == None:
            max_recur -= 1
            print(max_recur, traverse)
            main(reverse_check)
        else:
            image(next, previous_square)
            main(number_check())
        

        
if __name__ == '__main__':
    layout_search()
    start = number_check()
    main(start)
