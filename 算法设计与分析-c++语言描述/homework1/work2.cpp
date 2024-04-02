/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02 13:11:56
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02 13:18:27
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\homework1\work2.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
对于一个采用字符数组存放的字符串 str，设计一个递归算法判断 str 是否为回文，并调试运行。（25分）
*/
#include <iostream>
#include <cstring>

using namespace std;

bool isPalindrome(char str[], int left, int right)
{
    // 如果左指针大于右指针,说明已经遍历完整个字符串,是回文
    if (left >= right)
    {
        return true;
    }

    // 如果当前字符不相等,说明不是回文
    if (str[left] != str[right])
    {
        return false;
    }

    // 递归检查左右两个指针指向的字符是否相等
    return isPalindrome(str, left + 1, right - 1);
}

int main()
{
    cout << "计算机科学与技术 计算机222 章崇文" << endl;
    char str[] = "abcba";
    int length = strlen(str);
    cout << "字符串为：";
    for (int i = 0; i < length; i++)
    {
        cout << str[i];
    }
    if (isPalindrome(str, 0, length - 1))
    {
        cout << "\n该字符串是回文字符串。" << endl;
    }
    else
    {
        cout << "该字符串不是回文字符串。" << endl;
    }

    return 0;
}