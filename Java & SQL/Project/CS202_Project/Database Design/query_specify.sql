# (???) means whatever input
#to check whether this username exists in database

SELECT COUNT(*)
FROM Users
WHERE username = (???)
;

#to print the detailed info for any ap_id 

SELECT *
FROM Appointments
WHERE ap_id = (???)
;

#to print all doctor names 

SELECT fullname, expertise
FROM Doctors natural join Users
;

#to print the name of the doctor for a specific expertise

SELECT fullname
FROM Doctors natural join Users
WHERE expertise = (???)
;


#to print detailed info about a doctors appointments

SELECT *
FROM Appointments
WHERE d_username = (???)
;

#when doctor checks for the availability of the room

SELECT COUNT(*)
FROM Rooms 
WHERE r_id = (???) and stay_until < now()
;

#to print detailed info about a patients appointments

SELECT *
FROM Appointments
WHERE p_username = (???)
;

#to print detailed info about apoointments of a doctor of an expertise

SELECT *
FROM Appointments natural join Doctors
  WHERE expertise = (???)
;


##doctors can list the availability of the rooms in the system and patient info

#doctors list the availability of the rooms
select r_id, (stay_until < now()) from Rooms;

#when he clicks detailed info button for a specific room, patient info is printed
SELECT fullname
FROM Appointments natural join (select fullname, p_username from (Users natural join Patients))
WHERE r_id = (???);
;

#to print patient info for the unavailable rooms
select r_id, ap_id, fullname
from (Rooms natural join Appointments) join (Patients natural join Users) on p_username = username