drop view if exists ListDoctors;
drop view if exists DoctorAppointmentList;
drop view if exists PatientAppointmentList;
drop view if exists ViewRoomAvailability;

create view ListDoctors as
select username,fullname, expertise
from users natural join doctors;

create view PatientsAppointmentList as
select p_username, fullname, expertise, d_room, ap_slot
from (doctors join appointments on username = d_username) join users on users.username = d_username;

create view DoctorsAppointmentList as
select d_username, fullname, age, gender, ap_slot
from (patients join appointments on username = p_username) join users on users.username = p_username;

create view ViewRoomAvailability as
SELECT r_id, stay_until < now() as availability, stay_until ,fullname,age,gender
FROM (rooms r left outer join appointments a on r.ap_id = a.ap_id) left outer join (users natural join patients)
on p_username = username;

