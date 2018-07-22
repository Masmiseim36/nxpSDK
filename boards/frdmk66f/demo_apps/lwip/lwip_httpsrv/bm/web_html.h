

#define HTTP_HEAD "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n"

#define LOGIN \
    "\
<html>\
<head>\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\
<title>Freescale LwIP httpserver test </title>\
</head>\
</body>\
<br><center><h2>Freescale</h2>\
<br><center><b>**********Freescale LwIP httpserver test ************</b>\
<form action=\"login\" method=\"post\">\
<p>  name:\
<input type=\"text\" name=\"UserName\">\
<p>	 password:\
<input type=\"password\" name=\"PassWord\">\
<p><input type=\"submit\" value=\"login\"> \
</form>\
</body> \
</html>\
"

#define COMMAND_A \
    "\
<html>\
<head>\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\
<title>Freescale LwIP httpserver test  </title>\
</head>\
</body>\
<br><center><h2>Freescale</h2>\
<br><center><b>***********Freescale LwIP httpserver test ***********</b>\
<br><br><br><center><b>***********！！！login successful！！！***********</b>\
<br><br><br>command status:  <input type=\"button\" id=\"b1\" style=\"background:#0000FF\" value=\"A\" > \
<form name=\"input\" action=\"/html/html_form_action.asp\" method=\"post\">\
<br>send A<input type=\"radio\" name=\"CMD_CTRL\" value=\"A\" checked=\"checked\">\
<br/>\
<br>send B<input type=\"radio\" name=\"CMD_CTRL\" value=\"B\">\
<br><br><center><input type =\"submit\" value =\"control\">\
</body> \
</html>\
"

#define COMMAND_B \
    "\
<html>\
<head>\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\
<title>Freescale LwIP httpserver test  </title>\
</head>\
</body>\
<br><center><h2>Freescale</h2>\
<br><center><b>***********Freescale LwIP httpserver test ***********</b>\
<br><br><br><center><b>***********！！！login successful！！！***********</b>\
<br><br><br>command status:  <input type=\"button\" id=\"b1\" style=\"background:#DCDCDC\" value=\"B \" > \
<form name=\"input\" action=\"/html/html_form_action.asp\" method=\"post\">\
<br>send A<input type=\"radio\" name=\"CMD_CTRL\" value=\"A\" checked=\"checked\">\
<br/>\
<br>send B<input type=\"radio\" name=\"CMD_CTRL\" value=\"B\" checked=\"checked\">\
<br><br><center><input type =\"submit\" value =\"control\">\
</body> \
</html>\
"
