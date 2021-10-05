drop table if exists Reservations;
drop table if exists Organizer;
drop table if exists Has;
drop table if exists EventType;
drop table if exists Location;

SET SQL_SAFE_UPDATES = 0;

create table Organizer
(
	o_id CHAR(3),
    o_name VARCHAR(30),
    PRIMARY KEY (o_id)
);

create table Location
(
   l_id CHAR(3),
   l_name VARCHAR(30),
   PRIMARY KEY (l_id)
);

create table EventType
(
   et_id CHAR(3),
   et_name VARCHAR(20) CHECK (et_name in ("Meeting", "Sports Organization", "Concert")),
   PRIMARY KEY (et_id)
);

create table Has
(
   l_id CHAR(3),
   et_id CHAR(3),
   CONSTRAINT et_loc_pair PRIMARY KEY (l_id, et_id),
   FOREIGN KEY (l_id) REFERENCES Location (l_id)  on delete cascade,
   FOREIGN KEY (et_id) REFERENCES EventType (et_id) on delete cascade
);

create table Reservations
(
   r_id  CHAR(3),
   o_id  CHAR(3),
   l_id  CHAR(3),
   et_id CHAR(3),
   day_n NUMERIC(2,0) CHECK (day_n BETWEEN 1 AND 31),
   start_h NUMERIC(2,0) CHECK (start_h BETWEEN 1 AND 24),
   end_h NUMERIC(2,0) CHECK (end_h BETWEEN 1 AND 24),
   PRIMARY KEY (r_id),
   FOREIGN KEY (o_id) REFERENCES Organizer (o_id) on delete cascade,
   FOREIGN KEY (l_id) REFERENCES Location (l_id)  on delete cascade,
   FOREIGN KEY (et_id) REFERENCES EventType (et_id)   on delete cascade
);

INSERT INTO EventType VALUES ('001', "Meeting");
INSERT INTO EventType VALUES ('002', "Sports Organization");
INSERT INTO EventType VALUES ('003', "Concert");