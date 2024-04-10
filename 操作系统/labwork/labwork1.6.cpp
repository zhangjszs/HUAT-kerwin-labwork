/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-10 22:19:17
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-10 22:19:22
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork\labwork1.6.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {
    string input;
    while (true) {
        // 输出提示符并获取用户输入
        cout << "kerwin shell begin to start!" << endl;
        cout << "$ ";
        getline(cin, input);

        // 将用户输入拆分成命令和参数
        istringstream iss(input);
        vector<string> args;
        string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        // 如果用户输入的是 "exit"，退出程序
        if (args[0] == "exit") {
            break;
        }

        // 创建子进程并执行命令
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            char* argv[args.size() + 1];
            for (int i = 0; i < args.size(); i++) {
                argv[i] = const_cast<char*>(args[i].c_str());
            }
            argv[args.size()] = nullptr; // 末尾添加一个空指针
            execvp(argv[0], argv);
            cerr << "Error: command not found" << endl;
            exit(1);
        } else if (pid > 0) {
            // 父进程
            int status;
            waitpid(pid, &status, 0);
        } else {
            // 创建子进程失败
            cerr << "Error: fork() failed" << endl;
            continue;
        }
    }
    return 0;
}
