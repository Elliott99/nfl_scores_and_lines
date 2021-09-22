#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>

//each score struct contains the home and away teams and scores, O/U and Moneyline for team currently winning
typedef struct score{
    char *home_team;
    char *away_team;
    uint32_t home_score;
    uint32_t away_score;
    double over_under;
    char *moneyline;
}Score;

void update_home_score(Score *new_score);
void update_away_score(Score *away_score);
void update_over_under(Score *new_over);
void populate_into_struct(Score *s1,char *string);


