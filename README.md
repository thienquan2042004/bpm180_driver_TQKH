# Tài liệu hướng dẫn sử dụng Driver cảm biến BMP180

*Thành viên thực hiện:*
•	Huỳnh Thiện Quân – MSSV: 22146382
•	Quách Đình Tuấn – MSSV: 22146444
•	Phạm Công Khanh – MSSV: 22146329
•	Nguyễn Quang Huy – MSSV: 22146314
 

---

## 1. Giới thiệu

Tài liệu này trình bày mã nguồn và hướng dẫn sử dụng driver nhân Linux cho cảm biến áp suất và nhiệt độ *BMP180*, sử dụng giao tiếp *I2C*. Driver được thiết kế để vận hành trên các thiết bị như Raspberry Pi, cho phép đọc và xử lý dữ liệu nhiệt độ, áp suất khí quyển, cũng như tính toán độ cao so với mực nước biển.

Driver hỗ trợ giao tiếp với không gian người dùng (user space) thông qua *character device* và các lệnh *ioctl*, cho phép người dùng lựa chọn chế độ đo (nhiệt độ, áp suất, độ cao) và thiết lập tần suất lấy mẫu phù hợp với yêu cầu ứng dụng thực tế.

---

## 2. Thông số kỹ thuật cảm biến

Nguồn cấp: 1.8V – 3.6V  

- Địa chỉ I2C mặc định: 0x77  
Dải đo áp suất: 30.000 – 110.000 Pa  
Độ phân giải: 1 Pa / 0.1°C  
Độ chính xác tuyệt đối: ±100 Pa / ±1.0°C  
Độ chính xác tương đối: ±12 Pa / ±1.0 m  


---

## 3. Tính năng driver

Tạo thiết bị ký tự (character device) cho phép giao tiếp với BMP180.

- Hỗ trợ nhiều mức oversampling_setting để điều chỉnh độ chính xác.
Cung cấp các chức năng:

  - Đọc ID cảm biến.
  - Đo và xử lý dữ liệu nhiệt độ (°C).
  - Đo áp suất khí quyển (Pa).
  - Tính toán độ cao (m) từ mức biển.
Giao tiếp user space để người dùng:

  - Chọn chế độ đo (nhiệt độ / áp suất / độ cao).
  - Chọn chu kỳ lấy mẫu.
  - Hiển thị kết quả đo trên màn hình.

---

## 4. Cài đặt và sử dụng

### 4.1 Kiểm tra kết nối cảm biến

sudo i2cdetect -y 1

Nếu thấy địa chỉ 0x77, cảm biến đã được nhận. Nếu thấy UU, hãy hủy liên kết driver đang chiếm dụng:

echo 1-0077 | sudo tee /sys/bus/i2c/devices/1-0077/driver/unbind

### 4.2 Thiết lập Device Tree

sudo dts -I dts -O dtb -o bcm2710-rpi-zero-2-w.dtb bcm2710-rpi-zero-2-w.dts
sudo dtc -I dts -O dtb -o bcm2710-rpi-zero-2-w.dtb bcm2710-rpi-zero-2-w.dts


Thêm node cấu hình vào file .dts:

dts
bmp180@77 {
    compatible = "bosch,bmp180";
    reg = <0x77>;
};

### 4.3 Biên dịch và nạp module

make
sudo insmod bmp180_driver.ko

### 4.4 Kiểm tra kernel log

dmesg | tail

### 4.5 Chạy chương trình test

gcc bmp180_test.c -o run
sudo ./run
## 6. Tài liệu tham khảo

BMP180 Datasheet từ Bosch Sensortec
