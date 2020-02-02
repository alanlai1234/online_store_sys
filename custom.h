#include <stdio.h>

typedef struct cart
{
    char idcode[10];
    struct cart *next;
}CART;

typedef struct item
{
    char name[50];
    char descrip[100];
    char seller[20];
    char idcode[10];
    float price;
    struct item *next;
}ITEM;

char accname[20];

void print_item(ITEM *head){
    ITEM *move=head;
    int cur=1;
    printf("\033[1;32m");

    while(move->next!=NULL){
        printf("(%d) $%f\nitem name : %s\n", cur++, move->price, move->name);
        printf("description : %s\n", move->descrip);
        printf("seller : %s\n", move->seller);
        printf("id code : %s\n\n", move->idcode);
        move=move->next;
    }

    printf("\033[0m");
}

int login(char filename[]){
    char name[20], pw[25], name_login[20], psw_login[25];
    int flag;
    FILE *fp;
    fp=fopen(filename, "r");
    printf("login page-----------------------------\n");

    while(1){
        flag=0;
        //back from start
        fseek(fp, 0, SEEK_SET);

        printf("enter user name, or 0 to exit : ");
        scanf("%s", name);

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
            while(1){
                printf("enter password, or 0 to enter username again : ");
                scanf("%s", pw);
                //back to input name
                if(!strcmp(pw, "0")){
                    break;
                }
                //password correct
                if(!strcmp(pw, psw_login)){
                    strcpy(accname, name);
                    printf("\033[0;31m");
                    printf("welcome back %s!\n", accname);
                    printf("\033[0m");
                    fclose(fp);
                    return 1;
                }
                printf("password not match\n");
            }
            continue;
        }
        printf("name not found\n");
    }
}
