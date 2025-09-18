#include <Arduino.h>
#include <M5Atom.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <UrlEncode.h>

const char *ssid = "";
const char *password = "";
String baseUrl = "https://wirefree-specific.sk-robot.com/api/";
String username = "";
String password = "";
String accessToken = "";

bool isAirfieldBlocked = false;

HTTPClient http;

void GetToken() {
  http.begin(baseUrl + "auth/oauth/token");

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", "Basic YXBwOmFwcA==");

  // Data to send with HTTP POST
  String httpRequestData = "username="+urlEncode(username)+"&password=" + urlEncode(password)+"&grant_type=password&scope=server";

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
    JsonDocument doc;
    deserializeJson(doc, response);
    const char *access_token = doc["access_token"];
    const char *refresh_token = doc["refresh_token"];
    accessToken = String(access_token); 
    Serial.print("Access Token: ");
    Serial.println(access_token);
    Serial.print("Refresh Token: ");
    Serial.println(refresh_token);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void setup() {

  M5.begin(true, false, true);
  M5.dis.setBrightness(10);
  
  M5.dis.fillpix(CRGB::Green);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  M5.update();
 
  if (M5.Btn.wasPressed())
  {
    Serial.println("Button Pressed");
    isAirfieldBlocked = !isAirfieldBlocked;
    if (isAirfieldBlocked)
    {
      M5.dis.fillpix(CRGB::Red);
      GetToken();
    }
    else
    {
      M5.dis.fillpix(CRGB::Green);
    }
  }
  
  
}
