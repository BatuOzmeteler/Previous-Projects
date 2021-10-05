import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;

public class DoctorServlet extends HttpServlet {

    DBManager dbManager = new DBManager();
    public static boolean[] flags = new boolean[5];
    public static boolean[] assign_flags = new boolean[6];

    protected void doPost(HttpServletRequest request, HttpServletResponse response){

        try{
            if (HMSUtils.checkCookies(request, "Doctor")) {
                Cookie cookie = HMSUtils.getCookie(request);
                String username = cookie.getValue();
                if(request.getParameter("button").equals("Submit Request")) {
                    Arrays.fill(assign_flags,false);
                    dayOffHandler(request, response, username, assign_flags);
                }else{
                    Arrays.fill(flags,false);
                    assignHandler(request, response, username, flags);
                }
            } else {
                System.out.println("Redirecting to Login Page");
                response.sendRedirect("/login");
            }
        }catch (Exception e){
            e.printStackTrace();
        }

    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        dbManager.init();
        Arrays.fill(flags,false);
        Arrays.fill(assign_flags,false);
        try {
            if (HMSUtils.checkCookies(request, "Doctor")) {
                Cookie cookie = HMSUtils.getCookie(request);
                String username = cookie.getValue();
                HttpServletRequest request_new = pageData(request, username, flags, assign_flags);
                request.getRequestDispatcher("/doctor.jsp").forward(request_new, response);
            } else {
                System.out.println("Redirecting to Login Page");
                response.sendRedirect("/login");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    // We might add a table to display Occupied Dates for the Doctor

    protected void dayOffHandler(HttpServletRequest request, HttpServletResponse response, String username,
                                 boolean[] assign_flags) throws Exception {

        String start_date = request.getParameter("start-date"); String start_hour = request.getParameter("start-hour");
        String end_date = request.getParameter("end-date"); String end_hour = request.getParameter("end-hour");

        System.out.println("Start Datetime: " + start_date + " " + start_hour + "\n" +
                "End Datetime: " + end_date + " " + end_hour);

        Date now = new Date();
        SimpleDateFormat only_d = new SimpleDateFormat("yyyy-MM-dd");
        SimpleDateFormat only_h = new SimpleDateFormat("HH:mm");
        String now_date = only_d.format(now); String now_hour = only_h.format(now);

        flags[0] = true;
        flags[1] = dbManager.compare_Dates(now_date, now_hour, start_date, start_hour);
        flags[2] = dbManager.compare_Dates(start_date, start_hour, end_date, end_hour);
        flags[3] = dbManager.check_DayOff(username, start_date, start_hour, end_date, end_hour);
        flags[4] = dbManager.check_OccupiedDates(username, start_date, start_hour, end_date, end_hour);

        System.out.println("Start_Now_Check: " + flags[1] + "\n" +
                "Start_End_Check: " + flags[2] + "\n" +
                "Day_Off_Check: " + flags[3] + "\n" +
                "Occupied_Check: " + flags[4]);

        if(flags[1] && flags[2] && flags[3] && flags[4]){
            dbManager.add_OccupiedDates(username, start_date,start_hour,end_date,end_hour);
        }
        HttpServletRequest request_new = pageData(request, username, flags, assign_flags);
        request.getRequestDispatcher("/doctor.jsp").forward(request_new, response);
    }

    protected HttpServletRequest pageData(HttpServletRequest request, String username, boolean[] flags,
                                          boolean[] assign_flags) throws SQLException {
        String [] doctorData = dbManager.get_DoctorData(username);
        ResultSet d_roomAvailability = dbManager.get_RoomAvailability(1);
        request.setAttribute("Doctor Data", doctorData);
        request.setAttribute("Doctor Room Availability", d_roomAvailability);
        request.setAttribute("Assign Flags", assign_flags);
        request.setAttribute("flags", flags);
        return request;
    }

    protected void assignHandler(HttpServletRequest request, HttpServletResponse response, String username,
                                 boolean[] flags)throws SQLException, ServletException, IOException {
        int ap_id = Integer.parseInt(request.getParameter("ap_id"));
        int room_id = Integer.parseInt(request.getParameter("room_id"));
        String stay_until = request.getParameter("stay_until");
        boolean [] assigned = dbManager.assign_Room(ap_id,room_id,stay_until,username);
        assign_flags[0] = true;
        assign_flags[1] = assigned[0]; assign_flags[2] = assigned[1];
        assign_flags[3] = assigned[2]; assign_flags[4] = assigned[3];
        assign_flags[5] = assigned[4];

        HttpServletRequest request_new = pageData(request, username, flags, assign_flags);

        request.getRequestDispatcher("/doctor.jsp").forward(request_new, response);
    }
}
