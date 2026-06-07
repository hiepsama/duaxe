# Tổng Quan Dự Án

## 1. Mô tả chung

Dự án là một game đua xe 3D được xây dựng bằng C++ với OpenGL, GLFW, GLAD và GLM. Đây là một bài tập phù hợp với phạm vi học phần Đồ họa máy tính vì thể hiện được các nội dung nền tảng như dựng hình 3D, phép biến đổi ma trận, camera bám theo đối tượng, chiếu sáng cơ bản và kiểm tra va chạm.

Trò chơi được thiết kế theo kiểu endless runner: xe di chuyển trên một trục chính, chướng ngại vật xuất hiện ngẫu nhiên phía trước, người chơi điều khiển xe để né tránh và ghi điểm theo quãng đường đã đi.

## 2. Cấu trúc chính

- `main.cpp`: khởi tạo cửa sổ, shader, vòng lặp render, sinh vật cản, tính điểm và trạng thái game.
- `Car.cpp` / `Car.h`: xử lý điều khiển xe, cập nhật tốc độ, hướng di chuyển, dựng mô hình xe từ nhiều khối lập phương và kiểm tra va chạm.
- `Camera.cpp` / `Camera.h`: camera góc nhìn thứ ba bám theo xe.
- `Shader.h`: lớp tiện ích để đọc, biên dịch và liên kết shader.
- `shader.vs` / `shader.fs`: shader đỉnh và shader mảnh cho chiếu sáng Phong cơ bản.
- `build.ps1` / `setup.ps1`: hỗ trợ thiết lập và biên dịch trong môi trường Windows.

## 3. Nhận xét

### Điểm mạnh

- Có đủ vòng lặp game cơ bản: điều khiển, cập nhật, render, va chạm và game over.
- Mô hình xe được ghép từ nhiều khối hình học, thể hiện rõ kỹ thuật dựng hình 3D đơn giản.
- Có ánh sáng, đổ bóng đơn giản và camera theo sau, giúp bài toán gần với một game hoàn chỉnh hơn là demo kỹ thuật.
- Cấu trúc mã chia theo lớp khá rõ ràng, dễ mở rộng thêm tính năng.

### Hạn chế

- Phần hình ảnh còn dùng nhiều khối hộp, chưa có texture hoặc tài nguyên nghệ thuật riêng.
- Va chạm hiện mới ở mức AABB đơn giản, chưa xét quay xe hay va chạm chính xác hơn.
- Chưa có giao diện menu, HUD trực quan, âm thanh hoặc hiệu ứng đặc biệt.
- Logic điều khiển, spawn vật cản và render vẫn còn tập trung nhiều trong `main.cpp`, nên khi mở rộng sẽ khó quản lý hơn.

## 4. Hướng phát triển

Trong các giai đoạn tiếp theo, dự án có thể được mở rộng theo bốn hướng chính để tăng tính hoàn thiện cả về kỹ thuật lẫn trải nghiệm người dùng.

Thứ nhất, cần hoàn thiện gameplay bằng cách bổ sung menu bắt đầu, chức năng tạm dừng, giao diện game over rõ ràng hơn, hệ thống điểm cao và cơ chế tăng độ khó theo quãng đường hoặc thời gian.

Thứ hai, cần nâng chất lượng đồ họa thông qua việc thêm texture cho đường, xe và vật cản; xây dựng skybox hoặc nền trời phù hợp; đồng thời điều chỉnh ánh sáng môi trường để khung cảnh tự nhiên và sinh động hơn.

Thứ ba, cần tối ưu cấu trúc mã nguồn bằng cách tách các thành phần như `GameState`, `ObstacleManager`, `Renderer` và `HUD` ra khỏi `main.cpp` để chương trình dễ mở rộng, dễ bảo trì và rõ trách nhiệm hơn. Nếu số lượng vật cản tăng nhiều, có thể áp dụng object pooling để giảm chi phí tạo và hủy đối tượng.

Thứ tư, cần chuẩn bị tốt hơn cho phần báo cáo học phần bằng cách bổ sung README mô tả công nghệ sử dụng, cách điều khiển, cách build và chạy chương trình, cùng với một số ảnh chụp màn hình hoặc video demo minh họa.

## 5. Kết luận

Đây là một project phù hợp với mục tiêu học phần Đồ họa máy tính vì đã thể hiện đầy đủ các thành phần cốt lõi của một ứng dụng OpenGL tương tác. Nếu tiếp tục phát triển theo các hướng trên, dự án có thể chuyển từ một demo kỹ thuật sang một trò chơi học thuật hoàn chỉnh hơn, vừa có tính trực quan vừa có giá trị trình bày trong báo cáo.
