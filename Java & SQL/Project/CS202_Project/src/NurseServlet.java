import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;

public class NurseServlet extends HttpServlet {
    DBManager dbManager = new DBManager();
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        dbManager.init();
        try {
            if (HMSUtils.checkCookies(request, "Nurse")) {
                Cookie cookie = HMSUtils.getCookie(request);
                String username = cookie.getValue();
                String [] nurseData = dbManager.get_NurseData(username);
                ResultSet n_roomAvailability = dbManager.get_RoomAvailability(2);
                request.setAttribute("Nurse Room Availability", n_roomAvailability);
                request.setAttribute("Nurse Data", nurseData);
                request.getRequestDispatcher("/nurse.jsp").forward(request, response);
            } else {
                System.out.println("Redirecting to Login Page");
                response.sendRedirect("/login");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
