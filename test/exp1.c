#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>

void child_terminated(int signum) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0) {
        printf("子进程 %d 已退出，状态 = %d\n", pid, status);
    }
}

int main() {
    pid_t pid;

    signal(SIGCHLD, child_terminated);  // 注册SIGCHLD信号处理函数

    pid = fork();
    if (pid < 0) {
        printf("创建进程失败！\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // 子进程
        printf("子进程 %d 显示文件列表：\n", getpid());
        while (1) {
            system("/bin/ls");  // 使用system函数执行ls命令
            sleep(3);
        }
    } else {  // 父进程
        printf("我是父进程 %d\n", getpid());
        while (1) {
            sleep(1);  // 父进程持续运行，等待子进程结束
        }
    }

    return EXIT_SUCCESS;
}

