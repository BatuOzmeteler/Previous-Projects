import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

public class PatientServlet extends HttpServlet {
    DBManager dbManager = new DBManager();
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        try {
            Cookie cookie = HMSUtils.getCookie(request);
            String username = cookie.getValue();

            // Getting Doctor Mail
            String doctor_mail = (String) request.getParameter("doctor-mail");
            System.out.println(doctor_mail);
            request.setAttribute("Doctor Name", "Please Select A Doctor");
            if (doctor_mail != null && !doctor_mail.equals("null")) {
                request.setAttribute("Selected Doctor", doctor_mail);
                String doctor_name = dbManager.get_DoctorName(doctor_mail);
                request.setAttribute("Doctor Name", doctor_name);
                request.setAttribute("Doctor Mail", doctor_mail);
            }
            // Filter Doctors
            ResultSet allDoctors = dbManager.get_AllDoctors();
            String filter_expertise = (String) request.getParameter("filter-expertise");
            System.out.println(filter_expertise);
            String filter_start_date = (String) request.getParameter("start-date");
            System.out.println(filter_start_date);
            String filter_end_date = (String) request.getParameter("end-date");
            System.out.println(filter_end_date);
            String filter_start_hours = (String) request.getParameter("start-hours");
            System.out.println(filter_start_hours);
            String filter_end_hours = (String) request.getParameter("end-hours");
            System.out.println(filter_end_hours);
            if (filter_expertise != null && (!filter_start_date.equals("") && !filter_end_date.equals("") || !filter_expertise.equals("starter")))
                allDoctors = dbManager.get_filteredDoctors(filter_expertise,filter_start_date,filter_start_hours,filter_end_date,filter_end_hours);

            // Getting Available Times for a Date
            String appo_date = (String) request.getParameter("appointment-date");
            String doctor1_mail = (String) request.getParameter("doctor1-mail");
            System.out.println("Appointment Date: " + appo_date);
            request.setAttribute("Appointment Hours", new ArrayList<String>());
            System.out.println("Doctor1: " + doctor1_mail);
            if (appo_date != null && !appo_date.equals("") && doctor1_mail != null && !doctor1_mail.equals("null")) {
                ArrayList<String> appo_hours = dbManager.get_AppointmentHours(appo_date, doctor1_mail);
                System.out.println("Number of Appointment Hours: " + appo_hours.size());
                request.setAttribute("Appointment Hours", appo_hours);
                request.setAttribute("Appointment Date", appo_date);
                request.setAttribute("Doctor Mail", doctor1_mail);
            }

            // Book Appointment
            String appo_slot = (String) request.getParameter("appointment-slot");
            String doctor2_mail = (String) request.getParameter("doctor2-mail");
            System.out.println("Appointment Slot: " + appo_slot);
            if (appo_slot != null && doctor2_mail != null){
                ResultSet check = dbManager.check_AppointmentExists(doctor2_mail, appo_slot);
                check.next();
                if (check.getString(1).equals("1")){
                    check.next();
                    if (check.getString(1).equals("1")) {
                        dbManager.book_appointment(appo_slot,doctor2_mail,username);
                        System.out.println("Booked Appointment for " + username + " " + doctor2_mail + " " + appo_slot);
                        request.setAttribute("BookProblem",false);
                    } else {
                        System.out.println("Couldn't Book Appointment for " + username + " " + doctor2_mail + " " + appo_slot);
                        request.setAttribute("BookProblem",true);
                    }
                } else {
                    System.out.println("Couldn't Book Appointment for " + username + " " + doctor2_mail + " " + appo_slot);
                    request.setAttribute("BookProblem",true);
                }
            } else {
                request.setAttribute("BookProblem",false);
            }

            // General Attributes
            String [] patientData = dbManager.get_PatientData(username);
            ResultSet allExpertises = dbManager.get_Expertises();
            request.setAttribute("Patient Data", patientData);
            request.setAttribute("All Doctors", allDoctors);
            request.setAttribute("All Expertise", allExpertises);

            request.getRequestDispatcher("/patient.jsp").forward(request, response);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        dbManager.init();
        try {
            if (HMSUtils.checkCookies(request, "Patient")) {
                Cookie cookie = HMSUtils.getCookie(request);
                String username = cookie.getValue();
                String [] patientData = dbManager.get_PatientData(username);
                ResultSet allDoctors = dbManager.get_AllDoctors();
                ResultSet allExpertises = dbManager.get_Expertises();
                request.setAttribute("Patient Data", patientData);
                request.setAttribute("All Doctors", allDoctors);
                request.setAttribute("All Expertise", allExpertises);
                request.setAttribute("Doctor Name", "Please Select A Doctor.");
                request.setAttribute("BookProblem",false);
                System.out.println("BAK");
                request.getRequestDispatcher("/patient.jsp").forward(request, response);
            } else {
                System.out.println("Redirecting to Login Page");
                response.sendRedirect("/login");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
