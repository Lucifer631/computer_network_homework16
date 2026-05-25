#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USER 100
#define HOUR 3600

// 用户结构体
typedef struct {
    char nick[20];
    char pwd[20];
    char email[50];
    char code[10];
    time_t create_time;
    int active;
} User;

User users[MAX_USER];
int user_cnt = 0;

// 生成验证码
void gen_code(const char *nick, char *code) {
    srand((unsigned int)time(NULL));
    sprintf(code, "%d", rand() % 1000000);
}

// 清理超时未激活用户
void clean_expired() {
    time_t now = time(NULL);
    for (int i = 0; i < user_cnt; i++) {
        if (!users[i].active && now - users[i].create_time > HOUR) {
            printf("[超时释放] 昵称 %s 已回收\n", users[i].nick);
            // 前移删除
            for (int j = i; j < user_cnt - 1; j++)
                users[j] = users[j + 1];
            user_cnt--;
            i--;
        }
    }
}

// 注册
int register_user(const char *nick, const char *pwd, const char *email) {
    clean_expired();
    for (int i = 0; i < user_cnt; i++) {
        if (strcmp(users[i].nick, nick) == 0)
            return -1; // 昵称已占用
    }
    User u;
    strcpy(u.nick, nick);
    strcpy(u.pwd, pwd);
    strcpy(u.email, email);
    u.create_time = time(NULL);
    u.active = 0;
    gen_code(nick, u.code);
    users[user_cnt++] = u;

    printf("===== 注册成功 =====\n");
    printf("验证链接: http://localhost/active?user=%s&code=%s\n", nick, u.code);
    printf("请在1小时内激活，否则昵称释放\n");
    return 1;
}

// 激活
int active_user(const char *nick, const char *code) {
    clean_expired();
    for (int i = 0; i < user_cnt; i++) {
        if (strcmp(users[i].nick, nick) == 0) {
            if (strcmp(users[i].code, code) == 0) {
                users[i].active = 1;
                return 1;
            } else return -2; // 验证码错
        }
    }
    return -1; // 不存在/超时
}

int main() {
    register_user("zhangsan", "123456", "test@qq.com");
    active_user("zhangsan", "验证码填上面输出的");
    return 0;
}
