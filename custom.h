#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>

typedef struct cart
{
    char idcode[10];
	int amount;
    struct cart *next;
}CART;

typedef struct item
{
    char name[50];
    char descrip[100];
    char seller[20];
    char idcode[10];
    float price;
    int amount;
    struct item *next;
}ITEM;

char accname[20];

int print_item(ITEM *head){
	int cur=1, ch, pos=1;
	refresh();
	WINDOW *win = newwin(LINES, COLS, 0, 0);
    ITEM *move=head;
	// initial new window's curses mode
	touchwin(win);
	wrefresh(win);
    scrollok(win,TRUE);
	idlok(win, TRUE);
	noecho();

	while (ch!='q'){
		cur=1;
		// while(move->next!=NULL && cur*2<LINES){
			// wprintw(win, "(%d) $%f - item name : %s\n\n", cur++, move->price, move->name);
			// move=move->next;
		// }
		move=head;
		while(move->next!=NULL){
			wprintw(win, "(%d) $%f - item name : %s\n\n", cur++, move->price, move->name);
			move=move->next;
		}
		wscrl(win, -5);
		ch=wgetch(win);
		switch(ch){
			case KEY_DOWN:
				if(pos==LINES && move->next!=NULL){
					pos+=2;
				}
				else if(pos<LINES){
					pos+=2;
				}
			default:
				break;
		}
	}
	delwin(win);
	attron(COLOR_PAIR(2));
	echo();
	return 0;
}

int login(char filename[]){
    char name[20], pw[25], name_login[20], psw_login[25];
	wchar_t key_icons = 0x1F511;
    int flag;
    FILE *fp;
	setlocale(LC_ALL, "");
    fp=fopen(filename, "r");
	clear();
	curs_set(1);
    while(1){
        flag=0;
        //back from start
        fseek(fp, 0, SEEK_SET);

        printw("enter user name, enter 0 to exit : ");
		scanw("%s", name);

        //exit
        if(!strcmp(name, "0")){
            fclose(fp);
            return 0;
        }

        //scan from inputted name
        while(fscanf(fp, "%s %s", name_login, psw_login)!=EOF){
            //inputted name found
            if(!strcmp(name, name_login)){
                flag=1;
                break;
            }
        }
        
        //if inputted name found
        if(flag){
			clear();
            while(1){
                printw("enter password, or 0 to enter username again : ");
				printw(L"%lc", key_icons);
				noecho();
                scanw("%s", pw);
				echo();
                //back to input name
                if(!strcmp(pw, "0")){
					clear();
                    break;
                }
                //password correct
                if(!strcmp(pw, psw_login)){
                    strcpy(accname, name);
                    fclose(fp);
					
                    return 1;
                }
				clear();
                printw("password not match\n");
            }
            continue;
        }
		clear();
        printw("name not found\n");
    }
}
