# simple_store_sys
a simple store system includes admin, buyer, seller accounts

the "custom.h" header file is where some functions and types

program is in the "main.c" as the file name suggest, and the output file is "main"

"admin.txt", "buyer.txt", "seller.txt" is where the accounts are stored

all the items are in "items.txt", and the transactions are documented in "itemlog.txt"

admin can show, add, delete items, print out item log, and shut down the system

buyer can buy items, add items to cart, also delete items from its cart.

seller can add item, delete its items, and show its items

a item includes a price, name, decription, a seller(can be a admin), idcode
item is stored in a linked list when in the program, and store back to the text file when exit program
