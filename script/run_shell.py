import sys
import subprocess

filepath = './draw_and_save.sh'

# f = open('../input/list.txt', 'r')
f = open(sys.argv[1], 'r')
skip_line_chunk = int(sys.argv[2])
count = 0
for line in f.readlines() :
    year, month, day, entry, bunch, eventid = line.split()
    if int(eventid) == -1 :
        continue
    count += 1
    if count <= skip_line_chunk * 1000 :
        continue
    if count > (skip_line_chunk + 1) * 1000 :
        break
    #print(year, month, day, entry, bunch, eventid)
    ientry = int(entry)
    subprocess.call([filepath + " " + str(year) + " " + str(month) + " " + str(day) + " " + str(ientry) + " " + str(bunch) + " " + str(eventid)], shell=True)

f.close()
