# C++ Tetris

Một phiên bản **Tetris đơn giản** được viết bằng **C++**, chạy trực tiếp trên **console**, kèm **hiệu ứng âm thanh `.wav`**.

---

## 📌 Giới thiệu

Dự án mô phỏng trò chơi **Tetris cổ điển** bằng ngôn ngữ **C++**.  
Người chơi điều khiển các khối rơi, xoay và sắp xếp chúng để hoàn thành các hàng và ghi điểm.

Project được thực hiện với mục đích **học tập và thực hành C++**, tập trung vào:

- Logic game  

- Xử lý bàn chơi  

- Điều khiển từ bàn phím  

- Âm thanh cơ bản  

---

## 🎮 Tính năng

- Các khối Tetris rơi tự động 

- Xoay và di chuyển khối  

- Xóa hàng khi lấp đầy  

- Hiệu ứng âm thanh:

  - Ghi điểm  

  - Hard drop  

  - Game over  

- Chạy trực tiếp trên console  

---

## 📁 Cấu trúc thư mục

```text
cpp_Tetris
├── .gitignore
├── README.md
├── libgcc_s_seh-1.dll
├── libstdc++-6.dll
├── libwinpthread-1.dll
├── main.cpp
├── main.exe
├── sfx_gameOver.wav
├── sfx_hardDrop.wav
└── sfx_score.wav
```

## 🛠️ Yêu cầu

- Trình biên dịch C++ (g++, clang hoặc MSVC)

- Hệ điều hành Windows

- Console hỗ trợ nhập phím

## ⚙️ Cách tải

```bash
git clone https://github.com/QZinhh26/cpp_Tetris.git C:\Users\Username\Downloads\tetris
```
> *(Lưu ý: repo mặc định được lưu ở folder download. Có thể thay đổi vị trí lưu repo bằng cách thay đổi đường dẫn `C:\Users\Username\Downloads\tetris`)*

## ▶️ Cách chạy

```bash
cd C:\Users\Username\Downloads\tetris
.\main.exe
```

## ⌨️ Điều khiển (tham khảo)

- `← / →` : Di chuyển trái / phải
- `↑` : Xoay khối
- `↓` : Rơi nhanh
- `Space` : Hard drop
- `Esc` : Thoát game

> *(Lưu ý: điều khiển cụ thể xem trong menu game)*

## 🔊 Âm thanh

- `sfx_score.wav` - ghi điểm
- `sfx_hardDrop.wav` – hard drop
- `sfx_gameOver.wav` – game over

## 👨‍💻 Tác giả
- **Nguyễn Quang Vinh - 24522020**

- **Nguyễn Đức Trọng - 24521864**

- **Phạm Nguyễn Trung Trính - 24521860**

## 📌 Ghi chú
-  Hợp cho sinh viên học C++

- Có thể mở rộng thêm GUI, menu, lưu điểm cao

## 📜 License
Sử dụng cho **mục đích học tập và phi thương mại.**
