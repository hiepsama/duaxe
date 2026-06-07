# Racing Game 3D

Game đua xe 3D sử dụng C++, OpenGL, GLFW, GLAD và GLM. Dự án được xây dựng theo kiểu endless runner: người chơi điều khiển xe chạy trên đường thẳng, tránh chướng ngại vật và tích lũy điểm theo quãng đường.

## Tính năng chính

- Camera góc nhìn thứ ba bám theo xe.
- Xe được ghép từ nhiều khối hình học 3D.
- Chướng ngại vật sinh ngẫu nhiên theo làn đường.
- Chiếu sáng Phong cơ bản với shader tự viết.
- Màn hình bắt đầu, tạm dừng và game over.
- Điểm cao được lưu vào `highscore.txt`.
- Độ khó tăng dần theo tiến trình chơi.

## Điều khiển

- `W`: tăng tốc.
- `S`: giảm tốc / lùi.
- `A`: rẽ trái.
- `D`: rẽ phải.
- `Enter`: bắt đầu game từ màn hình chờ.
- `P`: tạm dừng hoặc tiếp tục.
- `R`: chơi lại sau khi thua hoặc khi đang tạm dừng.
- `Esc`: thoát game.

## Cách build

Trên Windows, chạy:

```powershell
.\build.ps1
```

Script sẽ biên dịch ra file `RacingGame.exe` trong thư mục gốc của dự án.

## Cách chạy

Sau khi build thành công, chạy:

```powershell
.\RacingGame.exe
```

## Ghi chú

- Cấu hình thư viện được đặt trong thư mục `include/` và `lib/`.
- Nếu chưa có môi trường MinGW tương thích, bạn cần cập nhật lại đường dẫn trong `build.ps1`.
