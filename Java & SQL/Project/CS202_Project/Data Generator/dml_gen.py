import random
from enum import Enum

# Lists for User generation
alphabet = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p","q", "r", "s", "t", "u", "v", "w", "x", "y", "z"]
expertises = ['Immunologist', 'Cardiologist', 'Dermatologist', 'Neurologist', 'Gynecologist', 'Oncologist',
              'Physiatrist', 'Psychiatrist', 'Radiologist', 'Surgeon']

# Creating user types
class USER(Enum):
    PATIENT = 'Patient'
    DOCTOR = 'Doctor'
    MANAGER = 'Manager'
    NURSE = 'Nurse'

# Files for the name/surname lists
names = open('adsoyad/ad.txt', 'r', encoding='utf-8').readlines()
surnames = open('adsoyad/soyad.txt', 'r', encoding='utf-8').readlines()
out = open('dml.sql', 'w', encoding='utf-8')


# Global to keep track of it
TC = 10000000000

patient_list = []
doctor_list = []
room_stack = []

# Read friendly functions
def get_name():
    return names[random.randint(0, len(names)-1)]

def get_surname():
    return surnames[random.randint(0, len(surnames)-1)]

def get_letter():
    return random.choice(alphabet).upper()

def get_expertise():
    return expertises[random.randint(0, len(expertises)-1)]

def get_patient():
    return patient_list[random.randint(0, len(patient_list)-1)]

def get_doctor():
    return doctor_list[random.randint(0, len(doctor_list)-1)]

def get_gender():
    return ['Male','Female'][random.randint(0,1)]

def  get_age(maxage):
    return random.randint(0,maxage)

def get_datetime():
    YYYY = str(2019)
    MM = str(12)
    DD = random.randint(1, 31)
    DD = str(DD) if DD >= 10 else '0'+str(DD)
    HH = random.randint(8,16)
    HH = str(HH) if HH >= 10 else '0'+str(HH)
    MI = [0,30][random.randint(0,1)]
    MI = str(MI) if MI >= 10 else '0'+str(MI)
    SS = '00'
    return YYYY + '-' + MM + '-' + DD + ' ' + HH + ':' + MI + ':' + SS

# number_of_users : int | user_type : Enum
def generate_user(number_of_users,user_type):
    global TC
    password = 'password'
    room = 100
    for i in range(number_of_users):
        name = get_name().strip()
        surname = get_surname().strip()
        username = name[0].lower() + surname[0].lower() + str(TC)[6:11]
        fullname = name + ' ' + surname
        email = name.split(' ')[0].lower() + '.' + surname.lower() + '@gmail.com'
        address = get_letter() + '-' + str(random.randint(1,20))
        phoneNo = '53'
        for i in range(8):
            phoneNo = phoneNo + str(random.randint(1,9))
        ut_id = user_type.value
        gender = get_gender()
        age = get_age(100) if USER.PATIENT == user_type else get_age(60)
        out.write('INSERT INTO users VALUES(\'' + username + '\',\'' + fullname + '\',' + str(TC) + ',\'' + password + '\',\'' + email + '\',\'' + address + '\',' + phoneNo + ',\'' + str(ut_id) + '\',\'' + gender + '\',' + str(age) + ');\n')
        if (USER.PATIENT == user_type):
            out.write('INSERT INTO patients VALUES(\'' + username + '\');\n')
            patient_list.append(username)
        elif (USER.DOCTOR == user_type):
            expertise = get_expertise()
            out.write('INSERT INTO doctors VALUES(\'' + username + '\',\'' + expertise + '\',' + str(room) + ');\n')
            room = room + 1
            doctor_list.append(username)
        elif (USER.MANAGER == user_type):
            out.write('INSERT INTO managers VALUES(\'' + username + '\');\n')
        elif (USER.NURSE == user_type):
            out.write('INSERT INTO nurses VALUES(\'' + username + '\');\n')
        TC = TC + 1

# number_of_rooms : int
def generate_room(number_of_rooms):
    room = 2000
    for i in range(number_of_rooms):
        out.write('INSERT INTO rooms (r_id) VALUES(' + str(room) + ');\n')
        room = room + 1


def generate_appointment(number_of_appointments):
    for i in range(number_of_appointments):
        date_time = get_datetime()
        patient = get_patient()
        doctor = get_doctor()
        out.write('INSERT INTO appointments (d_username,p_username,ap_slot) VALUES(\'' + doctor + '\',\'' + patient + '\',\'' + date_time + '\');\n')


generate_user(1,USER.MANAGER)
generate_user(30,USER.DOCTOR)
generate_user(40,USER.NURSE)
generate_user(20,USER.PATIENT)
generate_room(50)
generate_appointment(20)
out.close()
