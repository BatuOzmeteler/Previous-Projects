# -*- coding: utf-8 -*-
from datetime import datetime

class Log:
    def __init__(self, timestamp = "-1", string_param = "-1", line_count = -1):
    
      self.timestamp = timestamp;
      self.string_param = string_param;
      self.line_count = line_count;
  
    def log2str(self):
       return str(" " + datetime.strftime(self.timestamp, "%Y-%m-%d %H:%M:%S") + " " + self.string_param + " Line: " + str(self.line_count))
     
def cleanLine(line):
    index = line.find(':') + 2
    line = line[index:].replace('\n', '')
    return line

def cleanTime(line):
    index = line.find(':') + 2
    timestamp = line[index:].replace('\n', '')
    return datetime.strptime(timestamp, "%Y-%m-%d %H:%M:%S")
    
def main():
    
    log_params = ["[TIME        ]:", "[StringPara  ]:"]
    log2check = input("Enter the path of the log file to check: ")
    keywords = input("Enter the keywords (separated with a comma) to look for in the file (press enter for default keywords): ")
    casual_log = input("Enter the path of the casual log file: ")
    time_interval = input("Enter time interval (separated with a comma) in format(YEAR-MONTH-DAY HOUR:MINUTE:SECOND)(press enter if not specified): ")
    
    default_keywords = "error,Error,ERROR,ERR,fail,Fail,FAIL,null,Null,NULL"
    
    if keywords == "":
        keywords = default_keywords
    
    if time_interval == "":
        time_interval = "1900-01-01 01:01:01,2100-01-01 01:01:01"
        
    time_min = datetime.strptime(time_interval.split(',')[0] , "%Y-%m-%d %H:%M:%S")
    time_max = datetime.strptime(time_interval.split(',')[1] , "%Y-%m-%d %H:%M:%S")
    
    keyword_list = keywords.replace(' ', '').split(',')
    
    log_list = []
    count = 0
    
    with open(log2check, encoding = 'latin-1', mode = 'r') as f:
        for line in f:
            count += 1
            if "=" in line: 
                log = Log();
            else:
                if log_params[0] in line:
                    log.timestamp = cleanTime(line);
                elif log_params[1] in line:
                    if (max(log.timestamp, time_min) == log.timestamp) and (max(log.timestamp,time_max) == time_max):
                        if any(keyword in line for keyword in keyword_list):
                            log.string_param = cleanLine(line);
                            log.line_count = str(count)
                            flag = True;
                            for item in log_list:
                                if item.string_param == log.string_param:
                                    flag = False;
                            if flag:
                                log_list.append(log)
                        
    f.close()     

    with open(casual_log, encoding = 'latin-1', mode = 'r') as c:    
        for line in c:
            if log_params[1] in line:
                for x in log_list:
                    if x.string_param in cleanLine(line):
                        log_list.remove(x)
                    
    c.close()

    filename = "LOG---" + datetime.strftime(datetime.now(), "%Y-%m-%d %H_%M_%S") + ".txt"
    file = open(filename, "a")
    for y in log_list:
        file.write(y.log2str() + "\n")
        
    file.close()           
    
if __name__ == "__main__":
    main()