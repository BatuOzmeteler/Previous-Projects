drop table if exists Appointments;
drop table if exists Patients;
drop table if exists OccupiedDates;
drop table if exists Doctors;
drop table if exists Nurses;
drop table if exists Managers;
drop table if exists Rooms;
drop table if exists Users;

SET SQL_SAFE_UPDATES = 0;

create table Users(
	username  varchar(15) primary key,
	fullname  varchar(30) not null,
    TC		  numeric(11,0) unique not null,
    password  varchar(25) not null,
	email 	  varchar(45) unique not null,
	address   varchar(30),
	phoneNo   numeric(10,0) unique,
	u_type varchar(10),
	gender varchar(10),
	check (gender in ('Male', 'Female', 'Non-Binary'),
	check (u_type in ('Manager', 'Nurse', 'Doctor', 'Patient'))
);

create table Patients(
   username varchar(15) primary key references Users(username) on delete cascade
);

create table Doctors(
	username varchar(15) primary key references Users(username) on delete cascade,
	expertise varchar(15) not null,
	d_room int unique not null
);

create table OccupiedDates(
	s_date datetime,
	f_date datetime,
	d_username varchar(15),
	foreign key (d_username) references Doctors(username),
	constraint od_pf primary key (s_date, f_date, d_username)
);

create table Managers(
   username varchar(15) primary key references Users(username) on delete cascade
);

create table Nurses(
   username varchar(15) primary key references Users(username) on delete cascade
);

create table Rooms(
	r_id int primary key,
	ap_id int default null references Appointments(ap_id),
	stay_until datetime default '0000-01-01 00:00:00'
);

create table Appointments(
	ap_id int auto_increment primary key,
	d_username varchar(15) not null,
	p_username varchar(15) not null,
	foreign key (d_username) references Doctors(username),
	foreign key (p_username) references Patients(username),
	ap_slot datetime not null
);