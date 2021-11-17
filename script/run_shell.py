import sys
import subprocess

filepath = './draw_and_save.sh'

# f = open('../input/list.txt', 'r')
f = open(sys.argv[1], 'r')

for line in f.readlines() :
    year, month, day, entry, eventid = line.split()
    if int(eventid) == -1 :
        continue
    #print(year, month, day, entry, eventid)
    ientry = int(entry)
    subprocess.call([filepath + " " + str(year) + " " + str(month) + " " + str(day) + " " + str(ientry) + " " + str(eventid)], shell=True)

f.close()
