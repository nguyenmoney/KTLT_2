// --- KHAI BAO CAC THU VIEN ---
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <time.h>

// ---KHAI BAO HANG SO PI---
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---KHAI BAO TEP GHI CHEP PHIEN LAM VIEC---
#define TEN_TEP_LOG "tichxuan_log.txt"

// --- DINH NGHIA CAU TRUC DU LIEU ---
typedef struct {
    double he_so;
    int bac;
} SoHang; //Cau truc cua 1 so hang

typedef struct {
    SoHang* cac_so_hang;
    int so_luong_so_hang;
    int kich_thuoc_da_cap_phat;
} DaThuc; //Cau truc cua 1 da thuc

typedef struct {
    double a;
    double b;
} HamLuongGiac; //Cau truc cua 1 ham luong giac

typedef struct {
    int n;
} HamCanThuc; //Cau truc cua 1 ham can thuc

typedef enum {
    DA_THUC,
    LUONG_GIAC,
    CAN_THUC
} LoaiHam; //Phan loai cac dang ham

typedef union {
    DaThuc daThuc;
    HamLuongGiac hamLuongGiac;
    HamCanThuc hamCanThuc;
} DuLieuHam; //Du lieu cua cac loai ham

typedef struct {
    LoaiHam loai;
    DuLieuHam duLieu;
} HamSo; //Cau truc cua 1 ham so, co the la da thuc, luong giac, hoac can thuc

// --- BIEN TOAN CUC ---
HamSo hamSoToanCuc;
int toaDoXMenu = 25;
int toaDoYMenu = 7;
int soChuSoThapPhan;
int chieuRongMenu = 70;
int chieuCaoMucMenu = 2;
int dongThongBaoY = 50;
FILE* g_tep_log = NULL;

// --- NGUYEN MAU HAM ---
void di_den_XY(int x, int y);
void chinh_mau_sac(int color);
void mau_chu(int color);
void hien_thi_trang_thai(int state);
void clear_stdin();
bool la_so_chan(int n);
void mo_tep_log();
void dong_tep_log();
void ghi_vao_log(const char* thong_diep);
void khoi_tao_da_thuc(DaThuc* dt);
void giai_phong_da_thuc(DaThuc* dt);
int them_so_hang_vao_da_thuc(DaThuc* dt, double hs, int b);
bool xau_sang_daThuc(const char* str, DaThuc* dt);
void nhap_da_thuc(DaThuc* dt);
void hien_thi_da_thuc(DaThuc dt);
double tinh_gia_tri_da_thuc(DaThuc dt, double giaTriX);
bool xau_sang_hamLuongGiac(const char* str, HamLuongGiac* hlg);
void nhap_ham_luong_giac(HamLuongGiac* hlg);
bool xau_sang_hamCanThuc(const char* str, HamCanThuc* hct);
void nhap_ham_can_thuc(HamCanThuc* hct);
void hien_thi_ham_luong_giac(HamLuongGiac hlg);
void hien_thi_ham_can_thuc(HamCanThuc hct);
void hien_thi_ham_so(HamSo hs);
double tinh_gia_tri_ham_so(HamSo hs, double giaTriX);
double phuong_phap_hinh_thang(HamSo hs, double a, double b, int n);
double phuong_phap_simpson(HamSo hs, double a, double b, int n);
void xu_ly_gia_tri_tai_X(HamSo hs);
void xu_ly_hinh_thang(HamSo hs);
void xu_ly_simpson(HamSo hs);
void hien_thi_tieu_de_chuong_trinh();
void hien_thi_huong_dan_phim(int y_pos);
void ve_khung(int x, int y, int w, int h, int mauVien, int mauNenTrongChuTrong, const char* noiDung, bool canGiua);
void to_sang(int x, int y, int w, int h, int mauHighlightKetHop, int mauVienHighlight, const char* noiDung, bool canGiua);
void ve_them_vien(int x, int y, int w, int h, int mauVien, int mauNenChuTrong, int soLuong, const char* cacMucMenu[]);
void menu_chon_ham();
int menu_chon_chuc_nang();

// --- HAM MAIN ---
int main() {
    mo_tep_log();
    ghi_vao_log("Chuong trinh bat dau.");

    char luaChonTiepTuc[10];
    int luaChonChucNang = 0;

    menu_chon_ham();

    do {
        luaChonChucNang = menu_chon_chuc_nang();

        if (luaChonChucNang >= 1 && luaChonChucNang <= 3) {
            di_den_XY(0, dongThongBaoY);
            hien_thi_trang_thai(1);
            printf("Nhan phim bat ky de tiep tuc..."); 
            int ch = _getch();  
            (void)ch; 
            hien_thi_trang_thai(0);
        }

        if (luaChonChucNang == 0) { // Neu ESC duoc nhan trong menu_chon_chuc_nang (thoat menu do)
            strcpy(luaChonTiepTuc, "khong");
            ghi_vao_log("Nguoi dung chon thoat tu menu chuc nang (ESC).");
        }
        else if (luaChonChucNang == 4) { // Neu chon "4. Quay ve chon lai ham"
            strcpy(luaChonTiepTuc, "co"); // De vong lap main tiep tuc
        }
        else if (luaChonChucNang >= 1 && luaChonChucNang <= 3) { // Neu da thuc hien chuc nang 1, 2, hoac 3
            di_den_XY(0, dongThongBaoY + 1);
            hien_thi_trang_thai(1);
            printf("Ban co muon thuc hien chuc nang khac khong? (co/khong): ");
            fgets(luaChonTiepTuc, sizeof(luaChonTiepTuc), stdin);
            luaChonTiepTuc[strcspn(luaChonTiepTuc, "\n")] = '\0';
            for (int i = 0; luaChonTiepTuc[i]; i++) {
                luaChonTiepTuc[i] = tolower(luaChonTiepTuc[i]);
            }
            if (strcmp(luaChonTiepTuc, "co") != 0 && strcmp(luaChonTiepTuc, "c") != 0) {
                ghi_vao_log("Nguoi dung chon khong tiep tuc.");
            }
            else {
                ghi_vao_log("Nguoi dung chon tiep tuc.");
            }
        }
        else { // Truong hop khong xac dinh, hoac menu_chon_ham vua ket thuc lan dau
            strcpy(luaChonTiepTuc, "co"); // Mac dinh la tiep tuc de vao lai menu_chon_chuc_nang
        }
    } while (strcmp(luaChonTiepTuc, "co") == 0 || strcmp(luaChonTiepTuc, "c") == 0);

    if (hamSoToanCuc.loai == DA_THUC) {
        giai_phong_da_thuc(&hamSoToanCuc.duLieu.daThuc);
    }
    // *** SUA LOI MAN HINH TRANG KHI THOAT ***
    mau_chu(7); // Dat lai mau mac dinh TRUOC KHI xoa man hinh
    system("cls");
    // mau_chu(7); // Dong nay da co o tren, khong can thiet nua
    printf("Chuong trinh da ket thuc.\n");
    ghi_vao_log("Chuong trinh ket thuc.");
    dong_tep_log();
    return 0;
}

// --- DINH NGHIA CAC HAM ---
// ... (Cac ham con lai giu nguyen nhu trong phien ban ban da cung cap) ...
// (Toi se khong dan lai toan bo cac ham con o day de tiet kiem khong gian,
//  vi thay doi chi nam o cuoi ham main)

// LOG: Dinh nghia cac ham quan ly log
void mo_tep_log() {
    g_tep_log = fopen(TEN_TEP_LOG, "a");
    if (g_tep_log == NULL) {
        perror("Khong the mo tep log");
    }
    else {
        time_t baygio = time(NULL);
        char* thoi_gian_hien_tai = ctime(&baygio);
        if (thoi_gian_hien_tai[strlen(thoi_gian_hien_tai) - 1] == '\n') {
            thoi_gian_hien_tai[strlen(thoi_gian_hien_tai) - 1] = '\0';
        }
        fprintf(g_tep_log, "\n--- PHIEN LAM VIEC MOI: %s ---\n", thoi_gian_hien_tai);
        fflush(g_tep_log);
    }
}

void dong_tep_log() {
    if (g_tep_log != NULL) {
        fprintf(g_tep_log, "--- KET THUC PHIEN LAM VIEC ---\n\n");
        fflush(g_tep_log);
        fclose(g_tep_log);
        g_tep_log = NULL;
    }
}

void ghi_vao_log(const char* thong_diep) {
    if (g_tep_log != NULL) {
        time_t baygio = time(NULL);
        struct tm* tinfo = localtime(&baygio);
        char buffer_thoi_gian[20];
        strftime(buffer_thoi_gian, sizeof(buffer_thoi_gian), "%H:%M:%S", tinfo);
        fprintf(g_tep_log, "[%s] %s\n", buffer_thoi_gian, thong_diep);
        fflush(g_tep_log);
    }
}

// 1. Console & Utilities
void di_den_XY(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void chinh_mau_sac(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void mau_chu(int color) {
    chinh_mau_sac(color);
}

void hien_thi_trang_thai(int state) {
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
    cci.bVisible = state;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool la_so_chan(int n) {
    return n % 2 == 0;
}

// 2. DaThuc Helpers
void khoi_tao_da_thuc(DaThuc* dt) {
    dt->cac_so_hang = NULL;
    dt->so_luong_so_hang = 0;
    dt->kich_thuoc_da_cap_phat = 0;
}

void giai_phong_da_thuc(DaThuc* dt) {
    if (dt) {
        free(dt->cac_so_hang);
        khoi_tao_da_thuc(dt);
    }
}

void xoa_so_hang(DaThuc* dt, int index) {
    if (!dt || index < 0 || index >= dt->so_luong_so_hang) {
        return;
    }
    memmove(&dt->cac_so_hang[index], &dt->cac_so_hang[index + 1], (dt->so_luong_so_hang - index - 1) * sizeof(SoHang));
    dt->so_luong_so_hang--;
}

int them_so_hang_vao_da_thuc(DaThuc* dt, double hs, int b) {
    if (!dt) {
        return 0;
    }
    if (fabs(hs) < 1e-9) {
        return 1;
    }
    for (int i = 0; i < dt->so_luong_so_hang; i++) {
        if (dt->cac_so_hang[i].bac == b) {
            dt->cac_so_hang[i].he_so += hs;
            if (fabs(dt->cac_so_hang[i].he_so) < 1e-9) {
                xoa_so_hang(dt, i);
            }
            return 1;
        }
    }
    if (dt->so_luong_so_hang >= dt->kich_thuoc_da_cap_phat) {
        int new_size = dt->kich_thuoc_da_cap_phat == 0 ? 10 : dt->kich_thuoc_da_cap_phat * 2;
        SoHang* temp = realloc(dt->cac_so_hang, new_size * sizeof(SoHang));
        if (!temp) {
            ghi_vao_log("Loi: Realloc that bai trong them_so_hang_vao_da_thuc.");
            return 0;
        }
        dt->cac_so_hang = temp;
        dt->kich_thuoc_da_cap_phat = new_size;
    }
    dt->cac_so_hang[dt->so_luong_so_hang] = (SoHang){ hs, b };
    dt->so_luong_so_hang++;
    return 1;
}

bool xau_sang_daThuc(const char* str, DaThuc* dt) {
    const char* p = str;
    char log_buf[256];
    bool success = true;
    bool found_term = false;

    const char* temp_p_check_empty = str;
    while (isspace((unsigned char)*temp_p_check_empty)) {
        temp_p_check_empty++;
    }
    if (*temp_p_check_empty == '\0' && strlen(str) > 0) { // Neu la chuoi toan khoang trang (khong rong)
        return false; // Coi nhu loi
    }
    if (*temp_p_check_empty == '\0' && strlen(str) == 0) { // Chuoi rong ""
        return true; // Coi nhu da thuc 0, parse thanh cong
    }


    while (*p) {
        while (isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') {
            break;
        }
        double sign = 1.0;
        if (*p == '-') {
            sign = -1.0;
            p++;
        }
        else if (*p == '+') {
            p++;
        }
        while (isspace((unsigned char)*p)) {
            p++;
        }

        double coeff = 1.0;
        char* end;
        bool coeff_parsed_explicitly = false;
        if (isdigit((unsigned char)*p) || *p == '.') {
            coeff = strtod(p, &end);
            if (p != end) {
                coeff_parsed_explicitly = true;
            }
            p = end;
        }

        int degree = 0;
        if (tolower((unsigned char)*p) == 'x') {
            found_term = true;
            p++;
            degree = 1;
            if (*p == '^') {
                p++;
                if (isdigit((unsigned char)*p)) {
                    degree = strtol(p, &end, 10);
                    p = end;
                }
                else {
                    success = false; // Loi: x^ khong co so mu
                    break;
                }
            }
        }
        else if (coeff_parsed_explicitly) { // Neu chi co so ma khong co x
            found_term = true;
            degree = 0; // So hang tu do
        }
        else if (*p != '\0' && !isspace((unsigned char)*p)) { // Ky tu la, khong phai x, khong phai so
            success = false; // Loi dinh dang
            break;
        }


        if (!them_so_hang_vao_da_thuc(dt, sign * coeff, degree)) {
            sprintf(log_buf, "Loi: Them so hang that bai khi parse: hs=%.2f, bac=%d", sign * coeff, degree);
            ghi_vao_log(log_buf);
            return false; // Loi cap phat bo nho
        }
    }
    while (isspace((unsigned char)*p)) { // Bo qua khoang trang thua o cuoi
        p++;
    }
    if (*p != '\0') { // Van con ky tu khong mong muon o cuoi
        success = false;
    }

    // Neu chuoi khong rong, khong loi ro rang, nhung khong tim thay term nao (vi du chuoi "abc")
    if (!found_term && strlen(str) > 0 && *temp_p_check_empty != '\0') {
        success = false;
    }


    return success;
}

void nhap_da_thuc(DaThuc* dt) {
    char input_buffer[1024];
    khoi_tao_da_thuc(dt); // Luon khoi tao truoc
    printf("Nhap da thuc (vi du: 3x^2 - 5.1x + 9): ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    char log_msg[1100];
    sprintf(log_msg, "Nguoi dung nhap da thuc: \"%s\"", input_buffer);
    ghi_vao_log(log_msg);
    if (!xau_sang_daThuc(input_buffer, dt)) {
        printf("CANH BAO: Chuoi da thuc nhap khong hop le hoac loi dinh dang!\n");
        printf("         Da thuc se duoc coi la 0.\n");
        ghi_vao_log("Loi phan tich cu phap da thuc. Da thuc duoc dat la 0.");
    }
}

int so_sanh_so_hang(const void* a, const void* b) {
    return ((SoHang*)b)->bac - ((SoHang*)a)->bac;
}

void hien_thi_da_thuc(DaThuc dt) {
    printf("Da thuc la: ");
    if (dt.so_luong_so_hang == 0) {
        printf("0");
        return;
    }
    qsort(dt.cac_so_hang, dt.so_luong_so_hang, sizeof(SoHang), so_sanh_so_hang);
    for (int i = 0; i < dt.so_luong_so_hang; i++) {
        SoHang sh = dt.cac_so_hang[i];
        if (i > 0) {
            printf(sh.he_so >= 0 ? " + " : " - ");
        }
        else if (sh.he_so < 0) {
            printf("-");
        }
        double hs_abs = fabs(sh.he_so);
        if (fabs(hs_abs - 1.0) > 1e-9 || sh.bac == 0) {
            printf("%.2f", hs_abs);
        }
        if (sh.bac > 0) {
            printf("x");
            if (sh.bac > 1) {
                printf("^%d", sh.bac);
            }
        }
    }
}

double tinh_gia_tri_da_thuc(DaThuc dt, double giaTriX) {
    double ketQua = 0.0;
    for (int i = 0; i < dt.so_luong_so_hang; ++i) {
        ketQua += dt.cac_so_hang[i].he_so * pow(giaTriX, dt.cac_so_hang[i].bac);
    }
    return ketQua;
}

// 3. Other Function Helpers
bool xau_sang_hamLuongGiac(const char* str, HamLuongGiac* hlg) {
    hlg->a = 0.0;
    hlg->b = 0.0;
    const char* p = str;
    bool found_any_valid_term = false;
    bool parse_ok = true;

    const char* p_check_empty = str;
    while (isspace((unsigned char)*p_check_empty)) {
        p_check_empty++;
    }
    if (*p_check_empty == '\0') { // Chuoi rong hoac toan khoang trang
        return true; // Chap nhan la 0sin + 0cos
    }

    while (*p) {
        while (isspace((unsigned char)*p)) { p++; }
        if (*p == '\0') { break; }

        double sign = 1.0;
        if (*p == '-') { sign = -1.0; p++; }
        else if (*p == '+') { p++; }
        while (isspace((unsigned char)*p)) { p++; }

        double coeff = 1.0;
        char* end;
        if (isdigit((unsigned char)*p) || *p == '.') {
            coeff = strtod(p, &end);
            p = end;
        }
        while (isspace((unsigned char)*p) || *p == '*') { p++; }

        if (strncmp(p, "sin(x)", 6) == 0) {
            hlg->a += sign * coeff;
            p += 6;
            found_any_valid_term = true;
        }
        else if (strncmp(p, "cos(x)", 6) == 0) {
            hlg->b += sign * coeff;
            p += 6;
            found_any_valid_term = true;
        }
        else {
            if (*p != '\0') { // Neu con ky tu ma khong phai la khoang trang
                parse_ok = false;
            }
            break;
        }
    }
    while (isspace((unsigned char)*p)) { p++; }
    if (*p != '\0') { // Ky tu thua o cuoi
        parse_ok = false;
    }

    if (!parse_ok || (!found_any_valid_term && strlen(str) > 0 && *p_check_empty != '\0')) {
        hlg->a = 0.0; // Reset neu co loi hoac khong tim thay term hop le trong chuoi khong rong
        hlg->b = 0.0;
        return false;
    }
    return true;
}

void nhap_ham_luong_giac(HamLuongGiac* hlg) {
    char input_buffer[256];
    hlg->a = 0.0; hlg->b = 0.0; // Khoi tao mac dinh
    printf("Nhap ham luong giac (dang: a*sin(x) + b*cos(x)): ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    char log_msg[300];
    sprintf(log_msg, "Nguoi dung nhap ham luong giac: \"%s\"", input_buffer);
    ghi_vao_log(log_msg);
    if (!xau_sang_hamLuongGiac(input_buffer, hlg)) {
        printf("CANH BAO: Chuoi ham luong giac nhap khong hop le!\n");
        printf("         Ham se duoc dat mac dinh la: 0.00*sin(x) + 0.00*cos(x)\n");
        ghi_vao_log("Loi phan tich cu phap ham luong giac. Ham duoc dat mac dinh.");
    }
}

bool xau_sang_hamCanThuc(const char* str, HamCanThuc* hct) {
    hct->n = 1; // Dat mac dinh truoc
    int n_val;
    char temp_str_cleaned[256];
    int k = 0;
    for (int i = 0; str[i]; i++) {
        if (!isspace((unsigned char)str[i])) {
            temp_str_cleaned[k++] = str[i];
        }
    }
    temp_str_cleaned[k] = '\0';

    if (k == 0) { // Chuoi rong, chap nhan n=1
        return true;
    }

    if (sscanf(temp_str_cleaned, "x^(1/%d)", &n_val) == 1) {
        char* closing_paren = strrchr(temp_str_cleaned, ')');
        if (closing_paren != NULL && *(closing_paren + 1) == '\0') {
            char check_format[32];
            sprintf(check_format, "x^(1/%d)", n_val);
            if (strcmp(temp_str_cleaned, check_format) == 0) { // Chi khop neu toan bo chuoi la dinh dang nay
                if (n_val > 0) {
                    hct->n = n_val;
                    return true;
                }
                else {
                    hct->n = 1; // n <= 0 khong hop le
                    return false;
                }
            }
            else { // Dinh dang khac, vd: x^(1/12)abc
                hct->n = 1;
                return false;
            }
        }
        else { // Thieu ')' hoac co ky tu thua
            hct->n = 1;
            return false;
        }
    }
    else if (strcmp(temp_str_cleaned, "sqrt(x)") == 0) {
        hct->n = 2;
        return true;
    }
    else if (strcmp(temp_str_cleaned, "cbrt(x)") == 0) {
        hct->n = 3;
        return true;
    }
    hct->n = 1; // Neu khong khop dinh dang nao
    return false;
}

void nhap_ham_can_thuc(HamCanThuc* hct) {
    char input_buffer[256];
    hct->n = 1; // Khoi tao mac dinh
    printf("Nhap ham can (dang: x^(1/n) hoac sqrt(x) hoac cbrt(x)): ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    char log_msg[300];
    sprintf(log_msg, "Nguoi dung nhap ham can: \"%s\"", input_buffer);
    ghi_vao_log(log_msg);
    if (!xau_sang_hamCanThuc(input_buffer, hct)) {
        printf("CANH BAO: Chuoi ham can nhap khong hop le!\n");
        printf("         Ham se duoc dat mac dinh la: x^(1/1)\n");
        ghi_vao_log("Loi phan tich cu phap ham can. Ham duoc dat mac dinh x^(1/1).");
    }
}

void hien_thi_ham_luong_giac(HamLuongGiac hlg) {
    printf("Ham luong giac: ");
    int printed_terms = 0;
    if (fabs(hlg.a) > 1e-9) {
        if (hlg.a < 0) { printf("-"); }
        if (fabs(fabs(hlg.a) - 1.0) > 1e-9) { printf("%.2f*", fabs(hlg.a)); }
        printf("sin(x)");
        printed_terms++;
    }
    if (fabs(hlg.b) > 1e-9) {
        if (printed_terms > 0) { printf(hlg.b > 0 ? " + " : " - "); }
        else if (hlg.b < 0) { printf("-"); }
        if (fabs(fabs(hlg.b) - 1.0) > 1e-9) { printf("%.2f*", fabs(hlg.b)); }
        printf("cos(x)");
        printed_terms++;
    }
    if (printed_terms == 0) { printf("0"); }
}

void hien_thi_ham_can_thuc(HamCanThuc hct) {
    printf("Ham can thuc: x^(1/%d)", hct.n);
}

void hien_thi_ham_so(HamSo hs) {
    char ham_so_str[512] = "";
    switch (hs.loai) {
    case DA_THUC:
        hien_thi_da_thuc(hs.duLieu.daThuc);
        // Logic de tao chuoi cho da thuc (co the phuc tap tuy theo yeu cau dinh dang)
        if (hs.duLieu.daThuc.so_luong_so_hang > 0) {
            // Tao chuoi dai dien cho da thuc de ghi log
            // Day la mot vi du don gian, ban co the lam chi tiet hon
            strcpy(ham_so_str, "Da thuc: ");
            char term_buffer[50];
            for (int i = 0; i < hs.duLieu.daThuc.so_luong_so_hang && strlen(ham_so_str) < 450; ++i) {
                SoHang sh = hs.duLieu.daThuc.cac_so_hang[i]; // Gia su da sap xep
                if (i > 0) strcat(ham_so_str, (sh.he_so >= 0 ? " + " : " - "));
                else if (sh.he_so < 0) strcat(ham_so_str, "-");
                sprintf(term_buffer, "%.2fx^%d", fabs(sh.he_so), sh.bac);
                strcat(ham_so_str, term_buffer);
            }
        }
        else {
            strcpy(ham_so_str, "Da thuc: 0");
        }
        break;
    case LUONG_GIAC:
        hien_thi_ham_luong_giac(hs.duLieu.hamLuongGiac);
        sprintf(ham_so_str, "Ham luong giac: %.2f*sin(x) + %.2f*cos(x)", hs.duLieu.hamLuongGiac.a, hs.duLieu.hamLuongGiac.b);
        break;
    case CAN_THUC:
        hien_thi_ham_can_thuc(hs.duLieu.hamCanThuc);
        sprintf(ham_so_str, "Ham can thuc: x^(1/%d)", hs.duLieu.hamCanThuc.n);
        break;
    default:
        printf("Loai ham khong xac dinh.");
        strcpy(ham_so_str, "Loai ham khong xac dinh.");
        break;
    }
    printf("\n"); // Xuong dong sau khi hien thi ham
    char log_msg[600];
    sprintf(log_msg, "Ham so hien tai duoc chon/hien thi: %s", ham_so_str);
    ghi_vao_log(log_msg);
}

// 4. Main Calculation Functions
double tinh_gia_tri_ham_so(HamSo hs, double giaTriX) {
    switch (hs.loai) {
    case DA_THUC:
        return tinh_gia_tri_da_thuc(hs.duLieu.daThuc, giaTriX);
    case LUONG_GIAC:
        return sin(giaTriX * M_PI / 180.0) * hs.duLieu.hamLuongGiac.a + cos(giaTriX * M_PI / 180.0) * hs.duLieu.hamLuongGiac.b;
    case CAN_THUC:
        if (la_so_chan(hs.duLieu.hamCanThuc.n) && giaTriX < 0) {
            return NAN;
        }
        if (hs.duLieu.hamCanThuc.n == 0) { // Tranh chia cho 0
            return NAN;
        }
        return pow(giaTriX, 1.0 / hs.duLieu.hamCanThuc.n);
    }
    return NAN;
}

double phuong_phap_hinh_thang(HamSo hs, double a, double b, int n) {
    if (n <= 0) {
        return NAN;
    }
    double h = (b - a) / n;
    double val_a = tinh_gia_tri_ham_so(hs, a);
    double val_b = tinh_gia_tri_ham_so(hs, b);
    if (isnan(val_a) || isnan(val_b)) {
        return NAN;
    }
    double sum = (val_a + val_b) / 2.0;

    for (int i = 1; i < n; i++) {
        double term = tinh_gia_tri_ham_so(hs, a + i * h);
        if (isnan(term)) {
            return NAN;
        }
        sum += term;
    }
    return sum * h;
}

double phuong_phap_simpson(HamSo hs, double a, double b, int n) {
    if (n <= 0 || !la_so_chan(n)) {
        return NAN;
    }
    double h = (b - a) / n;
    double val_a = tinh_gia_tri_ham_so(hs, a);
    double val_b = tinh_gia_tri_ham_so(hs, b);
    if (isnan(val_a) || isnan(val_b)) {
        return NAN;
    }
    double sum = val_a + val_b;

    for (int i = 1; i < n; i += 2) {
        double term = tinh_gia_tri_ham_so(hs, a + i * h);
        if (isnan(term)) {
            return NAN;
        }
        sum += 4.0 * term;
    }
    for (int i = 2; i < n; i += 2) { // Sua: Dieu kien vong lap cho Simpson
        double term = tinh_gia_tri_ham_so(hs, a + i * h);
        if (isnan(term)) {
            return NAN;
        }
        sum += 2.0 * term;
    }
    return sum * h / 3.0;
}

// 5. UI Logic Functions (Khong goi _getch() hay system("pause") o cuoi)
void xu_ly_gia_tri_tai_X(HamSo hs) {
    double x;
    int precision_val;
    char log_msg[200];
    do {
        printf("Nhap so chu so thap phan (0-15): ");
        if (scanf("%d", &precision_val) != 1) {
            printf("Loi: Nhap khong phai la so nguyen.\n");
            clear_stdin();
            precision_val = -1;
            ghi_vao_log("Loi: Nhap so chu so thap phan khong hop le (khong phai so nguyen).");
        }
        else {
            clear_stdin();
            if (precision_val < 0 || precision_val > 15) {
                printf("Loi: So chu so thap phan phai trong khoang 0-15.\n");
                ghi_vao_log("Loi: So chu so thap phan ngoai khoang (0-15).");
            }
        }
    } while (precision_val < 0 || precision_val > 15);
    soChuSoThapPhan = precision_val;
    sprintf(log_msg, "Chon so chu so thap phan: %d", soChuSoThapPhan);
    ghi_vao_log(log_msg);

    printf("Nhap gia tri x: ");
    if (scanf("%lf", &x) != 1) {
        printf("Loi: Gia tri x khong hop le.\n");
        clear_stdin();
        ghi_vao_log("Loi: Nhap gia tri x khong hop le.");
        return;
    }
    clear_stdin();
    sprintf(log_msg, "Nguoi dung nhap x = %.2f", x);
    ghi_vao_log(log_msg);

    double kq = tinh_gia_tri_ham_so(hs, x);
    if (!isnan(kq)) {
        printf("f(%.*lf) = %.*lf\n", soChuSoThapPhan, x, soChuSoThapPhan, kq);
        sprintf(log_msg, "Ket qua tinh gia tri: f(%.*lf) = %.*lf", soChuSoThapPhan, x, soChuSoThapPhan, kq);
        ghi_vao_log(log_msg);
    }
    else {
        printf("Khong the tinh gia tri ham so tai diem x = %.2f\n", x);
        sprintf(log_msg, "Khong the tinh gia tri ham so tai x = %.2f (NAN).", x);
        ghi_vao_log(log_msg);
    }
}

void xu_ly_hinh_thang(HamSo hs) {
    int a, b, n_val, precision_val;
    char log_msg[200];
    do {
        printf("Nhap so chu so thap phan (0-15): ");
        if (scanf("%d", &precision_val) != 1) {
            printf("Loi: Nhap khong phai la so nguyen.\n");
            clear_stdin();
            precision_val = -1;
            ghi_vao_log("Loi: Nhap so chu so thap phan khong hop le (Hinh Thang).");
        }
        else {
            clear_stdin();
            if (precision_val < 0 || precision_val > 15) {
                printf("Loi: So chu so thap phan phai trong khoang 0-15.\n");
                ghi_vao_log("Loi: So chu so thap phan ngoai khoang (0-15) (Hinh Thang).");
            }
        }
    } while (precision_val < 0 || precision_val > 15);
    soChuSoThapPhan = precision_val;
    sprintf(log_msg, "Chon so chu so thap phan (Hinh Thang): %d", soChuSoThapPhan);
    ghi_vao_log(log_msg);

    printf("Nhap can duoi a: ");
    if (scanf("%d", &a) != 1) { printf("Loi nhap a.\n"); clear_stdin(); ghi_vao_log("Loi nhap can a (Hinh Thang)."); return; }
    clear_stdin();
    printf("Nhap can tren b: ");
    if (scanf("%d", &b) != 1) { printf("Loi nhap b.\n"); clear_stdin(); ghi_vao_log("Loi nhap can b (Hinh Thang)."); return; }
    clear_stdin();
    printf("Nhap so khoang chia n (>0): ");
    if (scanf("%d", &n_val) != 1 || n_val <= 0) {
        printf("So khoang chia phai la so nguyen duong.\n");
        clear_stdin();
        ghi_vao_log("Loi nhap so khoang chia n (Hinh Thang).");
        return;
    }
    clear_stdin();
    sprintf(log_msg, "Tinh tich phan Hinh Thang: a=%d, b=%d, n=%d", a, b, n_val);
    ghi_vao_log(log_msg);

    printf("\n %-8s %-12s %-12s\n", "i", "xi", "f(xi)");
    printf("----------------------------------\n");
    for (int i = 0; i <= n_val; i++) {
        double xi = a + (double)i * (b - a) / n_val;
        double f_xi = tinh_gia_tri_ham_so(hs, xi);
        if (isnan(f_xi)) {
            printf(" %-8d %-12.*lf %-12s\n", i, soChuSoThapPhan, xi, "NAN");
        }
        else {
            printf(" %-8d %-12.*lf %-12.*lf\n", i, soChuSoThapPhan, xi, soChuSoThapPhan, f_xi);
        }
    }
    double tichPhan = phuong_phap_hinh_thang(hs, a, b, n_val);
    if (isnan(tichPhan)) {
        printf("\nTich phan xap xi theo Hinh Thang: Khong tinh duoc (NAN)\n");
        ghi_vao_log("Ket qua Hinh Thang: NAN");
    }
    else {
        printf("\nTich phan xap xi theo Hinh Thang: %.*lf\n", soChuSoThapPhan, tichPhan);
        sprintf(log_msg, "Ket qua Hinh Thang: %.*lf", soChuSoThapPhan, tichPhan);
        ghi_vao_log(log_msg);
    }
}

void xu_ly_simpson(HamSo hs) {
    int a, b, n_val, precision_val;
    char log_msg[200];
    do {
        printf("Nhap so chu so thap phan (0-15): ");
        if (scanf("%d", &precision_val) != 1) {
            printf("Loi: Nhap khong phai la so nguyen.\n");
            clear_stdin();
            precision_val = -1;
            ghi_vao_log("Loi: Nhap so chu so thap phan khong hop le (Simpson).");
        }
        else {
            clear_stdin();
            if (precision_val < 0 || precision_val > 15) {
                printf("Loi: So chu so thap phan phai trong khoang 0-15.\n");
                ghi_vao_log("Loi: So chu so thap phan ngoai khoang (0-15) (Simpson).");
            }
        }
    } while (precision_val < 0 || precision_val > 15);
    soChuSoThapPhan = precision_val;
    sprintf(log_msg, "Chon so chu so thap phan (Simpson): %d", soChuSoThapPhan);
    ghi_vao_log(log_msg);

    printf("Nhap can duoi a: ");
    if (scanf("%d", &a) != 1) {
        printf("Loi nhap a.\n");
        clear_stdin();
        ghi_vao_log("Loi nhap can a (Simpson).");
        return;
    }
    clear_stdin();
    printf("Nhap can tren b: ");
    if (scanf("%d", &b) != 1) {
        printf("Loi nhap b.\n");
        clear_stdin();
        ghi_vao_log("Loi nhap can b (Simpson).");
        return;
    }
    clear_stdin();
    do {
        printf("Nhap so khoang chia n (so chan > 0): ");
        if (scanf("%d", &n_val) != 1) {
            printf("Loi: Nhap khong phai la so nguyen.\n");
            clear_stdin();
            n_val = -1;
            ghi_vao_log("Loi: Nhap so khoang chia n khong phai so nguyen (Simpson).");
        }
        else {
            clear_stdin();
            if (n_val <= 0 || !la_so_chan(n_val)) {
                printf("n phai la so nguyen duong chan.\n");
                ghi_vao_log("Loi: n khong phai so nguyen duong chan (Simpson).");
            }
        }
    } while (n_val <= 0 || !la_so_chan(n_val));
    sprintf(log_msg, "Tinh tich phan Simpson: a=%d, b=%d, n=%d", a, b, n_val);
    ghi_vao_log(log_msg);

    printf("\n %-8s %-12s %-12s\n", "i", "xi", "f(xi)");
    printf("----------------------------------\n");
    for (int i = 0; i <= n_val; i++) {
        double xi = a + (double)i * (b - a) / n_val;
        double f_xi = tinh_gia_tri_ham_so(hs, xi);
        if (isnan(f_xi)) {
            printf(" %-8d %-12.*lf %-12s\n", i, soChuSoThapPhan, xi, "NAN");
        }
        else {
            printf(" %-8d %-12.*lf %-12.*lf\n", i, soChuSoThapPhan, xi, soChuSoThapPhan, f_xi);
        }
    }
    double tichPhan = phuong_phap_simpson(hs, a, b, n_val);
    if (isnan(tichPhan)) {
        printf("\nTich phan xap xi theo Simpson: Khong tinh duoc (NAN)\n");
        ghi_vao_log("Ket qua Simpson: NAN");
    }
    else {
        printf("\nTich phan xap xi theo Simpson: %.*lf\n", soChuSoThapPhan, tichPhan);
        sprintf(log_msg, "Ket qua Simpson: %.*lf", soChuSoThapPhan, tichPhan);
        ghi_vao_log(log_msg);
    }
}

// 6. Enhanced UI Drawing Functions
void hien_thi_tieu_de_chuong_trinh() {
    di_den_XY(toaDoXMenu - 15, 2);
    chinh_mau_sac(14); // Mau vang
    printf("*******************************************************************************\n");
    di_den_XY(toaDoXMenu - 15, 3);
    printf("* CHUONG TRINH TINH TICH PHAN SO                         *\n");
    di_den_XY(toaDoXMenu - 15, 4);
    printf("*******************************************************************************\n");
    chinh_mau_sac(7); // Tra lai mau mac dinh
}

void hien_thi_huong_dan_phim(int y_pos) {
    di_den_XY(toaDoXMenu - 15, y_pos);
    chinh_mau_sac(8); // Mau xam
    printf("MUI TEN: Di chuyen | ENTER: Chon | ESC: Thoat (trong Menu Chuc Nang)");
    chinh_mau_sac(7);
}

void ve_khung(int x, int y, int w, int h, int mauVien, int mauNenTrongChuTrong, const char* noiDung, bool canGiuaNoiDung) {
    mau_chu(mauVien);
    for (int ix = x; ix <= x + w; ix++) {
        di_den_XY(ix, y); printf("%c", 205);
        di_den_XY(ix, y + h); printf("%c", 205);
    }
    for (int iy = y; iy <= y + h; iy++) {
        di_den_XY(x, iy); printf("%c", 186);
        di_den_XY(x + w, iy); printf("%c", 186);
    }
    di_den_XY(x, y); printf("%c", 201);
    di_den_XY(x + w, y); printf("%c", 187);
    di_den_XY(x, y + h); printf("%c", 200);
    di_den_XY(x + w, y + h); printf("%c", 188);

    mau_chu(mauNenTrongChuTrong);
    for (int iy = y + 1; iy < y + h; iy++) {
        for (int ix = x + 1; ix < x + w; ix++) {
            di_den_XY(ix, iy);
            printf(" ");
        }
    }
    int text_x;
    if (canGiuaNoiDung) {
        text_x = x + (w - strlen(noiDung) + 1) / 2;
    }
    else {
        text_x = x + 3;
    }
    di_den_XY(text_x, y + h / 2);
    printf("%s", noiDung);
}

void to_sang(int x, int y, int w, int h, int mauHighlightKetHop, int mauVienHighlight, const char* noiDung, bool canGiuaNoiDung) {
    mau_chu(mauVienHighlight);
    for (int ix = x; ix <= x + w; ix++) { di_den_XY(ix, y); printf("%c", 205); di_den_XY(ix, y + h); printf("%c", 205); }
    for (int iy = y; iy <= y + h; iy++) { di_den_XY(x, iy); printf("%c", 186); di_den_XY(x + w, iy); printf("%c", 186); }
    di_den_XY(x, y); printf("%c", 201); di_den_XY(x + w, y); printf("%c", 187);
    di_den_XY(x, y + h); printf("%c", 200); di_den_XY(x + w, y + h); printf("%c", 188);

    mau_chu(mauHighlightKetHop);
    for (int iy = y + 1; iy < y + h; iy++) {
        for (int ix = x + 1; ix < x + w; ix++) {
            di_den_XY(ix, iy);
            printf(" ");
        }
    }
    char buffer[256];
    sprintf(buffer, ">> %s", noiDung);
    int text_x;
    if (canGiuaNoiDung) {
        text_x = x + (w - strlen(buffer) + 1) / 2;
    }
    else {
        text_x = x + 3;
    }
    di_den_XY(text_x, y + h / 2);
    printf("%s", buffer);
}

void ve_them_vien(int x, int y, int w, int h, int mauVien, int mauNenChuTrong, int soLuong, const char* cacMucMenu[]) {
    for (int i = 0; i < soLuong; i++) {
        ve_khung(x, y + (i * (h + 1)), w, h, mauVien, mauNenChuTrong, cacMucMenu[i], false);
    }
}

// 7. Menu Functions
void menu_chon_ham() {
    system("cls");
    hien_thi_tieu_de_chuong_trinh();
    hien_thi_trang_thai(0);
    ghi_vao_log("Vao menu_chon_ham.");

    int mauVien = 3;
    int mauNenChuTrong = 0x0F;
    int mauHighlightKetHop = 0xF0;
    int mauVienHighlight = 0;
    int soLuong = 3;
    const char* cacMucMenu[] = { "1. Ham Da Thuc", "2. Ham Luong Giac", "3. Ham Can Thuc" };

    ve_them_vien(toaDoXMenu, toaDoYMenu, chieuRongMenu, chieuCaoMucMenu, mauVien, mauNenChuTrong, soLuong, cacMucMenu);
    hien_thi_huong_dan_phim(toaDoYMenu + soLuong * (chieuCaoMucMenu + 1) + 2);

    int tungDoMoi = toaDoYMenu;
    int tungDoCu = toaDoYMenu;
    int mucChonHienTai = 0;
    int mucChonCu = 0;
    bool canVeLai = true;
    bool tiepTucLap = true;

    if (canVeLai) {
        to_sang(toaDoXMenu, tungDoMoi, chieuRongMenu, chieuCaoMucMenu, mauHighlightKetHop, mauVienHighlight, cacMucMenu[mucChonHienTai], false);
        canVeLai = false;
    }

    while (tiepTucLap) {
        if (canVeLai) {
            ve_khung(toaDoXMenu, tungDoCu, chieuRongMenu, chieuCaoMucMenu, mauVien, mauNenChuTrong, cacMucMenu[mucChonCu], false);
            to_sang(toaDoXMenu, tungDoMoi, chieuRongMenu, chieuCaoMucMenu, mauHighlightKetHop, mauVienHighlight, cacMucMenu[mucChonHienTai], false);
            canVeLai = false;
        }

        if (_kbhit()) {
            char kyTu = _getch();
            tungDoCu = tungDoMoi;
            mucChonCu = mucChonHienTai;
            char log_msg[100];

            if (kyTu == -32 || kyTu == 0) {
                kyTu = _getch();
                if (kyTu == 72 && mucChonHienTai > 0) {
                    mucChonHienTai--;
                    canVeLai = true;
                }
                else if (kyTu == 80 && mucChonHienTai < soLuong - 1) {
                    mucChonHienTai++;
                    canVeLai = true;
                }
                tungDoMoi = toaDoYMenu + mucChonHienTai * (chieuCaoMucMenu + 1);
            }
            else if (kyTu == '\r') {
                mau_chu(7);
                system("cls"); // Xoa man hinh DE IN PHAN NHAP LIEU
                hien_thi_trang_thai(1);
                mau_chu(11);
                sprintf(log_msg, "Nguoi dung chon loai ham: %s", cacMucMenu[mucChonHienTai]);
                ghi_vao_log(log_msg);

                if (hamSoToanCuc.loai == DA_THUC) {
                    giai_phong_da_thuc(&hamSoToanCuc.duLieu.daThuc);
                }

                hien_thi_tieu_de_chuong_trinh();
                di_den_XY(0, toaDoYMenu - 1); // Vi tri de bat dau nhap lieu

                switch (mucChonHienTai + 1) {
                case 1:
                    hamSoToanCuc.loai = DA_THUC;
                    nhap_da_thuc(&hamSoToanCuc.duLieu.daThuc);
                    break;
                case 2:
                    hamSoToanCuc.loai = LUONG_GIAC;
                    nhap_ham_luong_giac(&hamSoToanCuc.duLieu.hamLuongGiac);
                    break;
                case 3:
                    hamSoToanCuc.loai = CAN_THUC;
                    nhap_ham_can_thuc(&hamSoToanCuc.duLieu.hamCanThuc);
                    break;
                }
                // Sau khi nhap xong, va co the co canh bao, cho nguoi dung xem
                printf("\n"); // Dam bao xuong dong sau khi nhap/canh bao
                hien_thi_ham_so(hamSoToanCuc); // Hien thi ham so da duoc nhap/mac dinh
                printf("Nhan phim bat ky de vao Menu Chuc Nang...");              
                int ch = _getch(); // Doi nguoi dung xem  
                (void)ch; // Explicitly mark the return value as intentionally ignored
                tiepTucLap = 0;
            }
            else if (kyTu == 27) {
                mau_chu(7); system("cls");
                ghi_vao_log("Nguoi dung thoat chuong trinh tu menu_chon_ham (ESC).");
                dong_tep_log();
                exit(0);
            }
        }
    }
}

int menu_chon_chuc_nang() {
    system("cls");
    hien_thi_tieu_de_chuong_trinh();
    hien_thi_trang_thai(0);
    ghi_vao_log("Vao menu_chon_chuc_nang.");

    int mauVien = 2;
    int mauNenChuTrong = 0x0F;
    int mauHighlightKetHop = 0xF0;
    int mauVienHighlight = 0;
    int soLuong = 4;
    const char* cacMucMenu[] = {
        "1. Tinh gia tri tai diem x",
        "2. Tinh tich phan (Hinh Thang)",
        "3. Tinh tich phan (Simpson)",
        "4. Quay ve chon lai ham"
    };

    di_den_XY(10, toaDoYMenu - 2); chinh_mau_sac(14);
    hien_thi_ham_so(hamSoToanCuc);
    chinh_mau_sac(mauVien);

    ve_them_vien(toaDoXMenu, toaDoYMenu, chieuRongMenu, chieuCaoMucMenu, mauVien, mauNenChuTrong, soLuong, cacMucMenu);
    hien_thi_huong_dan_phim(toaDoYMenu + soLuong * (chieuCaoMucMenu + 1) + 2);

    int tungDoMoi = toaDoYMenu;
    int tungDoCu = toaDoYMenu;
    int mucChonHienTai = 0;
    int mucChonCu = 0;
    bool canVeLai = true;
    bool tiepTucLap = true;

    if (canVeLai) {
        to_sang(toaDoXMenu, tungDoMoi, chieuRongMenu, chieuCaoMucMenu, mauHighlightKetHop, mauVienHighlight, cacMucMenu[mucChonHienTai], false);
        canVeLai = false;
    }

    while (tiepTucLap) {
        if (canVeLai) {
            if (tungDoCu != tungDoMoi || tiepTucLap) {
                ve_khung(toaDoXMenu, tungDoCu, chieuRongMenu, chieuCaoMucMenu, mauVien, mauNenChuTrong, cacMucMenu[mucChonCu], false);
            }
            to_sang(toaDoXMenu, tungDoMoi, chieuRongMenu, chieuCaoMucMenu, mauHighlightKetHop, mauVienHighlight, cacMucMenu[mucChonHienTai], false);
            canVeLai = false;
        }
        if (_kbhit()) {
            char kyTu = _getch();
            tungDoCu = tungDoMoi;
            mucChonCu = mucChonHienTai;
            char log_msg[100];

            if (kyTu == -32 || kyTu == 0) {
                kyTu = _getch();
                if (kyTu == 72 && mucChonHienTai > 0) {
                    mucChonHienTai--;
                    canVeLai = true;
                }
                else if (kyTu == 80 && mucChonHienTai < soLuong - 1) {
                    mucChonHienTai++;
                    canVeLai = true;
                }
                tungDoMoi = toaDoYMenu + mucChonHienTai * (chieuCaoMucMenu + 1);
            }
            else if (kyTu == '\r') {
                mau_chu(7);
                system("cls"); // Xoa man hinh chuan bi cho output cua chuc nang
                hien_thi_trang_thai(1);
                mau_chu(11);
                sprintf(log_msg, "Nguoi dung chon chuc nang: %s", cacMucMenu[mucChonHienTai]);
                ghi_vao_log(log_msg);

                hien_thi_tieu_de_chuong_trinh();
                di_den_XY(10, toaDoYMenu - 2); chinh_mau_sac(14); hien_thi_ham_so(hamSoToanCuc);
                chinh_mau_sac(11);
                di_den_XY(0, toaDoYMenu); // Vi tri bat dau in ket qua cua chuc nang

                switch (mucChonHienTai + 1) {
                case 1: xu_ly_gia_tri_tai_X(hamSoToanCuc); break;
                case 2: xu_ly_hinh_thang(hamSoToanCuc); break;
                case 3: xu_ly_simpson(hamSoToanCuc); break;
                case 4:
                    ghi_vao_log("Nguoi dung chon quay lai menu_chon_ham.");
                    menu_chon_ham(); // Goi lai menu chon ham
                    return 4; // Tra ve de main biet la da quay lai menu chon ham
                }
                return mucChonHienTai + 1; // Tra ve de main xu ly "Nhan phim..."
            }
            else if (kyTu == 27) { // Phim Escape
                return 0; // Thoat khoi menu_chon_chuc_nang
            }
        }
    }
    return 0; // Neu vong lap thoat bat thuong 
}

