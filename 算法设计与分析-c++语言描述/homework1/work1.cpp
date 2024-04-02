/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02 13:11:37
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02 13:14:44
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\work1.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
给定一个字符串 s="axsfdkjgxka'klksydfgmx" 和一个字符 x='a'，编写递归算法实现以下功能： （25分）
（1） 检查 x 是否出现在 s 中；
（2） 计算 x 在 s 中出现的次数；
（3） 删除 s 中所有 x。
要求：答案包括源代码及执行结果的屏幕截图。源代码须有适当的注释，上机调试无误，输出结果的第一行必须是自己的“班级姓名”，第二行开始是执行结果。*/
#include <iostream>
#include <string>
using namespace std;

// 检查字符x是否出现在字符串s中
bool checkCharacter(const string &s, char x)
{
    // 如果字符串为空，则表示字符未找到
    if (s.empty())
    {
        return false;
    }
    // 检查当前字符是否是x
    if (s[0] == x)
    {
        return true;
    }
    // 否则继续检查剩余的字符串
    return checkCharacter(s.substr(1), x);
}

// 计算字符x在字符串s中出现的次数
int countCharacter(const string &s, char x)
{
    // 如果字符串为空，则出现次数为0
    if (s.empty())
    {
        return 0;
    }
    // 如果当前字符是x，则计数1，否则计数0
    int count = (s[0] == x) ? 1 : 0;
    // 继续计算剩余字符串中的字符x出现次数
    return count + countCharacter(s.substr(1), x);
}

// 删除字符串s中的所有字符x
// void removeCharacter(string& s, char x) {
//     // 如果字符串为空，则结束递归
//     if (s.empty()) {
//         return;
//     }
//     // 如果当前字符是x，则删除它
//     if (s[0] == x) {
//         s.erase(0, 1);
//     }
//     // 递归调用函数，处理剩余部分的字符串
//     removeCharacter(s, x);
// }
// why？为什么将两个if合并成一个if就可以运行 不会在s.empty那里报Segmentation fault
void removeCharacter(string &s, char x)
{
    // 如果字符串为空或当前字符不是x，则直接返回
    if (s.empty() || s[0] != x)
    {
        return;
    }
    // 如果当前字符是x，则删除它
    s.erase(0, 1);
    // 递归调用函数，处理剩余部分的字符串
    removeCharacter(s, x);
}

// void removeCharacter(string& s, char x) {
//     // 如果字符串为空，则结束递归
//     if (s.empty()) {
//         return;
//     }
//     // 如果当前字符是x，则删除它
//     if (s[0] == x) {
//         s.erase(0, 1);
//         // 递归调用函数，处理剩余部分的字符串
//         removeCharacter(s, x);
//     }
//     // 如果当前字符不是x，继续递归调用函数
//     removeCharacter(s, x);
// }

int main()
{
    string s = "axsfdkjgxka'klksydfgmx";
    char x = 'a';
    cout << "计算机科学与技术 计算机222 章崇文" << endl;
    // 检查字符x是否出现在字符串s中
    if (checkCharacter(s, x))
    {
        cout << "字符 " << x << " 在字符串中找到了。" << endl;
    }
    else
    {
        cout << "字符 " << x << " 没有在字符串中找到。" << endl;
    }
    // 计算字符x在字符串s中出现的次数
    int count = countCharacter(s, x);
    cout << "字符 " << x << " 在字符串中出现了 " << count << " 次。" << endl;
    // 删除字符串s中的所有字符x
    removeCharacter(s, x);
    cout << "删除所有字符 " << x << " 后的字符串为：" << s << endl;
    return 0;
}