# Normal Distribution Generator.
# Prints a list of n numbers such that they sum to 1 and are normally
# distributed. Uses a table to generate pascals triangle and exploit
# that the cross diagonals are allways normally distributed.
# 
# Editied 26/08/2017

table_size = int(input("Enter sequence length: "))

if not table_size & 1:
    print("Warning: The sequence is even.")

table = [[0] * table_size for _ in range(table_size)]

# set first row and col to 1
for i in range(table_size):
    table[0][i] = table[i][0] = 1

# total or cross diagonals so that we can normalise values
total = 1 + 1  # for the bounding 1's
for i in range(1, table_size):
    for j in range(1, table_size - i):
        table[i][j] = table[i - 1][j] + table[i][j - 1]

        if table_size - i - 1 == j:
            total += table[i][j]


# print sequence
print("{", end='')
for i in range(table_size):
    print(table[table_size - i - 1][i] / total, end=', ' if i != table_size - 1 else '')
print("}")
print()