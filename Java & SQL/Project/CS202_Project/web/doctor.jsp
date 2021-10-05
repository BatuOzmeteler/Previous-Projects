<%@ page import="java.sql.ResultSet" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
    <title>Doctor's Page</title>
    <link rel="stylesheet" type="text/css" href="/doctor_new.css">
</head>

<body>

<div id="main-box">
    <div class="login-box">

        <h1>Personal Information - Doctor</h1>

        <%! String [] doctorData;%>
        <% doctorData = (String[]) request.getAttribute("Doctor Data");%>

        <label type="text" name="username"/><strong>Username: </strong> <%=doctorData[0]%> <br>
        <label type="text" name="fullname"/><strong>FullName: </strong> <%=doctorData[1]%> <br>
        <label type="text" name="expertise"/><strong>Expertise: </strong> <%=doctorData[2]%> <br>
        <label type="text" name="roomnumber"/><strong>Room No: </strong> <%=doctorData[3]%> <br>
        <label type="text" name="email"/><strong>E-Mail: </strong> <%=doctorData[4]%> <br>
        <label type="text" name="phonenumber"/><strong>Phone No: </strong> <%=doctorData[5]%> <br>

        <a href="/login"><input id="log-out-button" type="button" value="Log Out" style="margin-top: 10px"/></a>

    </div>

</div>

<div id="doctor-box-rooms">

    <div class="refresh-box">

        <a href="/login/doctor"><input type="image" src="/reload.png" style="width: 50px; height: 50px;" alt="Refresh" align="right"></a>

    </div>

    <div class="doctor-table">

        <h2>Room Info</h2>

        <table cellspacing="0" cellpadding="0" border="0" style="width:100%">
            <tr>
                <td>
                    <table id="table-header" cellspacing="0" cellpadding="1" border="0" style="width:100%">
                        <tr>
                            <th name="secret-numbers" id="secret1">10101010101010101010</th>
                            <th name="secret-numbers" id="secret2">10101010101010101010</th>
                            <th name="secret-numbers" id="secret3">10101010101010101010</th>
                            <th name="secret-numbers" id="secret4">10101010101010101010</th>
                            <th name="secret-numbers" id="secret5">10101010101010101010</th>
                            <th name="secret-numbers" id="secret6">10101010101010101010</th>
                            <th name="secret-numbers" id="secret7">10101010101010101010</th>
                        </tr>
                        <tr>
                            <th>Room ID</th>
                            <th>Availability</th>
                            <th>Stays Until</th>
                            <th>Fullname</th>
                            <th>Age</th>
                            <th>Gender</th>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="height:280px; overflow:auto;">
                        <table id="table-body" cellspacing="0" cellpadding="1" border="0" style="width:100%">
                            <%! ResultSet d_roomAvailability; %>
                            <% d_roomAvailability = (ResultSet) request.getAttribute("Doctor Room Availability");%>
                            <% while(d_roomAvailability.next()) { %>
                            <tr>
                                <% for(int i = 1; i < d_roomAvailability.getMetaData().getColumnCount()+1; i++) { %>

                                <% try {
                                    if (d_roomAvailability.getString(i) != null && d_roomAvailability.getString(i).equals("1")) {
                                %>
                                <td id="doctor-available">Available</td>
                                <%} else if (d_roomAvailability.getString(i) != null && d_roomAvailability.getString(i).equals("0")) {%>
                                <td id="doctor-unavailable">Unavailable</td>
                                <%} else { %>
                                <td>
                                    <%= d_roomAvailability.getString(i)%>
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
                                <td name="secret-numbers" id="inside-secret4">10101010101010101010</td>
                                <td name="secret-numbers" id="inside-secret5">10101010101010101010</td>
                                <td name="secret-numbers" id="inside-secret6">10101010101010101010</td>
                                <td name="secret-numbers" id="inside-secret7">10101010101010101010</td>
                            </tr>
                        </table>
                    </div>
                </td>
            </tr>
        </table>

    </div>

</div>

<div id="doctor-box-assign">
    <div class="three-inputs">
        <h3>Assign Patient to a Room</h3>

        <form action = "/login/doctor" method = "post">
            <input type="number" name="ap_id" id="ap-id" placeholder="Appointment ID" min="1" required>
            <input type="number" name="room_id" id="room-id" placeholder="Room ID" min="2000" max="2049" required>
            <label for="stay-until"><strong>Stays Until: </strong></label>
            <input type="date"   name="stay_until" id="stay-until" placeholder="Stay Until" required>
            <a href="/login/doctor/pastAppointments"><input id="apply-button" type="submit" value="Apply" name="button"/></a>
        </form>

    </div>

</div>

<div id="doctor-box-appointments">

    <div class="button-appointments">

        <h3>Appointment Info</h3>

        <a href="/login/doctor/pastAppointments" target="_blank"><input id="past-button" type="submit" value="Past Appointments"/></a>

        <a href="/login/doctor/upcomingAppointments" target="_blank"><input id="upcoming-button" type="submit" value="Upcoming Appointments"/></a>

    </div>

</div>

<div id="doctor-box-off">

    <div class="date-off">

        <h4>Select Unavailable Days</h4>

        <form action = "/login/doctor" method = "post">
        <strong style="font-size: 25px; color: #454545" >Start Date:</strong> <input type="date" name="start-date" required>

        <strong style="font-size: 25px; color: #454545">Start Hour:</strong> <select name = "start-hour" id="start-hours" style="height:50px; overflow:auto;" required>
        <option value="09:00">09:00</option> <option value="09:30">09:30</option>
        <option value="10:00">10:00</option> <option value="10:30">10:30</option>
        <option value="11:00">11:00</option> <option value="11:30">11:30</option>
        <option value="12:00">12:00</option> <option value="12:30">12:30</option>
        <option value="13:00">13:00</option> <option value="13:30">13:30</option>
        <option value="14:00">14:00</option> <option value="14:30">14:30</option>
        <option value="15:00">15:00</option> <option value="15:30">15:30</option>
        <option value="16:00">16:00</option> <option value="16:30">16:30</option>
        <option value="17:00">17:00</option>
        </select><br> <br>

        <strong style="font-size: 25px; color: #454545">End Date:</strong> <input type="date" name="end-date" required>

        <strong style="font-size: 25px; color: #454545">End Hour:</strong> <select name = "end-hour" id="end-hours" style="height:50px; overflow:auto;" required>
        <option value="09:00">09:00</option> <option value="09:30">09:30</option>
        <option value="10:00">10:00</option> <option value="10:30">10:30</option>
        <option value="11:00">11:00</option> <option value="11:30">11:30</option>
        <option value="12:00">12:00</option> <option value="12:30">12:30</option>
        <option value="13:00">13:00</option> <option value="13:30">13:30</option>
        <option value="14:00">14:00</option> <option value="14:30">14:30</option>
        <option value="15:00">15:00</option> <option value="15:30">15:30</option>
        <option value="16:00">16:00</option> <option value="16:30">16:30</option>
        <option value="17:00">17:00</option>

        </select><br>

        <input id="day-off-button" type="submit" value="Submit Request" name="button" style="margin-top: 10px"/>
        </form>


        <script type="text/javascript">
            function alertDayOff() {

                <%! boolean[] flags;%>
                <%flags = (boolean[]) request.getAttribute("flags");%>
                if(<%= flags[0] %>) {
                    if (<%= flags[1] %>) {
                        if (<%= flags[2] %>) {
                            if (<%= flags[3] %>) {
                                if (<%= flags[4] %>) {
                                    alert("Day-off registered with the specified datetime");
                                }else{
                                    alert("A day-off is already registered between those dates");
                                }
                            }else{
                                alert("An appointment exists in between these dates");
                            }
                        }else{
                            alert("Start datetime of the day-off should be post " +
                                "end datetime");
                        }
                    }else{
                        alert("Start datetime of the day-off should be post " +
                            "current datetime");
                    }
                }
            }
            window.onload = alertDayOff()
        </script>

        <script type="text/javascript">
            function alertAssign() {

                <%! boolean[] assign_flags;%>
                <%assign_flags = (boolean[]) request.getAttribute("Assign Flags");%>
                if(<%= assign_flags[0] %>) {
                    if (<%= assign_flags[1] %>) {
                        if (<%= assign_flags[2] %>) {
                            if (<%= assign_flags[3] %>) {
                                if(<%= assign_flags[4] %>){
                                    if(<%= assign_flags[5] %>){
                                        alert("Specified room is assigned to the patient of this appointment ID");
                                    }else{
                                        alert("This patient is not your patient");
                                    }
                                }else{
                                    alert("Specified datetime for the patient to stay until is invalid");
                                }
                            }else{
                                alert("This room is already occupied");
                            }
                        }else{
                            alert("The appointment slot of this appointment ID is more than a day past");
                        }
                    }else{
                        alert("This appointment ID doesn't exist");
                    }
                }
            }
            window.onload = alertAssign()
        </script>
    </div>

</div>

</body>

</html>
