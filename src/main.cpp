#include <Arduino.h>
#include <M5Atom.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <UrlEncode.h>

const char *WifiSsid = "";
const char *WifiPassword = "";
String baseUrl = "https://wirefree-specific.sk-robot.com/api/";
String username = "";
String password = "";
String accessToken = "";
String userId = "";
String deviceSerialNumber = "";
String deviceId = "";

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
    int user_id = doc["user_id"];
    accessToken = String(access_token); 
    userId = String(user_id);
    Serial.print("Access Token: ");
    Serial.println(access_token);
    Serial.print("Refresh Token: ");
    Serial.println(refresh_token);
    Serial.print("User ID: ");
    Serial.println(user_id);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void GetSettings() {
  http.begin(baseUrl + "app_wireless_mower/device/info/" + deviceId);

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept-Language", "de");
  http.addHeader("Authorization", "Bearer " + accessToken);

  // Send HTTP GET request
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void GetAllDevices()
{
  http.begin(baseUrl + "app_wireless_mower/device-user/allDevice");

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept-Language", "de");
  http.addHeader("Authorization", "Bearer " + accessToken);

  // Send HTTP GET request
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
    JsonDocument doc;
    deserializeJson(doc, response);
    const char *deviceIdChar = doc["data"][0]["deviceId"];
    deviceId = String(deviceIdChar);
    const char *deviceSerialNumberChar = doc["data"][0]["deviceSn"];
    deviceSerialNumber = String(deviceSerialNumberChar);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void SetAction(String cmd, String id)
{
  http.begin(baseUrl + "iot_mower/wireless/device/action");

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept-Language", "de");
  http.addHeader("Authorization", "Bearer " + accessToken);

  // Data to send with HTTP POST
  String httpRequestBody =
      "{\"method\" : \"action\",\"appId\" : " 
      + userId 
      + ",\"deviceSn\" : \"" 
      + deviceSerialNumber 
      + "\",\"cmd\" : \"" 
      + cmd 
      + "\",\"id\" : \"" 
      + id 
      + "\"}";
  Serial.print("httpRequestBody: ");
  Serial.println(httpRequestBody);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestBody);
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void SetActionStop(){
  http.begin(baseUrl + "iot_mower/wireless/device/action");

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept-Language", "de");
  http.addHeader("Authorization", "Bearer " + accessToken);

  // Data to send with HTTP POST
  String httpRequestBody = 
  "{\"method\" : \"action\",\"appId\" : " 
  + userId 
  + ",\"deviceSn\" : \"" 
  + deviceSerialNumber 
  + "\",\"cmd\" : \"stop\",\"id\" : \"stopWork\"}";
  Serial.print("httpRequestBody: ");
  Serial.println(httpRequestBody);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestBody);
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void SetActionStart(){
  http.begin(baseUrl + "iot_mower/wireless/device/action");

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept-Language", "de");
  http.addHeader("Authorization", "Bearer " + accessToken);

  // Data to send with HTTP POST
  String httpRequestBody =
      "{\"method\" : \"action\",\"appId\" : " 
      + userId 
      + ",\"deviceSn\" : \"" 
      + deviceSerialNumber 
      + "\",\"cmd\" : \"start\",\"id\" : \"startWork\"}";
  Serial.print("httpRequestBody: ");
  Serial.println(httpRequestBody);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestBody);
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void SetActionReturnToDock(){
  http.begin(baseUrl + "iot_mower/wireless/device/action");

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept-Language", "de");
  http.addHeader("Authorization", "Bearer " + accessToken);

  // Data to send with HTTP POST
  String httpRequestBody =
      "{\"method\" : \"action\",\"appId\" : " 
      + userId 
      + ",\"deviceSn\" : \"" 
      + deviceSerialNumber 
      + "\",\"cmd\" : \"start_find_charger\",\"id\" : \"startFindCharger\"}";
  Serial.print("httpRequestBody: ");
  Serial.println(httpRequestBody);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestBody);
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void SetActionPause(){
  http.begin(baseUrl + "iot_mower/wireless/device/action");

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept-Language", "de");
  http.addHeader("Authorization", "Bearer " + accessToken);

  // Data to send with HTTP POST
  String httpRequestBody =
      "{\"method\" : \"action\",\"appId\" : " 
      + userId 
      + ",\"deviceSn\" : \"" 
      + deviceSerialNumber 
      + "\",\"cmd\" : \"pause\",\"id\" : \"pauseWork\"}";
  Serial.print("httpRequestBody: ");
  Serial.println(httpRequestBody);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestBody);
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void SetActionCancelWorkplan() {
  http.begin(baseUrl + "iot_mower/wireless/device/action");

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept-Language", "de");
  http.addHeader("Authorization", "Bearer " + accessToken);

  // Data to send with HTTP POST
  String httpRequestBody =
      "{\"method\" : \"action\",\"appId\" : " 
      + userId 
      + ",\"deviceSn\" : \"" 
      + deviceSerialNumber 
      + "\",\"cmd\" : \"cancel_time_tactics\",\"id\" : \"cancelTimeTactics\"}";
  Serial.print("httpRequestBody: ");
  Serial.println(httpRequestBody);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestBody);
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void ClearAirport() {
  SetActionReturnToDock();
  SetActionCancelWorkplan();
}

void setup() {

  M5.begin(true, false, true);
  M5.dis.setBrightness(10);
  
  M5.dis.fillpix(CRGB::Green);

  WiFi.begin(WifiSsid, WifiPassword);
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
      Serial.print("Access Token: ");
      Serial.println(accessToken);
      GetAllDevices();
      Serial.print("Device ID: ");
      Serial.println(deviceId);
      Serial.print("Device Serial Number: ");
      Serial.println(deviceSerialNumber);
      GetSettings();
      SetActionStart();
    }
    else
    {
      M5.dis.fillpix(CRGB::Green);
    }
  }
  
  
}
