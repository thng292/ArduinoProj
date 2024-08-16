## Stuff

- Embed a short wav file for the mic input
- Embed a song for buzzer and play when speaker run

## Phước

### MQTT topics

#### sensor_state

- Cập nhật các trạng thái sensor
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

- Thêm ảnh vào thư viện
- direction: client -> server
- topic: add_image
- payload: link to image

#### add_video

- Thêm video vào thư viện
- direction: client -> server
- topic: add_video
- payload: link to video

#### request_feed_time

- Máy yêu cầu server gửi lịch cho ăn xuống máy
- direction: client -> server
- topic: request_feed_time
- payload: None

#### write_feed_time

- Server sẽ lưu lịch cho ăn, mỗi lần cập nhật thì gửi lại tất cả xuống client, client sẽ ghi đè bằng dữ liệu mới
- direction: server -> client
- topic: write_feed_time
- payload: `{10 10 http://abcd/asb 20 30}{10 10 http://abcd/asb 20 30}{10 10 http://abcd/asb 20 30}`
- Note:
nhớ gửi cho đúng định dạng, mỗi trường phân cách bằng dấu cách, mỗi thời gian nằm trong cặp {}

```json
    {
        "hour": 10, // integer, time hour
        "minute": 10, // integer, time minute
        "audio_url": "", // string, audio file url, fake
        "amount": 20, // integer, food amount in gram
        "duration": 30, // integer, feed duration in minute
    },
```

#### feed_now

- Server gửi lệnh cho client nhả thức ăn ngay
- direction: server -> client
- topic: feed_now
- payload: 1

#### restart

- Server gửi lệnh cho client khởi động lại
- direction: server -> client
- topic: restart
- payload: None

#### log

- Client gửi log lên để server lưu lại
- direction: client -> server
- topic: restart
- payload: string, log

#### dev_info

- Client gửi cho server trạng thái hiện tại
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

#### get_dev_info

- Server yêu cầu client gửi trạng thái hiện tại
- direction: server -> client
- topic: get_dev_info
- payload: None

#### time_eat

- Client gửi cho server thời gian ăn của thú
- direction: client -> server
- topic: time_eat
- payload: number, time eat in second

#### water_added

- Client gửi cho server lượng nước đã nhả
- direction: client -> server
- topic: water_added
- payload: number, gram water added

#### call

- Server yêu cầu client thực hiện cuộc gọi video
- direction: server -> client
- topic: call
- payload: url to receive stream
