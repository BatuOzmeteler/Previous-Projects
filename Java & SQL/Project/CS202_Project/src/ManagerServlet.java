import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.sql.SQLException;


public class ManagerServlet extends HttpServlet {
    DBManager dbManager = new DBManager();
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        try {
            if (HMSUtils.checkCookies(request, "Manager")) {
                Cookie cookie = HMSUtils.getCookie(request);
                String username = cookie.getValue();
                String [] managerData = dbManager.get_ManagerData(username);
                request.setAttribute("Manager Data", managerData);
                HttpServletRequest request_new = statsHandler(request);
                request.getRequestDispatcher("/manager.jsp").forward(request_new, response);
            } else {
                System.out.println("Redirecting to Login Page");
                response.sendRedirect("/login");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        dbManager.init();
        try {
            if (HMSUtils.checkCookies(request, "Manager")) {
                Cookie cookie = HMSUtils.getCookie(request);
                String username = cookie.getValue();
                String [] managerData = dbManager.get_ManagerData(username);
                request.setAttribute("Manager Data", managerData);
                request.getRequestDispatcher("/manager.jsp").forward(request, response);
            } else {
                System.out.println("Redirecting to Login Page");
                response.sendRedirect("/login");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    protected HttpServletRequest statsHandler(HttpServletRequest request) throws SQLException {
        String start_date = request.getParameter("start_date");
        String end_date = request.getParameter("end_date");
        String expertise = request.getParameter("expertise");
        String timeInterval = start_date + " --- " + end_date;
        String[] stats = dbManager.get_AppoStats(start_date,end_date,expertise).split(",");
        String appoStats = stats[0] + ", with " + stats[1] + " appointments";
        int patientStats = Integer.parseInt(stats[2]);
        int doctorStats = Integer.parseInt(stats[3]);

        /*System.out.println(start_date + "\n" +
                end_date + "\n" +
                expertise + "\n" +
                "Appo Stats: " + appoStats + "\n" +
                "Patient Stats: " + patientStats + "\n" +
                "Doctor Stats: " + doctorStats);*/

        request.setAttribute("Time Interval", timeInterval);
        request.setAttribute("Expertise", expertise);
        request.setAttribute("Appo Stats", appoStats);
        request.setAttribute("Patient Stats", patientStats);
        request.setAttribute("Doctor Stats", doctorStats);
        return request;
    }

}
