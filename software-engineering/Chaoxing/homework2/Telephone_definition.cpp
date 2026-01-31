#include <string>
#include <regex>
#include <iostream>

// 基类：电话号码
class PhoneNumber {
public:
    virtual bool validate() const = 0;
    virtual std::string getNumber() const = 0;
    virtual ~PhoneNumber() = default;
};

// 校内电话：4位数字，第一位不是0
class InternalPhone : public PhoneNumber {
private:
    std::string number;  // 4位数字

public:
    InternalPhone(const std::string& num) : number(num) {}

    // 校内电话验证
    bool validate() const override {
        std::regex pattern("[1-9][0-9]{3}");
        return std::regex_match(number, pattern);
    }

    std::string getNumber() const override {
        return number;
    }
};

// 校外本市电话：0开头，后接8位数字，第一位不是0
class LocalPhone : public PhoneNumber {
private:
    std::string number;  // 0 + 8位数字

public:
    LocalPhone(const std::string& num) : number(num) {}

    // 本市电话验证
    bool validate() const override {
        std::regex pattern("0[1-9][0-9]{7}");
        return std::regex_match(number, pattern);
    }

    std::string getNumber() const override {
        return number;
    }
};

// 校外外地电话：0开头，后接3位区号和8位电话号码，第一位不是0
class LongDistancePhone : public PhoneNumber {
private:
    std::string areaCode;  // 3位区号
    std::string number;    // 8位电话号码

public:
    LongDistancePhone(const std::string& area, const std::string& num) 
        : areaCode(area), number(num) {}

    // 外地电话验证
    bool validate() const override {
        std::regex areaPattern("[0-9]{3}");
        std::regex numPattern("[1-9][0-9]{7}");
        return std::regex_match(areaCode, areaPattern) && std::regex_match(number, numPattern);
    }

    std::string getNumber() const override {
        return "0" + areaCode + number;
    }
};

// 示例代码测试
int main() {
    InternalPhone internal("1234");
    LocalPhone local("012345678");
    LongDistancePhone longDist("010", "12345678");

    if (internal.validate()) {
        std::cout << "校内电话有效: " << internal.getNumber() << std::endl;
    } else {
        std::cout << "校内电话无效" << std::endl;
    }

    if (local.validate()) {
        std::cout << "本市电话有效: " << local.getNumber() << std::endl;
    } else {
        std::cout << "本市电话无效" << std::endl;
    }

    if (longDist.validate()) {
        std::cout << "外地电话有效: " << longDist.getNumber() << std::endl;
    } else {
        std::cout << "外地电话无效" << std::endl;
    }

    return 0;
}
