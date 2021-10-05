<%@ page import="java.sql.ResultSet" %>
<%@ page import="java.util.ArrayList" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
    <title>Patient's Page</title>
    <link rel="stylesheet" type="text/css" href="/patient.css">
</head>

<body>
<script>
    var doctor_mail = "null";
    var appointment_hour = "null";
</script>
<script type="text/javascript">
    function set_doctor_mail(elem) {
        doctor_mail = elem.value;
    }
    function set_appointment_hour(elem) {
        appointment_hour = elem.value;
    }
    function get_appointmentTimes() {
        document.getElementById("doctor-mail").value = doctor_mail;
    }
    function book_appointment() {
        document.getElementById("appointment-slot").value = appointment_hour;
    }
    function alerts(){
        <%! boolean flag;%>
        <% flag = (boolean) request.getAttribute("BookProblem");%>
        if (<%=flag%>) {
            alert("Couldn't Book.")
        }
    }
    window.onload = alerts()
</script>

<div id="main-box">
    <div class="login-box">

        <h1>Personal Information - Patient</h1>

        <%! String [] patientData;%>
        <% patientData = (String[]) request.getAttribute("Patient Data");%>

        <label type="text" name="username"/><strong>Username: </strong> <%=patientData[0]%> <br>
        <label type="text" name="firstname"/><strong>Fullname: </strong> <%=patientData[1]%> <br>
        <label type="text" name="email"/><strong>E-Mail: </strong> <%=patientData[2]%> <br>
        <label type="text" name="phonenumber"/><strong>Phone No: </strong> <%=patientData[3]%> <br>

        <a href="/login"><input id="log-out-button" type="button" value="Log Out" style="margin-top: 20px"/></a>

    </div>

</div>

<div id="doctor-box-appointments">

    <div class="button-appointments">

        <h4>Appointment Info</h4>

        <a href="/login/patient/pastAppointments" target="_blank"><input id="past-button" type="button" value="Past Appointments"/></a>

        <a href="/login/patient/upcomingAppointments" target="_blank"><input id="upcoming-button" type="button" value="Upcoming Appointments"/></a>

    </div>

</div>

<div id="appointment-box">

    <div id = "patient-table" class="patient-table">
        <h2>Take an Appointment</h2>

        <div class="filter-table">
            <form id="doctor-filter-form" method="post">

                <label type="text" name="start-date"/><strong style="font-size: 20px; color: #454545; font-family: sans-serif">Start Date:</strong>

                <input type="date" name="start-date">

                <label type="text" name="end-date"/><strong style="font-size: 20px; color: #454545; font-family: sans-serif">End Date:</strong>

                <input type="date" max="2020-12-31" name="end-date">

                <label type="text" name="start-hour"/><strong style="font-size: 20px; color: #454545; font-family: sans-serif">Start Hour:</strong>

                <select name="start-hours" style="height:50px; overflow:auto;">
                    <option value="09:00">09:00</option>
                    <option value="09:30">09:30</option>
                    <option value="10:00">10:00</option>
                    <option value="10:30">10:30</option>
                    <option value="11:00">11:00</option>
                    <option value="11:30">11:30</option>
                    <option value="12:00">12:00</option>
                    <option value="12:30">12:30</option>
                    <option value="13:00">13:00</option>
                    <option value="13:30">13:30</option>
                    <option value="14:00">14:00</option>
                    <option value="14:30">14:30</option>
                    <option value="15:00">15:00</option>
                    <option value="15:30">15:30</option>
                    <option value="16:00">16:00</option>
                    <option value="16:30">16:30</option>
                </select>

                <label type="text" name="end-hour"/><strong style="font-size: 20px; color: #454545; font-family: sans-serif">End Hour:</strong>

                <select name="end-hours" style="height:50px; overflow:auto;">
                    <option value="09:00">09:00</option>
                    <option value="09:30">09:30</option>
                    <option value="10:00">10:00</option>
                    <option value="10:30">10:30</option>
                    <option value="11:00">11:00</option>
                    <option value="11:30">11:30</option>
                    <option value="12:00">12:00</option>
                    <option value="12:30">12:30</option>
                    <option value="13:00">13:00</option>
                    <option value="13:30">13:30</option>
                    <option value="14:00">14:00</option>
                    <option value="14:30">14:30</option>
                    <option value="15:00">15:00</option>
                    <option value="15:30">15:30</option>
                    <option value="16:00">16:00</option>
                    <option value="16:30">16:30</option>
                </select>

                <select name="filter-expertise" id="filter-expertise" style="height:50px" placeholder="Select an expertise">
                    <option value="starter" selected >Select an expertise</option>
                    <%! ResultSet expertises; %>
                    <% expertises = (ResultSet) request.getAttribute("All Expertise");%>
                    <% while(expertises.next()) { %>
                    <option value="<%=expertises.getString(1)%>"><%=expertises.getString(1)%></option>
                    <%}%>
                </select>

                <input type="submit" id="filter-doctors" value="Filter">
            </form>

        </div>

        <table id="doctors-list" cellspacing="0" cellpadding="0" border="0" style="width:100%">
            <tr>
                <td>
                    <table id="table-header1" cellspacing="0" cellpadding="1" border="0" style="width:100%">
                        <tr>
                            <th name="secret-numbers">1010101010101010</th>
                            <th name="secret-numbers">1010101010101010</th>
                            <th name="secret-numbers">1010101010101010</th>
                        </tr>
                        <tr>
                            <th>Select</th>
                            <th>Fullname</th>
                            <th>Expertise</th>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="height:230px; overflow:auto;">
                        <table id="table-body1" cellspacing="0" cellpadding="1" border="0" style="width:100%">
                            <%! ResultSet doctors; %>
                            <% doctors = (ResultSet) request.getAttribute("All Doctors");%>
                            <%! int k = 1; %>
                            <% while(doctors.next()) { %>
                            <tr>
                                <td>
                                    <div class="radio">
                                        <input type="radio" id="radio<%=k%>" name="radio-button" value="<%=doctors.getString(1)%>"
                                               onclick="set_doctor_mail(this)">
                                    </div>
                                </td>
                                <% for(int i = 2; i <= doctors.getMetaData().getColumnCount(); i++) { %>

                                <td>
                                    <% try { %>
                                    <%= doctors.getString(i)%>
                                    <% }catch(Exception e){ %>
                                    <% e.printStackTrace();%>
                                    <% }%>
                                </td>
                                <% } %>
                                <% } %>
                            </tr>
                            <tr>
                                <td name="secret-numbers">1010101010101010</td>
                                <td name="secret-numbers">1010101010101010</td>
                                <td name="secret-numbers">1010101010101010</td>
                            </tr>
                        </table>
                    </div>
                </td>
            </tr>
        </table>

    </div>

</div>

<div class="next-button">

    <form name="show-available-times" action="/login/patient" method="post">
        <input type="hidden" name="doctor-mail" id="doctor-mail" value="null">
        <input type="submit" onclick="get_appointmentTimes()" name="appointment-button-1" value="Select Doctor"/>
    </form>

</div>

<div id="appointment-times">

    <div class="times-table">

        <h3>Available Times</h3>

        <div id="appointment-times-filter">
            <form method="post">
                <label type="text" name="appo-date"/><strong style="font-size: 20px; color: #454545; font-family: sans-serif">Appointment Date:</strong>
                <input type="date" name="appointment-date" id="appointment-date">
                <input type="hidden" name="doctor1-mail" id="doctor1-mail" value="<%= request.getAttribute("Doctor Mail")%>">
                <input type="submit" name="check_date" value="Check Date">
                <label type="text" id="doctor-name" name="doctor-name"/><%= request.getAttribute("Doctor Name") %>
            </form>
        </div>

        <table cellspacing="0" cellpadding="0" border="0" style="width:100%">
            <tr>
                <td>
                    <table id="table-header2" cellspacing="0" cellpadding="1" border="0" style="margin-top:-40px; width:100%">
                        <tr>
                            <th name="secret-numbers">1010101010101010</th>
                            <th name="secret-numbers">1010101010101010</th>
                        </tr>
                        <tr>
                            <th>Select</th>
                            <th>Hours</th>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="height:230px; overflow:auto;">
                        <table id="table-body2" cellspacing="0" cellpadding="1" border="0" style="width:100%">
                            <%! ArrayList<String> appo_hours; %>
                            <% appo_hours = (ArrayList<String>) request.getAttribute("Appointment Hours");%>
                            <% for (int i = 0; i < appo_hours.size(); i++){
                                String value = appo_hours.get(i);
                            %>
                            <tr>
                                <td>
                                    <div class="radio">
                                        <input type="radio" id="radioHour<%=i%>" onclick="set_appointment_hour(this)" value="<%= value %>" name="radio-button">
                                    </div>
                                </td>
                                <td>
                                    <% try { %>
                                    <%= value %>
                                    <% }catch(Exception e){ %>
                                    <% e.printStackTrace();%>
                                    <% }%>
                                </td>
                                <% } %>
                            </tr>

                            <tr>
                                <td name="secret-numbers">1010101010101010</td>
                                <td name="secret-numbers">1010101010101010</td>
                            </tr>

                        </table>
                    </div>
                </td>
            </tr>
        </table>

    </div>

    <div class="finish-button">
        <form method="post">
            <input type="hidden" name="appointment-slot" id="appointment-slot" value="null">
            <input type="hidden" name="doctor2-mail" id="doctor2-mail" value="<%= request.getAttribute("Doctor Mail")%>">
            <input type="submit" name="appointment-button-2" onclick="book_appointment()" value="Book An Appointment">
        </form>
    </div>

</div>


</body>

</html>