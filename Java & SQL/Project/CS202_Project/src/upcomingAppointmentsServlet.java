import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;

public class upcomingAppointmentsServlet extends HttpServlet {
    DBManager dbManager = new DBManager();
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        if(request.getParameter("button").equals("Filter")) {
            String filter = request.getParameter("filter_day");
            try {
                pageData(request, response, 1, filter);
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }else{
            doGet(request,response);
        }
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        dbManager.init();
        try {
            pageData(request,response,0," ");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    protected void pageData(HttpServletRequest request, HttpServletResponse response, int filter_set,
                            String filter) throws SQLException, ServletException, IOException {
        if (HMSUtils.checkCookies(request, "Doctor")) {
            String username = HMSUtils.getCookie(request).getValue();
            ResultSet d_upcomingAppos = dbManager.get_Appos(username, 4, filter_set, filter);
            request.setAttribute("Doctor Upcoming Appos", d_upcomingAppos);
            request.getRequestDispatcher("/upcomingAppointments.jsp").forward(request, response);
        } else {
            System.out.println("Redirecting to Login Page");
            response.sendRedirect("/login");
        }
    }
}
