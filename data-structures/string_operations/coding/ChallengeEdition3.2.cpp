/*
 * @Description: 
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-11-16 22:28:40
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-11-16 22:35:39
 */
#include <iostream>
#include <fstream>
#include <regex>

void countBasicInfo(const std::string& file_path) {
    int chars = 0;
    int spaces = 0;
    int letters = 0;
    int digits = 0;
    int punctuations = 0;
    int chinese_chars = 0;
    int words = 0;
    int lines = 0;
    int paragraphs = 0;

    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    std::regex word_regex("\\b\\w+\\b");
    std::string line;

    std::regex chinese_regex("[\\u4e00-\\u9fff]");
    while (std::getline(file, line)) {
        chars += line.length();
        spaces += std::count(line.begin(), line.end(), ' ');
        letters += std::count_if(line.begin(), line.end(), [](unsigned char c) { return std::isalpha(c); });
        digits += std::count_if(line.begin(), line.end(), [](unsigned char c) { return std::isdigit(c); });
        punctuations += std::count_if(line.begin(), line.end(), [](unsigned char c) { return std::ispunct(c); });
        chinese_chars += std::distance(std::sregex_iterator(line.begin(), line.end(), chinese_regex), std::sregex_iterator());
        std::string::const_iterator line_begin = line.begin();
        std::string::const_iterator line_end = line.end();
        words += std::distance(std::sregex_iterator(line_begin, line_end, word_regex), std::sregex_iterator());
        lines++;
        if (line.empty()) {
            paragraphs++;
        }
    }

    file.close();

    std::cout << "Character count (including spaces): " << chars << std::endl;
    std::cout << "Space count: " << spaces << std::endl;
    std::cout << "Letter count: " << letters << std::endl;
    std::cout << "Digit count: " << digits << std::endl;
    std::cout << "Punctuation count: " << punctuations << std::endl;
    std::cout << "Chinese character count: " << chinese_chars << std::endl;
    std::cout << "Word count: " << words << std::endl;
    std::cout << "Line count: " << lines << std::endl;
    std::cout << "Paragraph count: " << paragraphs << std::endl;
}

int main() {
    std::string file_path = "text.txt";  // 替换为实际的文本文件路径
    countBasicInfo(file_path);

    return 0;
}