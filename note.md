## Stuff
- Embed a short wav file for the mic input
- Embed a song for buzzer and play when speaker run

## Phước
### MQTT topics
#### sensor_state
- direction: client -> server
- topic: device_state
- payload:
```json
{
    "temp": 30, // float, temperature in C
    "humid": 30, // float, humidity in %
    "food": 10, // float, food left in kg
    "water": 10, // float, water left in kg
}
```

#### add_image
- direction: client -> server
- topic: add_image
- payload: link to image

#### add_video
- direction: client -> server
- topic: add_video
- payload: link to video

#### write_feed_time
- direction: server -> client
- topic: write_feed_time
- payload:
```json
[
    {
        "time": "10:10", .// string, time hour:minute
        "audio_url": "", // string, audio file url, fake
        "amount": 20, // integer, food amount in gram
        "duration": 30, // integer, feed duration in minute
    },
]
```

#### feed_now
- direction: server -> client
- topic: feed_now
- payload: 1

#### restart
- direction: server -> client
- topic: restart
- payload: 1

#### log
- direction: client -> server
- topic: restart
- payload: string, log

#### dev_info
- direction: client -> server
- topic: dev_info
- payload:
```json
{
    "software": "0.10.0-alpha", // string, software version
    "ip": "192.168.1.2", // string, client ip addr
    "board": "esp32", // string, client board
    "wifi": "Wokwi-GUEST", // string, current connected wifi
}
```

#### time_eat
- direction: client -> server
- topic: time_eat
- payload: number, time eat in second

#### call
- direction: server -> client
- topic: call
- payload: url to receive stream