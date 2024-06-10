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
        }
        char ch;
        while (inFile.get(ch))
        { // 逐字节读取
            // 对字符进行XOR加密
            char encryptedCh = ch ^ key;
            outFile.put(encryptedCh); // 写入加密后的字符
        }
        inFile.close();
        outFile.close();
        return true; // 如果一切顺利，返回true
    }

    bool decryptFile(const std::string &inputFile, const std::string &outputFile)
    {
        // 使用与加密时相同的密钥
        const char key = 0x7F;
        std::ifstream inFile(inputFile, std::ios::binary);
        std::ofstream outFile(outputFile, std::ios::binary);
        if (!inFile.is_open() || !outFile.is_open())
        {
            std::cerr << "无法打开文件" << std::endl;
            return false;
        }
        char ch;
        while (inFile.get(ch))
        {
            // 对字符进行XOR解密，注意加密和解密使用的操作是相同的（XOR的逆运算是其本身）
            char decryptedCh = ch ^ key;
            outFile.put(decryptedCh);
        }
        inFile.close();
        outFile.close();
        return true; // 解密成功
    }

    // 文件合并函数
    bool mergeFiles(const std::vector<std::string> &files, const std::string &outputFile)
    {
        std::ofstream outFile(outputFile, std::ios::binary | std::ios::app); // 打开或创建输出文件，并设置为追加模式

        if (!outFile.is_open())
        {
            std::cerr << "无法打开输出文件: " << outputFile << std::endl;
            return false;
        }

        for (const auto &file : files)
        {
            std::ifstream inFile(file, std::ios::binary);
            if (!inFile.is_open())
            {
                std::cerr << "无法打开输入文件: " << file << std::endl;
                outFile.close(); // 记得关闭已打开的输出文件
                return false;
            }

            // 将当前文件内容追加到输出文件
            outFile << inFile.rdbuf(); // 使用rdbuf()效率较高，直接拷贝缓冲区

            inFile.close(); // 关闭当前输入文件
        }

        outFile.close(); // 最后关闭输出文件
        return true;     // 如果一切顺利，返回true
    }

    void searchFile(const std::string &directory, const std::string &filename)
    {
        for (const auto &entry : std::filesystem::recursive_directory_iterator(directory))
        {
            if (entry.is_regular_file() && entry.path().filename() == filename)
            {
                std::cout << "找到文件: " << entry.path() << std::endl;
            }
        }
    }

    // 文件复制
    bool copyFile(const std::string &source, const std::string &destination)
    {
        if (std::filesystem::exists(source))
        {
            try
            {
                std::filesystem::copy(source, destination);
                return true;
            }
            catch (std::filesystem::filesystem_error &e)
            {
                std::cerr << "复制文件时出错：" << e.what() << std::endl;
            }
        }
        else
        {
            std::cerr << "源文件不存在: " << source << std::endl;
        }
        return false;
    }

    // 文件重命名
    bool renameFile(const std::string &oldName, const std::string &newName)
    {
        if (std::filesystem::exists(oldName))
        {
            try
            {
                std::filesystem::rename(oldName, newName);
                return true;
            }
            catch (std::filesystem::filesystem_error &e)
            {
                std::cerr << "重命名文件时出错：" << e.what() << std::endl;
            }
        }
        else
        {
            std::cerr << "原文件不存在: " << oldName << std::endl;
        }
        return false;
    }
};

int main()
{
    FileSystem fs;
    std::string filename, directory, inputFile, outputFile, oldName, newName,file;
    std::vector<std::string> files;

    while (true)
    {
        std::cout << "请选择要执行的操作(输入对应数字):" << std::endl;
        std::cout << "1. 创建文件" << std::endl;
        std::cout << "2. 显示文件内容" << std::endl;
        std::cout << "3. 列出目录内容" << std::endl;
        std::cout << "4. 删除文件" << std::endl;
        std::cout << "5. 加密文件" << std::endl;
        std::cout << "6. 解密文件" << std::endl;
        std::cout << "7. 合并文件" << std::endl;
        std::cout << "8. 搜索文件" << std::endl;
        std::cout << "9. 复制文件" << std::endl;
        std::cout << "10. 重命名文件" << std::endl;
        std::cout << "11. 退出" << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "请输入要创建的文件名: ";
            std::cin >> filename;
            if (fs.createFile(filename))
            {
                std::cout << "文件创建成功!" << std::endl;
            }
            else
            {
                std::cout << "文件创建失败!" << std::endl;
            }
            break;

        case 2:
            std::cout << "请输入要显示的文件名: ";
            std::cin >> filename;
            fs.displayFile(filename);
            break;

        case 3:
            std::cout << "请输入要列出的目录路径: ";
            std::cin >> directory;
            fs.listDirectory(directory);
            break;

        case 4:
            std::cout << "请输入要删除的文件名: ";
            std::cin >> filename;
            if (fs.deleteFile(filename))
            {
                std::cout << "文件删除成功!" << std::endl;
            }
            else
            {
                std::cout << "文件删除失败!" << std::endl;
            }
            break;

        case 5:
            std::cout << "请输入要加密的文件路径: ";
            std::cin >> inputFile;
            std::cout << "请输入加密后的输出文件路径: ";
            std::cin >> outputFile;
            if (fs.encryptFile(inputFile, outputFile))
            {
                std::cout << "文件加密成功!" << std::endl;
            }
            else
            {
                std::cout << "文件加密失败!" << std::endl;
            }
            break;

        case 6:
            std::cout << "请输入要解密的文件路径: ";
            std::cin >> inputFile;
            std::cout << "请输入解密后的输出文件路径: ";
            std::cin >> outputFile;
            if (fs.decryptFile(inputFile, outputFile))
            {
                std::cout << "文件解密成功!" << std::endl;
            }
            else
            {
                std::cout << "文件解密失败!" << std::endl;
            }
            break;

        case 7:
            std::cout << "请输入要合并的文件路径(用空格分隔): ";
            while (std::cin >> file)
            {
                files.push_back(file);
                if (std::cin.peek() == '\n')
                    break;
            }
            std::cout << "请输入合并后的输出文件路径: ";
            std::cin >> outputFile;
            if (fs.mergeFiles(files, outputFile))
            {
                std::cout << "文件合并成功!" << std::endl;
            }
            else
            {
                std::cout << "文件合并失败!" << std::endl;
            }
            files.clear();
            break;

        case 8:
            std::cout << "请输入要搜索的目录路径: ";
            std::cin >> directory;
            std::cout << "请输入要搜索的文件名: ";
            std::cin >> filename;
            fs.searchFile(directory, filename);
            break; // Add a break statement here

        case 9:
            std::cout << "请输入要复制的源文件路径: ";
            std::cin >> inputFile;
            std::cout << "请输入要复制到的目标文件路径: ";
            std::cin >> outputFile;
            if (fs.copyFile(inputFile, outputFile))
            {
                std::cout << "文件复制成功!" << std::endl;
            }
            else
            {
                std::cout << "文件复制失败!" << std::endl;
            }
            break;

        case 10:
            std::cout << "请输入要重命名的文件路径: ";
            std::cin >> oldName;
            std::cout << "请输入新的文件名: ";
            std::cin >> newName;
            if (fs.renameFile(oldName, newName))
            {
                std::cout << "文件重命名成功!" << std::endl;
            }
            else
            {
                std::cout << "文件重命名失败!" << std::endl;
            }
            break;

        case 11:
            std::cout << "谢谢使用,再见!" << std::endl;
            return 0;

        default:
            std::cout << "无效的选择,请重试." << std::endl;
            break;
        }
    }
}