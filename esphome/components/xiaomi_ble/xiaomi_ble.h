#pragma once

#include "esphome/core/component.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/core/helpers.h"

#ifdef ARDUINO_ARCH_ESP32

namespace esphome {
namespace xiaomi_ble {

struct XiaomiParseResult {
  enum { TYPE_LYWSDCGQ, TYPE_HHCCJCY01, TYPE_LYWSD02, TYPE_CGG1, TYPE_LYWSD03MMC } type;
  optional<float> temperature;
  optional<float> humidity;
  optional<float> battery_level;
  optional<float> conductivity;
  optional<float> illuminance;
  optional<float> moisture;
};

struct XiaomiAESVector {
  uint8_t key[16];
  uint8_t plaintext[16];
  uint8_t ciphertext[16];
  uint8_t authdata[16];
  uint8_t iv[16];
  uint8_t tag[16];
  size_t keysize;
  size_t authsize;
  size_t datasize;
  size_t tagsize;
  size_t ivsize;
};

bool parse_xiaomi_data_byte(uint8_t data_type, const uint8_t *data, uint8_t data_length, XiaomiParseResult &result);
bool decrypt_xiaomi_payload(std::vector<uint8_t> &raw, const uint8_t *bindkey);

optional<XiaomiParseResult> parse_xiaomi(const esp32_ble_tracker::ESPBTDevice &device);

class XiaomiListener : public esp32_ble_tracker::ESPBTDeviceListener {
 public:
  bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;
};

}  // namespace xiaomi_ble
}  // namespace esphome

#endif
