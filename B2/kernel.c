#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Bạn đã được cấp quyền truy cập kernel thành công!\n");

    // Giải nén file users.txt từ kernel.zip
    system("unzip -o kernel.zip users.txt");

    // Ghi đè trạng thái access = 0 vào users.txt
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("Lỗi: Không thể mở users.txt để reset quyền truy cập!\n");
        return 1;
    }
    fprintf(file, "admin|1234|0\n");  // Reset quyền truy cập
    fclose(file);

    // Cập nhật lại file users.txt vào kernel.zip
    system("zip -uj kernel.zip users.txt");

    printf("Quyền truy cập đã được reset. Bạn cần đăng nhập lại lần sau!\n");
    
    return 0;
}