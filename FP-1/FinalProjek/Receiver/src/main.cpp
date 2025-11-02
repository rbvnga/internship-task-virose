#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <esp_err.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define MAC_ADDRESS_TOTAL 12
#define MAC_ADDRESS_LENGTH 6

esp_now_peer_info_t peer_info;

uint8_t mac_addresses[MAC_ADDRESS_TOTAL][MAC_ADDRESS_LENGTH] = {
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x10},  // Fauzan Firdaus
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x11},  // Africha Sekar wangi
    {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x10},  // Rafaina Erin Sadia
    {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x11},  // Antonius Michael Yordanis Hartono
    {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x10},  // Dinda Sofi Azzahro
    {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x11},  // Muhammad Fahmi Ilmi
    {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x10},  // Dhanishara Zaschya Putri Syamsudin
    {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x11},  // Irsa Fairuza
    {0x24, 0x0A, 0xC4, 0x0A, 0x14, 0x10},  // Revalinda Bunga Nayla Laksono
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x11},  // BISMA
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x10},  // JSON
    {0x24, 0x0A, 0xC4, 0x0A, 0x20, 0x11},  // FARUG
};

const char* mac_names[MAC_ADDRESS_TOTAL] = {
    "Fauzan Firdaus",                      // 0
    "Africha Sekar Wangi",                 // 1
    "Rafaina Erin Sadia",                  // 2
    "Antonius Michael Yordanis Hartono",   // 3
    "Dinda Sofi Azzahro",                  // 4
    "Muhammad Fahmi Ilmi",                 // 5
    "Dhanishara Zaschya Putri Syamsudin",  // 6
    "Irsa Fairuza",                        // 7
    "Revalinda Bunga Nayla Laksono",       // 8
    "BISMA",                               // 9
    "JSON",                                // 10
    "FARUG",                               // 11
};

typedef enum {
    FAUZAN_FIRDAUS = 0,
    AFRICHA_SEKAR_WANGI,
    RAFAINA_ERIN_SADIA,
    ANTONIUS_MICHAEL_YORDANIS_HARTONO,
    DINDA_SOFI_AZZAHRO,
    MUHAMMAD_FAHMI_ILMI,
    DHANISHARA_ZASCHYA_PUTRI_SYAMSUDIN,
    IRSA_FAIRUZA,
    REVALINDA_BUNGA_NAYLA_LAKSONO,
    BISMA,
    JSON,
    FARUG,
} ADDRESS_ASSIGNMENT;


typedef struct {
  char command; // karakter tunggal dari keyboard
} KeyboardMessage;

KeyboardMessage incoming;

// Callback saat data diterima
void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(&incoming, data, sizeof(incoming));
  char key = incoming.command;

  Serial.print("Perintah diterima: ");
  if (key == '\t') Serial.println("[TAB]");
  else if (key == ' ') Serial.println("[SPACE]");
  else Serial.println(key);

  // Kirim ke laptop / Webots
  Serial.write(key);
}

// Fungsi inisialisasi ESP-NOW
esp_err_t mulai_esp_now(int index_mac_address) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK)
    return ESP_FAIL;

  // Atur MACku yaitu indeks 8
  uint8_t mac[MAC_ADDRESS_LENGTH];
  memcpy(mac, mac_addresses[index_mac_address], MAC_ADDRESS_LENGTH);
  esp_wifi_set_mac(WIFI_IF_STA, mac);

  // Tambahkan semua peer
  memset(&peer_info, 0, sizeof(peer_info));
  peer_info.channel = 0;
  peer_info.encrypt = false;
  for (int i = 0; i < MAC_ADDRESS_TOTAL; i++) {
    memcpy(peer_info.peer_addr, mac_addresses[i], MAC_ADDRESS_LENGTH);
    esp_now_add_peer(&peer_info);
  }

  // Daftarkan callback
  esp_now_register_recv_cb(onReceive);

  return ESP_OK;
}

void setup() {
  Serial.begin(115200);
  int index_mac_address = 8; 
  if (mulai_esp_now(index_mac_address) == ESP_OK)
    Serial.println("Receiver siap menerima perintah...");
  else
    Serial.println("Gagal inisialisasi ESP-NOW!");
}

void loop() {
}