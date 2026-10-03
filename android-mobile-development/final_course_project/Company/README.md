# Company — 企业内部管理 Android 应用

![Kotlin](https://img.shields.io/badge/Kotlin-1.9-purple.svg)
![Room](https://img.shields.io/badge/Room-2.6-green.svg)
![Architecture](https://img.shields.io/badge/Architecture-MVVM-blue.svg)
![MinSdk](https://img.shields.io/badge/minSdk-24-brightgreen.svg)

Android 移动开发课程最终设计：一款企业内部管理应用，覆盖**员工端**与**管理端**双角色的完整业务闭环——考勤签到（含定位校验）、请假申请与审批、申诉处理、企业公告与员工检索。

## 功能总览

### 员工端

| 功能 | 说明 |
| ---- | ---- |
| 登录 / 注册 | 员工账号体系，启动页即登录 |
| 企业公告（News） | 浏览管理员发布的企业动态 |
| 考勤签到（Qiandao） | 运行时申请定位权限，记录签到时间与位置 |
| 考勤历史 | 按时间回看个人签到记录 |
| 请假申请 | 提交请假单，跟踪审批状态 |
| 申诉 | 对考勤/审批结果发起申诉 |
| 个人资料 | 查看与维护个人信息 |

### 管理端

| 功能 | 说明 |
| ---- | ---- |
| 管理员登录（Glogin） | 与员工端隔离的管理入口 |
| 管理面板（Guanli） | 管理功能聚合入口 |
| 员工检索（Search） | 按条件查询员工信息 |
| 请假审批（LeaveManage） | 审批员工请假单 |
| 申诉处理（AppealManage） | 复核并处理员工申诉 |

## 技术架构

**Kotlin + MVVM + Room + Repository**，按功能特性（feature）分包：

```
com.example.company/
├── ui/                  # 按特性分包的 Activity + ViewModel
│   ├── login/ register/ intro/ main/ news/
│   ├── attendance/ history/        # 签到与考勤历史
│   ├── leave/ appeal/ profile/     # 请假、申诉、个人资料
│   └── admin/                      # 管理端（登录、面板、检索）
├── data/
│   ├── entity/          # Room 实体：User, AdminUser, News,
│   │                    #   AttendanceRecord, LeaveRequest, Appeal
│   ├── dao/             # 每实体一个 DAO
│   ├── repository/      # 仓库层，隔离数据源与 UI
│   └── CompanyDatabase  # Room 数据库入口
└── util/                # LocationHelper 等工具
```

**技术要点**：

- **MVVM**：每个特性配备独立 ViewModel，`LiveData` 驱动界面刷新，Activity 不直接触达数据层
- **Room 持久化**：6 个实体 + kapt 编译期校验 SQL，`room-ktx` 提供协程挂起函数支持
- **Repository 模式**：DAO 之上统一封装业务数据访问，为后续接入远程数据源留出接口
- **定位签到**：`LocationHelper` 封装定位能力，`QiandaoActivity` 处理 `ACCESS_FINE_LOCATION` 运行时权限申请与拒绝降级
- **双角色会话**：员工与管理员账号体系分离，入口 Activity 隔离

## 构建运行

```bash
git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
cd HUAT-kerwin-labwork/android-mobile-development/final_course_project/Company
./gradlew assembleDebug
# 或用 Android Studio（AGP 8.5+）直接打开 Company 目录运行
```

- minSdk 24 / targetSdk 35
- 首次启动自动建库；管理员账号由数据库预置

## 设计文档

[`docs/plans/2026-02-24-modernization-refactor.md`](./docs/plans/2026-02-24-modernization-refactor.md) 记录了项目从「Java + SQLiteOpenHelper 无架构」向「Kotlin + Room + MVVM」的现代化重构计划，可作为 Android 课设架构演进的参考。

---

> 📁 本项目是 [HUAT-kerwin-labwork](https://github.com/zhangjszs/HUAT-kerwin-labwork) 课程作品集的一部分，项目展示页见 [huat-showcase](https://github.com/zhangjszs/huat-showcase)。
