# nfl_scores_and_lines
Program that constantly prints updates in NFL game scores and live lines / spreads to terminal, updated every 30 seconds. Python utilizes HTTP GET method with
SportsData.IO API and writes json object to line separated, comma separated text file which is then parsed by C file, which pretty prints said text file by reading
into struct and sleeps after every score update for 15 seconds before checking if the text file the python script writes out to has been updated
