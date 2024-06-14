-- 创建数据库
CREATE DATABASE Onlinebookstoremanagementsystem
USE Onlinebookstoremanagementsystem
-- 创建用户表
CREATE TABLE User (
                      UserID INT PRIMARY KEY AUTO_INCREMENT,
                      Username VARCHAR(50) NOT NULL,
                      Password VARCHAR(50) NOT NULL,
                      Email VARCHAR(100) NOT NULL,
                      RegistrationDate DATE NOT NULL
);

-- 创建书籍表
CREATE TABLE Book (
                      BookID INT PRIMARY KEY AUTO_INCREMENT,
                      Title VARCHAR(100) NOT NULL,
                      Author VARCHAR(100) NOT NULL,
                      PublicationDate DATE NOT NULL,
                      Price DECIMAL(10, 2) NOT NULL
);

-- 创建订单表
CREATE TABLE `order` (
                         OrderID INT PRIMARY KEY AUTO_INCREMENT,
                         UserID INT NOT NULL,
                         OrderDate DATE NOT NULL,
                         TotalAmount DECIMAL(10, 2) NOT NULL,
                         Status VARCHAR(20) NOT NULL,
                         FOREIGN KEY (UserID) REFERENCES User(UserID)
);

-- 创建订单明细表
CREATE TABLE OrderDetail (
                             OrderDetailID INT PRIMARY KEY AUTO_INCREMENT,
                             OrderID INT NOT NULL,
                             BookID INT NOT NULL,
                             Quantity INT NOT NULL,
                             UnitPrice DECIMAL(10, 2) NOT NULL,
                             FOREIGN KEY (OrderID) REFERENCES `order`(OrderID),
                             FOREIGN KEY (BookID) REFERENCES Book(BookID)
);

-- 创建评论表
CREATE TABLE Review (
                        ReviewID INT PRIMARY KEY AUTO_INCREMENT,
                        BookID INT NOT NULL,
                        UserID INT NOT NULL,
                        Rating INT NOT NULL,
                        Comment TEXT NOT NULL,
                        ReviewDate DATE NOT NULL,
                        FOREIGN KEY (BookID) REFERENCES Book(BookID),
                        FOREIGN KEY (UserID) REFERENCES User(UserID)
);
