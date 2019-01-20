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
    int Q_num;
    struct problem *next;
};
struct Problem_Score{//struct for score_board
    char name[20];
    int people;
    int court;
    int treasury;
};
void swap(struct Problem_Score *xp, struct Problem_Score *yp)
{
    struct Problem_Score temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(struct Problem_Score arr[], int n) {

    int i, j;
    for (i = 0; i < n - 1; i++) {
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j].people < arr[j + 1].people) {
                swap(&(arr[j]), &(arr[j + 1]));
            } else if (arr[j].people == arr[j + 1].people) {
                if (arr[j].court < arr[j + 1].court) {
                    swap(&(arr[j]), &(arr[j + 1]));
                } else if (arr[j].court == arr[j + 1].court) {
                    if (arr[j].court < arr[j + 1].court) {
                        swap(&(arr[j]), &(arr[j + 1]));
                    }
                }
            }
        }
    }
}
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
void Game_save(char *user_name , struct problem *head,int *remain , int status , int people , int court , int treasure){
    FILE *save;
    char Add[strlen(user_name) + 5];
    strcpy(Add , user_name);
    char plus[] = ".bin";
    strcat(Add , plus);
    save = fopen(Add , "wb+");
    FILE *name_save;
    name_save = fopen("name_save.bin" ,"ab");
    fwrite(user_name , sizeof(char *) , 1 ,name_save);
    fwrite(user_name , sizeof(char *) , 1 , save);
    fwrite(&status , sizeof(int) , 1 ,save);
    fwrite(remain , sizeof(int *) , 1 ,save);
    fwrite(&people , sizeof(int) , 1 , save);
    fwrite(&court , sizeof(int) , 1 , save);
    fwrite(&treasure , sizeof(int) , 1 ,save);
    fclose(name_save);
    fclose(save);
}
void My_exit(char *user_name , struct problem *head,int * Arr_remain , int status , int people , int court , int treasure , char F_choice){
    if(F_choice == '1'){
        Game_save(user_name , head, Arr_remain ,  status ,  people , court , treasure);
        printf("The game was saved.");
        exit(-1);
    } else if(F_choice == '2'){
        exit(-1);
    } else{
        printf("No valid input.\n");
        exit(-1);
    }
}
void score_board(void){
    FILE * score;
    if(fopen("name_save.bin" , "rb") == NULL){
        printf("There is no saved play to score!!!\n");
        return;
    }
    score = fopen("name_save.bin" , "rb");
    int i = 1;
    struct Problem_Score  P_score[100];
    while (!feof(score)){
        char Add[25];
        fread(Add , sizeof(char *) , 1 ,score);
        strcat(Add , ".bin");
        FILE * Player_score = fopen(Add , "rb");
        if(Player_score == NULL){
            break;
        }
        fread((P_score[i-1].name) , sizeof(char *) , 1 , Player_score);
        fseek(Player_score , sizeof(int) , SEEK_CUR);
        fseek(Player_score , sizeof(int *) , SEEK_CUR);
        fread(&(P_score[i-1].people) , sizeof(int) , 1 , Player_score);
        fread(&(P_score[i-1].court) , sizeof(int) , 1 , Player_score);
        fread(&(P_score[i-1].treasury) , sizeof(int) , 1 ,Player_score);
        i++;
        fclose(Player_score);
    }
    bubbleSort(P_score , (--i));
    int k = 1;
    for (int j = 0; j < i; ++j) {
        printf("%d- %s\n" , k , P_score[j].name);
        k++;
    }
}
void file_maker(void){
    printf("Please enter the name of your file");
}
int main() {
    char name[20];
    char Address_prob[20];
    int status = 1;
    FILE *choices;
    choices = fopen("CHOICES.txt" , "r");
    printf("Please enter your name:");
    gets(name);
    printf("Please choose :\n");
    printf("[1]Start a new game\n");
    printf("[2]Load saved game\n");
    printf("[3]Show the scoreboard\n");
    printf("In any part of the game enter -1 to exit the game\n");
    char strchoose;
    int *Arr_remain;
    while(1) {
        scanf(" %c", &strchoose);
        if(choices == NULL && strchoose != 4 ){
            printf("Couldn't Find The Valid Files\n");
            exit(-1);
        }
        if (strchoose == '1') {
            int people_effect = 50;
            int court_effect = 50;
            int treasury_effect = 50;
            double average_effect = 50;
            struct problem *head;
            head = create_node();
            struct problem *holder;
            holder = head;
            int i;
            for (i = 1; !feof(choices); ++i) {
                fscanf(choices, "%s", Address_prob);//Getting c[i].txt from CHOICES.txt
                FILE *Main_File;
                Main_File = fopen(Address_prob, "r");
                if (Main_File == NULL) {
                    printf("No file found");
                }
                Linked_info(holder, Main_File);
                holder->Q_num = i;
                Add_end(head, create_node());//Add another node to the end for next problem
                while (holder->next != NULL) {//Change holder to the last node
                    holder = holder->next;
                }
                fclose(Main_File);
            }
            Arr_remain = (int *) malloc(sizeof(int) * (--i));
            struct problem *temp;
            temp = head;
            for (int j = 0; temp != NULL; ++j) {
                Arr_remain[j] = temp->possibility;
                temp = temp->next;
            }

            while (average_effect > 10 && people_effect > 0 && court_effect > 0 &&
                   treasury_effect > 0) {//showing the problems
                struct problem *prob_rand = NULL;
                prob_rand = Linked_Rand(head);
                (prob_rand->possibility)--;
                Arr_remain[(prob_rand->Q_num) - 1] = prob_rand->possibility;
                char dec = '0';
                while(!((dec == '1') || (dec == '2') || (dec == '-'))) {
                    printf("%s\n", prob_rand->prob_def);
                    printf("[1]%s\n", prob_rand->first_dec);
                    printf("[2]%s\n", prob_rand->second_dec);
                    scanf(" %c", &dec);//Get the dec for problem
                }
                char F_choice[2];
                switch (dec) {
                    case '1': {
                        people_effect += prob_rand->fp;
                        if (people_effect > 100)
                            people_effect = 100;
                        court_effect += prob_rand->fc;
                        if (court_effect > 100)
                            court_effect = 100;
                        treasury_effect += prob_rand->ft;
                        if (treasury_effect > 100)
                            treasury_effect = 100;
                        break;
                    }
                    case '2': {
                        people_effect += prob_rand->sp;
                        if (people_effect > 100)
                            people_effect = 100;
                        court_effect += prob_rand->sc;
                        if (court_effect > 100)
                            court_effect = 100;
                        treasury_effect += prob_rand->st;
                        if (treasury_effect > 100)
                            treasury_effect = 100;
                        break;
                    }
                    case '-': {
                        printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
                        getchar();
                        while (1) {
                            scanf("%s" , F_choice);
                            if (F_choice[0] == '1') {
                                My_exit(name, head, Arr_remain, status, people_effect, court_effect, treasury_effect,
                                        F_choice[0]);
                                printf("The game was saved.");
                                break;
                            } else if(F_choice[0] == '2' || F_choice[0] == '-'){
                                exit(-1);
                            } else if(!(F_choice[0] == '2' || F_choice[0] == '-')){
                                printf("No valid input\n");
                            }
                        }
                    }
                    default: {
                        printf("No valid input\n");
                    }
                }
                printf("People: %d Court: %d Treasury: %d\n", people_effect, court_effect, treasury_effect);
                average_effect = (people_effect + court_effect + treasury_effect) / 3;
                struct problem *del_node = Check_cnt(head);
                if (del_node != NULL) {
                    head = delete_node(head, del_node);
                }
                if (head == NULL) {//Check to see if all the problems were deleted
                    head = create_node();
                    holder = head;
                    rewind(choices);
                    fscanf(choices, "%s", Address_prob);//Getting c[i].txt from CHOICES.txt
                    FILE *Main_File;
                    Main_File = fopen(Address_prob, "r");
                    if (Main_File == NULL) {
                        printf("No file found");
                    }
                    Linked_info(holder, Main_File);
                    Add_end(head, create_node());//Add another node to the end for next problem
                    while (holder->next != NULL) {//Change holder to the last node
                        holder = holder->next;
                    }
                }
            }
            status = 0;
            printf("You have lost!\n");
            printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
            char F_choice[2];
            getchar();
                while (1) {
                    scanf("%s" , F_choice);
                    if (F_choice[0] == '1') {
                        My_exit(name, head, Arr_remain, status, people_effect, court_effect, treasury_effect,
                                F_choice[0]);
                        printf("The game was saved.");
                        break;
                    } else if(F_choice[0] == '2' || F_choice[0] == '-'){
                        exit(-1);
                    } else if(!(F_choice[0] == '2' || F_choice[0] == '-')){
                        printf("No valid input\n");
                    }
                }
        } else if (strchoose == '2') {//Loading the game from the last saved file
            int people_effect;
            int court_effect;
            int treasury_effect;
            FILE *Load;
            char Add[strlen(name) + 5];
            strcpy(Add, name);
            char plus[] = ".bin";
            strcat(Add, plus);
            /*if(fopen(Add , "rb")==NULL){
                printf("There is no saved game.");
                exit(-1);
            }*/
            Load = fopen(Add, "rb+");
            if (Load == NULL) {
                printf("There is no saved game.");
                exit(-1);
            }
            int Arr_remain[50];
            fread(name, sizeof(char *), 1, Load);//Fetching info from the saved file
            fread(&status, sizeof(int), 1, Load);
            fread(Arr_remain, sizeof(int *), 1, Load);
            fread(&people_effect, sizeof(int), 1, Load);
            fread(&court_effect, sizeof(int), 1, Load);
            fread(&treasury_effect, sizeof(int), 1, Load);
            if (status == 1) {
                struct problem *head;
                head = create_node();
                struct problem *holder;
                holder = head;
                for (int i = 1; !feof(choices); ++i) {
                    fscanf(choices, "%s", Address_prob);//Getting c[i].txt from CHOICES.txt
                    FILE *Main_File;
                    Main_File = fopen(Address_prob, "r");
                    if (Main_File == NULL) {
                        printf("No file found");
                    }
                    Linked_info(holder, Main_File);
                    holder->possibility = Arr_remain[i - 1];
                    Add_end(head, create_node());//Add another node to the end for next problem
                    while (holder->next != NULL) {//Change holder to the last node
                        holder = holder->next;
                    }
                    fclose(Main_File);
                }
                struct problem *del_node = Check_cnt(head);
                if (del_node != NULL) {
                    head = delete_node(head, del_node);
                }
                printf("People: %d Court: %d Treasury: %d\n", people_effect, court_effect, treasury_effect);
                int average_effect = (people_effect + court_effect + treasury_effect) / 3;
                while (average_effect > 10 && people_effect > 0 && court_effect > 0 &&
                       treasury_effect > 0) {//showing the problems
                    struct problem *prob_rand = NULL;
                    prob_rand = Linked_Rand(head);
                    (prob_rand->possibility)--;
                    struct problem *temp;
                    temp = head;
                    for (int j = 0; temp != NULL; ++j) {
                        Arr_remain[j] = temp->possibility;
                        temp = temp->next;
                    }
                    char dec = '0';
                    while (!((dec == '1') || (dec == '2') || (dec == '-'))) {
                        printf("%s\n", prob_rand->prob_def);
                        printf("[1]%s\n", prob_rand->first_dec);
                        printf("[2]%s\n", prob_rand->second_dec);
                        scanf(" %c", &dec);//Get the dec for problem
                    }
                    switch (dec) {
                        case '1': {
                            people_effect += prob_rand->fp;
                            if (people_effect > 100)
                                people_effect = 100;
                            court_effect += prob_rand->fc;
                            if (court_effect > 100)
                                court_effect = 100;
                            treasury_effect += prob_rand->ft;
                            if (treasury_effect > 100)
                                treasury_effect = 100;
                            break;
                        }
                        case '2': {
                            people_effect += prob_rand->sp;
                            if (people_effect > 100)
                                people_effect = 100;
                            court_effect += prob_rand->sc;
                            if (court_effect > 100)
                                court_effect = 100;
                            treasury_effect += prob_rand->st;
                            if (treasury_effect > 100)
                                treasury_effect = 100;
                            break;
                        }
                        case '-': {
                            printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
                            char F_choice[2];
                            getchar();
                            while (1) {
                                scanf("%s" , F_choice);
                                if (F_choice[0] == '1') {
                                    My_exit(name, head, Arr_remain, status, people_effect, court_effect, treasury_effect,
                                            F_choice[0]);
                                    printf("The game was saved.");
                                    break;
                                } else if(F_choice[0] == '2' || F_choice[0] == '-'){
                                    exit(-1);
                                } else if(!(F_choice[0] == '2' || F_choice[0] == '-')){
                                    printf("No valid input\n");
                                }
                            }
                        }
                        default: {
                            printf("No valid input\n");
                            exit(-1);
                        }
                    }
                    printf("People: %d Court: %d Treasury: %d\n", people_effect, court_effect, treasury_effect);
                    average_effect = (people_effect + court_effect + treasury_effect) / 3;
                    struct problem *del_node = Check_cnt(head);
                    if (del_node != NULL) {
                        head = delete_node(head, del_node);
                    }
                    if (head == NULL) {//Check to see if all the problems were deleted
                        head = create_node();
                        holder = head;
                        rewind(choices);
                        fscanf(choices, "%s", Address_prob);//Getting c[i].txt from CHOICES.txt
                        FILE *Main_File;
                        Main_File = fopen(Address_prob, "r");
                        if (Main_File == NULL) {
                            printf("No file found");
                        }
                        Linked_info(holder, Main_File);
                        Add_end(head, create_node());//Add another node to the end for next problem
                        while (holder->next != NULL) {//Change holder to the last node
                            holder = holder->next;
                        }
                    }
                }
                status = 0;
                printf("You have lost!\n");
                printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
                char F_choice[2];
            getchar();
            while (1) {
                scanf("%s" , F_choice);
                if (F_choice[0] == '1') {
                    My_exit(name, head, Arr_remain, status, people_effect, court_effect, treasury_effect,
                            F_choice[0]);
                    printf("The game was saved.");
                    break;
                } else if(F_choice[0] == '2' || F_choice[0] == '-'){
                    exit(-1);
                } else if(!(F_choice[0] == '2' || F_choice[0] == '-')){
                    printf("No valid input\n");
                }
                }
            } else {//if the player has lost and saved the game
                int people_effect = 50;
                int court_effect = 50;
                int treasury_effect = 50;
                double average_effect = 50;
                int *Arr_remain;
                struct problem *head;
                head = create_node();
                struct problem *holder;
                holder = head;
                int i;
                for (i = 1; !feof(choices); ++i) {
                    fscanf(choices, "%s", Address_prob);//Getting c[i].txt from CHOICES.txt
                    FILE *Main_File;
                    Main_File = fopen(Address_prob, "r");
                    if (Main_File == NULL) {
                        printf("No file found");
                    }
                    Linked_info(holder, Main_File);
                    Add_end(head, create_node());//Add another node to the end for next problem
                    while (holder->next != NULL) {//Change holder to the last node
                        holder = holder->next;
                    }
                    fclose(Main_File);
                }
                Arr_remain = (int *) malloc(sizeof(int) * (--i));
                struct problem *temp;
                temp = head;
                for (int j = 0; temp != NULL; ++j) {
                    Arr_remain[j] = temp->possibility;
                    temp = temp->next;
                }
                while (average_effect > 10 && people_effect > 0 && court_effect > 0 &&
                       treasury_effect > 0) {//showing the problems
                    struct problem *prob_rand = NULL;
                    prob_rand = Linked_Rand(head);
                    (prob_rand->possibility)--;
                    Arr_remain[(prob_rand->Q_num) - 1] = prob_rand->possibility;
                    char dec = '0';
                    while (!((dec == '1') || (dec == '2') || (dec == '-'))) {
                        printf("%s\n", prob_rand->prob_def);
                        printf("[1]%s\n", prob_rand->first_dec);
                        printf("[2]%s\n", prob_rand->second_dec);
                        scanf(" %c", &dec);//Get the dec for problem
                    }
                    switch (dec) {
                        case '1': {
                            people_effect += prob_rand->fp;
                            if (people_effect > 100)
                                people_effect = 100;
                            court_effect += prob_rand->fc;
                            if (court_effect > 100)
                                court_effect = 100;
                            treasury_effect += prob_rand->ft;
                            if (treasury_effect > 100)
                                treasury_effect = 100;
                            break;
                        }
                        case '2': {
                            people_effect += prob_rand->sp;
                            if (people_effect > 100)
                                people_effect = 100;
                            court_effect += prob_rand->sc;
                            if (court_effect > 100)
                                court_effect = 100;
                            treasury_effect += prob_rand->st;
                            if (treasury_effect > 100)
                                treasury_effect = 100;
                            break;
                        }
                        case '-': {
                            printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
                            char F_choice[2];
                            getchar();
                            while (1) {
                                scanf("%s" , F_choice);
                                if (F_choice[0] == '1') {
                                    My_exit(name, head, Arr_remain, status, people_effect, court_effect, treasury_effect,
                                            F_choice[0]);
                                    printf("The game was saved.");
                                    break;
                                } else if(F_choice[0] == '2' || F_choice[0] == '-'){
                                    exit(-1);
                                } else if(!(F_choice[0] == '2' || F_choice[0] == '-')){
                                    printf("No valid input\n");
                                }
                            }
                        }
                        default: {
                            printf("No valid input\n");
                            exit(-1);
                        }
                    }
                    printf("People: %d Court: %d Treasury: %d\n", people_effect, court_effect, treasury_effect);
                    average_effect = (people_effect + court_effect + treasury_effect) / 3;
                    struct problem *del_node = Check_cnt(head);
                    if (del_node != NULL) {
                        head = delete_node(head, del_node);
                    }
                    if (head == NULL) {//Check to see if all the problems were deleted
                        head = create_node();
                        holder = head;
                        rewind(choices);
                        fscanf(choices, "%s", Address_prob);//Getting c[i].txt from CHOICES.txt
                        FILE *Main_File;
                        Main_File = fopen(Address_prob, "r");
                        if (Main_File == NULL) {
                            printf("No file found");
                        }
                        Linked_info(holder, Main_File);
                        Add_end(head, create_node());//Add another node to the end for next problem
                        while (holder->next != NULL) {//Change holder to the last node
                            holder = holder->next;
                        }
                    }
                }
                status = 0;
                printf("You have lost!\n");
                printf("Do you want to save the game?\n[1]Yes,I want to save the game.\n[2]No,exit.\n");
                char F_choice[2];
                getchar();
                while (1) {
                    scanf("%s" , F_choice);
                    if (F_choice[0] == '1') {
                        My_exit(name, head, Arr_remain, status, people_effect, court_effect, treasury_effect,
                                F_choice[0]);
                        printf("The game was saved.");
                        break;
                    } else if(F_choice[0] == '2' || F_choice[0] == '-'){
                        exit(-1);
                    } else if(!(F_choice[0] == '2' || F_choice[0] == '-')){
                        printf("No valid input\n");
                    }
                }
            }
        } else if (strchoose == '3') {
            score_board();
        } else if (strchoose == '-') {
            exit(-1);
        } else {
            printf("Inavalid number\n");
        }
    }

    return 0;
}