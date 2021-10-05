import javax.servlet.ServletException;
import javax.servlet.http.*;
import java.io.IOException;
import java.sql.SQLException;
import java.util.Arrays;

public class LoginServlet extends HttpServlet {

    public static boolean[] flags = new boolean[2];
    DBManager dbManager = new DBManager();

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        try {
            String username = request.getParameter("username");
            String password = request.getParameter("password");
            loginHandler(request, response, username, password);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        dbManager.init();
        Arrays.fill(flags,true);
        request.setAttribute("flags", flags);
        for (Cookie c: request.getCookies()) {
            if (c.getName().equals("UserInfo")) {
                c.setMaxAge(0);
            }
            response.addCookie(c);
        }
        request.getRequestDispatcher("/login.jsp").forward(request, response);
    }

    protected void loginHandler(HttpServletRequest request, HttpServletResponse response, String username, String password) throws ServletException, IOException, SQLException {

        flags[0] = dbManager.check_Login(username, password);
        flags[1] = false;

        request.setAttribute("flags", flags);

        if (flags[0]) {
            String u_type = dbManager.get_UserType(username);

            System.out.println("User type for " + username + " is " + u_type);
            Cookie cookie = new Cookie("UserInfo", username);
            cookie.setMaxAge(60*60);
            response.addCookie(cookie);

            if (u_type.equals("Patient")) {
                System.out.println("Redirecting to Patient Page");
                response.sendRedirect("/login/patient");
            } else if (u_type.equals("Doctor")) {
                System.out.println("Redirecting to Doctor Page");
                response.sendRedirect("/login/doctor");
            } else if (u_type.equals("Manager")) {
                System.out.println("Redirecting to Manager Page");
                response.sendRedirect("/login/manager");
            } else if (u_type.equals("Nurse")) {
                System.out.println("Redirecting to Nurse Page");
                response.sendRedirect("/login/nurse");
            } else {
                System.out.println("Redirecting to Login Page");
                response.sendRedirect("/login");
            }
        } else {
            request.getRequestDispatcher("/login.jsp").forward(request,response);
        }
    }

}
