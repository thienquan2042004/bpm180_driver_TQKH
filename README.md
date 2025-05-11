# Tài liệu hướng dẫn sử dụng Driver cảm biến BMP180

*Thành viên thực hiện:*
•	Huỳnh Thiện Quân – MSSV: 22146382

•	Quách Đình Tuấn – MSSV: 22146444

•	Phạm Công Khanh – MSSV: 22146329

•	Nguyễn Quang Huy – MSSV: 22146314
 

---

## 1. Giới thiệu

Driver BMP180 là một trình điều khiển thiết bị nhân Linux dành cho cảm biến áp suất và nhiệt độ BMP180 của Bosch. Driver này cho phép hệ điều hành Linux giao tiếp với cảm biến thông qua giao thức I2C để thu thập dữ liệu môi trường, hỗ trợ các ứng dụng về đo độ cao, khí tượng, và kiểm soát môi trường.

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
