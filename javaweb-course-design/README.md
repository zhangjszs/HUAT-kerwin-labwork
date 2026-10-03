# JavaWeb 课程设计：ForestBlog 博客系统

![Java](https://img.shields.io/badge/Java-8-orange.svg)
![SSM](https://img.shields.io/badge/SSM-Spring%204.3%20%7C%20SpringMVC%20%7C%20MyBatis-green.svg)
![JSP](https://img.shields.io/badge/View-JSP-blue.svg)

JavaWeb 课程设计作品：一个基于 **SSM（Spring + SpringMVC + MyBatis）+ JSP** 的博客系统。Maven 构建、`war` 打包，需部署到外部 Tomcat 运行（无内嵌容器）。功能覆盖文章、分类、标签、评论、公告、友情链接、页面与多用户后台管理。

> 说明：本目录基于上游开源项目 [saysky/ForestBlog](https://github.com/saysky/ForestBlog) 完成课程设计存档。`ForestBlog-master/README.md` 是**上游作者原文**（含作者的联系方式与付费服务信息，与本课程设计无关），其运行细节可作参考，但请以本 README 与仓库实际代码为准。

## 技术栈

依据 [`ForestBlog-master/ForestBlog/pom.xml`](ForestBlog-master/ForestBlog/pom.xml)：

| 层次 | 技术 |
| ---- | ---- |
| 控制层 | Spring MVC 4.3.19（`spring-webmvc`） |
| 业务 / 持久层 | Spring 4.3.19 + MyBatis 3.4.0 / mybatis-spring 1.3.0 |
| 视图层 | JSP + JSTL（`javax.servlet.jsp-api` 2.3.1） |
| 数据库 | MySQL（Connector/J 8.0.11）+ Druid 连接池 1.0.16 |
| 日志 | Logback 1.1.2 |
| 构建 / 运行 | Maven · `war` 打包 · Java 8 编译目标 · 外部 Tomcat |

## 目录结构

```text
javaweb-course-design/
├── ForestBlog-master/
│   ├── ForestBlog/                  # Maven 工程根目录（pom.xml 所在）
│   │   ├── pom.xml
│   │   ├── src/main/java/com/kerwin/ssm/blog/
│   │   │   ├── controller/          # admin/（后台）、home/（前台）控制器
│   │   │   ├── service/ entity/ mapper/ dto/ enums/ util/
│   │   ├── src/main/resources/
│   │   │   ├── spring/              # spring-mvc.xml、spring-mybatis.xml
│   │   │   ├── mybatis/ mapper/     # MyBatis 全局配置与各表 Mapper XML
│   │   │   ├── db.properties        # 数据库连接配置（需自行填写）
│   │   │   └── logback.xml
│   │   └── src/main/webapp/
│   │       ├── WEB-INF/view/        # JSP 页面（Admin/ 后台、Home/ 前台）
│   │       └── resource/            # 前端静态资源（css/js/img/plugin）
│   ├── uploads/                     # 上传图片目录（与源码分离）
│   ├── forest_blog.sql              # 数据库初始化脚本（12 张表）
│   └── README.md                    # 上游作者原文
└── README.md
```

## 环境要求

| 组件 | 版本 |
| ---- | ---- |
| JDK | 8（`pom.xml` 编译目标 1.8） |
| Maven | 3.x |
| MySQL | 5.7 / 8.0 |
| Tomcat | 8.x / 8.5 / 9.x（Servlet 3.1 规范） |

## 运行步骤

1. 克隆仓库并进入目录：

   ```bash
   git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
   cd HUAT-kerwin-labwork/javaweb-course-design
   ```

2. 创建数据库（`forest_blog.sql` 不含建库语句，需先手动建库），再导入脚本：

   ```sql
   CREATE DATABASE forest_blog DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
   ```

   ```bash
   mysql -u root -p forest_blog < ForestBlog-master/forest_blog.sql
   ```

3. 配置数据源：编辑 `ForestBlog-master/ForestBlog/src/main/resources/db.properties`，将 `mysql.url` / `mysql.username` / `mysql.password` 改为本机 MySQL 的实际值（默认 url 指向 `127.0.0.1:3306/forest_blog`）。

4. 构建 WAR：

   ```bash
   cd ForestBlog-master/ForestBlog
   mvn clean package
   # 默认产物：target/ForestBlog-1.0.0-SNAPSHOT.war
   ```

5. 部署到 Tomcat：将 war 放入 Tomcat 的 `webapps/`，并把应用上下文设为根路径 `/`（例如重命名为 `ROOT.war`；IDEA 中则把 Application context 配为 `/`）。前端资源使用 `/css/**`、`/js/**` 等绝对路径映射，上下文不是 `/` 时页面样式会全部丢失。

6. 访问入口：

   | 入口 | 地址 |
   | ---- | ---- |
   | 前台首页 | `http://localhost:8080/` |
   | 后台管理 | `http://localhost:8080/admin` |
   | 登录 | `http://localhost:8080/login` |
   | 注册 | `http://localhost:8080/register` |

   初始化脚本含演示用户数据（见 `forest_blog.sql` 的 `user` 表），也可直接通过 `/register` 注册新用户。

7. 图片上传（可选）：上传目录与源码分离，需两处配置后才能使用——
   - `ForestBlog-master/ForestBlog/src/main/java/com/kerwin/ssm/blog/controller/admin/UploadFileController.java` 中的 `rootPath` 目前是作者机器的硬编码路径，需改为本机 `uploads/` 所在目录；
   - 在 Tomcat 中把 `/uploads` 映射到该目录（IDEA 在 Deployment 里添加；Tomcat 在 `conf/server.xml` 的 `<Host>` 内添加 `<Context path="/uploads" docBase="..."/>`）。

   更详细的图文步骤见 `ForestBlog-master/README.md`（上游原文）。

> ⚠️ 本文的构建与部署步骤基于 `pom.xml`、`web.xml`、`spring-*.xml` 静态核对编写，**未在本机实测**（本机无 JDK / Maven / MySQL / Tomcat 环境）。

## 来源与许可

- 上游项目：[saysky/ForestBlog](https://github.com/saysky/ForestBlog)，MIT License
- `ForestBlog-master/` 下代码与 `README.md` 均来自上游，保持存档原貌
- 本人课程设计环节基于该项目完成并提交

---

> 📁 本项目是 [HUAT-kerwin-labwork](https://github.com/zhangjszs/HUAT-kerwin-labwork) 课程作品集的一部分，项目展示页见 [huat-showcase](https://github.com/zhangjszs/huat-showcase)。
