<%@ page import="java.sql.ResultSet" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
  <title>Past Appointments - Doctor</title>
  <link rel="stylesheet" type="text/css" href="/past_Appointments.css">
</head>

<body>

<div id="past-box">

  <div class="past-table">

    <h1>Past Appointments - Doctor</h1>

    <form name = "filterer" action = "/login/doctor/pastAppointments" method = "post">
      <input type="number" name = "filter_month" id="filter-month" placeholder="Enter a month" min="1" max = "12" required>
      <input type="number" name = "filter_year"  id="filter-year" placeholder="Enter a year" min="2000" required>
      <input type="submit" name="button" id="filter-button" value="Filter">
    </form>

    <form name = "no_filterer" action = "/login/doctor/pastAppointments" method = "post">
      <input type="submit" name="button" id="no-filter-button" value="No Filter">
    </form>

    <table cellspacing="0" cellpadding="0" border="0" style="width:100%; margin-top: -40px">
      <tr>
        <td>
          <table id="table-header" cellspacing="0" cellpadding="1" border="0" style="width:100%">
            <tr>
              <th name="secret-numbers" id="secret1">1010101010101010101</th>
              <th name="secret-numbers" id="secret2">1010101010101010101</th>
              <th name="secret-numbers" id="secret3">1010101010101010101</th>
              <th name="secret-numbers" id="secret4">1010101010101010101</th>
              <th name="secret-numbers" id="secret5">1010101010101010101</th>
              <th name="secret-numbers" id="secret6">1010101010101010101</th>
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
              <%! ResultSet pastAppos; %>
              <% pastAppos = (ResultSet) request.getAttribute("Doctor Past Appos");%>
              <% while(pastAppos.next()) { %>
              <tr>
                <% for(int i = 1; i < pastAppos.getMetaData().getColumnCount(); i++) { %>
                <td>
                  <% try { %>
                  <%= pastAppos.getString(i)%>
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
                <td name="secret-numbers" id="inside-secret6">1010101010101010101</td>
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