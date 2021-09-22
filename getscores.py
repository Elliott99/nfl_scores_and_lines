import requests
from datetime import date
import time
api_key = '7647b9fef41b4276a3ea1e73ff057249'
headers = {'Ocp-Apim-Subscription-Key': '{key}'.format(key=api_key)}

def get_todays_scores():
    #keys I am interested: home team, away team, home team score, away team score, OverUnder
    relevant_keys = ['HomeTeam','AwayTeam','HomeScore','AwayScore','OverUnder']
    #open file to write to
    newfile = open('scores.txt','w')
    td = date.today()
    #url for scores
    scores_url = 'https://api.sportsdata.io/v3/nfl/scores/json/ScoresByDate/{date}'.format(date=td)
    #get json object of all the scores, which is a list of json objects
    score_list = (requests.get(url = scores_url , headers = headers)).json()
    #loop through each json object
    for score in score_list:
        #I want the MoneyLine that corresponds to whichever team is winning
        if score['HomeScore'] > score['AwayScore']:
            ml_string = ", {} {}".format(score['HomeTeam'],score['PointSpreadHomeTeamMoneyLine'])
        elif score['HomeScore'] < score['AwayScore']:
            ml_string = ", {} {}".format(score['AwayTeam'],score['PointSpreadAwayTeamMoneyLine'])

        #filter each json object to contain only the keys I want in relevant_keys and then stringify and write to text file
        filtered_dict_string = str({key:value for key,value in score.items() if key in relevant_keys})
        #append the moneyline string generated in previous if/else condition
        filtered_dict_string+=ml_string
        filtered_dict_string = filtered_dict_string.replace('}',"")
        filtered_dict_string = filtered_dict_string.replace('{',"")
        filtered_dict_string = filtered_dict_string.replace("'","")
        newfile.write(filtered_dict_string)
        newfile.write('\n')
    return score_list


def check_in_progress():
    in_progress_url ='https://api.sportsdata.io/v3/nfl/scores/json/AreAnyGamesInProgress'
    json_in_progress = requests.get(url = in_progress_url, headers = headers).json()
    if (json_in_progress is True):
        return True
    else:
        return False

#check if games are in progres. If they are, continously update them by recursively calling self every 5 seconds to get scores
#if not just print "No games in progress" and exit
def check_and_update():
    if not (check_in_progress()):
        print("No games in progress")
        exit()
    else:
        score_dict = get_todays_scores()
        print(score_dict)
        time.sleep(5.0)
        check_and_update()


def main():
    check_and_update()

if __name__ == 'main':
    main()

main()


