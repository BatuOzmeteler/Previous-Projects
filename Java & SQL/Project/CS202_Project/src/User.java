public class User {

    private String username; private String name; private String surname; private long TC;
    private String password; private String email; private String address; private long phoneNo;
    private String u_type; private String gender; private int age;

    public User(String username, String name, String surname, long TC, String password, String email,
                String address, long phoneNo, String u_type, String gender, int age) {
        this.username = username; this.name = name; this.surname = surname;
        this.TC = TC; this.password = password;
        this.email = email; this.address = address; this.phoneNo = phoneNo;
        this.u_type = u_type; this.gender = gender; this.age = age;
    }

    public String getUsername() { return username; }
    public void setUsername(String username) { this.username = username; }

    public String getName() { return name; }
    public void setName(String name) { this.name = name; }

    public String getSurname() { return surname; }
    public void setSurname(String surname) { this.surname = surname; }

    public long getTC() { return TC; }
    public void setTC(long TC) { this.TC = TC; }

    public String getPassword() { return password; }
    public void setPassword(String password) { this.password = password; }

    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }

    public String getAddress() { return address; }
    public void setAddress(String address) { this.address = address; }

    public long getPhoneNo() { return phoneNo; }
    public void setPhone(long phoneNo) { this.phoneNo = phoneNo; }

    public String getU_Type() { return u_type; }
    public void setU_Type(String u_type) { this.u_type = u_type; }

    public String getGender() { return gender; }
    public void setGender(String gender) { this.gender = gender; }

    public int getAge() { return age; }
    public void setAge(int age) { this.age = age; }

}
