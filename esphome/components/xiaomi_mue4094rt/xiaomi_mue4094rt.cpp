#include "xiaomi_mue4094rt.h"
#include "esphome/core/log.h"

#ifdef ARDUINO_ARCH_ESP32

namespace esphome {
namespace xiaomi_mue4094rt {

static const char *TAG = "xiaomi_mue4094rt";

void XiaomiMUE4094RT::dump_config() {
  ESP_LOGCONFIG(TAG, "Xiaomi MUE4094RT");
  LOG_SENSOR("  ", "Motion", this->motion_);
}

bool XiaomiMUE4094RT::parse_device(const esp32_ble_tracker::ESPBTDevice &device) {
  if (device.address_uint64() != this->address_) {
    ESP_LOGVV(TAG, "parse_device(): unknown MAC address.");
    return false;
  }
  ESP_LOGVV(TAG, "parse_device(): MAC address %s found.", device.address_str().c_str());

  bool success = false;
  for (auto &service_data : device.get_service_datas()) {
    auto res = xiaomi_ble::parse_xiaomi_header(service_data);
    if (!res.has_value()) {
      continue;
    }
    if (res->is_duplicate) {
      continue;
    }
    if (res->has_encryption) {
      ESP_LOGVV(TAG, "parse_device(): payload decryption is currently not supported on this device.");
      continue;
    }
    if (!(xiaomi_ble::parse_xiaomi_message(service_data.data, *res))) {
      continue;
    }
    if (!(xiaomi_ble::report_xiaomi_results(res, device.address_str()))) {
      continue;
    }
    if (res->motion.has_value() && this->motion_ != nullptr)
      this->motion_->publish_state(*res->motion);
    success = true;
  }

  if (!success) {
    return false;
  }

  return true;
}

}  // namespace xiaomi_mue4094rt
}  // namespace esphome

#endif
