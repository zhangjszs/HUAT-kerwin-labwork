# JavaWeb 基础实验

本目录包含 JavaWeb 基础课程的实验代码，涵盖 Servlet、JSP、MySQL 等技术。

## 📋 实验列表

| 实验编号 | 实验名称 | 目录 | 主要内容 | 技术栈 |
|-----------|---------|------|---------|---------|
| 实验一 | JavaWeb 基础入门 | [WebLab1/](./WebLab1/) | Servlet 基础、JSP 页面 | Java, Servlet |
| 实验二 | 数据库操作 | [WebLab2/](./WebLab2/) | JDBC 连接、CRUD 操作 | Java, MySQL |
| 实验三 | 会话管理 | [WebLab3/](./WebLab3/) | Session、Cookie | Java, Servlet |
| 实验四 | 过滤器与监听器 | [webLab4/](./webLab4/) | Filter、Listener | Java, Servlet |
| 实验五 | MVC 模式 | [WebLab5/](./WebLab5/) | MVC 架构实现 | Java, Servlet |

## 🚀 快速开始

### 环境要求

- **JDK**: JDK 8 或更高版本
- **Web 容器**: Tomcat 8.5 或更高版本
- **数据库**: MySQL 5.7 或更高版本
- **IDE**: IntelliJ IDEA / Eclipse

### 运行步骤

```bash
# 进入实验目录
cd <实验目录>

# 使用 Maven 构建
mvn clean package

# 部署到 Tomcat
# 将 target 目录下的 war 文件复制到 Tomcat webapps 目录

# 启动 Tomcat
cd <tomcat-path>/bin
./startup.sh  # Linux/Mac
# startup.bat  # Windows
```

## 📊 实验亮点

- ✨ **Servlet 规范**：遵循 Servlet 规范
- 🎯 **数据库操作**：使用 JDBC 进行数据库连接和操作
- 🔍 **会话管理**：正确使用 Session 和 Cookie
- 📝 **详细注释**：便于理解 JavaWeb 开发流程

## 📚 学习资源

- [JavaWeb 实验一.docx](./JavaWeb实验一.docx)
- [JavaWeb 实验二.docx](./javaweb实验二.docx)
- [JavaWeb 实验三.docx](./javaweb实验三.docx)
- [JavaWeb 实验四.docx](./javaweb实验四.docx)
- [JavaWeb 实验五.docx](./javaweb实验五.docx)
- [JAVA高级编程实验2023.doc](./JAVA高级编程实验2023.doc)

## 💡 学习建议

1. **理解 Web 生命周期**：掌握 Servlet 的生命周期
2. **熟练 JDBC 操作**：掌握数据库连接、查询、更新、删除
3. **理解 MVC 模式**：理解 Model-View-Controller 的分离
4. **注意安全问题**：SQL 注入、XSS 攻击的防护

## 📝 实验报告

每个实验目录下都包含相应的实验文档，详细记录：
- 实验目的
- 实验环境
- 实验原理
- 实验步骤
- 实验结果
- 心得体会

## ⚠️ 注意事项

- 本目录代码仅供学习参考
- 请理解后用自己的方式实现
- 注意数据库连接信息的配置
- 注意 Tomcat 端口配置

---

**最后更新：2024年**
