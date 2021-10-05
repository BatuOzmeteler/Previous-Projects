drop table if exists appointments;
drop table if exists patients;
drop table if exists occupieddates;
drop table if exists doctors;
drop table if exists nurses;
drop table if exists managers;
drop table if exists rooms;
drop table if exists users;
drop view if exists viewroomavailability;
drop view if exists viewpatientpastappos;
drop view if exists viewpatientupcomingappos;
drop view if exists viewdoctorpastappos;
drop view if exists viewdoctorupcomingappos;

SET SQL_SAFE_UPDATES = 0;

create table users(
	username  varchar(15) primary key,
	fullname  varchar(30) not null,
    TC		  numeric(11,0) unique not null,
    password  varchar(25) not null,
	email 	  varchar(45) unique not null,
	address   varchar(30),
	phoneNo   numeric(10,0) unique,
	u_type varchar(10),
	gender varchar(10),
	age int,
	check (gender in ('Male', 'Female')),
	check (u_type in ('Manager', 'Nurse', 'Doctor', 'Patient'))
);

create table patients(
   username varchar(15) primary key references users(username) on delete cascade
);

create table doctors(
	username varchar(15) primary key references users(username) on delete cascade,
	expertise varchar(15) not null,
	d_room int unique not null
);

create table occupieddates(
	s_date datetime,
	f_date datetime,
	d_username varchar(15),
	foreign key (d_username) references doctors(username),
	constraint od_pf primary key (s_date, f_date, d_username)
);

create table managers(
   username varchar(15) primary key references users(username) on delete cascade
);

create table nurses(
   username varchar(15) primary key references users(username) on delete cascade
);

create table rooms(
	r_id int primary key,
	ap_id int default '0' references appointments(ap_id),
	stay_until datetime default '0000-01-01 00:00:00'
);

create table appointments(
	ap_id int auto_increment primary key,
	d_username varchar(15) not null,
	p_username varchar(15) not null,
	foreign key (d_username) references doctors(username),
	foreign key (p_username) references patients(username),
	ap_slot datetime not null
);

create view viewroomavailability as
SELECT r_id, stay_until < now() as availability, stay_until ,fullname, age, gender
FROM (rooms left outer join appointments on rooms.ap_id = appointments.ap_id)
left outer join (users natural join patients)
on p_username = username;

create view viewpatientpastappos as
SELECT ap_id,fullname,expertise,d_room,ap_slot,p_username FROM
((users NATURAL JOIN doctors) JOIN appointments) WHERE ap_slot < now()
AND username = d_username;

create view viewpatientupcomingappos as
SELECT ap_id,fullname,expertise,d_room,ap_slot,p_username FROM
((users NATURAL JOIN doctors) JOIN appointments) WHERE ap_slot > now()
AND username = d_username;

create view viewdoctorpastappos as
SELECT ap_id,fullname,age,gender,ap_slot,d_username FROM
((users NATURAL JOIN patients) JOIN appointments) WHERE ap_slot < now()
AND username = p_username;

create view viewdoctorupcomingappos as
SELECT ap_id,fullname,age,gender,ap_slot,d_username FROM
((users NATURAL JOIN patients) JOIN appointments) WHERE ap_slot > now()
AND username = p_username;