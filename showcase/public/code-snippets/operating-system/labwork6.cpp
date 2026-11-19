#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem> // 需要C++17支持，用于文件系统操作

class FileSystem
{
public:
    // 文件创建
    bool createFile(const std::string &filename)
    {
        std::ofstream file(filename);
        if (file.is_open())
        {
            file.close();
            return true;
        }
        return false;
    }

    // 文件显示（简单读取并打印到控制台）
    void displayFile(const std::string &filename)
    {
        std::ifstream file(filename);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                std::cout << line << std::endl;
            }
            file.close();
        }
        else
        {
            std::cerr << "无法打开文件: " << filename << std::endl;
        }
    }

    // 文件列目录
    void listDirectory(const std::string &directory)
    {
        for (const auto &entry : std::filesystem::directory_iterator(directory))
        {
            std::cout << entry.path() << std::endl;
        }
    }

    // 文件删除
    bool deleteFile(const std::string &filename)
    {
        if (std::filesystem::exists(filename))
        {
            try
            {
                std::filesystem::remove(filename);
                return true;
            }
            catch (std::filesystem::filesystem_error &e)
            {
                std::cerr << "删除文件时出错：" << e.what() << std::endl;
            }
        }
        else
        {
            std::cerr << "文件不存在: " << filename << std::endl;
        }
        return false;
    }

    bool encryptFile(const std::string &inputFile, const std::string &outputFile)
    {
        const char key = 0x7F; // 示例密钥，可以替换为任意字符
        std::ifstream inFile(inputFile, std::ios::binary);
        std::ofstream outFile(outputFile, std::ios::binary);
        if (!inFile.is_open() || !outFile.is_open())
        {
            std::cerr << "无法打开文件" << std::endl;
            return false;
