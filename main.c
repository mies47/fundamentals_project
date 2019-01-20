#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
struct problem{
    char prob_def[200];
    char first_dec[200];
    int fp;//people
    int fc;//court
    int ft;//treasury
    char second_dec[200];
    int sp;//people
    int sc;//court
    int st;//treasury
    int possibility;
    struct problem *next;
};
struct Problem_Score{
    char name[20];
    int people;
    int court;
    int treasury;
};
struct problem * create_node(void){
    struct problem * new_node;
    new_node = (struct problem *)malloc(sizeof(struct problem));
    new_node->next = NULL;
    return new_node;
}

void Add_end(struct problem *head , struct problem *new_node){
    struct problem * temp = NULL;
    temp = head;
    while (temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->next = NULL;
}
void Get_prob(struct problem *head ,struct problem * holder , FILE * Main_File){
    while (1){//Fetching information from files to link list
        fgets(holder->prob_def , 200 , Main_File);
        fgets(holder->first_dec , 200 , Main_File);
        fscanf(Main_File , "%d" , &(holder->fp));
        fscanf(Main_File , "%d" , &(holder->fc));
        fscanf(Main_File , "%d\n" , &(holder->ft));
        fgets(holder->second_dec , 200 , Main_File);
        fscanf(Main_File , "%d" , &(holder->sp));
        fscanf(Main_File , "%d" , &(holder->sc));
        fscanf(Main_File , "%d" , &(holder->st));
        holder->possibility = 3;
        break;
    }
    Add_end(head , create_node());//Add another node to the end for next problem
    while (holder->next != NULL){//Change holder to the last node
        holder = holder->next;
    }
}
int linked_num(struct problem * str){//Check the number of remained problems
    int cnt = 0;
    while(str != NULL){
        str = str->next;
        cnt++;
    }
    cnt--;//cnt was increased one extra
    return cnt;
}
struct problem * Linked_Rand(struct problem * str){//Choose a random problem
    struct problem *temp;
    temp = str;
    int cnt = linked_num(str);
    srand(time(NULL));
    int Rand = ((int)rand())%cnt;
    for (int i = 0; i < Rand ; ++i) {
        temp = temp->next;
    }
    return temp;
}
struct problem * delete_node(struct problem *str , struct problem *mem){
    struct problem *temp = NULL;
    temp = str;
    if(mem == str){//delete the first node
        temp = str->next;
        free(str);
        return temp;
    } else{
        while (temp->next != mem){
            temp = temp->next;
        }
        temp->next = mem->next;
        free(mem);
    }
    return str;
}
struct problem * Check_cnt(struct problem *str){//check for 0 problem possibility
    struct problem *temp = NULL;
    temp = str;
    while (temp!=NULL){
        if(temp->possibility <= 0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}


int * Arr_possibility(struct problem *head){//save problem possibility in an Array for save
    int size = linked_num(head);
    int *Arr_poss;
    Arr_poss = malloc(sizeof(int) * size);
    struct problem *temp = NULL;
    temp = head;
    for (int i = 0; temp != NULL ; ++i) {
        Arr_poss[i] = temp->possibility;
        temp = temp->next;
    }
    return Arr_poss;
}
void Linked_info(struct problem * holder , FILE * Main_File){
    while (1){//Fetching information from files to link list
        fgets(holder->prob_def , 200 , Main_File);
        fgets(holder->first_dec , 200 , Main_File);
        fscanf(Main_File , "%d" , &(holder->fp));
        fscanf(Main_File , "%d" , &(holder->fc));
        fscanf(Main_File , "%d\n" , &(holder->ft));
        fgets(holder->second_dec , 200 , Main_File);
        fscanf(Main_File , "%d" , &(holder->sp));
        fscanf(Main_File , "%d" , &(holder->sc));
        fscanf(Main_File , "%d" , &(holder->st));
        holder->possibility = 3;
        break;
    }
}
void Game_save(char *user_name , struct problem *head, int status , int people , int court , int treasure){
    FILE *save;
    char Add[strlen(user_name) + 5];
    strcpy(Add , user_name);
    char plus[] = ".bin";
    strcat(Add , plus);
    save = fopen(Add , "wb+");
    FILE *name_save;
    name_save = fopen("name_save.txt" ,"a");
    fputs(user_name , name_save);
    char temp[] = "\n";
    fputs(temp, name_save);
    int *remain = Arr_possibility(head);
    fwrite(user_name , sizeof(char *) , 1 , save);
    fwrite(&status , sizeof(int) , 1 ,save);
    fwrite(remain , sizeof(int *) , 1 ,save);
    fwrite(&people , sizeof(int) , 1 , save);
    fwrite(&court , sizeof(int) , 1 , save);
    fwrite(&treasure , sizeof(int) , 1 ,save);
    fclose(name_save);
    fclose(save);
}
void My_exit(char *user_name , struct problem *head, int status , int people , int court , int treasure , int F_choice){
    if(F_choice == 1){
        Game_save(user_name , head,  status ,  people , court , treasure);
        exit(-1);
        //Game_save(name , head , status , people_effect , court_effect , treasury_effect);
    } else if(F_choice == 2){
        exit(-1);
    } else{
        printf("No valid input.\n");
        exit(-1);
    }
}

int main() {
    char name[20];
    char Address_prob[20];
    int status = 1;
    FILE *choices;
    choices = fopen("CHOICES.txt" , "r");
    if(choices == NULL){
        printf("Couldn't Find The Valid Files");
        exit(-1);
    }
    printf("Please enter your name:");
    gets(name);
    printf("Please choose :\n");
    printf("[1]Start a new game\n");
    printf("[2]Load saved game\n");
    printf("In any part of the game enter -1 to exit the game\n");
    int strchoose;
    scanf("%d" , &strchoose);
    if(strchoose == 1){
        int people_effect = 50;
        int court_effect = 50;
        int treasury_effect = 50;
        double average_effect = 50;
        struct problem *head;
        head = create_node();
        struct problem* holder;
        holder = head;
        for (int i = 1; !feof(choices) ; ++i) {
            fscanf(choices , "%s" , Address_prob);//Getting c[i].txt from CHOICES.txt
            FILE * Main_File;
            Main_File = fopen(Address_prob , "r");
            if(Main_File == NULL){
                printf("No file found");
            }
            Linked_info(holder ,Main_File);
            Add_end(head , create_node());//Add another node to the end for next problem
            while (holder->next != NULL){//Change holder to the last node
                holder = holder->next;
            }
            fclose(Main_File);
        }
        while (average_effect>10 && people_effect > 0 && court_effect > 0 && treasury_effect > 0){//showing the problems
            struct problem *prob_rand = NULL;
            prob_rand = Linked_Rand(head);
            (prob_rand->possibility)--;
            printf("%s\n" , prob_rand->prob_def);
            printf("[1]%s\n" , prob_rand->first_dec);
            printf("[2]%s\n" , prob_rand->second_dec);
            int dec;
            scanf("%d" , &dec);//Get the dec for problem
            switch (dec) {
                case 1:{
                    people_effect += prob_rand->fp;
                    if (people_effect > 100)
                        people_effect = 100;
                    court_effect += prob_rand->fc;
                    if (court_effect > 100)
                        court_effect = 100;
                    treasury_effect += prob_rand->ft;
                    if(treasury_effect > 100)
                        treasury_effect = 100;
                    break;}
                case 2:{
                    people_effect += prob_rand->sp;
                    if (people_effect > 100)
                        people_effect = 100;
                    court_effect += prob_rand->sc;
                    if (court_effect > 100)
                        court_effect = 100;
                    treasury_effect += prob_rand->st;
                    if(treasury_effect > 100)
                        treasury_effect = 100;
                    break;}
                case -1:{
                    printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
                    int F_choice;
                    scanf("%d" , &F_choice);
                    My_exit(name , head , status , people_effect , court_effect , treasury_effect , F_choice);
                    break;
                }
                default: {
                    printf("No valid input");
                    exit(-1);
                }
            }
            printf("People: %d Court: %d Treasury: %d\n" , people_effect , court_effect , treasury_effect);
            average_effect = (people_effect + court_effect +treasury_effect)/3;
            struct problem * del_node = Check_cnt(head);
            if(del_node!= NULL){
                head = delete_node(head , del_node);
            }
            if(head == NULL){//Check to see if all the problems were deleted
                head = create_node();
                holder = head;
                rewind(choices);
                fscanf(choices , "%s" , Address_prob);//Getting c[i].txt from CHOICES.txt
                FILE * Main_File;
                Main_File = fopen(Address_prob , "r");
                if(Main_File == NULL){
                    printf("No file found");
                }
                Linked_info(holder , Main_File);
                Add_end(head , create_node());//Add another node to the end for next problem
                while (holder->next != NULL){//Change holder to the last node
                    holder = holder->next;
                }
            }
        }
        status = 0;
        printf("You have lost!\n");
        printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
        int F_choice;
        scanf("%d" , &F_choice);
        My_exit(name , head , status , people_effect , court_effect , treasury_effect , F_choice);
    }else if(strchoose == 2){//Loading the game from the last saved file
        int people_effect;
        int court_effect;
        int treasury_effect;
        FILE * Load;
        char Add[strlen(name) + 5];
        strcpy(Add , name);
        char plus[] = ".bin";
        strcat(Add , plus);
        if(fopen(Add , "rb")==NULL){
            printf("There is no saved game.");
            exit(-1);
        }
        Load = fopen(Add , "rb+");
        int Arr_remain[50];
        fread(name , sizeof(char *) , 1 , Load);//Fetching info from the saved file
        fread(&status , sizeof(int) , 1 , Load);
        fread(Arr_remain , sizeof(int *) , 1 , Load);
        fread(&people_effect , sizeof(int) , 1 , Load);
        fread(&court_effect , sizeof(int) , 1 , Load);
        fread(&treasury_effect , sizeof(int) , 1 , Load);
        if(status == 1){
            struct problem *head;
            head = create_node();
            struct problem* holder;
            holder = head;
            for (int i = 1; !feof(choices) ; ++i) {
                fscanf(choices , "%s" , Address_prob);//Getting c[i].txt from CHOICES.txt
                FILE * Main_File;
                Main_File = fopen(Address_prob , "r");
                if(Main_File == NULL){
                    printf("No file found");
                }
                Linked_info(holder ,Main_File);
                holder->possibility = Arr_remain[i-1];
                Add_end(head, create_node());//Add another node to the end for next problem
                while (holder->next != NULL) {//Change holder to the last node
                    holder = holder->next;
                }
                fclose(Main_File);
            }
            printf("People: %d Court: %d Treasury: %d\n" , people_effect , court_effect , treasury_effect);
            int average_effect = (people_effect + court_effect +treasury_effect)/3;
            while (average_effect>10 && people_effect > 0 && court_effect > 0 && treasury_effect > 0){//showing the problems
                struct problem *prob_rand = NULL;
                prob_rand = Linked_Rand(head);
                (prob_rand->possibility)--;
                printf("%s\n" , prob_rand->prob_def);
                printf("[1]%s\n" , prob_rand->first_dec);
                printf("[2]%s\n" , prob_rand->second_dec);
                int dec;
                scanf("%d" , &dec);//Get the dec for problem
                switch (dec) {
                    case 1:{
                        people_effect += prob_rand->fp;
                        if (people_effect > 100)
                            people_effect = 100;
                        court_effect += prob_rand->fc;
                        if (court_effect > 100)
                            court_effect = 100;
                        treasury_effect += prob_rand->ft;
                        if(treasury_effect > 100)
                            treasury_effect = 100;
                        break;}
                    case 2:{
                        people_effect += prob_rand->sp;
                        if (people_effect > 100)
                            people_effect = 100;
                        court_effect += prob_rand->sc;
                        if (court_effect > 100)
                            court_effect = 100;
                        treasury_effect += prob_rand->st;
                        if(treasury_effect > 100)
                            treasury_effect = 100;
                        break;}
                    case -1:{
                        printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
                        int F_choice;
                        scanf("%d" , &F_choice);
                        My_exit(name , head , status , people_effect , court_effect , treasury_effect , F_choice);
                        break;
                    }
                    default: {
                        printf("No valid input");
                        exit(-1);
                    }
                }
                printf("People: %d Court: %d Treasury: %d\n" , people_effect , court_effect , treasury_effect);
                average_effect = (people_effect + court_effect +treasury_effect)/3;
                struct problem * del_node = Check_cnt(head);
                if(del_node!= NULL){
                    head = delete_node(head , del_node);
                }
                if(head == NULL){//Check to see if all the problems were deleted
                    head = create_node();
                    holder = head;
                    rewind(choices);
                    fscanf(choices , "%s" , Address_prob);//Getting c[i].txt from CHOICES.txt
                    FILE * Main_File;
                    Main_File = fopen(Address_prob , "r");
                    if(Main_File == NULL){
                        printf("No file found");
                    }
                    Linked_info(holder , Main_File);
                    Add_end(head , create_node());//Add another node to the end for next problem
                    while (holder->next != NULL){//Change holder to the last node
                        holder = holder->next;
                    }
                }
            }
            status = 0;
            printf("You have lost!\n");
            printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
            int F_choice;
            scanf("%d" , &F_choice);
            My_exit(name , head , status , people_effect , court_effect , treasury_effect , F_choice);
        } else{//if the player has lost and saved the game
            int people_effect = 50;
            int court_effect = 50;
            int treasury_effect = 50;
            double average_effect = 50;
            struct problem *head;
            head = create_node();
            struct problem* holder;
            holder = head;
            for (int i = 1; !feof(choices) ; ++i) {
                fscanf(choices , "%s" , Address_prob);//Getting c[i].txt from CHOICES.txt
                FILE * Main_File;
                Main_File = fopen(Address_prob , "r");
                if(Main_File == NULL){
                    printf("No file found");
                }
                Linked_info(holder ,Main_File);
                Add_end(head , create_node());//Add another node to the end for next problem
                while (holder->next != NULL){//Change holder to the last node
                    holder = holder->next;
                }
                fclose(Main_File);
            }
            while (average_effect>10 && people_effect > 0 && court_effect > 0 && treasury_effect > 0){//showing the problems
                struct problem *prob_rand = NULL;
                prob_rand = Linked_Rand(head);
                (prob_rand->possibility)--;
                printf("%s\n" , prob_rand->prob_def);
                printf("[1]%s\n" , prob_rand->first_dec);
                printf("[2]%s\n" , prob_rand->second_dec);
                int dec;
                scanf("%d" , &dec);//Get the dec for problem
                switch (dec) {
                    case 1:{
                        people_effect += prob_rand->fp;
                        if (people_effect > 100)
                            people_effect = 100;
                        court_effect += prob_rand->fc;
                        if (court_effect > 100)
                            court_effect = 100;
                        treasury_effect += prob_rand->ft;
                        if(treasury_effect > 100)
                            treasury_effect = 100;
                        break;}
                    case 2:{
                        people_effect += prob_rand->sp;
                        if (people_effect > 100)
                            people_effect = 100;
                        court_effect += prob_rand->sc;
                        if (court_effect > 100)
                            court_effect = 100;
                        treasury_effect += prob_rand->st;
                        if(treasury_effect > 100)
                            treasury_effect = 100;
                        break;}
                    case -1:{
                        printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
                        int F_choice;
                        scanf("%d" , &F_choice);
                        My_exit(name , head , status , people_effect , court_effect , treasury_effect , F_choice);
                        break;
                    }
                    default: {
                        printf("No valid input");
                        exit(-1);
                    }
                }
                printf("People: %d Court: %d Treasury: %d\n" , people_effect , court_effect , treasury_effect);
                average_effect = (people_effect + court_effect +treasury_effect)/3;
                struct problem * del_node = Check_cnt(head);
                if(del_node!= NULL){
                    head = delete_node(head , del_node);
                }
                if(head == NULL){//Check to see if all the problems were deleted
                    head = create_node();
                    holder = head;
                    rewind(choices);
                    fscanf(choices , "%s" , Address_prob);//Getting c[i].txt from CHOICES.txt
                    FILE * Main_File;
                    Main_File = fopen(Address_prob , "r");
                    if(Main_File == NULL){
                        printf("No file found");
                    }
                    Linked_info(holder , Main_File);
                    Add_end(head , create_node());//Add another node to the end for next problem
                    while (holder->next != NULL){//Change holder to the last node
                        holder = holder->next;
                    }
                }
            }
            status = 0;
            printf("You have lost!\n");
            printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
            int F_choice;
            scanf("%d" , &F_choice);
            My_exit(name , head , status , people_effect , court_effect , treasury_effect , F_choice);
        }
    } else if(strchoose == -1){
        exit(-1);
    }
    else{
        printf("Inavalid number");
        exit(-1);
    }


    return 0;
}