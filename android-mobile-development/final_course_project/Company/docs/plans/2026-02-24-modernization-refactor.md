# Company App 完整现代化重构实施计划

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** 将 Java + SQLiteOpenHelper + 无架构的 Android 课设项目，重构为 Kotlin + Room + MVVM + Clean Architecture + Hilt + Coroutines 的现代化项目。

**Architecture:** MVVM + Clean Architecture 三层分离（UI Layer → Domain Layer → Data Layer）。UI 层使用 ViewModel + LiveData 管理状态，Data 层使用 Room + Repository 模式，Domain 层使用 UseCase 封装业务逻辑，Hilt 负责依赖注入，Coroutines 处理异步操作。

**Tech Stack:** Kotlin 1.9.x, Room 2.6.x, Hilt 2.51.x, Coroutines 1.8.x, Lifecycle (ViewModel + LiveData) 2.8.x, AGP 8.5.x, Gradle 8.7

---

## Task 1: 升级 Gradle + AGP + compileSdk（基础设施）

**Files:**
- Modify: `gradle/wrapper/gradle-wrapper.properties`
- Modify: `build.gradle` (root)
- Modify: `app/build.gradle`
- Modify: `gradle.properties`
- Modify: `settings.gradle`

**Step 1: 升级 gradle-wrapper.properties**

```properties
distributionBase=GRADLE_USER_HOME
distributionPath=wrapper/dists
zipStoreBase=GRADLE_USER_HOME
zipStorePath=wrapper/dists
distributionUrl=https\://services.gradle.org/distributions/gradle-8.7-bin.zip
```

**Step 2: 升级 root build.gradle**

```groovy
buildscript {
    repositories {
        google()
        mavenCentral()
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:8.5.2'
        classpath 'org.jetbrains.kotlin:kotlin-gradle-plugin:1.9.24'
        classpath 'com.google.dagger:hilt-android-gradle-plugin:2.51.1'
    }
}

allprojects {
    repositories {
        google()
        mavenCentral()
    }
}

task clean(type: Delete) {
    delete rootProject.buildDir
}
```

**Step 3: 升级 app/build.gradle**

```groovy
plugins {
    id 'com.android.application'
    id 'org.jetbrains.kotlin.android'
    id 'kotlin-kapt'
    id 'com.google.dagger.hilt.android'
}

android {
    namespace 'com.example.company'
    compileSdk 35

    defaultConfig {
        applicationId "com.example.company"
        minSdk 24
        targetSdk 35
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "androidx.test.runner.AndroidJUnitRunner"
    }

    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 'proguard-rules.pro'
        }
    }

    compileOptions {
        sourceCompatibility JavaVersion.VERSION_17
        targetCompatibility JavaVersion.VERSION_17
    }

    kotlinOptions {
        jvmTarget = '17'
    }
}

dependencies {
    // AndroidX
    implementation 'androidx.core:core-ktx:1.13.1'
    implementation 'androidx.appcompat:appcompat:1.7.0'
    implementation 'androidx.constraintlayout:constraintlayout:2.1.4'
    implementation 'com.google.android.material:material:1.12.0'

    // Lifecycle (ViewModel + LiveData)
    implementation 'androidx.lifecycle:lifecycle-viewmodel-ktx:2.8.7'
    implementation 'androidx.lifecycle:lifecycle-livedata-ktx:2.8.7'
    implementation 'androidx.lifecycle:lifecycle-runtime-ktx:2.8.7'
    implementation 'androidx.activity:activity-ktx:1.9.3'

    // Room
    implementation 'androidx.room:room-runtime:2.6.1'
    implementation 'androidx.room:room-ktx:2.6.1'
    kapt 'androidx.room:room-compiler:2.6.1'

    // Hilt
    implementation 'com.google.dagger:hilt-android:2.51.1'
    kapt 'com.google.dagger:hilt-android-compiler:2.51.1'

    // Coroutines
    implementation 'org.jetbrains.kotlinx:kotlinx-coroutines-android:1.8.1'

    // Testing
    testImplementation 'junit:junit:4.13.2'
    androidTestImplementation 'androidx.test.ext:junit:1.2.1'
    androidTestImplementation 'androidx.test.espresso:espresso-core:3.6.1'
}

kapt {
    correctErrorTypes = true
}
```

**Step 4: 更新 gradle.properties**

追加一行：
```properties
android.nonTransitiveRClass=true
```

**Step 5: 更新 settings.gradle**

```groovy
pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.PREFER_SETTINGS)
    repositories {
        google()
        mavenCentral()
    }
}
rootProject.name = "Company"
include ':app'
```

**Step 6: 更新 AndroidManifest.xml**

移除 `package` 属性（已迁移到 build.gradle 的 `namespace`），移除不再需要的存储权限。

**Step 7: Commit**

```bash
git add -A && git commit -m "chore: upgrade Gradle 8.7, AGP 8.5.2, compileSdk 35, add Kotlin/Room/Hilt/Coroutines deps"
```

---

## Task 2: 将 Model 类转换为 Kotlin data class + Room Entity

**Files:**
- Delete: `app/src/main/java/com/example/company/User.java`
- Delete: `app/src/main/java/com/example/company/AdminUser.java`
- Delete: `app/src/main/java/com/example/company/News.java`
- Delete: `app/src/main/java/com/example/company/AttendanceRecord.java`
- Create: `app/src/main/java/com/example/company/data/entity/User.kt`
- Create: `app/src/main/java/com/example/company/data/entity/AdminUser.kt`
- Create: `app/src/main/java/com/example/company/data/entity/News.kt`
- Create: `app/src/main/java/com/example/company/data/entity/AttendanceRecord.kt`

**Step 1: 创建 User.kt**

```kotlin
package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "USER")
data class User(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "name") val name: String,
    @ColumnInfo(name = "password") val password: String,
    @ColumnInfo(name = "sex") val sex: String = "未指定",
    @ColumnInfo(name = "tel") val tel: String = "未指定",
    @ColumnInfo(name = "position") val position: String = "未指定",
    @ColumnInfo(name = "TIME") val time: String = "1970-01-01"
)
```

**Step 2: 创建 AdminUser.kt**

```kotlin
package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "ADMINUSER")
data class AdminUser(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "admin_name") val adminName: String,
    @ColumnInfo(name = "admin_password") val adminPassword: String
)
```

**Step 3: 创建 News.kt**

```kotlin
package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "NEWS")
data class News(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "TITLE") val title: String,
    @ColumnInfo(name = "IMAGE_RESOURCE_ID") val imageResourceId: Int,
    @ColumnInfo(name = "CONTENT") val content: String
)
```

**Step 4: 创建 AttendanceRecord.kt**

```kotlin
package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.ForeignKey
import androidx.room.PrimaryKey

@Entity(
    tableName = "ATTENDANCE",
    foreignKeys = [ForeignKey(
        entity = User::class,
        parentColumns = ["_id"],
        childColumns = ["user_id"],
        onDelete = ForeignKey.CASCADE
    )]
)
data class AttendanceRecord(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "user_id") val userId: Int,
    @ColumnInfo(name = "date") val date: String,
    @ColumnInfo(name = "time_in") val timeIn: String = "09:00:00",
    @ColumnInfo(name = "time_out") val timeOut: String = "18:00:00",
    @ColumnInfo(name = "status_in") val statusIn: String = "",
    @ColumnInfo(name = "status_out") val statusOut: String = ""
)
```

**Step 5: 删除旧 Java Model 文件，Commit**

```bash
git rm app/src/main/java/com/example/company/User.java
git rm app/src/main/java/com/example/company/AdminUser.java
git rm app/src/main/java/com/example/company/News.java
git rm app/src/main/java/com/example/company/AttendanceRecord.java
git add app/src/main/java/com/example/company/data/
git commit -m "refactor: convert Java models to Kotlin data classes with Room Entity annotations"
```

---

## Task 3: 创建 Room DAO + Database

**Files:**
- Create: `app/src/main/java/com/example/company/data/dao/UserDao.kt`
- Create: `app/src/main/java/com/example/company/data/dao/AdminUserDao.kt`
- Create: `app/src/main/java/com/example/company/data/dao/NewsDao.kt`
- Create: `app/src/main/java/com/example/company/data/dao/AttendanceDao.kt`
- Create: `app/src/main/java/com/example/company/data/CompanyDatabase.kt`

**Step 1: 创建 UserDao.kt**

```kotlin
package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.User
import kotlinx.coroutines.flow.Flow

@Dao
interface UserDao {
    @Query("SELECT * FROM USER WHERE name = :name AND password = :password LIMIT 1")
    suspend fun validateUser(name: String, password: String): User?

    @Query("SELECT * FROM USER WHERE name = :name LIMIT 1")
    suspend fun findByName(name: String): User?

    @Query("SELECT * FROM USER ORDER BY name DESC")
    fun getAllUsers(): Flow<List<User>>

    @Insert(onConflict = OnConflictStrategy.ABORT)
    suspend fun insert(user: User): Long

    @Update
    suspend fun update(user: User): Int

    @Query("DELETE FROM USER WHERE name = :name")
    suspend fun deleteByName(name: String): Int

    @Query("SELECT _id FROM USER WHERE name = :name LIMIT 1")
    suspend fun getUserIdByName(name: String): Int?
}
```

**Step 2: 创建 AdminUserDao.kt**

```kotlin
package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.AdminUser

@Dao
interface AdminUserDao {
    @Query("SELECT * FROM ADMINUSER WHERE admin_name = :name AND admin_password = :password LIMIT 1")
    suspend fun validateAdmin(name: String, password: String): AdminUser?

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(admin: AdminUser): Long
}
```

**Step 3: 创建 NewsDao.kt**

```kotlin
package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.News
import kotlinx.coroutines.flow.Flow

@Dao
interface NewsDao {
    @Query("SELECT * FROM NEWS")
    fun getAllNews(): Flow<List<News>>

    @Query("SELECT * FROM NEWS WHERE _id = :id")
    suspend fun getNewsById(id: Int): News?

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(news: News): Long
}
```

**Step 4: 创建 AttendanceDao.kt**

```kotlin
package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.AttendanceRecord

@Dao
interface AttendanceDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(record: AttendanceRecord): Long

    @Query("SELECT * FROM ATTENDANCE WHERE user_id = :userId AND date = :date LIMIT 1")
    suspend fun getByUserAndDate(userId: Int, date: String): AttendanceRecord?

    @Update
    suspend fun update(record: AttendanceRecord): Int

    @Query("SELECT * FROM ATTENDANCE WHERE user_id = :userId ORDER BY date DESC")
    suspend fun getByUserId(userId: Int): List<AttendanceRecord>

    @Query("DELETE FROM ATTENDANCE WHERE user_id = :userId")
    suspend fun deleteByUserId(userId: Int)
}
```

**Step 5: 创建 CompanyDatabase.kt**

```kotlin
package com.example.company.data

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase
import androidx.sqlite.db.SupportSQLiteDatabase
import com.example.company.R
import com.example.company.data.dao.*
import com.example.company.data.entity.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

@Database(
    entities = [User::class, AdminUser::class, News::class, AttendanceRecord::class],
    version = 1,
    exportSchema = false
)
abstract class CompanyDatabase : RoomDatabase() {
    abstract fun userDao(): UserDao
    abstract fun adminUserDao(): AdminUserDao
    abstract fun newsDao(): NewsDao
    abstract fun attendanceDao(): AttendanceDao

    companion object {
        @Volatile
        private var INSTANCE: CompanyDatabase? = null

        fun getDatabase(context: Context): CompanyDatabase {
            return INSTANCE ?: synchronized(this) {
                val instance = Room.databaseBuilder(
                    context.applicationContext,
                    CompanyDatabase::class.java,
                    "company.db"
                )
                .addCallback(SeedDatabaseCallback(context))
                .fallbackToDestructiveMigration()
                .build()
                INSTANCE = instance
                instance
            }
        }
    }

    // 预填充初始数据的回调
    private class SeedDatabaseCallback(
        private val context: Context
    ) : RoomDatabase.Callback() {
        override fun onCreate(db: SupportSQLiteDatabase) {
            super.onCreate(db)
            INSTANCE?.let { database ->
                CoroutineScope(Dispatchers.IO).launch {
                    seedDatabase(database, context)
                }
            }
        }

        private suspend fun seedDatabase(db: CompanyDatabase, context: Context) {
            // 插入默认用户
            val users = listOf(
                User(name = "testuser", password = "123", sex = "男", tel = "12345678901", position = "员工", time = "2024-01-01"),
                User(name = "user001", password = "password01", sex = "女", tel = "13012345678", position = "经理", time = "2023-12-20"),
                User(name = "user002", password = "password02", sex = "男", tel = "13198765432", position = "工程师", time = "2024-01-15"),
                User(name = "zhangsan", password = "zs123", sex = "男", tel = "18812345678", position = "主管", time = "2024-06-01"),
                User(name = "lisi", password = "ls456", sex = "女", tel = "15098765432", position = "助理", time = "2023-07-05"),
                User(name = "1", password = "1", sex = "男", tel = "12345678901", position = "员工", time = "2024-01-01")
            )
            users.forEach { db.userDao().insert(it) }

            // 插入默认管理员
            db.adminUserDao().insert(AdminUser(adminName = "admin", adminPassword = "123"))

            // 插入新闻
            db.newsDao().insert(News(title = "通知1", imageResourceId = R.drawable.news01,
                content = "亲爱的员工们，公司将在本周五举办一场关于人工智能最新进展的研讨会，欢迎大家踊跃参加。"))
            db.newsDao().insert(News(title = "通知2", imageResourceId = R.drawable.news02,
                content = "我们的新软件产品的开发已经进入了关键阶段，各部门需要紧密合作。"))
        }
    }
}
```

**Step 6: 删除旧 CompanyDatabaseHelper.java，Commit**

```bash
git rm app/src/main/java/com/example/company/CompanyDatabaseHelper.java
git add app/src/main/java/com/example/company/data/
git commit -m "refactor: replace SQLiteOpenHelper with Room Database + DAOs"
```

---

## Task 4: 创建 Repository 层

**Files:**
- Create: `app/src/main/java/com/example/company/data/repository/UserRepository.kt`
- Create: `app/src/main/java/com/example/company/data/repository/AdminRepository.kt`
- Create: `app/src/main/java/com/example/company/data/repository/NewsRepository.kt`
- Create: `app/src/main/java/com/example/company/data/repository/AttendanceRepository.kt`

**Step 1: 创建 UserRepository.kt**

```kotlin
package com.example.company.data.repository

import com.example.company.data.dao.UserDao
import com.example.company.data.entity.User
import kotlinx.coroutines.flow.Flow
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class UserRepository @Inject constructor(private val userDao: UserDao) {

    suspend fun validateUser(name: String, password: String): User? =
        userDao.validateUser(name, password)

    suspend fun findByName(name: String): User? =
        userDao.findByName(name)

    fun getAllUsers(): Flow<List<User>> =
        userDao.getAllUsers()

    suspend fun addUser(user: User): Long =
        userDao.insert(user)

    suspend fun updateUser(user: User): Int =
        userDao.update(user)

    suspend fun deleteByName(name: String): Int =
        userDao.deleteByName(name)

    suspend fun getUserIdByName(name: String): Int? =
        userDao.getUserIdByName(name)
}
```

**Step 2: 创建 AdminRepository.kt**

```kotlin
package com.example.company.data.repository

import com.example.company.data.dao.AdminUserDao
import com.example.company.data.entity.AdminUser
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class AdminRepository @Inject constructor(private val adminUserDao: AdminUserDao) {

    suspend fun validateAdmin(name: String, password: String): AdminUser? =
        adminUserDao.validateAdmin(name, password)
}
```

**Step 3: 创建 NewsRepository.kt**

```kotlin
package com.example.company.data.repository

import com.example.company.data.dao.NewsDao
import com.example.company.data.entity.News
import kotlinx.coroutines.flow.Flow
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class NewsRepository @Inject constructor(private val newsDao: NewsDao) {

    fun getAllNews(): Flow<List<News>> =
        newsDao.getAllNews()

    suspend fun getNewsById(id: Int): News? =
        newsDao.getNewsById(id)
}
```

**Step 4: 创建 AttendanceRepository.kt**

```kotlin
package com.example.company.data.repository

import com.example.company.data.dao.AttendanceDao
import com.example.company.data.entity.AttendanceRecord
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class AttendanceRepository @Inject constructor(private val attendanceDao: AttendanceDao) {

    suspend fun signIn(record: AttendanceRecord): Long =
        attendanceDao.insert(record)

    suspend fun signOut(record: AttendanceRecord): Int =
        attendanceDao.update(record)

    suspend fun getTodayRecord(userId: Int, date: String): AttendanceRecord? =
        attendanceDao.getByUserAndDate(userId, date)

    suspend fun getByUserId(userId: Int): List<AttendanceRecord> =
        attendanceDao.getByUserId(userId)

    suspend fun deleteByUserId(userId: Int) =
        attendanceDao.deleteByUserId(userId)
}
```

**Step 5: Commit**

```bash
git add app/src/main/java/com/example/company/data/repository/
git commit -m "feat: add Repository layer for User, Admin, News, Attendance"
```

---

## Task 5: 配置 Hilt 依赖注入

**Files:**
- Create: `app/src/main/java/com/example/company/CompanyApplication.kt`
- Create: `app/src/main/java/com/example/company/di/DatabaseModule.kt`
- Modify: `app/src/main/AndroidManifest.xml` (添加 android:name)

**Step 1: 创建 CompanyApplication.kt**

```kotlin
package com.example.company

import android.app.Application
import dagger.hilt.android.HiltAndroidApp

@HiltAndroidApp
class CompanyApplication : Application()
```

**Step 2: 创建 DatabaseModule.kt**

```kotlin
package com.example.company.di

import android.content.Context
import com.example.company.data.CompanyDatabase
import com.example.company.data.dao.*
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object DatabaseModule {

    @Provides
    @Singleton
    fun provideDatabase(@ApplicationContext context: Context): CompanyDatabase =
        CompanyDatabase.getDatabase(context)

    @Provides
    fun provideUserDao(db: CompanyDatabase): UserDao = db.userDao()

    @Provides
    fun provideAdminUserDao(db: CompanyDatabase): AdminUserDao = db.adminUserDao()

    @Provides
    fun provideNewsDao(db: CompanyDatabase): NewsDao = db.newsDao()

    @Provides
    fun provideAttendanceDao(db: CompanyDatabase): AttendanceDao = db.attendanceDao()
}
```

**Step 3: 更新 AndroidManifest.xml**

在 `<application>` 标签中添加 `android:name=".CompanyApplication"`，移除 `package` 属性（已在 build.gradle namespace 中定义）。

```xml
<application
    android:name=".CompanyApplication"
    android:requestLegacyExternalStorage="true"
    ...>
```

**Step 4: Commit**

```bash
git add app/src/main/java/com/example/company/CompanyApplication.kt
git add app/src/main/java/com/example/company/di/
git add app/src/main/AndroidManifest.xml
git commit -m "feat: configure Hilt DI with DatabaseModule and Application class"
```

---

## Task 6: 创建 ViewModel 层

**Files:**
- Create: `app/src/main/java/com/example/company/ui/login/LoginViewModel.kt`
- Create: `app/src/main/java/com/example/company/ui/main/MainViewModel.kt`
- Create: `app/src/main/java/com/example/company/ui/attendance/AttendanceViewModel.kt`
- Create: `app/src/main/java/com/example/company/ui/admin/AdminLoginViewModel.kt`
- Create: `app/src/main/java/com/example/company/ui/admin/ManageViewModel.kt`
- Create: `app/src/main/java/com/example/company/ui/admin/SearchViewModel.kt`
- Create: `app/src/main/java/com/example/company/ui/news/NewsViewModel.kt`
- Create: `app/src/main/java/com/example/company/ui/register/RegisterViewModel.kt`

**Step 1: 创建 LoginViewModel.kt**

```kotlin
package com.example.company.ui.login

import androidx.lifecycle.*
import com.example.company.data.entity.User
import com.example.company.data.repository.UserRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class LoginState {
    object Idle : LoginState()
    object Loading : LoginState()
    data class Success(val user: User) : LoginState()
    data class Error(val message: String) : LoginState()
}

@HiltViewModel
class LoginViewModel @Inject constructor(
    private val userRepository: UserRepository
) : ViewModel() {

    private val _loginState = MutableLiveData<LoginState>(LoginState.Idle)
    val loginState: LiveData<LoginState> = _loginState

    fun login(name: String, password: String) {
        if (name.isBlank()) {
            _loginState.value = LoginState.Error("请输入用户名")
            return
        }
        if (password.isBlank()) {
            _loginState.value = LoginState.Error("请输入密码")
            return
        }
        _loginState.value = LoginState.Loading
        viewModelScope.launch {
            val user = userRepository.validateUser(name, password)
            if (user != null) {
                _loginState.value = LoginState.Success(user)
            } else {
                _loginState.value = LoginState.Error("用户名或密码不正确，请重新输入")
            }
        }
    }
}
```

**Step 2: 创建 RegisterViewModel.kt**

```kotlin
package com.example.company.ui.register

import androidx.lifecycle.*
import com.example.company.data.entity.User
import com.example.company.data.repository.UserRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class RegisterState {
    object Idle : RegisterState()
    data class Success(val userId: Long) : RegisterState()
    data class Error(val message: String) : RegisterState()
}

@HiltViewModel
class RegisterViewModel @Inject constructor(
    private val userRepository: UserRepository
) : ViewModel() {

    private val _registerState = MutableLiveData<RegisterState>(RegisterState.Idle)
    val registerState: LiveData<RegisterState> = _registerState

    fun register(username: String, password1: String, password2: String) {
        if (username.isBlank() || password1.isBlank() || password2.isBlank()) {
            _registerState.value = RegisterState.Error("未完善信息，注册失败")
            return
        }
        if (password1 != password2) {
            _registerState.value = RegisterState.Error("密码不一致，注册失败")
            return
        }
        viewModelScope.launch {
            val user = User(name = username, password = password1)
            val id = userRepository.addUser(user)
            if (id != -1L) {
                _registerState.value = RegisterState.Success(id)
            } else {
                _registerState.value = RegisterState.Error("注册失败，用户可能已存在")
            }
        }
    }
}
```

**Step 3: 创建 MainViewModel.kt**

```kotlin
package com.example.company.ui.main

import androidx.lifecycle.*
import com.example.company.data.entity.News
import com.example.company.data.repository.AttendanceRepository
import com.example.company.data.repository.NewsRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class MainViewModel @Inject constructor(
    private val newsRepository: NewsRepository,
    private val attendanceRepository: AttendanceRepository
) : ViewModel() {

    val newsList: LiveData<List<News>> = newsRepository.getAllNews()
        .asLiveData()

    private val _isSignedIn = MutableLiveData(false)
    val isSignedIn: LiveData<Boolean> = _isSignedIn

    fun checkSignInStatus(userId: Int, date: String) {
        viewModelScope.launch {
            val record = attendanceRepository.getTodayRecord(userId, date)
            _isSignedIn.value = record != null
        }
    }
}
```

**Step 4: 创建 AttendanceViewModel.kt**

```kotlin
package com.example.company.ui.attendance

import androidx.lifecycle.*
import com.example.company.data.entity.AttendanceRecord
import com.example.company.data.repository.AttendanceRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import java.text.SimpleDateFormat
import java.util.*
import javax.inject.Inject

sealed class AttendanceState {
    object Idle : AttendanceState()
    data class SignedIn(val status: String) : AttendanceState()
    data class SignedOut(val status: String) : AttendanceState()
    data class AlreadySignedIn(val record: AttendanceRecord) : AttendanceState()
    data class Error(val message: String) : AttendanceState()
}

@HiltViewModel
class AttendanceViewModel @Inject constructor(
    private val attendanceRepository: AttendanceRepository
) : ViewModel() {

    private val _state = MutableLiveData<AttendanceState>(AttendanceState.Idle)
    val state: LiveData<AttendanceState> = _state

    private val _todayRecord = MutableLiveData<AttendanceRecord?>()
    val todayRecord: LiveData<AttendanceRecord?> = _todayRecord

    fun loadTodayRecord(userId: Int, date: String) {
        viewModelScope.launch {
            val record = attendanceRepository.getTodayRecord(userId, date)
            _todayRecord.value = record
            if (record != null) {
                _state.value = AttendanceState.AlreadySignedIn(record)
            }
        }
    }

    fun signIn(userId: Int, date: String) {
        viewModelScope.launch {
            val existing = attendanceRepository.getTodayRecord(userId, date)
            if (existing != null) {
                _state.value = AttendanceState.Error("今日已签到")
                return@launch
            }
            val timeNow = SimpleDateFormat("HH:mm", Locale.getDefault()).format(Date())
            val status = getSignInStatus(timeNow)
            val record = AttendanceRecord(
                userId = userId, date = date,
                timeIn = timeNow, timeOut = "",
                statusIn = status, statusOut = ""
            )
            attendanceRepository.signIn(record)
            _todayRecord.value = record
            _state.value = AttendanceState.SignedIn(status)
        }
    }

    fun signOut(userId: Int, date: String) {
        viewModelScope.launch {
            val existing = attendanceRepository.getTodayRecord(userId, date)
            if (existing == null) {
                _state.value = AttendanceState.Error("请先签到")
                return@launch
            }
            val timeNow = SimpleDateFormat("HH:mm", Locale.getDefault()).format(Date())
            val status = getSignOutStatus(timeNow)
            val updated = existing.copy(timeOut = timeNow, statusOut = status)
            attendanceRepository.signOut(updated)
            _todayRecord.value = updated
            _state.value = AttendanceState.SignedOut(status)
        }
    }

    private fun getSignInStatus(time: String): String {
        return try {
            val sdf = SimpleDateFormat("HH:mm", Locale.getDefault())
            val signIn = sdf.parse(time)
            val workStart = sdf.parse("09:00")
            if (signIn != null && workStart != null && signIn.before(workStart)) "早签到" else "正常签到"
        } catch (e: Exception) { "异常签到" }
    }

    private fun getSignOutStatus(time: String): String {
        return try {
            val sdf = SimpleDateFormat("HH:mm", Locale.getDefault())
            val signOut = sdf.parse(time)
            val workEnd = sdf.parse("18:00")
            if (signOut != null && workEnd != null && signOut.before(workEnd)) "异常签退" else "正常签退"
        } catch (e: Exception) { "异常签退" }
    }
}
```

**Step 5: 创建 AdminLoginViewModel.kt**

```kotlin
package com.example.company.ui.admin

import androidx.lifecycle.*
import com.example.company.data.repository.AdminRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class AdminLoginState {
    object Idle : AdminLoginState()
    object Success : AdminLoginState()
    data class Error(val message: String) : AdminLoginState()
}

@HiltViewModel
class AdminLoginViewModel @Inject constructor(
    private val adminRepository: AdminRepository
) : ViewModel() {

    private val _state = MutableLiveData<AdminLoginState>(AdminLoginState.Idle)
    val state: LiveData<AdminLoginState> = _state

    fun login(name: String, password: String) {
        if (name.isBlank() || password.isBlank()) {
            _state.value = AdminLoginState.Error("请输入用户名和密码")
            return
        }
        viewModelScope.launch {
            val admin = adminRepository.validateAdmin(name, password)
            _state.value = if (admin != null) AdminLoginState.Success
                else AdminLoginState.Error("用户名或密码不正确")
        }
    }
}
```

**Step 6: 创建 ManageViewModel.kt**

```kotlin
package com.example.company.ui.admin

import androidx.lifecycle.*
import com.example.company.data.entity.User
import com.example.company.data.repository.AttendanceRepository
import com.example.company.data.repository.UserRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class ManageResult {
    object Idle : ManageResult()
    data class Success(val message: String) : ManageResult()
    data class Error(val message: String) : ManageResult()
}

@HiltViewModel
class ManageViewModel @Inject constructor(
    private val userRepository: UserRepository,
    private val attendanceRepository: AttendanceRepository
) : ViewModel() {

    private val _result = MutableLiveData<ManageResult>(ManageResult.Idle)
    val result: LiveData<ManageResult> = _result

    fun addUser(name: String, password: String, sex: String, tel: String, position: String, time: String) {
        if (name.isBlank() || password.isBlank()) {
            _result.value = ManageResult.Error("用户名和密码不能为空")
            return
        }
        viewModelScope.launch {
            val existing = userRepository.findByName(name)
            if (existing != null) {
                _result.value = ManageResult.Error("添加失败，用户已存在")
                return@launch
            }
            val id = userRepository.addUser(User(name = name, password = password, sex = sex, tel = tel, position = position, time = time))
            _result.value = if (id != -1L) ManageResult.Success("添加成功") else ManageResult.Error("添加失败")
        }
    }

    fun deleteUser(name: String) {
        if (name.isBlank()) {
            _result.value = ManageResult.Error("请输入要删除的用户名")
            return
        }
        viewModelScope.launch {
            val userId = userRepository.getUserIdByName(name)
            if (userId != null) {
                attendanceRepository.deleteByUserId(userId)
            }
            val count = userRepository.deleteByName(name)
            _result.value = if (count > 0) ManageResult.Success("删除成功") else ManageResult.Error("删除失败，用户不存在")
        }
    }

    fun updateUser(name: String, password: String, sex: String, tel: String, position: String, time: String) {
        if (name.isBlank()) {
            _result.value = ManageResult.Error("请输入要更新的用户名")
            return
        }
        viewModelScope.launch {
            val existing = userRepository.findByName(name) ?: run {
                _result.value = ManageResult.Error("更新失败，用户不存在")
                return@launch
            }
            val updated = existing.copy(password = password, sex = sex, tel = tel, position = position, time = time)
            val count = userRepository.updateUser(updated)
            _result.value = if (count > 0) ManageResult.Success("更新成功") else ManageResult.Error("更新失败")
        }
    }

    fun resetResult() { _result.value = ManageResult.Idle }
}
```

**Step 7: 创建 SearchViewModel.kt**

```kotlin
package com.example.company.ui.admin

import androidx.lifecycle.*
import com.example.company.data.entity.User
import com.example.company.data.repository.UserRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class SearchViewModel @Inject constructor(
    private val userRepository: UserRepository
) : ViewModel() {

    private val _searchResult = MutableLiveData<User?>()
    val searchResult: LiveData<User?> = _searchResult

    private val _error = MutableLiveData<String?>()
    val error: LiveData<String?> = _error

    fun search(name: String) {
        if (name.isBlank()) {
            _error.value = "请输入要搜索的用户名"
            return
        }
        viewModelScope.launch {
            val user = userRepository.findByName(name)
            if (user != null) {
                _searchResult.value = user
                _error.value = null
            } else {
                _searchResult.value = null
                _error.value = "未找到该用户"
            }
        }
    }
}
```

**Step 8: 创建 NewsViewModel.kt**

```kotlin
package com.example.company.ui.news

import androidx.lifecycle.*
import com.example.company.data.entity.News
import com.example.company.data.repository.NewsRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class NewsViewModel @Inject constructor(
    private val newsRepository: NewsRepository
) : ViewModel() {

    private val _news = MutableLiveData<News?>()
    val news: LiveData<News?> = _news

    fun loadNews(id: Int) {
        viewModelScope.launch {
            _news.value = newsRepository.getNewsById(id)
        }
    }
}
```

**Step 9: Commit**

```bash
git add app/src/main/java/com/example/company/ui/
git commit -m "feat: add ViewModel layer with LiveData for all screens"
```

---

## Task 7: 重写 Activity 层为 Kotlin（使用 ViewModel）

**Files:**
- Delete all Java Activity files in `app/src/main/java/com/example/company/`
- Create: `app/src/main/java/com/example/company/ui/login/LoginActivity.kt`
- Create: `app/src/main/java/com/example/company/ui/register/RegisterActivity.kt`
- Create: `app/src/main/java/com/example/company/ui/main/FirstActivity.kt`
- Create: `app/src/main/java/com/example/company/ui/attendance/QiandaoActivity.kt`
- Create: `app/src/main/java/com/example/company/ui/admin/GloginActivity.kt`
- Create: `app/src/main/java/com/example/company/ui/admin/GuanliActivity.kt`
- Create: `app/src/main/java/com/example/company/ui/admin/SearchActivity.kt`
- Create: `app/src/main/java/com/example/company/ui/news/NewsActivity.kt`
- Create: `app/src/main/java/com/example/company/ui/intro/IntroductionActivity.kt`
- Modify: `app/src/main/AndroidManifest.xml` (更新 Activity 路径)

**Step 1: 创建 LoginActivity.kt**

```kotlin
package com.example.company.ui.login

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.ui.admin.GloginActivity
import com.example.company.ui.main.FirstActivity
import com.example.company.ui.register.RegisterActivity
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class LoginActivity : AppCompatActivity() {

    private val viewModel: LoginViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)

        val etUsername = findViewById<EditText>(R.id.et_loginactivity_username)
        val etPassword = findViewById<EditText>(R.id.et_loginactivity_password)

        findViewById<Button>(R.id.bt_loginactivity_login).setOnClickListener {
            viewModel.login(
                etUsername.text.toString().trim(),
                etPassword.text.toString().trim()
            )
        }

        findViewById<Button>(R.id.bt_loginactivity_register).setOnClickListener {
            startActivity(Intent(this, RegisterActivity::class.java))
        }

        findViewById<Button>(R.id.bt_loginactivityGl).setOnClickListener {
            startActivity(Intent(this, GloginActivity::class.java))
        }

        viewModel.loginState.observe(this) { state ->
            when (state) {
                is LoginState.Success -> {
                    Toast.makeText(this, "登录成功", Toast.LENGTH_SHORT).show()
                    startActivity(Intent(this, FirstActivity::class.java).apply {
                        putExtra("user_id", state.user.id)
                        putExtra("username", state.user.name)
                    })
                    finish()
                }
                is LoginState.Error -> Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                else -> {}
            }
        }
    }
}
```

**Step 2: 创建 RegisterActivity.kt**

```kotlin
package com.example.company.ui.register

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.ui.login.LoginActivity
import com.example.company.ui.main.FirstActivity
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class RegisterActivity : AppCompatActivity() {

    private val viewModel: RegisterViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_register)

        val etUsername = findViewById<EditText>(R.id.et_registeractivity_username)
        val etPassword1 = findViewById<EditText>(R.id.et_registeractivity_password1)
        val etPassword2 = findViewById<EditText>(R.id.et_registeractivity_password2)

        findViewById<Button>(R.id.bt_registeractivity_register).setOnClickListener {
            viewModel.register(
                etUsername.text.toString().trim(),
                etPassword1.text.toString().trim(),
                etPassword2.text.toString().trim()
            )
        }

        findViewById<Button>(R.id.bt_registeractivity_back).setOnClickListener {
            startActivity(Intent(this, LoginActivity::class.java))
            finish()
        }

        viewModel.registerState.observe(this) { state ->
            when (state) {
                is RegisterState.Success -> {
                    Toast.makeText(this, "验证通过，注册成功", Toast.LENGTH_SHORT).show()
                    startActivity(Intent(this, LoginActivity::class.java))
                    finish()
                }
                is RegisterState.Error -> Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                else -> {}
            }
        }
    }
}
```

**Step 3: 创建 FirstActivity.kt**

```kotlin
package com.example.company.ui.main

import android.content.Intent
import android.os.Bundle
import android.text.format.DateFormat
import android.widget.*
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.ui.attendance.QiandaoActivity
import com.example.company.ui.intro.IntroductionActivity
import com.example.company.ui.login.LoginActivity
import com.example.company.ui.news.NewsActivity
import dagger.hilt.android.AndroidEntryPoint
import java.util.Calendar

@AndroidEntryPoint
class FirstActivity : AppCompatActivity() {

    private val viewModel: MainViewModel by viewModels()

    private var userId: Int = -1
    private var username: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_first)

        userId = intent.getIntExtra("user_id", -1)
        username = intent.getStringExtra("username")

        if (userId != -1 && !username.isNullOrEmpty()) {
            Toast.makeText(this, "欢迎回来, $username", Toast.LENGTH_SHORT).show()
        }

        val btnQiandao = findViewById<Button>(R.id.bt_qiandao)
        val tvTime = findViewById<TextView>(R.id.tv_qiandaotime)
        val currentDate = getCurrentDate()

        // 签到按钮
        btnQiandao.setOnClickListener {
            if (userId != -1 && username != null) {
                startActivity(Intent(this, QiandaoActivity::class.java).apply {
                    putExtra("user_id", userId)
                    putExtra("username", username)
                })
            } else {
                Toast.makeText(this, "用户信息获取失败，无法签到", Toast.LENGTH_SHORT).show()
            }
        }

        // 退出登录
        findViewById<Button>(R.id.login).setOnClickListener {
            startActivity(Intent(this, LoginActivity::class.java))
            finish()
        }

        // 公司介绍
        findViewById<Button>(R.id.introduction).setOnClickListener {
            startActivity(Intent(this, IntroductionActivity::class.java))
        }

        // 新闻列表
        val listNews = findViewById<ListView>(R.id.list_news)
        viewModel.newsList.observe(this) { newsList ->
            val adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1,
                newsList.map { it.title })
            listNews.adapter = adapter
            listNews.setOnItemClickListener { _, _, position, _ ->
                val newsId = newsList[position].id
                startActivity(Intent(this, NewsActivity::class.java).apply {
                    putExtra(NewsActivity.EXTRA_NEWS_ID, newsId)
                })
            }
        }

        // 签到状态
        viewModel.checkSignInStatus(userId, currentDate)
        viewModel.isSignedIn.observe(this) { signedIn ->
            if (signedIn) {
                tvTime.text = getString(R.string.date_signed_in, currentDate)
                btnQiandao.setBackgroundResource(R.drawable.yqd)
                btnQiandao.text = "✔签到"
            } else {
                tvTime.text = getString(R.string.date_not_signed, currentDate) +
                    "\n上班时间：09:00:00  下班时间：18:00:00"
                btnQiandao.setBackgroundResource(R.drawable.qd)
                btnQiandao.text = "签到"
            }
        }
    }

    private fun getCurrentDate(): String {
        return DateFormat.format("yyyy年MM月dd日", Calendar.getInstance()).toString()
    }
}
```

**Step 4: 创建 QiandaoActivity.kt**

```kotlin
package com.example.company.ui.attendance

import android.os.Bundle
import android.text.format.DateFormat
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint
import java.util.Calendar

@AndroidEntryPoint
class QiandaoActivity : AppCompatActivity() {

    private val viewModel: AttendanceViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_qiandao)

        val userId = intent.getIntExtra("user_id", -1)
        val username = intent.getStringExtra("username")
        val currentDate = DateFormat.format("yyyy年MM月dd日", Calendar.getInstance()).toString()

        val tvTime = findViewById<TextView>(R.id.tv_qiandao_time)
        val btnSignIn = findViewById<Button>(R.id.btn_qiandao_confirm)
        val btnSignOut = findViewById<Button>(R.id.btn_qiantui_confirm)

        tvTime.text = "今天日期：$currentDate"

        if (userId != -1 && username != null) {
            Toast.makeText(this, "欢迎 $username 签到！", Toast.LENGTH_SHORT).show()
        }

        // 加载今日记录
        viewModel.loadTodayRecord(userId, currentDate)

        btnSignIn.setOnClickListener { viewModel.signIn(userId, currentDate) }
        btnSignOut.setOnClickListener { viewModel.signOut(userId, currentDate) }

        // 观察状态
        viewModel.state.observe(this) { state ->
            when (state) {
                is AttendanceState.SignedIn -> {
                    Toast.makeText(this, "签到成功！状态：${state.status}", Toast.LENGTH_SHORT).show()
                    btnSignIn.text = "今日已签到"
                    btnSignIn.isEnabled = false
                }
                is AttendanceState.SignedOut -> {
                    Toast.makeText(this, "签退成功！状态：${state.status}", Toast.LENGTH_SHORT).show()
                    btnSignOut.text = "今日已签退"
                    btnSignOut.isEnabled = false
                }
                is AttendanceState.AlreadySignedIn -> {
                    btnSignIn.text = "今日已签到"
                    btnSignIn.isEnabled = false
                    if (state.record.timeOut.isNotEmpty()) {
                        btnSignOut.text = "今日已签退"
                        btnSignOut.isEnabled = false
                    }
                }
                is AttendanceState.Error -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                }
                else -> {}
            }
        }
    }
}
```

**Step 5: 创建 GloginActivity.kt**

```kotlin
package com.example.company.ui.admin

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.ui.login.LoginActivity
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class GloginActivity : AppCompatActivity() {

    private val viewModel: AdminLoginViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_glogin)

        val etUsername = findViewById<EditText>(R.id.et_loginactivity_username)
        val etPassword = findViewById<EditText>(R.id.et_loginactivity_password)

        findViewById<Button>(R.id.bt_loginactivity_login).setOnClickListener {
            viewModel.login(etUsername.text.toString().trim(), etPassword.text.toString().trim())
        }

        findViewById<Button>(R.id.b_gloginback).setOnClickListener {
            startActivity(Intent(this, LoginActivity::class.java))
            finish()
        }

        viewModel.state.observe(this) { state ->
            when (state) {
                is AdminLoginState.Success -> {
                    Toast.makeText(this, "登录成功", Toast.LENGTH_SHORT).show()
                    startActivity(Intent(this, GuanliActivity::class.java))
                    finish()
                }
                is AdminLoginState.Error -> Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                else -> {}
            }
        }
    }
}
```

**Step 6: 创建 GuanliActivity.kt**

```kotlin
package com.example.company.ui.admin

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class GuanliActivity : AppCompatActivity() {

    private val viewModel: ManageViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_guanli)

        val edtName = findViewById<EditText>(R.id.edt_name)
        val edtPassword = findViewById<EditText>(R.id.edt_password)
        val edtSex = findViewById<EditText>(R.id.edt_sex)
        val edtTel = findViewById<EditText>(R.id.edt_tel)
        val edtPosition = findViewById<EditText>(R.id.edt_position)
        val edtTime = findViewById<EditText>(R.id.edt_time)

        fun clearFields() {
            listOf(edtName, edtPassword, edtSex, edtTel, edtPosition, edtTime).forEach { it.setText("") }
        }

        fun fields() = listOf(
            edtName.text.toString().trim(), edtPassword.text.toString().trim(),
            edtSex.text.toString().trim(), edtTel.text.toString().trim(),
            edtPosition.text.toString().trim(), edtTime.text.toString().trim()
        )

        findViewById<Button>(R.id.bt_add).setOnClickListener {
            val f = fields()
            viewModel.addUser(f[0], f[1], f[2], f[3], f[4], f[5])
        }

        findViewById<Button>(R.id.bt_delete).setOnClickListener {
            AlertDialog.Builder(this)
                .setTitle("删除用户信息").setMessage("确认删除该用户吗？")
                .setPositiveButton("确定") { _, _ -> viewModel.deleteUser(edtName.text.toString().trim()) }
                .setNegativeButton("取消", null).show()
        }

        findViewById<Button>(R.id.bt_update).setOnClickListener {
            val f = fields()
            viewModel.updateUser(f[0], f[1], f[2], f[3], f[4], f[5])
        }

        findViewById<Button>(R.id.bt_search).setOnClickListener {
            startActivity(Intent(this, SearchActivity::class.java))
        }

        findViewById<Button>(R.id.bt_guanliback).setOnClickListener {
            startActivity(Intent(this, GloginActivity::class.java))
            finish()
        }

        viewModel.result.observe(this) { result ->
            when (result) {
                is ManageResult.Success -> {
                    Toast.makeText(this, result.message, Toast.LENGTH_SHORT).show()
                    clearFields()
                    viewModel.resetResult()
                }
                is ManageResult.Error -> {
                    Toast.makeText(this, result.message, Toast.LENGTH_SHORT).show()
                    viewModel.resetResult()
                }
                else -> {}
            }
        }
    }
}
```

**Step 7: 创建 SearchActivity.kt**

```kotlin
package com.example.company.ui.admin

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class SearchActivity : AppCompatActivity() {

    private val viewModel: SearchViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_search)

        val tvId = findViewById<TextView>(R.id.s_id)
        val tvName = findViewById<TextView>(R.id.s_name)
        val tvPassword = findViewById<TextView>(R.id.s_password)
        val tvSex = findViewById<TextView>(R.id.s_sex)
        val tvTel = findViewById<TextView>(R.id.s_tel)
        val tvPosition = findViewById<TextView>(R.id.s_position)
        val tvTime = findViewById<TextView>(R.id.s_time)
        val edtSearch = findViewById<EditText>(R.id.edt_search)

        findViewById<Button>(R.id.bt_ok).setOnClickListener {
            viewModel.search(edtSearch.text.toString().trim())
        }

        findViewById<Button>(R.id.bt_searchback).setOnClickListener {
            startActivity(Intent(this, GuanliActivity::class.java))
            finish()
        }

        viewModel.searchResult.observe(this) { user ->
            if (user != null) {
                tvId.text = user.id.toString()
                tvName.text = user.name
                tvPassword.text = "********"
                tvSex.text = user.sex
                tvTel.text = user.tel
                tvPosition.text = user.position
                tvTime.text = user.time
                Toast.makeText(this, "查询成功", Toast.LENGTH_SHORT).show()
            }
        }

        viewModel.error.observe(this) { error ->
            if (error != null) {
                listOf(tvId, tvName, tvSex, tvTel, tvPosition, tvTime).forEach { it.text = "" }
                tvPassword.text = "********"
                Toast.makeText(this, error, Toast.LENGTH_SHORT).show()
            }
        }
    }
}
```

**Step 8: 创建 NewsActivity.kt**

```kotlin
package com.example.company.ui.news

import android.os.Bundle
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class NewsActivity : AppCompatActivity() {

    companion object {
        const val EXTRA_NEWS_ID = "newsId"
    }

    private val viewModel: NewsViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_news)

        val newsId = intent.getIntExtra(EXTRA_NEWS_ID, -1)
        if (newsId == -1) {
            Toast.makeText(this, "新闻不存在", Toast.LENGTH_SHORT).show()
            finish()
            return
        }

        viewModel.loadNews(newsId)

        viewModel.news.observe(this) { news ->
            if (news != null) {
                findViewById<TextView>(R.id.title).text = news.title
                findViewById<ImageView>(R.id.image_news).apply {
                    setImageResource(news.imageResourceId)
                    contentDescription = news.title
                }
                findViewById<TextView>(R.id.content).text = news.content
            }
        }
    }
}
```

**Step 9: 创建 IntroductionActivity.kt**

```kotlin
package com.example.company.ui.intro

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R

class IntroductionActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_introduction)
    }
}
```

**Step 10: 更新 AndroidManifest.xml**

```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android">

    <application
        android:name=".CompanyApplication"
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:roundIcon="@mipmap/ic_launcher_round"
        android:supportsRtl="true"
        android:theme="@style/AppTheme">

        <activity android:name=".ui.admin.SearchActivity" />
        <activity android:name=".ui.admin.GuanliActivity" />
        <activity android:name=".ui.admin.GloginActivity" />
        <activity android:name=".ui.register.RegisterActivity" />
        <activity android:name=".ui.intro.IntroductionActivity" />
        <activity android:name=".ui.news.NewsActivity" />
        <activity android:name=".ui.main.FirstActivity" />
        <activity android:name=".ui.attendance.QiandaoActivity" />

        <activity android:name=".ui.login.LoginActivity"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
```

**Step 11: 删除所有旧 Java Activity 文件，Commit**

```bash
git rm app/src/main/java/com/example/company/LoginActivity.java
git rm app/src/main/java/com/example/company/RegisterActivity.java
git rm app/src/main/java/com/example/company/FirstActivity.java
git rm app/src/main/java/com/example/company/QiandaoActivity.java
git rm app/src/main/java/com/example/company/GloginActivity.java
git rm app/src/main/java/com/example/company/GuanliActivity.java
git rm app/src/main/java/com/example/company/SearchActivity.java
git rm app/src/main/java/com/example/company/NewsActivity.java
git rm app/src/main/java/com/example/company/IntroductionActivity.java
git add app/src/main/java/com/example/company/ui/
git add app/src/main/AndroidManifest.xml
git commit -m "refactor: rewrite all Activities in Kotlin with MVVM + Hilt"
```

---

## Task 8: 修复业务 Bug

**问题清单:**
1. 旧 LoginActivity 每次登录都插入硬编码考勤记录 → 已在新代码中移除
2. 签到/签退只存 SharedPreferences 不存数据库 → 新 AttendanceViewModel 直接写入 Room
3. RegisterActivity 注册成功后跳转 FirstActivity 但没传 user_id → 新代码跳转到 LoginActivity 让用户登录

**这些 Bug 已在 Task 7 的新代码中全部修复，无需额外步骤。**

**Commit:**
```bash
git commit --allow-empty -m "fix: resolve hardcoded attendance, sign-in not persisted, register redirect bugs"
```

---

## 最终项目结构

```
app/src/main/java/com/example/company/
├── CompanyApplication.kt              # Hilt Application
├── di/
│   └── DatabaseModule.kt              # Hilt DI Module
├── data/
│   ├── CompanyDatabase.kt             # Room Database
│   ├── entity/
│   │   ├── User.kt
│   │   ├── AdminUser.kt
│   │   ├── News.kt
│   │   └── AttendanceRecord.kt
│   ├── dao/
│   │   ├── UserDao.kt
│   │   ├── AdminUserDao.kt
│   │   ├── NewsDao.kt
│   │   └── AttendanceDao.kt
│   └── repository/
│       ├── UserRepository.kt
│       ├── AdminRepository.kt
│       ├── NewsRepository.kt
│       └── AttendanceRepository.kt
└── ui/
    ├── login/
    │   ├── LoginActivity.kt
    │   └── LoginViewModel.kt
    ├── register/
    │   ├── RegisterActivity.kt
    │   └── RegisterViewModel.kt
    ├── main/
    │   ├── FirstActivity.kt
    │   └── MainViewModel.kt
    ├── attendance/
    │   ├── QiandaoActivity.kt
    │   └── AttendanceViewModel.kt
    ├── admin/
    │   ├── GloginActivity.kt
    │   ├── AdminLoginViewModel.kt
    │   ├── GuanliActivity.kt
    │   ├── ManageViewModel.kt
    │   ├── SearchActivity.kt
    │   └── SearchViewModel.kt
    ├── news/
    │   ├── NewsActivity.kt
    │   └── NewsViewModel.kt
    └── intro/
        └── IntroductionActivity.kt
```
