<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
    <title>Login</title>
    <link rel="stylesheet" type="text/css" href="login.css">
</head>

<body>

<div id="main-box">
    <div class="login-box">

        <h1>Login</h1>

        <form action = "/login" method = "post">

            <input type="text" name="username" maxlength="15" placeholder="Username"/>
            <input type="password" name="password" maxlength="15" placeholder="Password"/>
            <input type="submit" name="login-button" value="Login"/>

        </form>

    </div>

    <div class="sign-up-box">

        <label>Don't have an account?</label>
        <a href="/signup">Sign up</a>

    </div>

    <script type="text/javascript">
        function alertMessages() {
            <%! boolean[] flags; %>
            <%flags = (boolean[]) request.getAttribute("flags");%>
            if (<%=!flags[1]%>) {
                if (<%=!flags[0]%>) {
                    alert("Invalid Credentials")
                }
            }

        }
        window.onload = alertMessages()
    </script>

</div>

</body>

</html>
