/*
 * @Description:
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-12-18 13:47:48
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-12-19 13:19:44
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_set>

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

    int count = 0; // 用于计数每行已打印的元素个数

    for (int i : matrix)
    {
        if (count == 20)
        {
            std::cout << std::endl; // 打印满 20 个元素后换行
            count = 0;              // 重置计数器
        }

        if (i == searchNum)
        {
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << std::setw(5) << i;
            SetConsoleTextAttribute(consoleHandle, csbi.wAttributes);
        }
        else
        {
            std::cout << std::setw(5) << i;
        }

        count++;
    }
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void initializeMatrix()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<int> randomNumbers;
    for (int i = 1; i <= ROWS * COLS; i++)
    {
        int randomNumber = std::rand() % 1000 + 1;
        randomNumbers.push_back(randomNumber);
    }
    // std::random_shuffle(randomNumbers.begin(), randomNumbers.end());
    for (int i = 0; i < ROWS * COLS; i++)
    {
        matrix[i] = randomNumbers[i];
        OrderMatrix[i] = randomNumbers[i];
    }
    std::sort(OrderMatrix.begin(), OrderMatrix.end());
}

void sequentialSearch(int target)
{
    auto start = std::chrono::steady_clock::now(); // 记录开始时间
    bool found = false;
    for (int i : OrderMatrix)
    {
        printMatrix(OrderMatrix, i);
        if (i == target)
        {
            int m = i / COLS;
            int n = i % COLS;
            std::cout << "Found " << target << " at (" << m << ", " << n << ")" << std::endl;
            found = true;
            break;
        }
    }
    if (!found)
    {
        std::cout << "Target element not found." << std::endl;
    }
    auto end = std::chrono::steady_clock::now(); // 记录结束时间
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
}

void binarySearch(int target)
{
    auto start = std::chrono::steady_clock::now(); // 记录开始时间
    int left = 0;
    int right = OrderMatrix.size() - 1;
    bool found = false;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        printMatrix(OrderMatrix, OrderMatrix[mid]);

        if (OrderMatrix[mid] == target)
        {
            int i = mid / COLS;
            int j = mid % COLS;
            std::cout << "Found " << target << " at (" << i << ", " << j << ")" << std::endl;
            found = true;
            break;
        }
        else if (OrderMatrix[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    if (!found)
    {
        std::cout << "Target element not found." << std::endl;
    }
    auto end = std::chrono::steady_clock::now(); // 记录结束时间
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
}

void interpolationSearch(int target)
{
    auto start = std::chrono::steady_clock::now(); // 记录开始时间
    int left = 0;
    int right = ROWS * COLS - 1;
    bool found = false;

    while (left <= right)
    {
        int range = OrderMatrix[right] - OrderMatrix[left];
        if (range == 0)
        {
            if (OrderMatrix[left] == target)
            {
                int m = left / COLS;
                int n = left % COLS;
                std::cout << "Found " << target << " at (" << m << ", " << n << ")" << std::endl;
                found = true;
                break;
            }
            else
            {
                break;
            }
        }

        int mid = left + ((target - OrderMatrix[left]) * (right - left)) / range;

        printMatrix(OrderMatrix, OrderMatrix[mid]);

        if (OrderMatrix[mid] == target)
        {
            int m = mid / COLS;
            int n = mid % COLS;
            std::cout << "Found " << target << " at (" << m << ", " << n << ")" << std::endl;
            found = true;
            break;
        }
        else if (OrderMatrix[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    if (!found)
    {
        std::cout << "Target element not found." << std::endl;
    }
    auto end = std::chrono::steady_clock::now(); // 记录结束时间
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
}

void fibonacciSearch(int target)
{
    auto start = std::chrono::steady_clock::now(); // 记录开始时间

    int size = OrderMatrix.size();
    int fib2 = 0;          // 第二个斐波那契数
    int fib1 = 1;          // 第一个斐波那契数
    int fib = fib1 + fib2; // 当前斐波那契数

    // 找到大于等于数组大小的最小斐波那契数
    while (fib < size)
    {
        fib2 = fib1;
        fib1 = fib;
        fib = fib1 + fib2;
    }

    int offset = -1; // 偏移量

    while (fib > 1)
    {
        int i = std::min(offset + fib2, size - 1);
        printMatrix(OrderMatrix, OrderMatrix[i]);
        if (OrderMatrix[i] < target)
        {
            fib = fib1;
            fib1 = fib2;
            fib2 = fib - fib1;
            offset = i;
        }
        else if (OrderMatrix[i] > target)
        {
            fib = fib2;
            fib1 = fib1 - fib2;
            fib2 = fib - fib1;
        }
        else
        {
            auto end = std::chrono::steady_clock::now(); // 记录结束时间
            std::chrono::duration<double> elapsed_seconds = end - start;
            int m = i / COLS;
            int n = i % COLS;
            std::cout << "Found " << target << " at (" << m << ", " << n << ")" << std::endl;
            std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
            return;
        }
    }

    if (fib1 == 1 && OrderMatrix[offset + 1] == target)
    {
        auto end = std::chrono::steady_clock::now(); // 记录结束时间
        std::chrono::duration<double> elapsed_seconds = end - start;
        int m = (offset + 1) / COLS;
        int n = (offset + 1) % COLS;
        std::cout << "Found " << target << " at (" << m << ", " << n << ")" << std::endl;
        std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
        return;
    }

    auto end = std::chrono::steady_clock::now(); // 记录结束时间
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Target element not found." << std::endl;
    std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
}

void blockSearch(int target)
{
    auto start = std::chrono::steady_clock::now(); // 记录开始时间
    int numBlocks = std::sqrt(OrderMatrix.size()); // 计算块数

    // 计算每个块的大小
    int blockSize = std::ceil(static_cast<double>(OrderMatrix.size()) / numBlocks);

    // 在块中进行顺序查找
    int blockIndex = -1;
    for (int i = 0; i < numBlocks; ++i)
    {
        int startNum = i * blockSize;
        int endNum = std::min((i + 1) * blockSize, static_cast<int>(OrderMatrix.size()));

        if (OrderMatrix[startNum] <= target && target <= OrderMatrix[endNum - 1])
        {
            blockIndex = i;
            break;
        }
    }

    if (blockIndex == -1)
    {
        std::cout << "Target element not found." << std::endl;
        auto end = std::chrono::steady_clock::now(); // 记录结束时间
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
        return;
    }

    // 在选定的块中进行线性查找
    int startNum = blockIndex * blockSize;
    int endNum = std::min((blockIndex + 1) * blockSize, static_cast<int>(OrderMatrix.size()));
    for (int i = startNum; i < endNum; ++i)
    {
        printMatrix(OrderMatrix, OrderMatrix[i]);
        if (OrderMatrix[i] == target)
        {
            int m = i / COLS;
            int n = i % COLS;
            std::cout << "Found " << target << " at (" << m << ", " << n << ")" << std::endl;
            auto end = std::chrono::steady_clock::now(); // 记录结束时间
            std::chrono::duration<double> elapsed_seconds = end - start;
            std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
            return;
        }
    }

    std::cout << "Target element not found." << std::endl;
    auto end = std::chrono::steady_clock::now(); // 记录结束时间
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
}

void HashSearch(int target)
{
    auto start = std::chrono::steady_clock::now(); // 记录开始时间
    std::unordered_set<int> hashSet;
    for (const auto& num : matrix)
    {
        hashSet.insert(num);
    }
    int position = -1;
    for (int i = 0; i < matrix.size(); i++)
    {
        if (matrix[i] == target)
        {
            position = i;
            break;
        }
    }
    printMatrix(matrix, target);
    if (position == -1)
    {
        std::cout << "Target element not found." << std::endl;
        return;
    }
    int m = position / COLS;
    int n = position % COLS;
    std::cout << "Found " << target << " at (" << m << ", " << n << ")" << std::endl;
    auto end = std::chrono::steady_clock::now(); // 记录结束时间
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" << std::endl;
}

int main()
{
    bool quit = false;
    initializeMatrix();
    while (!quit)
    {
        int choice = 0;
        printMatrix(matrix);

        std::cout << "Please select a search method:" << std::endl;
        std::cout << "1. Sequential Search" << std::endl;
        std::cout << "2. Binary Search" << std::endl;
        std::cout << "3. interpolationSearch" << std::endl;
        std::cout << "4. fibonacciSearch" << std::endl;
        std::cout << "5. blockSearch" << std::endl;
        std::cout << "6. HashSearch" << std::endl;
        std::cout << "7. Quit" << std::endl;
        std::cout << "Your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int searchNum = 0;
            std::cout << "Please input a number to search: ";
            std::cin >> searchNum;
            sequentialSearch(searchNum);
            break;
        }
        case 2:
        {
            int searchNum = 0;
            std::cout << "Please input a number to search: ";
            std::cin >> searchNum;
            binarySearch(searchNum);
            break;
        }
        case 3:
        {
            int searchNum = 0;
            std::cout << "Please input a number to search: ";
            std::cin >> searchNum;
            interpolationSearch(searchNum);
            break;
        }
        case 4:
        {
            int searchNum = 0;
            std::cout << "Please input a number to search: ";
            std::cin >> searchNum;
            fibonacciSearch(searchNum);
            break;
        }
        case 5:
        {
            int searchNum = 0;
            std::cout << "Please input a number to search: ";
            std::cin >> searchNum;
            blockSearch(searchNum);
            break;
        }
        case 6:
        {
            int searchNum = 0;
            std::cout << "Please input a number to search: ";
            std::cin >> searchNum;
            HashSearch(searchNum);
            break;
        }
        case 7:
            quit = true;
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
            break;
        }
        if (!quit)
        {
            std::cout << "Press any key to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    }

    return 0;
}