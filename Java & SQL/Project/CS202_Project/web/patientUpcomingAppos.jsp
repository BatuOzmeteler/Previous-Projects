<%@ page import="java.sql.ResultSet" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
    <title>Upcoming Appointments - Patient</title>
    <link rel="stylesheet" type="text/css" href="/patientUpcoming-Appos.css">
</head>

<body>
<script>
    var ap_id = null; var ap_month = null;
    var ap_day = null; var ap_hour = null;
    var today = new Date(); var ap_slot = null;
</script>
<div id="upcoming-box">

    <div class="upcoming-table">

        <h1>Upcoming Appointments - Patient</h1>

        <form name = "filterer" action = "/login/patient/upcomingAppointments" method = "post">
            <input type="number" name="filter_day" id="filter-day" placeholder="Enter a day" min="1" max="31" required>
            <input type="submit" name="button" id="filter-button" value="Filter">
        </form>

        <form name = "no_filterer" action = "/login/patient/upcomingAppointments" method = "post">
            <input type="submit" name="button" id="no-filter-button" value="No Filter">
        </form>

        <form name ="canceler" action = "/login/patient/upcomingAppointments" method = "post">
            <input type="hidden" name = "ap_id" id="apID"/>
            <input type="hidden" name = "ap_slot" id="apSlot"/>
            <input type="submit" onclick="alerter()" name="button" id="cancel-button" value="Cancel">
        </form>

        <table cellspacing="0" cellpadding="0" border="0" style="width:100%; margin-top: -60px;">
            <tr>
                <td>
                    <table id="table-header" cellspacing="0" cellpadding="1" border="0" style="width:100%">
                        <tr>
                            <th name="secret-numbers" id="secret1">10101010101010101</th>
                            <th name="secret-numbers" id="secret2">10101010101010101</th>
                            <th name="secret-numbers" id="secret3">10101010101010101</th>
                            <th name="secret-numbers" id="secret4">10101010101010101</th>
                            <th name="secret-numbers" id="secret5">10101010101010101</th>
                            <th name="secret-numbers" id="secret6">10101010101010101</th>
                            <th name="secret-numbers" id="secret7">10101010101010101</th>
                        </tr>
                        <tr>
                            <th>Appointment ID</th>
                            <th>Fullname</th>
                            <th>Expertise</th>
                            <th>Doctor's Room</th>
                            <th>Appointment Slot</th>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="height:210px; overflow:auto;">
                        <table id="table-body" cellspacing="0" cellpadding="1" border="0" style="width:100%">
                            <%! ResultSet patientUpcomingAppos; int count_radio; int count_form;%>
                            <% patientUpcomingAppos = (ResultSet) request.getAttribute("Patient Upcoming Appos");%>
                            <%count_radio = 1;%>
                            <%count_form = 1;%>
                            <% while(patientUpcomingAppos.next()) { %>
                            <tr>
                                <% for(int i = 1; i < patientUpcomingAppos.getMetaData().getColumnCount(); i++) { %>
                                <td>
                                    <input type="hidden" id="apID<%=count_radio++%>" value="<%=patientUpcomingAppos.getString(i)%>" >
                                    <% try { %>
                                    <%= patientUpcomingAppos.getString(i)%>
                                    <% }catch(Exception e){ %>
                                    <% e.printStackTrace();%>
                                    <% }%>
                                </td>
                                <% } %>
                                <td>
                                    <form name ="radio-form<%=count_form++%>" action = "/login/patient/upcomingAppointments" method = "post">
                                        <div class="radio">
                                            <input type="radio" id="radio<%=count_radio++%>" name="radio-button" onclick="setAp('<%=count_radio-6%>')" >
                                        </div>
                                    </form>
                                </td>
                                <% } %>
                            </tr>
                            <script>
                                function alerter() {
                                    if (ap_id == null){
                                        alert("No appointment is selected")
                                    }else{
                                        if(ap_month > today.getMonth() + 1){
                                            alert("Appointment canceled")
                                        }else{
                                            if(ap_day > today.getDay()){
                                                alert("Appointment canceled")
                                            }else{
                                                alert("Appointments can only be " +
                                                    "canceled a day before")
                                            }
                                        }
                                    }
                                }
                            </script>
                            <script>
                                function setAp(count){
                                   var apID_new = "apID" + count;
                                   var apSlot_new = "apID" + (parseInt(count, 10) + 4).toString();
                                   ap_id = document.getElementById(apID_new).value;
                                   var ap_slot = document.getElementById(apSlot_new).value.toString();
                                   ap_month  = parseInt(ap_slot.substr(5,7), 10);
                                   ap_day    = parseInt(ap_slot.substr(8,10), 10);
                                   document.getElementById("apID").value = ap_id;
                                   document.getElementById("apSlot").value = ap_slot;
                                }
                            </script>
                            <tr>
                                <td name="secret-numbers" id="inside-secret1">10101010101010101</td>
                                <td name="secret-numbers" id="inside-secret2">10101010101010101</td>
                                <td name="secret-numbers" id="inside-secret3">10101010101010101</td>
                                <td name="secret-numbers" id="inside-secret4">10101010101010101</td>
                                <td name="secret-numbers" id="inside-secret5">10101010101010101</td>
                                <td name="secret-numbers" id="inside-secret6">10101010101010101</td>
                                <td name="secret-numbers" id="inside-secret7">10101010101010101</td>
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