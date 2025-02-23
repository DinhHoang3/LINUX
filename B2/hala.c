#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZIP_FILE "kernel.zip"
#define USERS_FILE "users.txt"
#define KERNEL_FILE "kernel.c"

// Not use minzip.h because many file.h missing: "miniz_common.h", "miniz_tdef.h", "miniz_tdef.h", "miniz_zip.h", "miniz_export.h"

// Function to read the contents of users.txt from ZIP using unzip -p
char *read_users_from_zip() {
    FILE *fp;
    char *buffer = NULL;
    size_t size = 0;
    
    // Open unzip -p command to read the file from ZIP
    char command[256];
    snprintf(command, sizeof(command), "unzip -p %s %s", ZIP_FILE, USERS_FILE);
    
    fp = popen(command, "r");
    if (!fp) {
        printf("Lỗi: Không thể mở file ZIP!\n");
        return NULL;
    }
    
    // Read content from the output of unzip command
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    
    buffer = malloc(size + 1);
    if (!buffer) {
        printf("Lỗi: Không thể cấp phát bộ nhớ!\n");
        pclose(fp);
        return NULL;
    }

    fread(buffer, 1, size, fp);

    // Null-terminate the string
    buffer[size] = '\0'; 
    
    pclose(fp);
    return buffer;
}

// Function to authenticate user from users.txt content
int authenticate_user(const char *username, const char *password) {
    char stored_user[50], stored_pass[50];
    int access;

    char *file_data = read_users_from_zip();
    if (!file_data) {
        printf("Lỗi: Không thể đọc dữ liệu từ users.txt trong ZIP!\n");
        return 0;
    }
    
    char *ptr = strtok(file_data, "\n");
    while (ptr) {
        sscanf(ptr, "%[^|]|%[^|]|%d", stored_user, stored_pass, &access);
        if (strcmp(username, stored_user) == 0 && strcmp(password, stored_pass) == 0) {
            free(file_data);
            return 1;
        }
        ptr = strtok(NULL, "\n");
    }
    
    free(file_data);
    return 0;
}

// Function to extract a file from ZIP
void extract_file(const char *zipname, const char *filename) {
    char command[256];
    snprintf(command, sizeof(command), "unzip -o %s %s", zipname, filename);
    system(command);
}

// Function to compile and run kernel
void compile_and_run_kernel() {
    printf("Biên dịch kernel...\n");
    system("gcc kernel.c -o kernel");
    printf("Chạy kernel...\n");
    system("./kernel");
}

int main() {
    char username[50], password[50];
   
    // Prompt for user login
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    // Authenticate user
    if (authenticate_user(username, password)) {
        printf("Đăng nhập thành công! Cấp quyền truy cập...\n");

        // Extract, compile, and run kernel.c
        extract_file(ZIP_FILE, KERNEL_FILE);
        compile_and_run_kernel();
    } else {
        printf("Sai tên đăng nhập hoặc mật khẩu! Truy cập bị từ chối.\n");
    }

    return 0;
}
