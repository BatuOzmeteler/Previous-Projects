import java.sql.*;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;

public class DBManager {
    private static final String DB_URL = "jdbc:mysql://localhost:3306/hospital";

    private static final String USER = "client0";
    private static final String PASS = "client0!";

    private static String sqlCheck = null;
    private static String sqlInsert = null;
    private static Connection conn = null;
    private static PreparedStatement pstmt = null;
    private static ResultSet rs = null;
    private static int length = 0;

    public void init() {
        //connects to database with the specified credentials

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            conn = DriverManager.getConnection(DB_URL, USER, PASS);
            //System.out.println("Connected to Database!");
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Database Access Error!");
        }
    }

    public boolean check_IfUsernameExists(String username, boolean boolPrint) throws SQLException {
        /*checks if username exists
          boolPrint = true   =>   only prints error message when login fails
          boolPrint = false  =>   only prints error message when sign up fails*/

        boolean check = false;
        sqlCheck = "SELECT COUNT(*) FROM users WHERE username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        rs = pstmt.executeQuery();
        rs.absolute(1);

        if(rs.getInt(1) == 1){
            if(boolPrint == false) { System.out.println("This username already exists"); }
            check = true;
        }else{
            if(boolPrint){ System.out.println("This username doesn't exist"); }
            check = false;
        }
        return check;
    }

    public boolean check_Login(String username, String password) throws SQLException {
        //checks if username and password strings match, decides if the user can login or not

        boolean check = false;
       if(check_IfUsernameExists(username, true)){
           String hashedPass = Salter.salt(password, username);
           sqlCheck = "SELECT COUNT(*) FROM users WHERE username = ? AND password = ?";
           pstmt = conn.prepareStatement(sqlCheck);
           pstmt.setString(1, username);
           pstmt.setString(2, hashedPass);
           rs = pstmt.executeQuery();
           rs.absolute(1);

           if(rs.getInt(1) == 1) {
               check = true;
           }else {
               System.out.println("This username and password pair doesn't match");
               check = false;
           }
       }
       return check;
    }

    public String get_UserType(String username) throws SQLException{
        /*returns the user type of the user who logs in
          returns "None" if the user doesn't exist*/

        String u_type = "None";
        sqlCheck = "SELECT u_type FROM users WHERE username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        rs = pstmt.executeQuery();
        rs.absolute(1);
        u_type =  rs.getString(1);

        return u_type;
    }

    public boolean check_Password(String password){
        //checks if the password is valid

        boolean check = false;
        length = password.length();
        int numCount = 0;
        int uppercase = 0;
        int lowercase = 0;
        for (int i = 0; i < length; i++) {
            char ch = password.charAt(i);
            if (is_Numeric(ch)) {
                numCount++;
            } else if (is_Letter(ch)) {
                if(Character.isLowerCase(ch)){
                    lowercase++;
                }else{
                    uppercase++;
                }
            } else {
                System.out.println("Password can only contain letters from " +
                        "the English alphabet or numbers");
                check = false;
                break;
            }
        }

        if(lowercase  > 0 && uppercase > 0 && numCount > 0 && (lowercase+uppercase+numCount == length)){
            check = true;
        }else{
            System.out.println("Password must contain lowercase, uppercase characters and numbers");
            check = false;
        }
        return check;
    }

    public boolean check_Username(String username) throws SQLException {
        //checks if the username is valid

        length = username.length();
        boolean check = false;
        int count = 0;
        if(check_Validity(username, 1)){
            if(check_IfUsernameExists(username,false) == false){
                check = true;
            } else {
                check = false;
            }
        }else{
            System.out.println("Username can only contain letters from "
                    + "the English alphabet or numbers");
            check = false;
        }
        return check;
    }

    public boolean check_TC_PhoneNo(String number, int checkID) throws SQLException {
        //if checkID = 1    ==>    checks if the TC is valid
        //if checkID = 2    ==>    checks if the phoneNo is valid

        boolean check = false;
        if(checkID == 1){
            sqlCheck = "SELECT COUNT(*) FROM users WHERE TC = ?";
        }else{
            sqlCheck = "SELECT COUNT(*) FROM users WHERE phoneNo = ?";
        }
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, number);
        rs = pstmt.executeQuery();
        rs.absolute(1);
        if(rs.getInt(1) == 0){
            check = true;
        }else{
            if(checkID == 1){
                System.out.println("This TC already exists");
                check = false;
            }else{
                System.out.println("This phone number already exists");
                check = false;
            }
        }
        return check;
    }

    public boolean check_Name(String name){
        //checks if the name is valid

        boolean check = false;
        if(check_Validity(name, 2)){
            check = true;
        }else{
            System.out.println("Name can only contain letters from the English alphabet");
            check = false;
        }
        return check;
    }

    public boolean check_Surname(String surname){
        //checks if the surname is valid

        boolean check = false;
        if(check_Validity(surname, 2)){
            check = true;
        }else{
            System.out.println("Surname can only contain letters from the English alphabet");
            check = false;
        }
        return check;
    }

    public boolean check_Email(String email) throws SQLException {
        //checks if the e-mail is valid

        boolean check = false;

        sqlCheck = "SELECT COUNT(*) FROM users WHERE email = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, email);
        rs = pstmt.executeQuery();
        rs.absolute(1);
        if(rs.getInt(1) == 0){
            check = true;
        }else{
            System.out.println("This e-mail already exists");
            check = false;
        }
        return check;
    }

    public boolean check_Validity(String str, int checkID){
        //checks if the input string contains
        // if checkID = 1      ==>    only letters from English Alphabet or numbers
        // if checkID = 2      ==>    only letters from English Alphabet
        // if checkID = else   ==>    only numbers

        length = str.length();
        boolean check = false;
        int count = 0;

        for (int i = 0; i < length; i++) {
            char ch = str.charAt(i);
            if(checkID == 1){
                if (is_Numeric(ch) || is_Letter(ch)) {count++;}
            }else if(checkID == 2){
                if (is_Letter(ch)) {count++;}
            }else{
                if (is_Numeric(ch)) {count++;}
            }
        }
        if(count == length){
                check = true;
        }
        return check;
    }

    public void add_User(User user) throws SQLException {
        //adds user (patient) into database

        String fullname = user.getName() + " " + user.getSurname();
        sqlInsert = "INSERT INTO users VALUES(?,?,?,?,?,?,?,?,?,?)";
        pstmt = conn.prepareStatement(sqlInsert);
        pstmt.setString(1, user.getUsername());
        pstmt.setString(2, fullname);
        pstmt.setLong(3, user.getTC());
        pstmt.setString(4, user.getPassword());
        pstmt.setString(5, user.getEmail());
        pstmt.setString(6, user.getAddress());
        pstmt.setLong(7, user.getPhoneNo());
        pstmt.setString(8, user.getU_Type());
        pstmt.setString(9, user.getGender());
        pstmt.setInt(10, user.getAge());

        pstmt.executeUpdate();

        sqlInsert = "INSERT INTO patients VALUES(?)";
        pstmt = conn.prepareStatement(sqlInsert);
        pstmt.setString(1, user.getUsername());
        pstmt.executeUpdate();
    }

    public String[] get_DoctorData(String username) throws SQLException {
        //retrieves doctor data with the specified username

        String [] doctor = new String[6];
        sqlCheck = "SELECT username,fullname,expertise,d_room,email,phoneNo FROM doctors " +
                "NATURAL JOIN users WHERE username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        rs = pstmt.executeQuery();

        rs.absolute(0);
        if(rs.next()){
            doctor[0] = rs.getString(1);
            doctor[1] = rs.getString(2);
            doctor[2] = rs.getString(3);
            doctor[3] = Integer.toString(rs.getInt(4));
            doctor[4] = rs.getString(5);
            doctor[5] = Long.toString(rs.getLong(6));
        }
        return doctor;
    }

    public String[] get_NurseData(String username) throws SQLException {
        //retrieves nurse data with the specified username

        String [] nurse = new String[4];
        sqlCheck = "SELECT username,fullname,email,phoneNo FROM nurses NATURAL JOIN users WHERE username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        rs = pstmt.executeQuery();

        rs.absolute(0);
        if(rs.next()){
            nurse[0] = rs.getString(1);
            nurse[1] = rs.getString(2);
            nurse[2] = rs.getString(3);
            nurse[3] = Long.toString(rs.getLong(4));
        }
        return nurse;
    }

    public String[] get_PatientData(String username) throws SQLException{
        //retrieves patient data with the specified username

        String [] patient = new String[4];
        sqlCheck = "SELECT username,fullname,email,phoneNo FROM patients NATURAL JOIN users WHERE username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        rs = pstmt.executeQuery();

        rs.absolute(0);
        if(rs.next()){
            patient[0] = rs.getString(1);
            patient[1] = rs.getString(2);
            patient[2] = rs.getString(3);
            patient[3] = Long.toString(rs.getLong(4));
        }
        return patient;
    }

    public String[] get_ManagerData(String username) throws SQLException{
        //retrieves manager data with the specified username

        String [] manager = new String[4];
        sqlCheck = "SELECT username,fullname,email,phoneNo FROM managers NATURAL JOIN users WHERE username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        rs = pstmt.executeQuery();

        rs.absolute(0);
        if(rs.next()){
            manager[0] = rs.getString(1);
            manager[1] = rs.getString(2);
            manager[2] = rs.getString(3);
            manager[3] = Long.toString(rs.getLong(4));
        }
        return manager;
    }

    public ResultSet get_RoomAvailability(int type) throws SQLException {
        //retrieves room availability
        //if type = 1    ==>    gets doctor view
        //if type = else    ==>    gets nurse view

        if(type == 1){
            sqlCheck = "SELECT * FROM ViewRoomAvailability";
        }else{
            sqlCheck = "SELECT r_id,availability,stay_until FROM ViewRoomAvailability";
        }

        pstmt = conn.prepareStatement(sqlCheck);
        rs = pstmt.executeQuery();
        return rs;
    }

    public String get_DoctorName(String email) throws SQLException {
        sqlCheck = "select fullname from users natural join doctors where email = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1,email);
        ResultSet rs = pstmt.executeQuery();
        rs.next();
        return rs.getString(1);
    }

    public boolean compare_Dates(String start_d, String start_h, String end_d, String end_h) throws ParseException {
        //compares 2 dates input as string (date and time), returns true if end date time is bigger

        boolean check = false;
        SimpleDateFormat only_d = new SimpleDateFormat("yyyy-MM-dd");
        SimpleDateFormat only_h = new SimpleDateFormat("HH:mm");
        Date start_date = only_d.parse(start_d);
        Date start_hour = only_h.parse(start_h);
        Date end_date = only_d.parse(end_d);
        Date end_hour = only_h.parse(end_h);

        if((start_date.compareTo(end_date) < 0)) {
            check = true;
        }else if(start_date.compareTo(end_date) == 0){
            if(start_hour.compareTo(end_hour) < 0) {
                check = true;
            }else{
                check = false;
            }
        }else{
            check = false;
        }
        return check;
    }

    public boolean check_DayOff(String username, String start_d, String start_h,
    String end_d, String end_h) throws SQLException, ParseException {
        //checks if the doctor can take the day off with the specified datetime

        boolean check = true;
        sqlCheck = "SELECT ap_slot FROM (doctors JOIN appointments) WHERE username = d_username AND d_username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        rs = pstmt.executeQuery();

        while (rs.next()){
            String app_d = rs.getString(1).substring(0,10);
            String app_h = rs.getString(1).substring(11,16);
            System.out.println(app_d + " " +  app_h);
            System.out.println("start_app: " + compare_Dates(start_d, start_h, app_d, app_h));
            System.out.println("app_end: " + compare_Dates(app_d, app_h, end_d, end_h));

            if(compare_Dates(start_d, start_h, app_d, app_h) && compare_Dates(app_d, app_h, end_d, end_h)){
                check = false;
            }

        }
        return check;
    }

    public int[] parseDatetime(String datetime){
        int[] parsed = new int[5];
        parsed[0] = Integer.parseInt(datetime.substring(8,10)); //day
        parsed[1] = Integer.parseInt(datetime.substring(5,7));  //month
        parsed[2] = Integer.parseInt(datetime.substring(0,4));  //year
        parsed[3] = Integer.parseInt(datetime.substring(11,13));//hour
        parsed[4] = Integer.parseInt(datetime.substring(14,16)); //minute
        return parsed;
    }

    public void add_OccupiedDates(String username, String start_d, String start_h, String end_d, String end_h)
            throws SQLException {
        //adds occupied dates for doctors with the specified username

        sqlInsert = "INSERT INTO occupieddates VALUES(?,?,?)";

        start_h = start_h + ":00"; end_h = end_h + ":00";
        String start = start_d + " " + start_h; String end = end_d + " " + end_h;

        pstmt = conn.prepareStatement(sqlInsert);
        pstmt.setString(1, start);
        pstmt.setString(2, end);
        pstmt.setString(3, username);
        pstmt.executeUpdate();

    }

    public boolean check_OccupiedDates(String username, String start_d, String start_h, String end_d, String end_h)
            throws SQLException, ParseException {
        boolean check = true;
        sqlCheck = "SELECT s_date,f_date FROM occupieddates WHERE d_username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        rs = pstmt.executeQuery();

        while(rs.next()){
            String occ_sd = rs.getString(1).substring(0,10); String occ_sh = rs.getString(1).substring(11,16);
            String occ_ed = rs.getString(2).substring(0,10); String occ_eh = rs.getString(2).substring(11,16);

            if(compare_Dates(occ_sd, occ_sh, start_d, start_h) && compare_Dates(start_d, start_h, occ_ed, occ_eh)){
                check = false;
            }else if (compare_Dates(occ_sd, occ_sh, end_d, end_h) && compare_Dates(end_d, end_h, occ_ed, occ_eh)){
                check = false;
            }else if(start_d.equals(occ_sd)&&start_h.equals(occ_sh) || end_d.equals(occ_ed)&&end_h.equals(occ_eh)){
                check = false;
            }else if(compare_Dates(start_d, start_h, occ_sd, occ_sh) && compare_Dates(occ_ed, occ_eh, end_d, end_h)){
                check = false;
            }
        }

        return check;
    }

    public ResultSet get_Appos(String username, int appoType, int filter_set, String filter) throws SQLException {
        //gets appointments for the specified username
        // if appoType = 1      ==>    pastAppointments for patients
        // if appoType = 2      ==>    upcomingAppointments for patients
        // if appoType = 3      ==>    pastAppointments for doctors
        // if appoType = else   ==>    upcomingAppointments for doctors
        // if filter_set = 0        ==>    no-filter
        // if filter_set = 1        ==>    day-filter
        // if filter_set = else     ==>    month+year-filter
        String[] split = filter.split(" ");
        if(appoType == 1){
            if(filter_set == 0){
                sqlCheck = "SELECT * FROM viewpatientpastappos WHERE p_username = ?";
            }else if(filter_set == 1){
                sqlCheck = "SELECT * FROM viewpatientpastappos WHERE p_username = ? AND ap_slot < (now() + interval ? day)";
            }else{
                sqlCheck = "SELECT * FROM viewpatientpastappos WHERE p_username = ? AND month(ap_slot) = ?" +
                        " AND year(ap_slot) = ?";
            }
        }else if (appoType == 2){
            if(filter_set == 0){
                sqlCheck = "SELECT * FROM viewpatientupcomingappos WHERE p_username = ?";
            }else if(filter_set == 1){
                sqlCheck = "SELECT * FROM viewpatientupcomingappos WHERE p_username = ? AND ap_slot < (now() + interval ? day)";
            }else{
                sqlCheck = "SELECT * FROM viewpatientupcomingappos WHERE p_username = ? AND month(ap_slot) = ?" +
                        " AND year(ap_slot) = ?";
            }
        }else if (appoType == 3){
            if(filter_set == 0){
                sqlCheck = "SELECT * FROM viewdoctorpastappos WHERE d_username = ?";
            }else if(filter_set == 1){
                sqlCheck = "SELECT * FROM viewdoctorpastappos WHERE d_username = ? AND ap_slot < (now() + interval ? day)";
            }else{
                sqlCheck = "SELECT * FROM viewdoctorpastappos WHERE d_username = ? AND month(ap_slot) = ?" +
                        " AND year(ap_slot) = ?";
            }
        }else{
            if(filter_set == 0){
                sqlCheck = "SELECT * FROM viewdoctorupcomingappos WHERE d_username = ?";
            }else if(filter_set == 1){
                sqlCheck = "SELECT * FROM viewdoctorupcomingappos WHERE d_username = ? AND ap_slot < (now() + interval ? day)";
            }else{
                sqlCheck = "SELECT * FROM viewdoctorupcomingappos WHERE d_username = ? AND month(ap_slot) = ?" +
                        " AND year(ap_slot) = ?";
            }
        }
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, username);
        if(filter_set == 1){
            pstmt.setInt(2, Integer.parseInt(split[0]));
        }else if(filter_set == 2) {
            pstmt.setInt(2, Integer.parseInt(split[0]));
            pstmt.setInt(3, Integer.parseInt(split[1]));
        }

        return pstmt.executeQuery();
    }

    public boolean is_Letter(char ch) {return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));}

    public boolean is_Numeric(char ch) {return (ch >= '0' && ch <= '9');}

    public void cancelAppointment(String ap_id, String ap_slot) throws SQLException{
        //cancels the appointment with the specified ap_id

        boolean canDelete = false;
        int[] parsed = parseDatetime(ap_slot);

        Date today = new Date(); Calendar cal = Calendar.getInstance();
        cal.setTime(today); int today_d = cal.get(Calendar.DAY_OF_WEEK);
        int today_m = cal.get(Calendar.MONTH);
        int today_y = cal.get(Calendar.YEAR);

        int ap_day = parsed[1]; int ap_month = parsed[2]; int ap_year = parsed[3];

        if(ap_year > today_y){
            canDelete = true;
        }else{
            if(ap_month > today_m){
                canDelete = true;
            }else{
                if(ap_day > today_d){
                    canDelete = true;
                }else{
                    canDelete = false;
                }
            }
        }

        if(canDelete){
            sqlCheck = "DELETE FROM appointments WHERE ap_id = ?";
        }
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, ap_id);
        pstmt.executeUpdate();
    }

    public boolean[] assign_Room(int ap_id, int room_id, String stay_until, String username) throws SQLException{
        //assigns rooms to a patient with doctor's input if the conditions are met, returns flags
        //flag[0] ==> checks whether this ap_id exists in database
        //flag[1] ==> checks whether the ap_slot of this ap_id is at most 1 day past
        //flag[2] ==> checks whether this room_id is occupied at the moment
        //flag[3] ==> checks whether this stay_until is later than today
        //flag[4] ==> checks whether this appointment is from the doctor's expertise

        boolean[] assign_flags = new boolean[5];
        Arrays.fill(assign_flags,false);

        if(is_ApIDValid(ap_id)){
            assign_flags[0] = true;
            if(check_ApSlot(ap_id)){
                assign_flags[1] = true;
                if(check_RoomID(room_id)){
                    assign_flags[2] = true;
                    if(check_StayUntil(stay_until)){
                        assign_flags[3] = true;
                        if(check_Expertise(username,ap_id)){
                            assign_flags[4] = true;
                            sqlCheck = "UPDATE rooms SET rooms.ap_id = ?, stay_until = ? WHERE r_id = ?";
                            pstmt = conn.prepareStatement(sqlCheck);
                            pstmt.setInt(1, ap_id);
                            pstmt.setString(2, stay_until);
                            pstmt.setInt(3, room_id);
                            pstmt.executeUpdate();
                        }
                    }
                }
            }
        }
        return assign_flags;
    }

    public boolean check_Expertise(String username, int ap_id) throws SQLException {
        boolean check = false;
        sqlCheck = "SELECT COUNT(*) FROM (appointments join doctors d on appointments.d_username = d.username) " +
                "WHERE ap_id = ? AND d_username = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setInt(1, ap_id);
        pstmt.setString(2, username);
        rs = pstmt.executeQuery();
        rs.absolute(1);
        if(rs.getInt(1) == 1){
            check = true;
        }
        return check;
    }

    public boolean is_ApIDValid(int ap_id) throws SQLException {
        boolean check = false;
        sqlCheck = "SELECT COUNT(*) FROM appointments WHERE ap_id = ?";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setInt(1, ap_id);
        rs = pstmt.executeQuery();
        rs.absolute(1);
        if(rs.getInt(1) == 1){
            check = true;
        }
        return check;
    }

    public boolean check_ApSlot(int ap_id) throws SQLException {
        boolean check = false;
        sqlCheck = "SELECT COUNT(*) FROM appointments WHERE ap_id = ? AND ap_slot IN (SELECT ap_slot " +
                "FROM appointments WHERE ap_slot > (now() - interval 1 day) AND ap_slot < now() )";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setInt(1, ap_id);
        rs = pstmt.executeQuery();
        rs.absolute(1);
        if(rs.getInt(1) == 1){
            check = true;
        }
        return check;
    }

    public boolean check_RoomID(int room_id) throws SQLException {
        boolean check = false;
        sqlCheck = "SELECT COUNT(*) FROM viewroomavailability WHERE r_id = ? AND availability = '1'";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setInt(1, room_id);
        rs = pstmt.executeQuery();
        rs.absolute(1);
        if(rs.getInt(1) == 1){
            check = true;
        }
        return check;
    }

    public boolean check_StayUntil(String stay_until) {
        boolean check = false;
        Date today = new Date();
        SimpleDateFormat only_d = new SimpleDateFormat("dd");
        SimpleDateFormat only_m = new SimpleDateFormat("MM");
        SimpleDateFormat only_y = new SimpleDateFormat("yyyy");
        int stay_y = Integer.parseInt(stay_until.substring(0,4));
        int stay_d = Integer.parseInt(stay_until.substring(5,7));
        int stay_m = Integer.parseInt(stay_until.substring(8,10));
        int today_d = Integer.parseInt(only_d.format(today));
        int today_y = Integer.parseInt(only_y.format(today));
        int today_m = Integer.parseInt(only_m.format(today));

        if(stay_y > today_y){
            check = true;
        }else{
            if(stay_m > today_m){
                check = true;
            }else{
                if(stay_d > today_d){
                    check = true;
                }else{
                    check = false;
                }
            }
        }
        return check;
    }

    public String get_AppoStats(String start_date, String end_date, String expertise) throws SQLException {

            String [] expertises = {"Oncologist", "Gynecologist", "Neurologist", "Surgeon", "Radiologist",
                    "Immunologist", "Cardiologist", "Psychiatrist", "Dermatologist"};
            int max = 0; String expertise_max = ""; int selected = 0; int all = 0;
            int selected_doctors = 0; int all_doctors = 0;

            for(int i = 0; i < expertises.length; i++){
                sqlCheck = "SELECT COUNT(*) FROM (appointments join doctors) WHERE d_username = username " +
                        "AND expertise = ? AND ap_slot > ? AND ap_slot <  ?";
                pstmt = conn.prepareStatement(sqlCheck);
                pstmt.setString(1, expertises[i]);
                pstmt.setString(2, start_date);
                pstmt.setString(3, end_date);
                rs = pstmt.executeQuery();
                rs.absolute(1);
                int data = rs.getInt(1);
                all += data;
                if(data >= max){ max = data; expertise_max = expertises[i];}
                if(expertises[i].equals(expertise)){ selected = data; }
            }

            if(expertise.equals("All Expertises")){
                sqlCheck = "SELECT COUNT(*) FROM doctors";
                pstmt = conn.prepareStatement(sqlCheck);
                rs = pstmt.executeQuery();
                rs.absolute(1);
                all_doctors = rs.getInt(1);
                return expertise_max + "," + max + "," + all + "," + all_doctors;
            }else{
                sqlCheck = "SELECT COUNT(*) FROM doctors WHERE expertise = ?";
                pstmt = conn.prepareStatement(sqlCheck);
                pstmt.setString(1, expertise);
                rs = pstmt.executeQuery();
                rs.absolute(1);
                selected_doctors = rs.getInt(1);
                return expertise_max + "," + max + "," + selected + "," + selected_doctors;
            }
    }

    public ResultSet get_AllDoctors() throws SQLException {
        //retrieves all doctors

        sqlCheck = "SELECT email,fullname,expertise FROM doctors NATURAL JOIN users";
        pstmt = conn.prepareStatement(sqlCheck);
        rs = pstmt.executeQuery();
        return rs;
    }

    public ResultSet get_filteredDoctors(String expertise, String start_date, String start_hour,String end_date, String end_hour)
            throws SQLException {
        //retrieves filtered doctors

        sqlCheck = "SELECT email,fullname,expertise FROM doctors d NATURAL JOIN users WHERE ";
        if (!start_date.equals("") && ! end_date.equals("")) {
            String formated_start_date = start_date + " " + start_hour + ":00";
            String formated_end_date = end_date + " " + end_hour + ":00";
            pstmt = conn.prepareStatement("set @start_date = ?;");
            pstmt.setString(1,formated_start_date);
            pstmt.executeUpdate();
            pstmt = conn.prepareStatement("set @end_date = ?;");
            pstmt.setString(1,formated_end_date);
            pstmt.executeUpdate();
            sqlCheck += "(select (ifnull((select  sum((timediff(f_date,s_date)*16 + (((time(f_date) - time(s_date)) div 5000) + ((time(f_date) - time(s_date)) % 10000) div 3000)))\n" +
                    "                from occupieddates o\n" +
                    "                where o.d_username = d.username\n" +
                    "                    and (s_date between @start_date and @end_date\n" +
                    "                        and f_date between @start_date and @end_date))\n" +
                    "            ,0)\n" +
                    "            + ifnull((select  sum((timediff(f_date,@start_date)*16 + (((time(f_date) - time(@start_date)) div 5000) + ((time(f_date) - time(@start_date)) % 10000) div 3000)))\n" +
                    "                from occupieddates o\n" +
                    "                where o.d_username = d.username\n" +
                    "                    and @start_date > s_date and @start_date < f_date)\n" +
                    "                ,0)\n" +
                    "            + ifnull((select  sum((timediff(@end_date,s_date)*16 + (((time(@end_date) - time(s_date)) div 5000) + ((time(@end_date) - time(s_date)) % 10000) div 3000)))\n" +
                    "                from occupieddates o\n" +
                    "                where o.d_username = d.username\n" +
                    "                    and @end_date > s_date and @end_date < f_date)\n" +
                    "                ,0)\n" +
                    "            + (select  count(*)\n" +
                    "                from appointments\n" +
                    "                where d_username = d.username\n" +
                    "                  and ap_slot between @start_date and @end_date))\n" +
                    "        < (timediff(@end_date,@start_date)*16 + (((time(@end_date) - time(@start_date)) div 5000) + ((time(@end_date) - time('2020-01-08 09:30:00')) % 10000) div 3000))\n" +
                    "    as available) = 1";
            if (!expertise.equals("starter"))
                sqlCheck += " and ";

        } else {
            pstmt = conn.prepareStatement("set @start_date = ?;");
            pstmt.setString(1,"0000-01-01 00:00:00");
            pstmt.executeUpdate();
            pstmt = conn.prepareStatement("set @end_date = ?;");
            pstmt.setString(1,"9999-12-31 23:59:59");
            pstmt.executeUpdate();
        }
        if (!expertise.equals("starter")) {
            sqlCheck += " expertise = ?;";
            pstmt = conn.prepareStatement(sqlCheck);
            pstmt.setString(1, expertise);
        } else {
            sqlCheck += ";";
            pstmt = conn.prepareStatement(sqlCheck);
        }
        rs = pstmt.executeQuery();
        return rs;
    }

    public void book_appointment(String ap_slot, String d_email, String p_username) throws SQLException {
        sqlCheck = "insert into appointments (d_username, p_username, ap_slot)\n" +
                "values ((select username from users natural join doctors where email = ?), ?,?);";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1, d_email);
        pstmt.setString(2, p_username);
        pstmt.setString(3, ap_slot);
        pstmt.executeUpdate();
    }

    public ArrayList<String> get_AppointmentHours(String day, String doctor_mail) throws SQLException {
        int hour = 8;
        int min = 0;
        String formated_day = "";
        ArrayList<String> availableHours = new ArrayList<>();
        for (int i = 0; i < 16; i++) {
            if (i % 2 == 0) {
                formated_day = day + " " + (++hour) + ":00:00";
            } else {
                formated_day = day + " " + (hour) + ":30:00";
            }
            //System.out.print("Testing: " + doctor_mail + " for " + formated_day);
            rs = check_AppointmentExists(doctor_mail, formated_day);
            rs.next();
            //System.out.print(" Appointments: " + rs.getString(1));
            if (rs.getString(1).equals("1")) {
                rs.next();
                //System.out.println(" Occupied Dates: " + rs.getString(1));
                if (rs.getString(1).equals("1")) {
                    //System.out.println(" Adding to list.");
                    availableHours.add(formated_day);
                }
            }
        }
        return availableHours;
    }

    public ResultSet get_Expertises() throws SQLException {
        sqlCheck = "SELECT DISTINCT expertise FROM doctors order by expertise";
        pstmt = conn.prepareStatement(sqlCheck);
        rs = pstmt.executeQuery();
        return rs;
    }

    ResultSet check_AppointmentExists(String d_email, String date) throws SQLException{
        sqlCheck = "select count(*) = '0' from appointments a join (doctors d natural join users) on a.d_username = d.username where email = ? and ap_slot = ?" +
                " union  all " +
                "select count(*) = '0' from occupieddates o join (doctors d natural join users) on o.d_username = d.username where email = ? and ? between s_date and f_date;";
        pstmt = conn.prepareStatement(sqlCheck);
        pstmt.setString(1,d_email);
        pstmt.setString(2,date);
        pstmt.setString(3,d_email);
        pstmt.setString(4,date);
        return pstmt.executeQuery();
    }

    public static void main(String[] args)throws SQLException {
        DBManager dbManager = new DBManager();
    }
}
