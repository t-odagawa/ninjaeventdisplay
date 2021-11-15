import subprocess

filepath = './draw_and_save.sh'

# f = open('../input/list.txt', 'r')
f = open('../input/sharing_file_connected_result_1.txt', 'r')

for line in f.readlines() :
    year, month, day, entry, eventid = line.split()
    print(year, month, day, entry, eventid)
    ientry = int(entry)
    subprocess.call([filepath + " " + str(year) + " " + str(month) + " " + str(day) + " " + str(ientry) + " " + str(eventid)], shell=True)

f.close()
