# Android 移动开发 — 期末课程设计

本目录包含《Android 移动开发》课程的期末作业与项目源码。

## 📁 目录结构

| 目录/文件 | 说明 |
|-----------|------|
| [移动开发期末作业.docx](./移动开发期末作业.docx) | 期末作业文档（Git LFS 管理） |
| [Company/](./Company/) | Android Studio 项目源码 |
| [.gitignore](./.gitignore) | Git 忽略规则 |
| [.idea/](./.idea/) | IntelliJ IDEA 项目配置 |

## 📱 Company 项目

`Company/` 是一个完整的 **Android Studio (Gradle)** 项目：

```
Company/
├── app/                # 应用模块（源码、资源、Manifest）
│   └── src/
├── docs/plans/         # 项目文档
├── build.gradle        # 项目级构建脚本
├── gradle.properties   # Gradle 配置
├── gradlew             # Gradle Wrapper（Unix）
├── gradlew.bat         # Gradle Wrapper（Windows）
└── settings.gradle     # 模块设置
```

## 🚀 运行方式

1. 使用 **Android Studio** 打开 `Company/` 目录
2. 等待 Gradle 同步完成
3. 连接设备或启动模拟器后运行

## ⚠️ 注意事项

- 期末作业文档使用 Git LFS 管理，克隆后请执行 `git lfs pull`
- 首次打开项目时 Gradle 会自动下载依赖，请确保网络畅通

---

*最后更新：2026年7月27日*
