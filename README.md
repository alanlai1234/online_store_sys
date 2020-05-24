# simple_store_sys (new version under construction! use old_main.c)

the current version of main.c uses ncurses that detects arrow key inputs to move through different selections, instead of typing selection numbers out, but im still working on it, so don't use it. You can use old_main.c which is the version that you need to type numbers in, but its stable and its pretty much done.

a simple store system that includes admin, buyer, seller accounts

the "custom.h" header file is where some functions and types

program is in the "main.c" as the file name suggest

"admin.txt", "buyer.txt", "seller.txt" is where the accounts are stored

all the items are in "items.txt", and the transactions are documented in "itemlog.txt"

admin can show, add, delete items, print out item log, and shut down the system

buyer can buy items, add items to cart(and checkout the whole cart), and also delete items from its cart.

seller can add item, delete its items, and show its items

a item includes a price, name, decription, a seller(can be a admin), idcode, amount

items are stored in a linked list when inside the program, and stored back to the text file when exiting
