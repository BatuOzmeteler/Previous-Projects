import javax.servlet.ServletException;
import javax.servlet.http.*;
import java.io.IOException;
import java.sql.SQLException;
import java.util.Arrays;

public class SignUpServlet extends HttpServlet {

    public static boolean[] flags = new boolean[8];
    DBManager dbManager = new DBManager();

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        try {
            signUpHandler(request,response);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

        dbManager.init();
        Arrays.fill(flags,true);
        request.setAttribute("flags", flags);
        request.getRequestDispatcher("signup.jsp").forward(request, response);
    }

    protected void signUpHandler(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException, SQLException {
        String username = request.getParameter("username");   String name = request.getParameter("name"); String surname = request.getParameter("surname");
        String TC = request.getParameter("tc");               String password = Salter.salt(request.getParameter("password"),username);
        String email = request.getParameter("email");         String address = request.getParameter("address");
        String phoneNo = request.getParameter("phonenumber"); String u_type = "Patient";
        String gender = request.getParameter("gender");       int age = Integer.parseInt(request.getParameter("age"));

        flags[0] = dbManager.check_Name(request.getParameter("name")); flags[1] = dbManager.check_Surname(request.getParameter("surname"));
        flags[2] = dbManager.check_Username(username);                    flags[3] = dbManager.check_Password(request.getParameter("password"));
        flags[4] = dbManager.check_Email(email);                          flags[5] = dbManager.check_TC_PhoneNo(TC,1);
        flags[6] = dbManager.check_TC_PhoneNo(phoneNo,2);         flags[7] = false;

        request.setAttribute("flags", flags);

        /*System.out.println("name: " + flags[0] + "\n" +
                "surname: " + flags[1] + "\n" +
                "username: " + flags[2] + "\n" +
                "password: " + flags[3] + "\n" +
                "email: " + flags[4] + "\n" +
                "TC: " + flags[5] + "\n" +
                "phoneNo: " + flags[6]);*/

        if(flags[0] && flags[1] && flags[2] && flags[3] && flags[4] && flags[5] && flags[6]){
            System.out.println("Logging in as " + username);
            response.addCookie(new Cookie("UserInfo", username));

            name = (name.substring(0,1).toUpperCase()) + (name.substring(1).toLowerCase());
            surname = (surname.substring(0,1).toUpperCase()) + (surname.substring(1).toLowerCase());

            User user = new User(username, name, surname, Long.parseLong(TC), password,
                    email, address, Long.parseLong(phoneNo), u_type, gender, age);
            dbManager.add_User(user);
            response.sendRedirect("/login/patient");
        }else{
            request.getRequestDispatcher("signup.jsp").forward(request,response);
        }
    }
}
