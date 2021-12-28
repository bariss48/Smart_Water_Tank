#include <ArduinoJson.h> // JSON datası için import ediyoruz
#include <SocketIOClient.h> // Socket.io ile Node Server'a bağlantı kurmak için import ediyoruzç
#include <ESP8266WiFi.h> // Nodemcu nun WiFi bağlantı modülü
#define BLYNK_PRINT Serial // Blyank IoT uygulamasına çıktı verdiğimiz Serial
#include <BlynkSimpleEsp8266.h> // Blyank ile Nodemcu bağlantısını sağlayan Modül
#define  trig  D2 // BreadBoraddaki outputlarımız
#define  echo  D1 // BreadBoraddaki outputlarımız
long duration; // aralık
int distance; // mesafe
char auth[] = "LX9WqbGF6BhVYo4AWMYD2ebiGeXquopA"; // Blynk Auth token
char ssid[] = "baris"; // WiFi veya Network adı
char pass[] = "12345678"; // WiFi password
BlynkTimer timer; // Blyak üzerinden timer nesnesi yapıyoruz

// SOCKET.IO WIFI CONFIG
// SOCKET IO ICIN TEKRARDAN WIFI ADI VE ŞİFRESİ GİRİYORUZ CHAR OLARAK
const char* ssid_2 = 'YOUR NETWORK NAME' 
const char* pass_2 = 'YOUR WIFI OR NETWORK PASSWORD'
String host = 'YOUR HOST IP'; // IP ADRESİ
int port = 'YOUR PORT '; //PORT NUMARASI

SocketIOClient socket; // socket nesnesi yapıyoruz

void setup() {
  Serial.begin(9600);
  // wifi bağlantısı socket.io için
  WiFi.begin(ssid_2, pass_2);
     while(Wifi.status() != WL_CONNECTED) {
       delay(1000);
       Serial.println('.');
     }
     Serial.println(WiFi.localIP());
     socket.connect(host,port);
       // Debug console
      pinMode(trig, OUTPUT);  // trig için outputu tanımlıyoruz 
      pinMode(echo, INPUT);   // echo için inputu tanımlıyoruz 
      Blynk.begin(auth, ssid, pass); // blyank wifi bağlama
      timer.setInterval(1000L, sendSensor); // ultrasonik sensorden gelen veriyi her saniye için çağırıyoruz
}

void loop() {
  socket.monitor(); //socket.io bağlantısını consoleda görmek için
  Blynk.run(); // blyanki çalışıtrıyoruz
  timer.run(); // timeri çalıştırıyoruz
  delay(1000);
}

void sendSensor()
{
  digitalWrite(trig, LOW);   // trig'i düşük olarak ayarlıyoruz
  delayMicroseconds(2);       // 2 mikrosaniye delay
  // tigPin'i yüksek olarak 10 mikro saniye olacak şekilde ayarlıyoruz
  digitalWrite(trig, HIGH);  
  delayMicroseconds(10);     
  digitalWrite(trig, LOW);  

  duration = pulseIn(echo, HIGH);   //Echodan geleni mikrosaniye cinsinden alıyoruz
  distance = duration * 0.034 / 2;   // gerçek mesafeyi hesasplıyoruz

  Serial.print("Distance = ");        //mesafeyi bastırıyoruz
  Serial.println(distance);
  
  Blynk.virtualWrite(V0, distance);
  delay(1000);                      
  // Socket üzerinden mesafeyi Node serverimiza aktarıyoruz
  StaticJsonDocument<70> doc;
  String json;
  doc['distance'] = 'distance'; // mesafeyi alıyoruz 
  doc['port'] = 'host port'
  serializeJson(doc, json); // Json formatına getiriyor
    Serial.println('[click]');
    socket.emit('message', json); // iletiyor

}
