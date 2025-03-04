// MP3
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// Bluetooth
#include <BluetoothSerial.h>

// OLED Libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize OLED display with I2C address 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define Bluetooth Serial for ESP32
BluetoothSerial SerialBT;

// Define DFPlayer Mini Serial communication pins
#define DFPLAYER_RX 16  // Connect to TX of DFPlayer Mini
#define DFPLAYER_TX 17  // Connect to RX of DFPlayer Mini

// Use hardware serial for DFPlayer Mini
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);       // Debugging serial
  SerialBT.begin("ESP32_BT"); // Start Bluetooth with name ESP32_BT

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println("Waiting for data...");
  display.display();

  // Initialize DFPlayer Mini with ESP32 hardware serial
  delay(1000); // Short delay before DFPlayer initialization
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // Corrected

  if (!myDFPlayer.begin(Serial2)) {  // Fixed DFPlayer initialization
    Serial.println(F("Unable to initialize DFPlayer Mini."));
    Serial.println(F("Check wiring and insert SD card."));

    // Show error on OLED
    display.clearDisplay();
    display.setCursor(10, 20);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("DFPlayer Init Failed!");
    display.display();

    while (true); // Stop execution if DFPlayer fails
  }

  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(10);  // Set volume

  //  greeting
  myDFPlayer.play(11);
}

String cmd = "#";

void loop() {
  if (SerialBT.available()) {
    cmd = SerialBT.readString();
    Serial.println("Received: " + cmd);  // Debug output

    cmd.toLowerCase();  // Convert input to lowercase
    cmd.trim();         // Remove leading and trailing spaces

    if (cmd.indexOf("body temperature") != -1) {
      Serial.println("Ans : The normal body temperature is around 98.6°F (37°C) but can range from 97°F to 99°F (36.1°C - 37.2°C).");
      showAns("Ans : 36.1C - 37.2C", 1);
    }
    else if (cmd.indexOf("how much water should a person drink daily") != -1)
    {
      Serial.println("Ans : An adult should drink around 2-3 liters (8-12 cups) of water daily.");
      showAns("Ans : 2-3 liters", 2);
    }
    else if (cmd.indexOf("common symptoms of diabetes") != -1)
    {
      Serial.println("Ans : Symptoms can include increased thirst, frequent urination, unexplained weight loss, fatigue, and blurred vision. Regular monitoring and medical consultation are important.");
      showAns("thirst, fatigue, ", 3);
    }
    else if (cmd.indexOf("what are common causes of headaches") != -1)
    {
      Serial.println("Ans : Headaches can be caused by stress, dehydration, lack of sleep, eye strain, or migraines.");
      showAns("stress, dehydration, ", 4);
    }
    else if (cmd.indexOf("what should i do if i have a fever") != -1)
    {
      Serial.println("Ans :  Stay hydrated, rest, wear light clothing, and take fever-reducing medicines like paracetamol if necessary. Seek medical help if it persists for more than 3 days or goes above 103°F (39.4°C).");
      showAns("hydrated, paracetamol", 5);
    }
    else if (cmd.indexOf("how can i treat a common cold at home") != -1)
    {
      Serial.println("Ans : Drink plenty of fluids, get enough rest, use saline nasal drops, and take over-the-counter medications like antihistamines or decongestants if needed.");
      showAns("fluids, rest, etc.", 6);
    }
    else if (cmd.indexOf("how can i get rid of a headache quickly") != -1)
    {
      Serial.println("Ans : Drink water, rest in a quiet, dark room, apply a cold or warm compress, and take a mild pain reliever like ibuprofen or paracetamol.");
      showAns("water, rest", 7);
    }
    else if (cmd.indexOf("how can i treat mild skin rashes") != -1)
    {
      Serial.println("Ans : Apply calamine lotion, use an anti-itch cream, and avoid allergens like dust, pollen, or chemicals. If a rash worsens, consult a doctor.");
      showAns("lotion, pollen", 8);
    }
    else if (cmd.indexOf("what are common causes of skin allergies") != -1)
    {
      Serial.println("Ans :  Detergents, soaps, certain foods, insect bites, dust, and pet dander are common triggers.");
      showAns("Detergents,soaps", 9);
    }
    else if (cmd.indexOf("how can i relieve back pain at home") != -1)
    {
      Serial.println("Ans : Apply hot or cold packs, do gentle stretching exercises, maintain good posture, and take a pain reliever if needed. If pain persists, consult a doctor.");
      showAns("exercises,stretching", 10);
    }
    else if (cmd == "stop") {
      showAns("Terminated!!!", 12);

      // Wait for MP3 12 to finish playing before stopping
      while (myDFPlayer.readState() == 1) {  // 1 means playing
        delay(500);
      }
      Serial.println("Playback Stopped.");
    }

    //valume up and down
    if(cmd == "lights on")
    {
      myDFPlayer.volumeUp();
    }
    else if(cmd == "lights off")
    {
      myDFPlayer.volumeDown();
    }

  }
}

void showAns(String str, int n) {
  // Display received data on OLED
  display.clearDisplay();
  display.setCursor(2, 2);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(str);
  display.display();

  // Play MP3 file n from SD card
  myDFPlayer.play(n);
}
