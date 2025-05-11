// Huỳnh Thiện Quân 22146382
// Quách Đình Tuấn 22146444
// Phạm Công Khanh 22146329
// Nguyễn Quang Huy 22146314

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

// Đường dẫn đến thiết bị BMP180 đã đăng ký trong /dev
#define DEVICE_PATH "/dev/bmp180"

// Định nghĩa mã ioctl (magic number) cho thiết bị BMP180
#define BMP180_IOCTL_MAGIC 'b'

// Các mã lệnh ioctl để giao tiếp với driver kernel
#define BMP180_IOCTL_TEMPERATURE _IOR(BMP180_IOCTL_MAGIC, 1, int) // Lệnh đọc nhiệt độ
#define BMP180_IOCTL_PRESSURE _IOR(BMP180_IOCTL_MAGIC, 2, int)    // Lệnh đọc áp suất
#define BMP180_IOCTL_SET_OSS _IOW(BMP180_IOCTL_MAGIC, 3, int)     // Lệnh thiết lập hệ số lấy mẫu OSS
#define BMP180_IOCTL_ALTITUDE _IOR(BMP180_IOCTL_MAGIC, 4, int)    // Lệnh đọc độ cao (nếu được hỗ trợ trong driver)

int main(int argc, char *argv[]) {
    int fd;
    int oss = 3; // Giá trị mặc định cho hệ số lấy mẫu (Oversampling Setting)
    int temp_raw, pressure_raw; // Biến lưu giá trị nhiệt độ và áp suất thô
    float temperature, pressure; // Biến lưu giá trị sau khi chuyển đổi sang đơn vị có ý nghĩa

    // Nếu có đối số dòng lệnh, sử dụng nó làm OSS
    if (argc == 2) {
        oss = atoi(argv[1]); // Chuyển đối số thành số nguyên
        if (oss < 0 || oss > 3) {
            fprintf(stderr, "OSS must be between 0 and 3\n"); // Kiểm tra OSS hợp lệ
            return 1;
        }
    }

    // Mở thiết bị BMP180
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device"); // Báo lỗi nếu không mở được
        return 1;
    }

    // Gửi lệnh ioctl để thiết lập OSS
    if (ioctl(fd, BMP180_IOCTL_SET_OSS, &oss) < 0) {
        perror("Failed to set OSS"); // Báo lỗi nếu không thiết lập được
        close(fd);
        return 1;
    }

    // Gửi lệnh ioctl để đọc nhiệt độ thô từ cảm biến
    if (ioctl(fd, BMP180_IOCTL_TEMPERATURE, &temp_raw) < 0) {
        perror("Failed to get temperature"); // Báo lỗi nếu không đọc được
        close(fd);
        return 1;
    }
    temperature = temp_raw / 10.0; // Chuyển đổi sang độ C (giả định driver trả về nhiệt độ x10)
    printf("Temperature: %.1f °C\n", temperature); // In kết quả

    // Gửi lệnh ioctl để đọc áp suất thô từ cảm biến
    if (ioctl(fd, BMP180_IOCTL_PRESSURE, &pressure_raw) < 0) {
        perror("Failed to get pressure"); // Báo lỗi nếu không đọc được
        close(fd);
        return 1;
    }
    pressure = pressure_raw / 1000.0; // Chuyển đổi sang đơn vị kPa
    printf("Pressure: %.3f kPa\n", pressure); // In kết quả

    // Đóng thiết bị
    close(fd);
    return 0;
}
