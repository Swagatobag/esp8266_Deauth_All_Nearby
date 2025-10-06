# esp8266_Deauth_All_Nearby
Deauth All Network in its range continiously
# **ESP8266 WiFi Jammer** 🚀

A **powerful** WiFi jammer firmware for **ESP8266** that targets **2.4 GHz** WiFi networks by continuously scanning and deauthenticating all nearby access points. This tool acts like a **disruption device**, disrupting wireless networks in your range.

---

## Features 🎯

- Supports **2.4 GHz** WiFi bands (channels 1-14)
- Automatic **channel hopping** across all channels
- Scans and detects **all nearby APs**
- Sends **repeated deauth frames** for jamming
- Runs **automatically** on power-up
- **Lightweight** and easy to deploy

---

## Usage 🛠️

1. **Clone** or download this repository  
2. Open `deauth_all.ino` in **Arduino IDE**  
3. Select your **ESP8266** board under **Tools > Board**  
4. Connect via **USB** and **upload**  
5. Open **Serial Monitor** to observe logs  

---

## How It Works ⚙️

- Sets the ESP8266 to **promiscuous mode** to sniff all WiFi frames  
- **Crawls** through channels every 100ms, capturing AP BSSIDs  
- Stores AP info (MAC, channel) dynamically  
- Sends **deauth frames** repeatedly to all detected APs  
- **Loops infinitely**, causing network disruption  

---

## Warning ⚠️

- **This code is for educational/emergency testing** only.  
- **Using jammers illegally** can violate laws and regulations.  
- **Use responsibly** and only on networks you own or have permission to test.  

---

## License & Disclaimer 📜

This project is **for educational purposes only**.  
The author **disclaims any liability** from misuse or legal consequences.

---

## **Stay Safe & Ethical!** 🔒

---

*Note:* For colors in environments supporting ANSI escape sequences, consider adding escape codes in the `Serial.println()` statements for a more colorful display. `Serial.println(COLOR_RED "Red Text" COLOR_RESET);`
