#include <stdio.h>
#include <string.h>
#include <ctype.h>

void lower(char *word) {
    for(int i = 0; i < strlen(word); i++) {
        if(word[i] >= 'A' && word[i] <= 'Z') {
            word[i] += 32;
        }
    }
}

// 维吉尼亚加密
void virginia_encryption(char *secretkey, char *plaintext, char *ciphertext) {
    int j = 0;
    for(int i = 0; i < strlen(plaintext); i++) {
        if(isalpha(plaintext[i])) {
            while(!isalpha(secretkey[j])) {
                j++;
                j %= strlen(secretkey);
            }
            if(plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
                ciphertext[i] = ((plaintext[i]-65) + (secretkey[j]-97))%26 + 65;
            } else if(plaintext[i] >= 'a' && plaintext[i] <= 'z') {
                ciphertext[i] = ((plaintext[i]-97) + (secretkey[j]-97))%26 + 97;
            }
            j++;
            j %= strlen(secretkey);
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

// 维吉尼亚加密
void virginia_decryption(char *secretkey, char *plaintext, char *ciphertext) {
    int j = 0;
    for(int i = 0; i < strlen(ciphertext); i++) {
        if(isalpha(ciphertext[i])) {
            while(!isalpha(secretkey[j])) {
                j++;
                j %= strlen(secretkey);
            }
            if(ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
                plaintext[i] = ((ciphertext[i]-65) + 26 - (secretkey[j]-97))%26 + 65;
            } else if(ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
                plaintext[i] = ((ciphertext[i]-97) + 26 - (secretkey[j]-97))%26 + 97;
            }
            j++;
            j %= strlen(secretkey);
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int get_order(char *secretkey, int* order) {
    char key[255] = {0};
    char c;             // 临时字符
    int alpha_cnt = 0;  // 记录密钥字母个数
    int empty_cnt = 0;  // 记录空白字符个数
    int cnt = 0;        // 排序计数
    for(int i = 0; i < strlen(secretkey); i++) {
        if(isalpha(secretkey[i])) {
            key[i] = secretkey[i];
            alpha_cnt++;
        }
    }

    empty_cnt = strlen(secretkey) - alpha_cnt;

    // 冒泡排序(稳定)
    for(int i = 0; i < strlen(secretkey); i++) {
        for(int j = 0; j < strlen(secretkey)-1; j++) {
            if(key[j] > key[j+1]) {
                c = key[j];
                key[j] = key[j+1];
                key[j+1] = c;
            }
        }
    }
    // 设置order
    for(int i = 0; i < strlen(secretkey); i++) {
        if(!isalpha(secretkey[i])) continue;
        for(int j = 0; j < strlen(secretkey); j++) {
            if(secretkey[i] == key[j]) {
                order[cnt] = j-empty_cnt+1;
                key[j] = '\0';
                break;
            }
        }
        cnt++;
    }
    return cnt;
}

// 列置换加密
void column_permutation_encryption(char *plaintext, char *ciphertext, int *order, int num) {
    int cnt = 1;
    int pos = 0;
    while(cnt <= num) {
        for(int i = 0; i < num; i++) {
            if(order[i] != cnt) continue;
            while(i < strlen(plaintext)) {
                ciphertext[pos++] = plaintext[i];
                i += num;
            }
            break;
        }
        cnt++;
    }
    ciphertext[pos] = '\0';
}

// 列置换解密
void column_permutation_decryption(char *plaintext, char *ciphertext, int *order, int num) {
    int cnt = 1;
    int pos = 0;
    while(cnt <= num) {
        for(int i = 0; i < num; i++) {
            if(order[i] != cnt) continue;
            while(i < strlen(ciphertext)) {
                plaintext[i] = ciphertext[pos++];
                i += num;
            }
            break;
        }
        cnt++;
    }
    plaintext[pos] = '\0';
}

void debug_order(int *order, int alpha_num) {
    for(int i = 0; i < alpha_num; i++) {
        printf("%d  ", order[i]);
    }
    printf("\n");
}

void debug_info(char *secretkey, char *plaintext, char *ciphertext) {
    printf("密钥：%s\n", secretkey);
    printf("明文：%s\n", plaintext);
    printf("密文：%s\n", ciphertext);
}

void swap(char *a, char *b) {
    char temp[255] = {0};
    for(int i = 0; i < strlen(a); i++) {
        temp[i] = a[i];
    }
    temp[strlen(a)] = '\0';
    for(int i = 0; i < strlen(b); i++) {
        a[i] = b[i];
    }
    a[strlen(b)] = '\0';
    for(int i = 0; i < strlen(temp); i++) {
        b[i] = temp[i];
    }
    b[strlen(temp)] = '\0';
}

void encryption(char *secretkey, char *plaintext, char *ciphertext) {
    int order[255];
    int alpha_num = get_order(secretkey, order);

    debug_order(order, alpha_num);
    
    // length align
    int len = strlen(plaintext);
    while(len%alpha_num) {
        plaintext[len] = 'a';
        len++;
    }
    plaintext[len] = '\0';

    // virginia
    virginia_encryption(secretkey, plaintext, ciphertext);
    swap(plaintext, ciphertext);

// debug_info(secretkey, plaintext, ciphertext);
    // column_permutation
    column_permutation_encryption(plaintext, ciphertext, order, alpha_num);
}

void decryption(char *secretkey, char *plaintext, char *ciphertext) {
    int order[255];
    int alpha_num = get_order(secretkey, order);

    debug_order(order, alpha_num);

    debug_info(secretkey, plaintext, ciphertext);

    // column_permutation
    column_permutation_decryption(plaintext, ciphertext, order, alpha_num);
    
    swap(plaintext, ciphertext);

// debug_info(secretkey, plaintext, ciphertext);
    // virginia
    virginia_decryption(secretkey, plaintext, ciphertext);
}

void info(){
    printf("================================================================\n");
    printf("自定义加密算法\n");
    printf("作者：刘陈陈(RealGUO)\n");
    printf("描述：\n");
    printf("a. 使用维吉尼亚算法+列置换算法加密\n");
    printf("b. 加密时自动忽略密钥中包含的非字母符号\n");
    printf("c. 只对明文中的字符进行加密，但保留明文中非字母符号和相对位置\n");
    printf("d. 列置换时，最后一行不足时，填充字符'a'\n");
    printf("e. 密钥、明文、密文长度最多255字符\n");
    printf("================================================================\n");
}

void menu() {
    printf("----------------------------------------------------------------\n");
    printf("1. 加密\n");
    printf("2. 解密\n");
    printf("3. 退出\n");
    printf("请选择操作编号：");
}

int main() {
    char secretkey[255];
    char plaintext[255];
    char ciphertext[255];
    
    int op = 0;

    info();
    menu();
    while(1) {
        scanf("%d", &op);
        getchar();
        if(op == 1) {
            printf("请输入密钥：");
            gets(secretkey);
            lower(secretkey);
            printf("请输入明文：");
            gets(plaintext);
            encryption(secretkey, plaintext, ciphertext);
            printf("加密结果（密文）：%s\n", ciphertext);
        } else if(op == 2) {
            printf("请输入密钥：");
            gets(secretkey);
            lower(secretkey);
            printf("请输入密文：");
            gets(ciphertext);
            decryption(secretkey, plaintext, ciphertext);
            printf("解密结果（明文）：%s\n", plaintext);
        } else if(op == 3) {
            break;
        } else {
            printf("输入编号无效，请重新输入编号：");
            continue;
        }
        menu();
    }
    return 0;
}