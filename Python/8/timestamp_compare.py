# -*- coding: utf-8 -*-
from datetime import datetime

def main():
    #str1 = "2020-09-23 15:35:11,2020-09-28 17:35:11"
    #str2 = "2020-09-25 17:35:11"
    
    #t1 = datetime.strptime(str1, "%Y-%m-%d %H:%M:%S")
    #t2 = datetime.strptime(str2, "%Y-%m-%d %H:%M:%S")
    
#    line = "[TIME		]:	2020-09-25 15:35:11"
#    print(cleanTime(line))

    #print(max((t1,t2)))
    
#    time_interval = input("Enter time interval (separated with a comma) in format(YEAR-MONTH-DAY HOUR:MINUTE:SECOND): ")
#    
#    time_min = datetime.strptime(time_interval.split(',')[0] , "%Y-%m-%d %H:%M:%S")
#    time_max = datetime.strptime(time_interval.split(',')[1] , "%Y-%m-%d %H:%M:%S")
    
    batu = ['FAIL', 'FAIL', '3', '4', '1']
    print(batu.count('FAIL'))
    
    
#    print(max((time_min,time_max)))
    
    
def cleanTime(line):
    index = line.find(':') + 1
    timestamp = line[index+1:]
    
    return timestamp;

if __name__ == "__main__":
    main()