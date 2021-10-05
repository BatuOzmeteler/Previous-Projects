import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;
import java.sql.SQLException;
import java.util.Enumeration;

public class HMSUtils {

    protected static Cookie getCookie(HttpServletRequest request){
        Cookie cookie = null;
        Cookie[] cookies = request.getCookies();
        for (Cookie c: cookies) {
            if (c.getName().equals("UserInfo"))
                cookie = c;
        }
        return cookie;
    }

    protected static boolean checkCookies(HttpServletRequest request, String u_type)
            throws ServletException, IOException, SQLException {
        Cookie cookie = getCookie(request);
        DBManager dbManager = new DBManager();
        if (cookie == null) {
            System.out.println("Couldn't find cookies");
            return false;
        } else {
            System.out.println(cookie.getName() + " : " + cookie.getValue());
            if (dbManager.get_UserType(cookie.getValue()).equals(u_type)) {
                return true;
            }
            return false;
        }
    }
    protected static void printSubmissionData(HttpServletRequest request){
        Enumeration param = request.getParameterNames();
        while(param.hasMoreElements()) {
            String name  =  (String) param.nextElement();
            System.out.println(name);
            System.out.println(request.getParameter(name));
        }
    }
}


