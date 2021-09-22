#include "nflscores.h"
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>


#define MAX_NFL_GAMES 16

#define MAXBUFLEN 1000

void get_away_team(Score *s1, char *string);
void get_home_team(Score *s1, char *string);
void get_away_score(Score *s1, char *string);
void get_home_score(Score *s1, char *string);
void get_over_under(Score *s1, char *string);
void get_moneyline(Score *s1,char*string);
void print_score_info(const Score s1);
char *remove_if_in(char remove,char *string);
int check_if_file_modified(const char *path,time_t previous_time);


int main(){
    struct timeval timeout;
    struct stat file_info;
    timeout.tv_sec = 5;
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

    while (1){
        fp = fopen(file_name,"r");
        if (fp == NULL){
            perror("Error occurred\n");
            printf("This error occured: %d\n",errno);
            exit(-1);
        }
        int err2 = stat(file_name,&file_info);
        if (file_info.st_mtime - old_modification_time == 0){
            select(NULL,NULL,NULL,NULL,&timeout);
            fclose(fp);
        }
        else{
            while (fgets(line,MAXBUFLEN,fp)){
                strncpy(buffer[i],line,MAXBUFLEN);
                populate_into_struct(&s1,buffer[i]);
                print_score_info(s1);
            }
            timeout.tv_sec = 15;
            select(NULL,NULL,NULL,NULL,&timeout);
            printf("Updating Score and Lines...\n");
            i=0;
            fclose(fp);
        }
    }
}
void populate_into_struct(Score *s1,char *string){
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

void get_away_team(Score *s1,char *string){
    char *away_team = strtok(string, ",");
    away_team = strtok(away_team," ");
    away_team = strtok(NULL," ");
    s1->away_team = away_team;
}

void get_home_team(Score *s1,char *string){
    string = strtok(string, ",");
    char *home_team = strtok(NULL,",");
    home_team = strtok(home_team," ");
    home_team = strtok(NULL," ");
    s1->home_team = home_team;

}

void get_away_score(Score *s1,char *string){
    string = strtok(string, ",");
    string = strtok(NULL,",");
    string =strtok(NULL,",");
    char *away_score = string;
    away_score = strtok(away_score," ");
    away_score = strtok(NULL," ");
    int away_points = atoi(away_score);
    s1->away_score = away_points;

    
}

void get_home_score(Score *s1,char *string){
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

void get_over_under(Score *s1,char *string){
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

void get_moneyline(Score *s1,char *string){
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

int check_if_modified(const char *path, time_t previous_time){
    struct stat file_stat;
    int err = stat(path,&file_stat);
    if (err!=0)
        perror("Something went wrong getting file info for scores.txt");
        exit(-1);

    return file_stat.st_mtime - previous_time;
}


