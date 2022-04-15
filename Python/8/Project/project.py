# -*- coding: utf-8 -*-
"""
Created on Tue Jun  1 22:10:39 2021

@author: mbatu

DESCRIPTION: This script sends an e-mail with a simple reminder to 
the next e-mail addresses on the e-mail contact list and logs its behavior.      
    
+ emails.txt file contains the e-mails of the contacts.

+ log.txt file contains the time e-mail was sent and 
the receiver's e-mail's index in the e-mails.txt file

***MAKE SURE TO ALLOW THE ACCESS OF LESS SECURE APPS FROM GOOGLE ACCOUNT***
***DISABLE ANY FIREWALL/MAIL SHIELD TO ENABLE COMMUNICATION***

"""

#Import necessary libraries
import smtplib
import os 
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import socket 
from datetime import datetime

#Read the e-mails from the emails.txt file
emails = 'emails.txt'

#Define number of e-mails
size_emails = 0;

with open(emails) as emails_f:
    #Put the emails in an array
    lines = emails_f.readlines();
    for line in lines:
        print("E-mail Address {}: {}".format(size_emails, line.strip()))
        size_emails = size_emails + 1;

#socket.getaddrinfo('localhost', 8080) #CHECK IF PORTS ARE AVAILABLE

print('Number of Emails: ' + str(size_emails));

#Define previous e-mailed index = 0
prevIndex = -1;

#Define how many people to e-mail 
pplCount = 2;

#Read the log.txt file if it doesn't exist generate a new one 
log = 'log.txt'
file_exists = os.path.isfile(log);

if file_exists:
   log_f = open(log, 'r');
    #If the log.txt file isn't empty, find the index of the last line. Assign previous e-mailed index
   if os.stat(log).st_size != 0:
      rows = log_f.readlines();
      size = len(rows)
      count = 0;
      for row in rows:
         if count == size - 1:
             arr = row.split(":");
             prevIndex = int(arr[4]);
         count += 1;
else:
    log_f = open(log, "w"); 
 
#Connect to e-mail server and login to your account
sender_email = "batu.ozmeteler@gmail.com"
sender_password = "B4tu&1996"
gmail_server = "smtp.gmail.com"

smtp = smtplib.SMTP(gmail_server, 587) # 587 for TLS, 465 for SSL, and 25 for SMTP relaying
#print(smtp.set_debuglevel(1)) #DISABLE ANY FIREWALL/MAIL SHIELD (AVAST) TO ENABLE COMMUNICATION
smtp.connect(gmail_server, 587)
#smtp.ehlo()
smtp.starttls() #transport layer security
smtp.ehlo() #identify computer
smtp.login(sender_email, sender_password);

if prevIndex >= size_emails:
    prevIndex = 0;

for i in range(0, pplCount):    
    
    prevIndex = prevIndex + 1;
    if prevIndex >= size_emails:
            prevIndex = 0;
            
    receiver_email = lines[prevIndex]
    
    content = MIMEMultipart(); 
    content["Subject"] = "Share Your Practice!";
    content["From"] = sender_email;
    content["To"] = receiver_email;
    temp = receiver_email.split("@");
    receiver_name = temp[0];
    message = 'Dear ' +  receiver_name +  ',\n\nIt is time to share your best/bad practice.' + \
    ' It is your turn! \n\nHuawei SW Department\n';
    content.attach(MIMEText(message, 'plain'))
    smtp.send_message(content);
    print("E-MAIL SENT!")
    
    with open(log, "w") as log_f:
        now = datetime.now();
        current_time = now.strftime("%H:%M:%S");
       
        next_log = '{}\nTime: {} Index: {}'.format(receiver_email, current_time, prevIndex)
       
        log_f.write(next_log)
     
