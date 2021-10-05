# -*- coding: utf-8 -*-
"""
Created on Fri Jun  4 18:41:02 2021

DESCRIPTION: This script should be run after making changes to emails.txt
file. This script updates the index of the last e-mail contact

@author: mbatu
"""

import os
import shutil
import sys


def main():

    original = r'emails.txt'
    target = r'backup.txt'

    backup = 'backup.txt'
    emails = 'emails.txt'
    log = 'log.txt'
    
    file_exists_backup = os.path.isfile(backup);
    file_exists_log = os.path.isfile(log);
    
    if file_exists_backup:
        backup_f = open(backup, 'r');
        if os.stat(backup).st_size != 0:
          with open(emails, 'r') as emails_f:    
              same = compareFiles(backup, emails);
              if same == 0:
                  if file_exists_log:
                      with open(log, 'r') as log_r:
                          lines = log_r.readlines();
                          email = lines[0];
                          lastIndex = findLastIndex()
                          newIndex = indexOut(emails, email)
                          if newIndex != -1:
                              if lastIndex != newIndex:
                                  changeLogFile(newIndex)
                          else:
                              backup_lines = backup_f.readlines()
                              size_backup = len(backup_lines);
                              found = False;
                              timer = 0;
                              while found == False:
                                  lastIndex += 1;
                                  if lastIndex >= size_backup:
                                      lastIndex = 0;
                                  new_email = backup_lines[lastIndex];                                      
                                  index_renewed = indexOut(emails, new_email);
                                  if index_renewed != -1:
                                      changeLogFile(index_renewed)
                                      found == True;
                                  timer = timer + 1;
                                  if timer >= size_backup:
                                      break;
                  else:
                      sys.exit("Log file doesn't exist")
        else:
            shutil.copyfile(original, target)
    else:
        shutil.copyfile(original, target)


def changeLogFile(newIndex):
    log = 'log.txt'
    with open(log, 'r') as log_r:
        lines = log_r.readlines();
        lastRow = findLastRow();
        temp = lastRow.split(':');
        tempo = temp[0] + ': ' + str(temp[1]) + ':' + \
            str(temp[2]) + ':' + temp[3] + ': ' + str(newIndex);
        lines[len(lines)-1] = tempo;
        with open(log, 'w') as log_w:
            for row in lines:
                log_w.write(list2str(row))
        log_w.close()
    

def indexOut(doc, keyword):
    with open(doc, 'r') as emails_f:
        emails = emails_f.readlines()
        count = 0;
        index = -1;
        for row in emails:
            if row == keyword:
                index = count;
            count += 1;
    return index

def compareFiles(file1, file2):
    with open(file1, 'r') as f1:
        with open(file2, 'r') as f2:
            lines1 = f1.readlines();
            lines2 = f2.readlines();
            len1 = len(lines1);
            len2 = len(lines2);
            compared = True;
            
            if len1 != len2:
                compared = False;
            else:
                count = 0;
                for line in lines1:
                    if line != lines2[count]:
                        compared = False;
                        break;
                    count += 1;
                    
            return compared
            
def findLastIndex():
    with open('log.txt', 'r') as log_f:
        rows = log_f.readlines();
        size = len(rows)
        count = 0;
        for row in rows:
           if count == size - 1:
               arr = row.split(":");
               return int(arr[4]);
           count += 1;
           
def findLastRow():
    with open('log.txt', 'r') as log_f:
        rows = log_f.readlines();
        size = len(rows)
        count = 0;
        for row in rows:
            if count == size - 1:
                return row;
            count += 1;        

def list2str(elements):
    out = "" 
    for element in elements:
        out += str(element) 
    return out

if __name__ == "__main__":
    main()           
            
            

      

            
            