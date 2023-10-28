#define REPORT_DESC_STEERING_WHEEL(...)                    \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                \
        HID_USAGE(HID_USAGE_DESKTOP_GAMEPAD),              \
        HID_COLLECTION(HID_COLLECTION_APPLICATION),        \
        __VA_ARGS__                                        \
        HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),            \
        HID_LOGICAL_MIN(0x0000),                           \
        HID_LOGICAL_MAX(0xffff),                           \
        HID_USAGE(HID_USAGE_DESKTOP_X),                    \
        HID_REPORT_COUNT(1),                               \
        HID_REPORT_SIZE(16),                               \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
        HID_COLLECTION_END
