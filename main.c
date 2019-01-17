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
int linked_num(struct problem * str){//Check the number of remained problems
    int cnt = 0;
    while(str != NULL){
        str = str->next;
        cnt++;
    }
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
int main() {
    char name[20];
    char Address_prob[20];
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
    int strchoose;
    scanf("%d" , &strchoose);
    if(strchoose == 1){
        int problem_count = 0;
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


            problem_count++;
        }

    }else if(strchoose == 2){

    }
    else{
        printf("Inavalid number");
        exit(-1);
    }


    return 0;
}