#System.out.println(">: " + salt("password1", "ci00098"));
INSERT INTO Users VALUES('ci00098','Cihan Ipekoglu',10000000098,'MwmhDLU0xly/+XSrMUD5WA==','cihan.ipekoglu@gmail.com','W-13',5391969821,'Patient');
INSERT INTO Patients VALUES('ci00098');

#System.out.println(">: " + salt("password2", "et00001"));
INSERT INTO Users VALUES('et00001','Ezgin Turkoglu',10000000001,'sN0nAutDVCQgUo090UzvZA==','ezgin.turkoglu@gmail.com','T-3',5378619138,'Doctor');
INSERT INTO Doctors VALUES('et00001','Oncologist',100);

#System.out.println(">: " + salt("password3", "sc00000"));
INSERT INTO Users VALUES('sc00000','Saime Cekmeci',10000000000,'AWuEunzxmQxTdFZHzLiNnQ==','saime.cekmeci@gmail.com','L-4',5366547165,'Manager');
INSERT INTO Managers VALUES('sc00000');

#System.out.println(">: " + salt("password4", "mm00061"));
INSERT INTO Users VALUES('mm00061','Mugenur Mp',10000000061,'ejMfvhUKfU7+kuC2rDtdDw==','mugenur.mp@gmail.com','V-20',5378158882,'Nurse');
INSERT INTO Nurses VALUES('mm00061');

