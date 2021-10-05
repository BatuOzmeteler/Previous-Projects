<%@ page import="java.sql.ResultSet" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
    <title>Nurse's Page</title>
    <link rel="stylesheet" type="text/css" href="/nurse.css">
</head>

<body>

<div id="main-box">
    <div class="login-box">

        <h1>Personal Information - Nurse</h1>

        <%! String [] nurseData;%>
        <% nurseData = (String[]) request.getAttribute("Nurse Data");%>

        <label type="text" name="username"/><strong>Username:</strong> <%=nurseData[0]%> <br>
        <label type="text" name="fullname"/><strong>Fullname:</strong> <%=nurseData[1]%> <br>
        <label type="text" name="email"/><strong>E-Mail:</strong> <%=nurseData[2]%> <br>
        <label type="text" name="phonenumber"/><strong>Phone No:</strong> <%=nurseData[3]%> <br>

        <a href="/login"><input id="log-out-button" type="button" value="Log Out" style="margin-top: 10px"/></a>

    </div>

</div>

<div id="nurse-box">

    <div class="refresh-box">

        <a href="/login/nurse"><input type="image" src="/reload.png" style="width: 50px; height: 50px;" alt="Refresh" align="right"></a>

    </div>

    <div class="nurse-table">

        <h2>Room Availability</h2>

        <table cellspacing="0" cellpadding="0" border="0" style="width:100%">
            <tr>
                <td>
                    <table id="table-header" cellspacing="0" cellpadding="1" border="0" style="width:100%">
                        <tr>
                            <th name="secret-numbers" id="secret1">10101010101010101010</th>
                            <th name="secret-numbers" id="secret2">10101010101010101010</th>
                            <th name="secret-numbers" id="secret3">10101010101010101010</th>
                        </tr>
                        <tr>
                            <th>Room ID</th>
                            <th>Availability</th>
                            <th>Stays Until</th>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="height:170px; overflow:auto;">
                        <table id="table-body" cellspacing="0" cellpadding="1" border="0" style="width:100%">

                            <%! ResultSet n_roomAvailability; %>
                            <% n_roomAvailability = (ResultSet) request.getAttribute("Nurse Room Availability");%>
                            <% while(n_roomAvailability.next()) { %>
                            <tr>
                                <% for(int i = 1; i < n_roomAvailability.getMetaData().getColumnCount()+1; i++) { %>

                                <% try {
                                    if (n_roomAvailability.getString(i) != null && n_roomAvailability.getString(i).equals("1")) {
                                %>
                                <td id="nurse-available">Available</td>
                                <%} else if (n_roomAvailability.getString(i) != null && n_roomAvailability.getString(i).equals("0")) {%>
                                <td id="nurse-unavailable">Unavailable</td>
                                <%} else { %>
                                <td>
                                    <%= n_roomAvailability.getString(i)%>
                                </td>
                                <%}
                                }catch(Exception e){ %>
                                <% e.printStackTrace();%>
                                <% }%>
                                <% } %>
                                <% } %>
                            </tr>
                            <tr>
                                <td name="secret-numbers" id="inside-secret1">10101010101010101010</td>
                                <td name="secret-numbers" id="inside-secret2">10101010101010101010</td>
                                <td name="secret-numbers" id="inside-secret3">10101010101010101010</td>
                            </tr>
                        </table>
                    </div>
                </td>
            </tr>
        </table>
    </div>
</div>

</body>

</html>