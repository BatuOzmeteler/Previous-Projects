# -*- coding: utf-8 -*-
from datetime import datetime

def main():
    string = "[LOG][2020/06/12 21:01:03]:[Def,INFO]Read Line middle nBytes:33"
    print(str(findTimestamp(string)))


def findTimestamp(line):
    timestamp = line.split('[')[2]
    timestamp = timestamp[:len(timestamp)-2]
    return datetime.strptime(timestamp, "%Y/%m/%d %H:%M:%S")

def findStringParam(line):
    string_param = ""
    line_array = line.split(':')
    for i in range(3, len(line_array)):
        string_param = string_param + line_array[i]
        if i != len(line_array)-1:
            string_param = string_param + ":"
     
    return string_param   
        
        
 
if __name__ == "__main__":
    main()
