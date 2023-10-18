#include "bsp/board.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "tusb.h"
#include "hardware/adc.h"


struct hid_report {
  uint16_t steering;
  uint16_t throttle;
};

volatile struct hid_report report = {.steering = 10000};

bool throttle_callback(struct repeating_timer* t) {
  report.throttle = adc_read();
  return true;
}

void pulse_callback(uint gpio, uint32_t events) {
  if ((gpio == 4 && ((events == GPIO_IRQ_EDGE_RISE && gpio_get(5)) ||
                     (events == GPIO_IRQ_EDGE_FALL && !gpio_get(5)))) ||
      (gpio == 5 && ((events == GPIO_IRQ_EDGE_RISE && !gpio_get(4)) ||
                     (events == GPIO_IRQ_EDGE_FALL && gpio_get(4))))) {
    report.steering--;
  } else {
    report.steering++;
  }
}

void tud_mount_cb(void) {}
void tud_umount_cb(void) {}
// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
  (void)remote_wakeup_en;
}
void tud_resume_cb(void) {}

uint16_t tud_hid_get_report_cb(uint8_t instance,
                               uint8_t report_id,
                               hid_report_type_t report_type,
                               uint8_t* buffer,
                               uint16_t reqlen) {
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;
  return 0;
}

void tud_hid_set_report_cb(uint8_t instance,
                           uint8_t report_id,
                           hid_report_type_t report_type,
                           uint8_t const* buffer,
                           uint16_t bufsize) {
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)bufsize;
}

void hid_task(void) {
  const uint32_t interval_ms = 1000 / 100;
  static uint32_t start_ms = 0;
  if (board_millis() - start_ms < interval_ms)
    return;
  start_ms += interval_ms;

  if (tud_suspended()) {
    tud_remote_wakeup();
  } else {
    if (!tud_hid_ready())
      return;
    tud_hid_report(1, &report, sizeof(report));
  }
}

int main() {
  gpio_init(4);
  gpio_init(5);
  gpio_set_dir(4, GPIO_IN);
  gpio_set_dir(5, GPIO_IN);
  gpio_pull_up(4);
  gpio_pull_up(5);
  gpio_set_irq_enabled_with_callback(4, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                     true, &pulse_callback);
  gpio_set_irq_enabled(5, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
  
  adc_init();
  adc_gpio_init(28);
  adc_select_input(2);
  struct repeating_timer timer;
  add_repeating_timer_ms(-1000 / 100, throttle_callback, NULL, &timer);
  
  stdio_init_all();
  tusb_init();

  while (1) {
    tud_task();
    hid_task();
  }
}
