/*
 * @Description:
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-12-18 13:47:48
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-12-18 22:56:23
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <windows.h>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#else
#include <unistd.h>
#define CLEAR_SCREEN() system("clear")
#endif

const int ROWS = 10;
const int COLS = 20;

int matrix[ROWS][COLS];
int OrderMatrix[ROWS][COLS];

using namespace std;

void clearScreen()
{
    CLEAR_SCREEN();
}

void printMatrix(int matrix[ROWS][COLS])
{
    clearScreen();

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            std::cout << std::setw(5) << matrix[i][j];
        }
        std::cout << std::endl;
    }
}

void printMatrix(int matrix[ROWS][COLS], int SearchNum)
{
    clearScreen();

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(consoleHandle, &csbi);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (matrix[i][j] == SearchNum)
            {
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << std::setw(5) << matrix[i][j];
                SetConsoleTextAttribute(consoleHandle, csbi.wAttributes);
            }
            else
            {
                std::cout << std::setw(5) << matrix[i][j];
            }
        }
        std::cout << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void InitializeMatrix()
{
    // 使用当前时间作为随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 生成不重复的随机数
    std::vector<int> randomNumbers;
    for (int i = 1; i <= ROWS * COLS; i++)
    {
        randomNumbers.push_back(i);
    }
    std::random_shuffle(randomNumbers.begin(), randomNumbers.end());

    // 填充矩阵并排序
    int index = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            matrix[i][j] = randomNumbers[index];
            OrderMatrix[i][j] = randomNumbers[index];
            index++;
        }
    }

    // 对 OrderMatrix 进行排序
    for (int i = 0; i < ROWS; i++)
    {
        std::sort(OrderMatrix[i], OrderMatrix[i] + COLS);
    }
}

void sequentialSearch(int target)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printMatrix(OrderMatrix, OrderMatrix[i][j]);
            if (OrderMatrix[i][j] == target)
            {
                std::cout << "Found " << target << " at (" << i << ", " << j << ")" << std::endl;
                return;
            }
        }
    }
}

void binarySearch(int target)
{
    for (int i = 0; i < ROWS; i++)
    {
        int left = 0;
        int right = COLS - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            printMatrix(OrderMatrix, matrix[i][mid]);
            
            if (matrix[i][mid] == target)
            {
                std::cout << "Found " << target << " at (" << i << ", " << mid << ")" << std::endl;
                return;
            }
            else if (matrix[i][mid] < target)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }
}

int main()
{
    InitializeMatrix();
    printMatrix(matrix);

    int SearchNum = 0;
    cout << "Please input a number to search: ";
    cin >> SearchNum;
    binarySearch(SearchNum);
    // sequentialSearch(SearchNum);
    return 0;
}