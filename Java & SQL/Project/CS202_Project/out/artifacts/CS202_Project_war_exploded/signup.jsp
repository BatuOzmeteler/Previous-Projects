<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
    <title>HMS Sign Up</title>
    <link rel="stylesheet" type="text/css" href="signup.css">
</head>

<body>

<div id="main-box">
    <div class="login-box">

        <h1>Sign Up</h1>

        <form action = "/signup" method = "post">

        <input type="text" name="name" maxlength="10" placeholder="Name" required autofocus/>
        <input type="text" name="surname" maxlength="20" placeholder="Surname" required/>
        <input type="text" name="username" minlength="5" maxlength="15" placeholder="Username" required/>
        <input type="password" id=pass name="password" minlength="5" maxlength="15" placeholder="Password" required/>
        <input type="checkbox" onclick="showPassword()">Show Password
        <input type="email" name="email" maxlength="45" placeholder="E-Mail" required/>
        <input type="text" name="tc" minlength="11" maxlength="11" pattern = "[1-9]\d{10}" placeholder="TC" required/>
        <input type="number" name="age" min = 1 max = 100 placeholder="Age" required/>
        <input type="text" name="address" maxlength="30" placeholder="Address" required/>
        <input type="text" name="phonenumber" minlength="10" maxlength="10" pattern = "[1-9]\d{9}" placeholder="Phone Number" required/>

        <input id="m"  type="radio" name="gender" value="Male" required/> Male<br>
        <input id="f"  type="radio" name="gender" value="Female" required/> Female<br>

        <input type="submit" name="signup-button" value="Sign in"/>

        </form>

        <script type="text/javascript">
            function alertMessages(){
                <%! boolean[] flags; %>
                <%flags = (boolean[]) request.getAttribute("flags");%>
                if(<%= !flags[7] %>) {
                    if (<%= flags[0] %>) {
                        if (<%= flags[1] %>) {
                            if (<%= flags[2] %>) {
                                if (<%= flags[3] %>) {
                                    if (<%= flags[4] %>) {
                                        if (<%= flags[5] %>) {
                                            if (<%= flags[6] %>) {
                                                alert("User has been registered!");
                                            } else {
                                                alert("Invalid Phone No");
                                            }
                                        } else {
                                            alert("Invalid TC");
                                        }
                                    } else {
                                        alert("Invalid E-mail");
                                    }
                                } else {
                                    alert("Password must contain lowercase, uppercase characters and numbers");
                                }
                            } else {
                                alert("Invalid Username");
                            }
                        } else {
                            alert("Invalid Surname");
                        }
                    } else {
                        alert("Invalid Name");
                    }
                }
            }
            window.onload = alertMessages()
        </script>

    </div>

    <div class="sign-up-box">

        <label>Already have an account?</label>
        <a href="/login">Login</a>

    </div>

    <script>
        function showPassword() {
            var x = document.getElementById("pass");
            if (x.type == "password") {
                x.type = "text";
            } else {
                x.type = "password";
            }
        }
    </script>

</div>

</body>

</html>