#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//item code format
// code[0]='#';
// for(int i=1;i<5;i++) {code[i]=(rand()%10)+'0';}
// code[5]='<';
// code[6]='\0';


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

ITEM *head, *end;
FILE *itemlog;
time_t now;

//login info
char accname[20];

int delete(char[], int);
int login(char[]);
void admin(void);
void buyer(void);
void seller(void);
void create_acc(void);

void print_item(void){
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

int main(void){
    time(&now);
    //enter items into ITEM pointers
    FILE *fp;
    fp=fopen("items.txt", "r");
    head=(ITEM*)malloc(sizeof(ITEM));
    ITEM *tmp=head;

    while(fgets(tmp->name, 50, fp)!=NULL){

        tmp->name[strlen(tmp->name)-1]='\0';
        fgets(tmp->descrip, 100, fp);
        tmp->descrip[strlen(tmp->descrip)-1]='\0';
        fgets(tmp->seller, 20, fp);
        tmp->seller[strlen(tmp->seller)-1]='\0';
        fscanf(fp, "%f\n", &tmp->price);
        fgets(tmp->idcode, 10, fp);
        tmp->idcode[strlen(tmp->idcode)-1]='\0';
        //open space for next slot
        tmp->next=(ITEM *)malloc(sizeof(ITEM));
        tmp->next->next=NULL;
        tmp=tmp->next;
    }
    end=tmp;

    //open item's log
    itemlog=fopen("itemlog.txt", "a+");

    printf("welcome to Alan's store system\n\n");
    
    int choice;
    while(1){

        printf("\033[0;31m");
        printf("main page------------------------------------------------\n");
        printf("\033[0m");
        printf("1)adminstrater 2)buyer 3)seller 4)sign up 0)exit : ");
        scanf("%d", &choice);

        //exit
        if(choice==0) {break;}

        switch (choice)
        {
        case 1:
            admin();
            break;

        case 2:
            buyer();
            break;
        
        case 3:
            seller();
            break;
        
        case 4:
            create_acc();
            break;
        
        default:
            break;
        }
    }

    //save item changes
    FILE *cpy;
    cpy=fopen("items.txt", "w+");
    ITEM *temp;
    srand(time(NULL));
    while(head->next!=NULL){
        fprintf(cpy, "%s\n%s\n%s\n%f\n%s\n", head->name, head->descrip, head->seller, head->price, head->idcode);
        temp=head;
        head=head->next;
        free(temp);
    }
    fclose(cpy);
    fclose(itemlog);
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

void admin(void){

    if(!login("admin.txt")){
        return;
    }
    while(1)
    {
        printf("adminstrater page-------------------------------\n");
        printf("1)show all items\n2)add item\n3)delete item\n4)print item log\n0)exit\n");
        printf("------------------------------------------------\n");

        int choice;
        scanf("%d", &choice);

        //exit
        if(choice==0){
            break;
        }
        switch(choice)
        {
            //show items
            case 1:
                print_item();
                break;
            
            //add item
            case 2:
                {
                //open space
                ITEM *tmp=end;
                tmp->next=(ITEM *)malloc(sizeof(ITEM));
                tmp->next->next=NULL;
                end=tmp->next;
                char buff[50];

                fflush(stdin);

                //name
                printf("input product name (max 50 words) : ");
                getchar();
                fgets(buff, 50, stdin);
                buff[strlen(buff)-1]='\0';
                strcpy(tmp->name, buff);

                fflush(stdin);

                //description
                printf("input product description (max 100 words) : ");
                fgets(buff, 100, stdin);
                buff[strlen(buff)-1]='\0';
                strcpy(tmp->descrip, buff);

                //price
                printf("set the price : ");
                scanf("%f", &tmp->price);

                //generate code
                char code[10];
                int f;
                ITEM *find;
                do{
                    f=1;
                    find=head;
                    code[0]='#';
                    for(int i=1;i<5;i++) {code[i]=(rand()%10)+'0';}
                    code[5]='<';
                    code[6]='\0';
                    //check if generated idcode is duplicated
                    while (find->next!=NULL)
                    {
                        if(!strcmp(find->idcode, code)){
                            f=0;
                            break;
                        }
                        find=find->next;
                    }
                }while(f==0);
                strcpy(tmp->idcode, code);

                //seller
                char tmpstr[]="(admin)";
                strcat(accname, tmpstr);
                strcpy(tmp->seller, accname);

                //item log
                fprintf(itemlog, "%s(added by admin)\n----seller : %s\n----name : %s\n    description : %s\n    price : %f\n\n", ctime(&now), tmp->seller, tmp->name, tmp->descrip, tmp->price);

                printf("successfully added\n");
                break;
                }

            //delete item
            case 3:
                {
                print_item();
                printf("enter id (-1 to exit) : ");
                scanf("%d", &choice);

                //exit
                if(choice==-1) {break;}

                //delete
                if(delete("removed by admin", choice)) {printf("item deleted\n");}
                
                break;
                }
            //print itemlog
            case 4:
                {
                
                fseek(itemlog, 0, SEEK_SET);
                char buff[80];
                while(fgets(buff, 80, itemlog)!=NULL){
                    printf("%s", buff);
                }
                }
            default:
                break;
            
        }
    }
}

void buyer (void){

    //login start
    char name[20], pw[25], name_login[20], psw_login[25], buff[50];
    int flag, flag2=0;
    FILE *fp;
    fp=fopen("buyer.txt", "r");
    printf("login page-----------------------------\n");

    while(1){
        flag=0;
        //back from start
        fseek(fp, 0, SEEK_SET);

        printf("enter user name, or 0 to exit : ");
        scanf("%s", name);

        //exit
        if(!strcmp(name, "0")){
            return;
        }

        //scan from inputted name
        while(fscanf(fp, "%s %s", name_login, psw_login)!=EOF){
            fgetc(fp);
            fgets(buff, 50, fp);
            buff[strlen(buff)-1]='\0';
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
                    flag2=1;
                    strcpy(accname, name);
                    printf("\033[0;31m");
                    printf("welcome back %s!\n", accname);
                    printf("\033[0m");
                    break;
                }
                printf("password not match\n");
            }
            //if password is correct
            if(flag2) {break;}
            continue;
        }
        printf("name not found\n");
    }
    //login ends
    
    int cartempty=1;
    CART *cart_head, *my_cart=(CART *)malloc(sizeof(CART)), *cart_end;
    cart_head=my_cart;

    //split string to CART pointers
    char *delim = " ";
    char *pch;
    pch = strtok(buff, delim);
    
    while (pch != NULL)
    {

        cartempty=0;
        strcpy(my_cart->idcode, pch);
        my_cart->next=(CART *)malloc(sizeof(CART));
        my_cart->next->next=NULL;
        my_cart=my_cart->next;
        pch = strtok(NULL, delim);

    }
    cart_end=my_cart;

    

    //start
    int choice, choice2;
    while(1)
    {

        ITEM *tmp=head, *prev;
        int cur=1, flag=0;

        printf("buyer page-----------------------------------------------------\n");
        print_item();
        printf("select item (0 show your cart, -1 exit) : ");

        scanf("%d", &choice);
        //exit
        if(choice==-1){
            break;
        }
        //into cart menu
        else if(choice==0){

            CART *cart_prev=cart_head;
            my_cart=cart_head;
            if(cartempty){
                printf("your cart is empty!\n");
                continue;
            }
            printf("your cart ------------\n");
            //print out cart
            while (my_cart->next!=NULL)
            {

                flag=0;
                tmp=head;

                //find matched item with the code
                while(tmp->next!=NULL){

                    if(!strcmp(tmp->idcode, my_cart->idcode)){
                        flag=1;
                        printf("(%d) $%f\nitem name : %s\n", cur, tmp->price, tmp->name);
                        printf("description : %s\n", tmp->descrip);
                        printf("seller : %s\n", tmp->seller);
                        printf("id code : %s\n\n", tmp->idcode);
                        break;
                    }
                    tmp=tmp->next;

                }

                //if the code is invalid, delete it from cart
                if(!flag){

                    if(cur==1){
                        cart_head=cart_head->next;
                        free(cart_prev);
                        cart_prev=cart_head;
                        my_cart=cart_head->next;
                    }
                    else{
                        cart_prev->next=my_cart->next;
                        free(my_cart);
                        my_cart=cart_prev->next;
                    }

                }

                else{
                    cart_prev=my_cart;
                    my_cart=my_cart->next;
                }
                cur++;

            }

            
            printf("1)delete 2)exit : ");
            scanf("%d", &choice);
            
            //buy from cart (TODO)
            // if(choice==1){
            //     printf("select item (-1 exit) : ");
            //     scanf("%d", &choice);
            //     if(choice==-1) {continue;}
            //     char str[30]="bought by ";
            //     strcat(str, accname);
            //     delete(str, choice);
            //     printf("transaction sucess\n");
            // }

            //delete item from cart
            if(choice==1){

                CART *cart_prev;
                cur=1;
                my_cart=cart_head;
                printf("select a item from your cart (-1 exit) : ");
                scanf("%d", &choice);
                if(choice==-1) {continue;}

                //delete
                if(choice==1){
                    cart_head=cart_head->next;
                    free(my_cart);
                    if(cart_head->next==NULL){
                        cartempty=1;
                    }
                }
                else{
                    while(my_cart->next!=NULL){
                        if(cur==choice){
                            cart_prev->next=my_cart->next;
                            free(my_cart);
                        }
                        cart_prev=my_cart;
                        my_cart=my_cart->next;
                        cur++;
                    }
                }
                printf("item deleted form your cart\n");
            }

        }
        //add to cart/buy menu
        else{
            printf("1)buy 2)add to cart 3)exit : ");
            scanf("%d", &choice2);

            //exit
            if(choice2==3) {continue;}

            //buy selected item
            else if(choice2==1){
                char str[30]="bought by ";
                strcat(str, accname);
                delete(str, choice);
                printf("transaction sucess\n");
            }

            // add selected item to cart
            else if(choice2==2){
                cartempty=0;
                //open space for new cart slot
                my_cart=cart_end;
                my_cart->next=(CART *)malloc(sizeof(CART));
                my_cart->next->next=NULL;
                cart_end=my_cart->next;

                //locate to selected ITEM pointer
                while(tmp->next!=NULL){
                    if(cur==choice){
                        break;
                    }
                    tmp=tmp->next;
                    cur++;
                }

                strcpy(my_cart->idcode, tmp->idcode);
                printf("%s\n", my_cart->idcode);
                printf("item added to your cart\n");
            }
        }
    }

    // store data to buyer.txt
    FILE *fp2=fopen("buyertmp.txt", "a+");
    fseek(fp, 0, SEEK_SET);
    while(fscanf(fp, "%s %s", name, pw)!=EOF){
        fgetc(fp);
        fgets(buff, 50, fp);
        fprintf(fp2, "%s %s\n", name, pw);
        if(!strcmp(name, accname)){
            if(!cartempty){

                while(cart_head->next!=NULL){
                    fprintf(fp2, "%s ", cart_head->idcode);
                    my_cart=cart_head;
                    cart_head=cart_head->next;
                    free(my_cart);
                }
            
            }
            fprintf(fp2, "\n");
        }
        else{
            fputs(buff, fp2);
        }
    }
    remove("buyer.txt");
    rename("buyertmp.txt", "buyer.txt");
}

void seller(void){

    if(!login("seller.txt")){
        return;
    }

    int choice;
    ITEM *tmp;
    while (1)
    {
        printf("seller page-------------------------------------\n");
        printf("1)add item\n2)delete\n3)show all products\n0)exit\n");
        printf("------------------------------------------------\n");

        tmp=head;
        scanf("%d", &choice);
        
        //exit
        if(choice==0) {break;}
        
        switch (choice)
        {
            //add item
            case 1:
                {
                //open space for new slot
                tmp=end;
                tmp->next=(ITEM *)malloc(sizeof(ITEM));
                tmp->next->next=NULL;
                end=tmp->next;
                char buff[50];
                fflush(stdin);

                //name
                printf("input product name (max 50 words) : ");
                getchar();
                fgets(buff, 50, stdin);
                buff[strlen(buff)-1]='\0';
                strcpy(tmp->name, buff);

                fflush(stdin);

                //description
                printf("input product description (max 100 words) : ");
                fgets(buff, 100, stdin);
                buff[strlen(buff)-1]='\0';
                strcpy(tmp->descrip, buff);

                //seller
                strcpy(tmp->seller, accname);

                //price
                printf("set the price : ");
                scanf("%f", &tmp->price);

                //generate code
                char code[10];
                int f;
                ITEM *find;
                do{
                    f=1;
                    find=head;
                    code[0]='#';
                    for(int i=1;i<5;i++) {code[i]=(rand()%10)+'0';}
                    code[5]='<';
                    code[6]='\0';
                    //check if generated idcode is duplicated
                    while (find->next!=NULL)
                    {
                        if(!strcmp(find->idcode, code)){
                            f=0;
                            break;
                        }
                        find=find->next;
                    }
                }while(f==0);
                strcpy(tmp->idcode, code);

                //item log
                fprintf(itemlog, "%s(added by seller)\n----seller : %s\n----name : %s\n    description : %s\n\n", ctime(&now), tmp->seller, tmp->name, tmp->descrip);

                printf("successfully added\n");
                break;
                }

            //delete
            case 2:
                {
                int cur=1, flag=0, choice, select_cur=1;
                printf("\033[0;35m");
                while(tmp->next!=NULL){
                    if(!strcmp(tmp->seller, accname)){
                        flag=1;
                        printf("(%d) $%f\nitem name : %s\n", cur++, tmp->price, tmp->name);
                        printf("description : %s\n", tmp->descrip);
                        printf("seller : %s\n", tmp->seller);
                        printf("id code : %s\n\n", tmp->idcode);
                    }
                    tmp=tmp->next;
                }
                if(!flag){
                    printf("you have nothing!\n");
                    break;
                }
                printf("\033[0m");

                //delete selected item
                
                printf("enter product id (-1 to exit) : ");
                scanf("%d", &choice);
                if(choice==-1){
                    break;
                }
                flag=0;
                cur=1;
                tmp=head;
                while(tmp->next!=NULL){
                    if(!strcmp(tmp->seller, accname) && select_cur==choice){
                        flag=1;
                        break;
                    }
                    else if(!strcmp(tmp->seller, accname)){
                        select_cur++;
                    }
                    tmp=tmp->next;
                    cur++;
                }

                if(!flag){
                    printf("not found\n");
                    break;
                }

                delete("remove by seller", cur);

                printf("item deleted\n");

                break;
                }

            //show user's products
            case 3:
                {
                int cur=1, flag=0;
                printf("\033[0;35m");
                while(tmp->next!=NULL){
                    if(!strcmp(tmp->seller, accname)){
                        flag=1;
                        printf("(%d) $%f\nitem name : %s\n", cur++, tmp->price, tmp->name);
                        printf("description : %s\n", tmp->descrip);
                        printf("seller : %s\n", tmp->seller);
                        printf("id code : %s\n\n", tmp->idcode);
                    }
                    tmp=tmp->next;
                }
                if(!flag){
                    printf("you have nothing!\n");
                }
                printf("\033[0m");
                break;
                }
            
            default:
                break;
        }
    }
    
}

void create_acc(void){
    printf("sign up page------------------------------------\n");
    printf("sign up for 1)buyer 2)seller 0)exit\n");
    printf("------------------------------------------------\n");

    FILE *fp;
    char buff[20], input[20];
    int choice, flag=0;

    scanf("%d", &choice);

    
    if(choice==0){
        return;
    }

    if(choice==1){
        
        fp=fopen("buyer.txt", "r+");
        while(!flag){
            flag=1;
            printf("enter username(no space) : ");
            scanf("%s", input);
            while (fscanf(fp, "%s", buff)!=EOF){
                if(!strcmp(buff, input)){
                    flag=0;
                    printf("username exist!\n");
                    break;
                }
            }
        }

        fprintf(fp, "\n%s ", input);
        fseek(fp, 0, SEEK_SET);
        char buff2[20];
        flag=0;
        while (!flag)
        {
            flag=1;
            printf("enter password(no space) : ");
            scanf("%s", input);
            while (fscanf(fp, "%s %s", buff2, buff)!=EOF){
                if(!strcmp(buff, input)){
                    flag=0;
                    printf("password exist!\n");
                    break;
                }
            }
        }
        fprintf(fp, "%s", input);
    }
    else if(choice==2)
    {
        fp=fopen("seller.txt", "r+");

        while(!flag){
            flag=1;
            fseek(fp, 0, SEEK_SET);
            printf("enter username(no space) : ");
            scanf("%s", input);
            while (fscanf(fp, "%s", buff)!=EOF){
                if(!strcmp(buff, input)){
                    flag=0;
                    printf("username exist!\n");
                    break;
                }
            }
        }

        fprintf(fp, "\n%s ", input);
        fseek(fp, 0, SEEK_SET);
        flag=0;
        char buff2[20];

        while (!flag)
        {
            flag=1;
            printf("enter password(no space) : ");
            scanf("%s", input);
            while (fscanf(fp, "%s %s", buff2, buff)!=EOF){
                if(!strcmp(buff, input)){
                    flag=0; 
                    printf("password exist!\n");
                    break;
                }
            }
        }

        fprintf(fp, "%s", input);
    }
    
}

int delete(char profile[], int choice){
    ITEM *tmp=head, *prev=NULL;
    int cur=1, flag=0;

    while(tmp->next!=NULL){
        if(cur==choice){
            flag=1;
            break;
        }
        prev=tmp;
        tmp=tmp->next;
        cur++;
    }

    if(!flag) {
        printf("item not found!\n");
        return 0;
    }

    if(prev==NULL){
        head=head->next;
    }
    else{
        prev->next=tmp->next;
    }

    fprintf(itemlog, "%s(%s)\n----seller : %s\n----name : %s\n    description : %s\n\n", ctime(&now), profile, tmp->seller, tmp->name, tmp->descrip);
    free(tmp);
    return 1;
}