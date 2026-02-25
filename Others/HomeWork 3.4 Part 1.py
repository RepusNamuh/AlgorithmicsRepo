import random

total = 1000000

# Generate a list of random integers
random_numbers = [random.randint(1, 100000000) for _ in range(total)]

with open('random_numbers.txt', 'w') as file:
    for _ in range(total):
        file.write(f"{random.randint(1, 100000000)}\n")