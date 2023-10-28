#include "bsp/board.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "tusb.h"

struct hid_report {
  uint16_t steering;
};

static volatile int16_t steering = 0;

void pulse_callback(uint gpio, uint32_t events) {
  if ((gpio == 2 && ((events == GPIO_IRQ_EDGE_RISE && gpio_get(3)) ||
                     (events == GPIO_IRQ_EDGE_FALL && !gpio_get(3)))) ||
      (gpio == 3 && ((events == GPIO_IRQ_EDGE_RISE && !gpio_get(2)) ||
                     (events == GPIO_IRQ_EDGE_FALL && gpio_get(2))))) {
    steering--;
  } else {
    steering++;
  }
}

void tud_mount_cb(void) {}
void tud_umount_cb(void) {}
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
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;
  if (board_millis() - start_ms < interval_ms)
    return;
  start_ms += interval_ms;

  if (tud_suspended()) {
    tud_remote_wakeup();
  } else {
    if (!tud_hid_ready())
      return;
    const struct hid_report report = {.steering = steering };
    tud_hid_report(1, &report, sizeof(report));
  }
}

int main() {
  gpio_init(2);
  gpio_init(3);
  gpio_set_dir(2, GPIO_IN);
  gpio_set_dir(3, GPIO_IN);
  gpio_pull_up(2);
  gpio_pull_up(3);
  gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                     true, &pulse_callback);
  gpio_set_irq_enabled(3, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

  stdio_init_all();
  tusb_init();

  while (1) {
    tud_task();
    hid_task();
  }

  return 0;
}
