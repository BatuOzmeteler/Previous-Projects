<%@ page import="java.util.Date" %>
<%@ page import="java.text.SimpleDateFormat" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
    <title>Manager's Page</title>
    <link rel="stylesheet" type="text/css" href="/manager.css">
</head>

<body>

<div id="main-box">
    <div class="login-box">

        <h1>Personal Information - Manager</h1>
        <%! String [] managerData;%>
        <% managerData = (String[]) request.getAttribute("Manager Data");%>

        <label type="text" name="username"/><strong>Username:</strong> <%=managerData[0]%> <br>
        <label type="text" name="fullname"/><strong>Fullname:</strong> <%=managerData[1]%> <br>
        <label type="text" name="email"/><strong>E-Mail:</strong> <%=managerData[2]%> <br>
        <label type="text" name="phonenumber"/><strong>Phone No:</strong> <%=managerData[3]%> <br>

        <a href="/login"><input id="log-out-button" type="button" value="Log Out" style="margin-top: 20px"/></a>

    </div>

</div>

<div id="manager-box">

    <a href="/login/manager"><input type="image" src="/reload.png" style="width: 50px; height: 50px" alt="Refresh" align="right"></a>

    <div class="manager-table">

        <h2>Statistics</h2>

        <div class="filter-table">
            <%! Date date; SimpleDateFormat formatter;%>
            <%  date = new Date(); formatter = new SimpleDateFormat("yyyy-MM-dd");
                String today = formatter.format(date);%>
            <form action = "/login/manager" name="stats" method = "post" onchange= Stats()>
                <label type="text"/><strong style="font-size: 20px; color: #454545; font-family: sans-serif">Start Date:</strong>

                <input type="date" max="<%=today%>" name="start_date" id="start-date">

                <label type="text"/><strong style="font-size: 20px; color: #454545; font-family: sans-serif">End Date:</strong>

                <input type="date" max="<%=today%>" name="end_date" id="end-date">

                <select name="expertise" id="expertise" style="height:50px" placeholder="All Expertises">
                    <option value="All Expertises" selected>All Expertises</option>
                    <option value="Oncologist">Oncologist</option>
                    <option value="Gynecologist">Gynecologist</option>
                    <option value="Neurologist">Neurologist</option>
                    <option value="Surgeon">Surgeon</option>
                    <option value="Radiologist">Radiologist</option>
                    <option value="Dermatologist">Dermatologist</option>
                    <option value="Immunologist">Immunologist</option>
                    <option value="Cardiologist">Cardiologist</option>
                    <option value="Psychiatrist">Psychiatrist</option>
                </select><br>

                <label type="text" id="result-text1"/><strong>Selected time interval: </strong>
                <label type="text" id="result1"/><%=request.getAttribute("Time Interval")%> <br> <br>

                <label type="text" id="result-text2"/><strong>Selected Expertise: </strong>
                <label type="text" id="result2"/><%=request.getAttribute("Expertise")%> <br> <br>

                <label type="text" id="result-text3"/><strong>Number of appointments in selected department and time interval: </strong>
                <label type="text" id="result3"/><%=request.getAttribute("Patient Stats")%> <br> <br>

                <label type="text" id="result-text4"/><strong>Most used facility in selected time interval: </strong>
                <label type="text" id="result4"/><%=request.getAttribute("Appo Stats")%> <br> <br>

                <label type="text" id="result-text5"/><strong>Number of doctors in selected department: </strong>
                <label type="text" id="result5"/><%=request.getAttribute("Doctor Stats")%> <br> <br>
            </form>

            <script type="text/javascript">
                function Stats() {
                    if((document.getElementById("end-date").value > document.getElementById("start-date").value)
                        && document.getElementById("end-date").value !== '' &&
                        document.getElementById("start-date").value !== ''){
                        document.forms["stats"].submit();
                    }
                }
            </script>
        </div>

    </div>

</div>

</body>

</html>