# ESP32 Flame Sensor Alarm

An ESP32 watches an IR flame sensor module. When it sees flame, it lights the on-board LED and sounds a passive piezo buzzer. The sensor's analog output is logged to the serial port (115200 baud) every 500 ms. Its `AO:<value>` format works with the Serial Plotter.

## Parts

- 1 × ESP32 DevKit - [https://amzn.to/4mJzOYi](https://amzn.to/4mJzOYi)
- 1 × IR flame sensor module - [https://amzn.to/4lxRSUl](https://amzn.to/4lxRSUl)
- 1 × passive piezo buzzer module (needs a PWM tone, not plain DC) - [https://amzn.to/45ZOEUU](https://amzn.to/45ZOEUU)

## Wiring

Both modules run on 3.3 V.

| Module pin | ESP32 |
|---|---|
| Flame sensor DO | GPIO23 |
| Flame sensor AO | GPIO34 (optional, analog level) |
| Buzzer signal | GPIO22 |
| VCC / GND | 3.3 V / GND |

## Build

Open `flame_detector/flame_detector.ino`, select your ESP32 board, and upload.

Flame modules differ on which logic level means "flame detected". If the alarm sounds with no flame present, change `FLAME_DETECTED` to `LOW` at the top of the sketch. Turn the module's trim pot to set the detection threshold.
