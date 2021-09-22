#include "nflscores.h"
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>

//maximum 16 games could be on at any one time
#define MAX_NFL_GAMES 16

//maximum string length of game score summary
#define MAXBUFLEN 1000

void set_away_team(Score *s1, char *string);
void set_home_team(Score *s1, char *string);
void set_away_score(Score *s1, char *string);
void set_home_score(Score *s1, char *string);
void set_over_under(Score *s1, char *string);
void set_moneyline(Score *s1,char*string);
void print_score_info(const Score s1);


int main(){
    struct timeval timeout;
    struct stat file_info;
    timeout.tv_sec = 5;
    //buffer is a 2-d array of chars. Buffer[0] = the string corresponding to the first game, Buffer[1] = string corresponding to game 2, etc
    char buffer[MAX_NFL_GAMES][MAXBUFLEN];
    int i=0;
    Score s1;
    FILE *fp;
    char line[MAXBUFLEN];
    char *file_name = "scores.txt";
    time_t old_modification_time;
    int err = stat(file_name,&file_info);
    if (err!=0){
        perror("Something went wrong with getting file info for scores.txt file");
        exit(-1);
    }
    else {   
         old_modification_time = file_info.st_mtime;
        }
     fp = fopen(file_name,"r");
    if (fp == NULL){
        perror("Error occurred\n");
        printf("This error occured: %d\n",errno);
        exit(-1);
    }

    //before entering the loop, save the last modification time of the scores.txt file using stat()
    while (1){
        int err2 = stat(file_name,&file_info);
        if (err2 != 0){
            perror("Something wrong with getting file info for sores.txt file after update");
            exit(-1);
        }
        //if the modification time before we entered the loop is the same as after, it means that we have no changes
        //we conitnue to sleep() using select and wait for something to change within the file
        if (file_info.st_mtime - old_modification_time == 0){
            select(NULL,NULL,NULL,NULL,&timeout);
        }
        else{
            //each line is a score summary
            while (fgets(line,MAXBUFLEN,fp)){
                //copy each line into the string buffer
                strncpy(buffer[i],line,MAXBUFLEN);
                //populate into structs and then call pretty print struct to terminal
                populate_into_struct(&s1,buffer[i]);
                print_score_info(s1);
            }
            //timeout 15 seconds before updating again
            timeout.tv_sec = 15;
            //select() to timeout 15 seconds before updating the scoreboard
            select(NULL,NULL,NULL,NULL,&timeout);
            printf("Updating Score and Lines...\n");
            i=0;
            //rewind to beginning of the file since the old scores/text file will be overwritten
            rewind(fp);
        }
        fclose(fp);
    }
}
//populate_into_struct parses the string that represents the score summary and places the score into the struct
void populate_into_struct(Score *s1,char *string){
    //save the original string aentence argument, and still be able to parse it use it using strtok, by copying into the string_buff argument
    //after every call to a "get" method
    char string_buff[MAXBUFLEN];
    strncpy(string_buff,string,MAXBUFLEN);
    get_away_team(s1,string_buff);
    strncpy(string_buff,string,MAXBUFLEN);
    get_home_team(s1,string_buff);
    strncpy(string_buff,string,MAXBUFLEN);
    get_away_score(s1,string_buff);
    strncpy(string_buff,string,MAXBUFLEN);
    get_home_score(s1,string_buff);
    strncpy(string_buff,string,MAXBUFLEN);
    get_over_under(s1,string_buff);
    strncpy(string_buff,string,MAXBUFLEN);
    get_moneyline(s1,string_buff);
}

//each get method makes use of the strtok() method to split the score summary sentence and place it into the struct passed in as the argument


void set_away_team(Score *s1,char *string){
    char *away_team = strtok(string, ",");
    away_team = strtok(away_team," ");
    away_team = strtok(NULL," ");
    s1->away_team = away_team;
}

void set_home_team(Score *s1,char *string){
    string = strtok(string, ",");
    char *home_team = strtok(NULL,",");
    home_team = strtok(home_team," ");
    home_team = strtok(NULL," ");
    s1->home_team = home_team;

}

void set_away_score(Score *s1,char *string){
    string = strtok(string, ",");
    string = strtok(NULL,",");
    string =strtok(NULL,",");
    char *away_score = string;
    away_score = strtok(away_score," ");
    away_score = strtok(NULL," ");
    int away_points = atoi(away_score);
    s1->away_score = away_points;

    
}

void set_home_score(Score *s1,char *string){
    string = strtok(string, ",");
    string = strtok(NULL,",");
    string =strtok(NULL,",");
    string = strtok(NULL, ",");
    char *home_score = string;
    home_score = strtok(home_score, " ");
    home_score = strtok(NULL, " ");
    int home_points = atoi(home_score);
    s1->home_score = home_points;
}

void set_over_under(Score *s1,char *string){
    string = strtok(string, ",");
    string = strtok(NULL,",");
    string = strtok(NULL,",");
    string = strtok(NULL, ",");
    string = strtok(NULL,",");
    char *over_under = string;
    over_under = strtok(over_under, " ");
    over_under = strtok(NULL, " ");
    double over_under_points = strtod(over_under,'\0');
    s1->over_under = over_under_points;
}

void set_moneyline(Score *s1,char *string){
    string = strtok(string, ",");
    string = strtok(NULL,",");
    string = strtok(NULL,",");
    string = strtok(NULL, ",");
    string = strtok(NULL,",");
    string = strtok(NULL,",");
    char *ml = string;
    s1->moneyline = ml;
}


void print_score_info(const Score s1){
    printf("%s: %d %s: %d O/U: %f ML: %s",s1.away_team,s1.away_score,s1.home_team,s1.home_score,s1.over_under,s1.moneyline);
}

