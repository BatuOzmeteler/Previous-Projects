drop view if exists ListDoctors;
drop view if exists DoctorAppointmentList;
drop view if exists PatientAppointmentList;

create view ListDoctors as
select fullname, expertise
from Users natural join Doctors;

create view DoctorAppointmentList as 
select d_username, fullname, expertise, d_room, ap_slot
from (Doctors join Appointments on username = d_username) join Users on Users.username = d_username;

create view PatientAppointmentList as
select p_username, fullname, ap_slot
from (Patients join Appointments on username = p_username) join Users on Users.username = p_username;

