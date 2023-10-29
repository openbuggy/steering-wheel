![wheel-front](https://github.com/openbuggy/steering-wheel/assets/19519902/9fcb5b38-6508-4717-b860-844cdd910eb8)

## parts
- raspberry pico
- rotary encoder lpd3806 ([amazon](https://www.amazon.de/dp/B015GYY7XU))
- flange bearing 10 mm ([ebay](https://www.ebay.de/itm/264931573128))
- 3d printed: wheel, steering column, case, case lid

## 3d printed parts
all parts are designed with [CadQuery](https://github.com/CadQuery/cadquery) - see `parts.py` in parts directory. the ready to print stl files are also in the parts directory.

## firmware
the firmware is based on the raspberry pico hid device example with tinyusb, but also reads the rotary encoder via interrupt handlers.

## webhid demo
run `python3 -m http.server` from the root directory of the repo and visit `localhost:8000/demo.html`. click on "connect steering wheel" and choose the "openbuggy steering-wheel". then you should see the steering values. 

## more pictures
![wheel-case](https://github.com/openbuggy/steering-wheel/assets/19519902/ba77604f-7dbd-437d-904e-ef4475d7f6e8)

![wheel-inner](https://github.com/openbuggy/steering-wheel/assets/19519902/99bbef49-43e6-4136-8295-8a79e2f21b2a)
