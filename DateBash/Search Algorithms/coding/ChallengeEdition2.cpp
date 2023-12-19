#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
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

std::vector<int> matrix(ROWS *COLS);
std::vector<int> OrderMatrix(ROWS *COLS);

void clearScreen()
{
    CLEAR_SCREEN();
}

void printMatrix(const std::vector<int> &matrix)
{
    clearScreen();

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            std::cout << std::setw(5) << matrix[i * COLS + j];
        }
        std::cout << std::endl;
    }
}

void printMatrix(const std::vector<int> &matrix, int searchNum)
{
    clearScreen();

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(consoleHandle, &csbi);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (matrix[i * COLS + j] == searchNum)
            {
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << std::setw(5) << matrix[i * COLS + j];
                SetConsoleTextAttribute(consoleHandle, csbi.wAttributes);
            }
            else
            {
                std::cout << std::setw(5) << matrix[i * COLS + j];
            }
        }
        std::cout << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void initializeMatrix()
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
    for (int i = 0; i < ROWS * COLS; i++)
    {
        matrix[i] = randomNumbers[i];
        OrderMatrix[i] = randomNumbers[i];
    }

    // 对 OrderMatrix 进行排序
    for (int i = 0; i < ROWS; i++)
    {
        // std::sort(OrderMatrix.begin() + i * COLS, OrderMatrix.begin() + (i + 1) * COLS);
        std::sort(OrderMatrix.begin(), OrderMatrix.end());
    }
}

void sequentialSearch(int target)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printMatrix(OrderMatrix, OrderMatrix[i * COLS + j]);
            if (OrderMatrix[i * COLS + j] == target)
            {
                std::cout << "Found " << target << " at (" << i << ", " << j << ")" << std::endl;
                return;
            }
        }
    }
    std::cout << "Target element not found." << std::endl;
}

void binarySearch(int target)
{
   
        int left = 0;
        int right = ROWS *COLS - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            printMatrix(OrderMatrix, matrix[mid]);

            if (matrix[mid] == target)
            {
                int i = mid / COLS;
                int j = mid % COLS;
                std::cout << "Found " << target << " at (" << i << ", " << j << ")" << std::endl;
                return;
            }
            else if (matrix[mid] < target)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
    }
    std::cout << "Target element not found." << std::endl;
}

int main()
{
    initializeMatrix();
    printMatrix(matrix);

    int searchNum = 0;
    std::cout << "Please input a number to search: ";
    std::cin >> searchNum;
    binarySearch(searchNum);
    // sequentialSearch(searchNum);
    return 0;
}