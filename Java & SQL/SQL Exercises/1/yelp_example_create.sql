-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2016-10-16 15:05:22.163

-- tables
-- Table: business
CREATE TABLE business (
    id int NOT NULL,
    name varchar(50) NOT NULL,
    full_address varchar(50) NOT NULL,
    city varchar(50) NOT NULL,
    state varchar(50) NOT NULL,
    stars decimal(2,1) NOT NULL,
    review_count int NOT NULL,
    open bool NOT NULL,
    CONSTRAINT business_pk PRIMARY KEY (id)
);

-- Table: review
CREATE TABLE review (
    business_id int NOT NULL,
    user_id int NOT NULL,
    stars int NOT NULL,
    text varchar(350) NOT NULL,
    date date NOT NULL,
    type varchar(6) NOT NULL
);

-- Table: user
CREATE TABLE user (
    id int NOT NULL,
    name varchar(30) NOT NULL,
    review_count int NOT NULL,
    useful_votes int NOT NULL,
    funny_votes int NOT NULL,
    cool_votes int NOT NULL,
    CONSTRAINT user_pk PRIMARY KEY (id)
);

-- foreign keys
-- Reference: review_business (table: review)
ALTER TABLE review ADD CONSTRAINT review_business FOREIGN KEY review_business (business_id)
    REFERENCES business (id);

-- Reference: review_user (table: review)
ALTER TABLE review ADD CONSTRAINT review_user FOREIGN KEY review_user (user_id)
    REFERENCES user (id);

-- End of file.

