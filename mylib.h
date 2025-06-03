#ifndef MYLIB_H_
#define MYLIB_H_

#include <windows.h> // Thư viện cần thiết cho các hàm xử lý console trên Windows

// Hàm di chuyển con trỏ đến vị trí (x, y) trên màn hình console
void di_den_XY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Hàm đặt màu cho chữ viết
// Các mã màu phổ biến:
// 0 = Black      8 = Gray
// 1 = Blue       9 = Light Blue
// 2 = Green      10 = Light Green
// 3 = Aqua       11 = Light Aqua
// 4 = Red        12 = Light Red
// 5 = Purple     13 = Light Purple
// 6 = Yellow     14 = Light Yellow
// 7 = White      15 = Bright White
void chinh_mau_sac(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Hàm đặt màu cho chữ viết (tên gọi khác, hoạt động tương tự chinh_mau_sac)
void mau_chu(int color) {
    chinh_mau_sac(color);
}


// Hàm ẩn hoặc hiện con trỏ console
// state = 0: Ẩn con trỏ
// state = 1: Hiện con trỏ
void hien_thi_trang_thai(int state) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = state;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

#endif // MYLIB_H_#pragma once
