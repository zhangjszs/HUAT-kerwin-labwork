package com.example.company.data

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase
import androidx.sqlite.db.SupportSQLiteDatabase
import com.example.company.R
import com.example.company.data.dao.AdminUserDao
import com.example.company.data.dao.AppealDao
import com.example.company.data.dao.AttendanceDao
import com.example.company.data.dao.LeaveRequestDao
import com.example.company.data.dao.NewsDao
import com.example.company.data.dao.UserDao
import com.example.company.data.entity.AdminUser
import com.example.company.data.entity.Appeal
import com.example.company.data.entity.AttendanceRecord
import com.example.company.data.entity.LeaveRequest
import com.example.company.data.entity.News
import com.example.company.data.entity.User
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

@Database(
    entities = [User::class, AdminUser::class, News::class, AttendanceRecord::class, Appeal::class, LeaveRequest::class],
    version = 2,
    exportSchema = false
)
abstract class CompanyDatabase : RoomDatabase() {
    abstract fun userDao(): UserDao
    abstract fun adminUserDao(): AdminUserDao
    abstract fun newsDao(): NewsDao
    abstract fun attendanceDao(): AttendanceDao
    abstract fun appealDao(): AppealDao
    abstract fun leaveRequestDao(): LeaveRequestDao

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

    private class SeedDatabaseCallback(
        private val context: Context
    ) : RoomDatabase.Callback() {
        override fun onCreate(db: SupportSQLiteDatabase) {
            super.onCreate(db)
            INSTANCE?.let { database ->
                CoroutineScope(Dispatchers.IO).launch {
                    seedDatabase(database)
                }
            }
        }

        private suspend fun seedDatabase(db: CompanyDatabase) {
            val users = listOf(
                User(name = "testuser", password = "123", sex = "男", tel = "12345678901", position = "员工", time = "2024-01-01"),
                User(name = "user001", password = "password01", sex = "女", tel = "13012345678", position = "经理", time = "2023-12-20"),
                User(name = "user002", password = "password02", sex = "男", tel = "13198765432", position = "工程师", time = "2024-01-15"),
                User(name = "user003", password = "password03", sex = "女", tel = "13255551212", position = "行政", time = "2023-11-01"),
                User(name = "user004", password = "password04", sex = "男", tel = "13300001111", position = "程序员", time = "2024-02-28"),
                User(name = "user005", password = "password05", sex = "女", tel = "13466667777", position = "设计师", time = "2024-03-10"),
                User(name = "user006", password = "password06", sex = "男", tel = "13588889999", position = "测试员", time = "2023-10-08"),
                User(name = "user007", password = "password07", sex = "女", tel = "13611112222", position = "财务", time = "2024-04-15"),
                User(name = "user008", password = "password08", sex = "男", tel = "13733334444", position = "销售", time = "2023-09-22"),
                User(name = "user009", password = "password09", sex = "女", tel = "13855556666", position = "客服", time = "2024-05-01"),
                User(name = "user010", password = "password10", sex = "男", tel = "13977778888", position = "运维", time = "2023-08-18"),
                User(name = "zhangsan", password = "zs123", sex = "男", tel = "18812345678", position = "主管", time = "2024-06-01"),
                User(name = "lisi", password = "ls456", sex = "女", tel = "15098765432", position = "助理", time = "2023-07-05"),
                User(name = "wangwu", password = "ww789", sex = "男", tel = "13911112222", position = "经理", time = "2024-07-12"),
                User(name = "zhaoliu", password = "zl000", sex = "女", tel = "18633334444", position = "实习生", time = "2023-06-29"),
                User(name = "sunqi", password = "sq111", sex = "男", tel = "15855556666", position = "顾问", time = "2024-08-08"),
                User(name = "1", password = "1", sex = "男", tel = "12345678901", position = "员工", time = "2024-01-01")
            )
            users.forEach { db.userDao().insert(it) }

            db.adminUserDao().insert(AdminUser(adminName = "admin", adminPassword = "123"))

            db.newsDao().insert(
                News(
                    title = "通知1",
                    imageResourceId = R.drawable.news01,
                    content = "亲爱的员工们，公司将在本周五举办一场关于人工智能最新进展的研讨会，欢迎大家踊跃参加，共同探讨如何将最新的 AI 技术应用到我们的产品开发中，提升我们的产品竞争力。研讨会将在公司的会议室举行，时间是下午 2 点至 4 点。"
                )
            )
            db.newsDao().insert(
                News(
                    title = "通知2",
                    imageResourceId = R.drawable.news02,
                    content = "我们的新软件产品的开发已经进入了关键阶段，为了确保项目的顺利进行，各部门需要紧密合作。开发团队正在进行最后的代码优化，测试团队请做好准备，将在下周开始进行全面的功能和性能测试。同时，我们将引入新的自动化测试工具，以提高测试效率和质量。期待大家齐心协力，共同推动项目的成功！"
                )
            )
        }
    }
}
