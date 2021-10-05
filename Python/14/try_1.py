# -*- coding: utf-8 -*-
import os

def main():

    #URL = '\\10.244.87.96\MBTS_Packages\21A\2020-10-03\2020-10-03-23-17'
    URL = str(input())
    
    final = ""
    for i in range(len(URL)):
        if (URL[i] == '\\') and (i > 1):
            final += (URL[i] + "\\")
        else:
            final += URL[i]    
    
    mylist = final.split('\\')
    
    while('' in mylist) : 
        mylist.remove('')
    print(mylist)
    
    IP = mylist[0]
    
    mylist[0] = "D:"
    
    final = ''
    for i in mylist:
        final += i + "\\"
   
    final = final[:-1]
    
    path = 'C:\stfp'
    if (os.path.exists(path) != True):
        os.mkdir('C:\stfp')
        
    os.chdir(path)
      
    command = "FtsClient.exe get " + IP + "D:\\sftp " + final + "9001" 
    
    os.system('cmd /k %s' %(command))

if __name__ == "__main__":
    main()