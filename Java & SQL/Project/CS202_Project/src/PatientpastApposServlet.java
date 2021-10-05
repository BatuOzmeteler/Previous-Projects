import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;

public class PatientpastApposServlet extends HttpServlet {
    DBManager dbManager = new DBManager();
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        if(request.getParameter("button").equals("Filter")) {
            String filter = request.getParameter("filter_month") + " " + request.getParameter("filter_year");
            try {
                pageData(request, response, 2 , filter);
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }else{
            doGet(request,response);
        }
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        dbManager.init();
        try {
            pageData(request,response,0, " ");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    protected void pageData(HttpServletRequest request, HttpServletResponse response, int filter_set,
                            String filter) throws SQLException, ServletException, IOException {
        if (HMSUtils.checkCookies(request, "Patient")) {
            String username = HMSUtils.getCookie(request).getValue();
            ResultSet patientPastAppos = dbManager.get_Appos(username,1, filter_set, filter);
            request.setAttribute("Patient Past Appos", patientPastAppos);
            request.getRequestDispatcher("/patientPastAppos.jsp").forward(request, response);
        } else {
            System.out.println("Redirecting to Login Page");
            response.sendRedirect("/login");
        }
    }
}
