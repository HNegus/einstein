import numpy as np
from collections import deque
from time import time
import os

EMPTY, FILLED, DISALLOWED = 0, 1, 2

def rot2d(degrees):
    """Return rotation matrix"""
    angle = np.radians(degrees)
    c, s = np.cos(angle), np.sin(angle)
    return np.array([[c, -s], [s, c]])


"""Functions to return coordinates for the different blocks"""
def get_a(x, y, rotation=0, mirror=False):
    A = np.array([[0, 0, 0, 0, 2, 2, 2, 2, 1, 1], 
                  [0, 1, 2, 3, 0, 1, 2, 3, 1, 3]])
    r = rot2d(rotation)
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_t(x, y, rotation=0, mirror=False):
    A = np.array([[0, 1, 1, 1, 1, 2, 2],
                  [2, 0, 1, 2, 3, 0, 2]])
    r = rot2d(rotation)
    if mirror:
        A[0] = -A[0]
        A[0] += 3
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_l(x, y, rotation=0, mirror=False):
    A = np.array([[0, 0, 0, 0, 1],
                  [0, 1, 2, 3, 0]])
    r = rot2d(rotation)
    if mirror:
        A[0] = -A[0]
        A[0] += 2
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_big_l(x, y, rotation=0, mirror=False):
    A = np.array([[0, 0, 0, 0, 1, 2],
                  [0, 1, 2, 3, 0, 0]])
    r = rot2d(rotation)
    if mirror:
        A[0] = -A[0]
        A[0] += 3
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_u(x, y, rotation=0, mirror=False):
    A = np.array([[0, 0, 0, 1, 2, 2, 2],
                  [0, 1, 2, 0, 0, 1, 2]])
    r = rot2d(rotation)
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_i(x, y, rotation=0, mirror=False):
    A = np.array([[0, 0, 0],
                  [0, 1, 2]])
    r = rot2d(rotation)
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_s(x, y, rotation=0, mirror=False):
    A = np.array([[0, 0, 1, 1],
                  [0, 1, 1, 2]])
    r = rot2d(rotation)
    if mirror:
        A[0] = -A[0]
        A[0] += 2
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_e(x, y, rotation=0, mirror=False):
    A = np.array([[0, 0, 1, 1, 2],
                  [0, 1, 1, 2, 2]])
    r = rot2d(rotation)
    if mirror:
        A[0] = -A[0]
        A[0] += 3
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_b(x, y, rotation=0, mirror=False):
    A = np.array([[0, 0, 0, 0, 1, 1],
                  [0, 0, 0, 0, 2, 3]])
    r = rot2d(rotation)
    if mirror:
        A[0] = -A[0]
        A[0] += 2
    A = (r @ A).astype('int')
    A[0] += x
    A[1] += y
    
    return A

def get_blocks():
    return [
            get_a,
            get_t, get_t, 
            get_big_l, 
            get_u, get_u,
            get_e, get_e, get_e,
            get_b,
            get_l, 
            get_i, get_i,
            get_s,
            ]

def get_block_names():
    """Return the block names"""
    return ["_".join(x.__name__.split("_")[1:]).upper() for x in get_blocks()]


def get_params():
    """Yield all possible parameters for the block functions"""
    for x in range(0, 10):
        for y in range(0, 10):
            for r in [0, 90, 180, 270]:
                yield (x, y, r, True)
                yield (x, y, r, False)

def print_board(board):
    for row in board:
        print(row)

def print_result(configuration):
    print("Letter \t Configuration (x, y, rotation, mirrored)")
    for letter, config in zip(get_block_names(), configuration):
        print(f"{letter} \t {config}")

def solve(board):
    # Store all possible block positions
    param_queue = [deque(get_params()) for _ in range(14)]
    
    # Store position and orientation for the placed blocks
    current_configuration = [0 for _ in range(14)]

    # Keep track of the percentage of invalid guesses (blocks placed outside 
    # the field) and the guesses placed on an occupied position 
    invalid, not_empty = 0, 0

    previous_changes = deque()
    blocks, block_index = get_blocks(), 0
    now, counter = time(), 0
    N = 1000

    while block_index < 14:
        counter += 1
        
        # Show info on search
        if counter % N == 0:
            os.system('cls' if os.name=='nt' else 'clear')
            print()
            print(f"Round {counter}")
            print(f"\t Placed blocks\t\t: {block_index}") 
            print(f"\t Invalid placements\t: {round(invalid/counter*100)}%")
            print(f"\t Occupied placements\t: {round(not_empty/counter*100, 3)}%")
            
            t, now = time() - now, time()
            print(f"\t Time per {N} rounds\t: {round((t), 3)}s")
            print(f"\t Iterations per second\t: {round(1 / t * N)}")

            print()
            print(f"Board state ({EMPTY} = EMPTY, {FILLED} = FILLED, {DISALLOWED} = DISALLOWED)\n")
            print_board(board)
        
        # Backtrack if no more positions for the current block are possible 
        while len(param_queue[block_index]) == 0:
            
            # Reinitialise possible block positions 
            param_queue[block_index] = deque(get_params())
            
            # Revisit previous block
            block_index -= 1

            # Restore previous board state
            changes = previous_changes.pop()
            for x, y in changes:
                board[y][x] = EMPTY
            # print_board(board)

            if block_index < 0:
                print("No solution found!")
                exit()

        # Get block coordinates for the next position 
        params = param_queue[block_index].pop()
        coords = blocks[block_index](*params)
        
        # Check if block is entirely on the board
        if np.any(coords[coords < 0]) or np.any(coords[coords >= 10]):
            invalid += 1
            continue
        
        changes = []

        # Place the block if possible
        for x, y in coords.T:
            if board[y][x] != EMPTY:
                not_empty += 1

                # Restore board state if blocks overlap
                for x, y in changes:
                    board[y][x] = EMPTY
                break
    
            board[y][x] = FILLED
            changes.append((x, y))

        # Move to next block if placement is succesful
        else:

            # Store changes so board state can be restored when backtracking
            previous_changes.append(changes)

            # Check if solution is found
            # NOTE: not necessary for challenge 6, but in general not all
            # blocks are needed
            done = True
            for row in board:
                if EMPTY in row:
                    done = False
                    break

            # Store the position and orientation of the placed block
            current_configuration[block_index] = params
            
            block_index += 1
            if done:
                print(f"{counter}: DONE")
                print_result(current_configuration)
                break

def challenge6():
    """Solve challenge 6"""
    disallowed = [(0, 0), (1, 0), (2, 0), (3, 0), (4, 0),
                  (0, 1), (1, 1), (2, 1), (3, 1),
                  (0, 2), (1, 2), (1, 2),
                  (0, 3), (1, 3),
                  (0, 4),
                  (4, 4),
                  (8, 8), (8, 9), (9, 8), (9, 9)]
    board = [[EMPTY for _ in range(10)] for _ in range(10)]
    for x, y in disallowed:
        board[y][x] = DISALLOWED
    solve(board)

if __name__ == '__main__':
    try:
        challenge6()
    except KeyboardInterrupt:
        pass
