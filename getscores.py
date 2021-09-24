import requests
import datetime
from datetime import date,timedelta,datetime

#monday1 = (d1 - timedelta(days=d1.weekday()))
#monday2 = (d2 - timedelta(days=d2.weekday()))

import time
api_key = '7647b9fef41b4276a3ea1e73ff057249'
headers = {'Ocp-Apim-Subscription-Key': '{key}'.format(key=api_key)}

def get_todays_scores():
    monday1= datetime(2021,8,30)
    todays_date = datetime.combine(date.today(), datetime.min.time())
    days_since_start_of_week_1 = todays_date - monday1
    #calculate current week number by getting num of days between week1/week18 and dividing by 7. It's an int, so it'll just return a floor value if there's a remainder
    week_no = (days_since_start_of_week_1 / 7).days
    #keys I am interested: home team, away team, home team score, away team score, OverUnder
    relevant_keys = ['Team','Opponent','Score','OpponentScore','OverUnder']
    #open file to write to
    newfile = open('scores.txt','w')
    scores_url = 'https://api.sportsdata.io/v3/nfl/scores/json/TeamGameStats/2021/{week}'.format(week=week_no)
    #url for scores
    #get json object of all the scores, which is a list of json objects
    score_list = (requests.get(url = scores_url , headers = headers)).json()
    #loop through each json object
    for score in score_list[0:2:len(score_list)]:
        #I want the MoneyLine that corresponds to whichever team is winning
        ml_string = ", {} {}".format(score['Team'],score['PointSpread'])

        #filter each json object to contain only the keys I want in relevant_keys and then stringify and write to text file
        
        fixed_dict = {key:value for key,value in score.items() if key in relevant_keys}
        #more API weirdness I had to fix. For some reasons scores were one points too high
        fixed_dict['Score'] = str(int(fixed_dict['Score']) -1)
        fixed_dict['OpponentScore'] = str(int(fixed_dict['OpponentScore']) -1)
        filtered_dict_string = str(fixed_dict)
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
        #recommended to wait 5 minutes between calls to the API
        time.sleep(5.0)
        check_and_update()


def main():
    check_and_update()

if __name__ == 'main':
    main()

main()


