<%@ page import="java.sql.ResultSet" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
  <title>Upcoming Appointments - Doctor</title>
  <link rel="stylesheet" type="text/css" href="/upcoming--Appointments.css">
</head>

<body>

<div id="upcoming-box">

  <div class="upcoming-table">

    <h1>Upcoming Appointments - Doctor</h1>

    <form name = "filterer" action = "/login/doctor/upcomingAppointments" method = "post">
      <input type="number" name="filter_day" id="filter-day" placeholder="Enter a day" min="1" max="31" required>
      <input type="submit" name="button" id="filter-button" value="Filter">
    </form>

    <form name = "no_filterer" action = "/login/doctor/upcomingAppointments" method = "post">
      <input type="submit" name="button" id="no-filter-button" value="No Filter">
    </form>

    <table cellspacing="0" cellpadding="0" border="0" style="width:100%; margin-top: -40px;">
      <tr>
        <td>
          <table id="table-header" cellspacing="0" cellpadding="1" border="0" style="width:100%">
            <tr>
              <th name="secret-numbers" id="secret1">1010101010101010101</th>
              <th name="secret-numbers" id="secret2">1010101010101010101</th>
              <th name="secret-numbers" id="secret3">1010101010101010101</th>
              <th name="secret-numbers" id="secret4">1010101010101010101</th>
              <th name="secret-numbers" id="secret5">1010101010101010101</th>
            </tr>
            <tr>
              <th>Appointment ID</th>
              <th>Fullname</th>
              <th>Age</th>
              <th>Gender</th>
              <th>Appointment Slot</th>
            </tr>
          </table>
        </td>
      </tr>
      <tr>
        <td>
          <div style="height:280px; overflow:auto;">
            <table id="table-body" cellspacing="0" cellpadding="1" border="0" style="width:100%">
              <%! ResultSet upcomingAppos; %>
              <% upcomingAppos = (ResultSet) request.getAttribute("Doctor Upcoming Appos");%>
              <% while(upcomingAppos.next()) { %>
              <tr>
                <% for(int i = 1; i < upcomingAppos.getMetaData().getColumnCount(); i++) { %>
                <td>
                  <% try { %>
                  <%= upcomingAppos.getString(i)%>
                  <% }catch(Exception e){ %>
                  <% e.printStackTrace();%>
                  <% }%>
                </td>
                <% } %>
                <% } %>
              </tr>
              <tr>
                <td name="secret-numbers" id="inside-secret1">1010101010101010101</td>
                <td name="secret-numbers" id="inside-secret2">1010101010101010101</td>
                <td name="secret-numbers" id="inside-secret3">1010101010101010101</td>
                <td name="secret-numbers" id="inside-secret4">1010101010101010101</td>
                <td name="secret-numbers" id="inside-secret5">1010101010101010101</td>
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