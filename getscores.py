import requests
import datetime
import time
api_key = '7647b9fef41b4276a3ea1e73ff057249'
headers = {'Ocp-Apim-Subscription-Key': '{key}'.format(key=api_key)}

def get_todays_scores():
    relevant_keys = ['HomeTeam','AwayTeam','HomeScore','AwayScore','OverUnder']
    newfile = open('scores.txt','w')
    scores_url = 'https://api.sportsdata.io/v3/nfl/scores/json/ScoresByDate/{date}'.format(date='2021-09-19')
    score_list = (requests.get(url = scores_url , headers = headers)).json()
    for score in score_list:
        if score['HomeScore'] > score['AwayScore']:
            ml_string = ", {} {}".format(score['HomeTeam'],score['PointSpreadHomeTeamMoneyLine'])
        elif score['HomeScore'] < score['AwayScore']:
            ml_string = ", {} {}".format(score['AwayTeam'],score['PointSpreadAwayTeamMoneyLine'])


        filtered_dict_string = str({key:value for key,value in score.items() if key in relevant_keys})
        filtered_dict_string+=ml_string
        filtered_dict_string = filtered_dict_string.replace('}',"")
        filtered_dict_string = filtered_dict_string.replace('{',"")
        filtered_dict_string = filtered_dict_string.replace("'","")
        newfile.write(filtered_dict_string)
        newfile.write('\n')
# my_dict = {key:value for (key,value) in score_dict.items() if key in relevant_keys}
    return score_list


def check_in_progress():
    in_progress_url ='https://api.sportsdata.io/v3/nfl/scores/json/AreAnyGamesInProgress'
    json_in_progress = requests.get(url = in_progress_url, headers = headers).json()
    if (json_in_progress is True):
        return True
    else:
        return False

def main():
    #if not (check_in_progress()):
     #   print("There are no games in progress")
    #else:
        score_dict = get_todays_scores()
        print(score_dict)
        time.sleep(5.0)
        main()

if __name__ == 'main':
    main()

main()


