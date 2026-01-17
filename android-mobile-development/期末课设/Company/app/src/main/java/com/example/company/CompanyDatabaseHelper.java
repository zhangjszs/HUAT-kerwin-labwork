package com.example.company;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class CompanyDatabaseHelper extends SQLiteOpenHelper {
    private static final String DB_NAME = "company.db";
    private static final int DB_VER = 2;

    // 表名常量
    private static final String TABLE_NEWS = "NEWS";
    private static final String TABLE_USER = "USER";
    private static final String TABLE_ATTENDANCE = "ATTENDANCE";
    private static final String TABLE_ADMIN = "ADMINUSER";

    // NEWS 表字段
    private static final String NEWS_ID = "_id";
    private static final String NEWS_TITLE = "TITLE";
    private static final String NEWS_IMAGE_RESOURCE_ID = "IMAGE_RESOURCE_ID";
    private static final String NEWS_CONTENT = "CONTENT";

    // USER 表字段
    private static final String USER_ID = "_id";
    private static final String USER_NAME = "name";
    private static final String USER_PASSWORD = "password";
    private static final String USER_SEX = "sex";
    private static final String USER_TEL = "tel";
    private static final String USER_POSITION = "position";
    private static final String USER_TIME = "TIME";

    // ATTENDANCE 表字段
    private static final String ATTENDANCE_ID = "_id";
    private static final String ATTENDANCE_USER_ID = "user_id";
    private static final String ATTENDANCE_DATE = "date";
    private static final String ATTENDANCE_TIME_IN = "time_in";
    private static final String ATTENDANCE_TIME_OUT = "time_out";
    private static final String ATTENDANCE_STATUS_IN = "status_in";
    private static final String ATTENDANCE_STATUS_OUT = "status_out";

    // ADMIN 表字段
    private static final String ADMIN_ID = "_id";
    private static final String ADMIN_NAME = "admin_name";
    private static final String ADMIN_PASSWORD = "admin_password";

    public static final String DEFAULT_TIME_IN = "09:00:00"; // 默认上班时间
    public static final String DEFAULT_TIME_OUT = "18:00:00"; // 默认下班时间

    public CompanyDatabaseHelper(Context context) {
        super(context, DB_NAME, null, DB_VER);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        // 建每日讯息表
        db.execSQL("CREATE TABLE " + TABLE_NEWS + "(" +
                NEWS_ID + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                NEWS_TITLE + " TEXT," +
                NEWS_IMAGE_RESOURCE_ID + " INTEGER," +
                NEWS_CONTENT + " TEXT);");

        // 建立用户表
        db.execSQL("CREATE TABLE " + TABLE_USER + "(" +
                USER_ID + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                USER_NAME + " TEXT," +
                USER_PASSWORD + " TEXT," +
                USER_SEX + " TEXT," +
                USER_TEL + " TEXT," +
                USER_POSITION + " TEXT," +
                USER_TIME + " DATE);");

        // 创建考勤表
        db.execSQL("CREATE TABLE " + TABLE_ATTENDANCE + "(" +
                ATTENDANCE_ID + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                ATTENDANCE_USER_ID + " INTEGER," +
                ATTENDANCE_DATE + " TEXT," +
                ATTENDANCE_TIME_IN + " TEXT," +
                ATTENDANCE_TIME_OUT + " TEXT," +
                ATTENDANCE_STATUS_IN + " TEXT," +
                ATTENDANCE_STATUS_OUT + " TEXT," +
                "FOREIGN KEY(" + ATTENDANCE_USER_ID + ") REFERENCES " + TABLE_USER + "(" + USER_ID + "));");

        // 创建管理员表
        db.execSQL("CREATE TABLE " + TABLE_ADMIN + "(" +
                ADMIN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                ADMIN_NAME + " TEXT," +
                ADMIN_PASSWORD + " TEXT);");

        // 插入初始数据（示例）
        insertUser(db, "testuser", "123", "男", "12345678901", "员工", "2024-01-01");
        insertUser(db, "user001", "password01", "女", "13012345678", "经理", "2023-12-20");
        insertUser(db, "user002", "password02", "男", "13198765432", "工程师", "2024-01-15");
        insertUser(db, "user003", "password03", "女", "13255551212", "行政", "2023-11-01");
        insertUser(db, "user004", "password04", "男", "13300001111", "程序员", "2024-02-28");
        insertUser(db, "user005", "password05", "女", "13466667777", "设计师", "2024-03-10");
        insertUser(db, "user006", "password06", "男", "13588889999", "测试员", "2023-10-08");
        insertUser(db, "user007", "password07", "女", "13611112222", "财务", "2024-04-15");
        insertUser(db, "user008", "password08", "男", "13733334444", "销售", "2023-09-22");
        insertUser(db, "user009", "password09", "女", "13855556666", "客服", "2024-05-01");
        insertUser(db, "user010", "password10", "男", "13977778888", "运维", "2023-08-18");
        insertUser(db, "zhangsan", "zs123", "男", "18812345678", "主管", "2024-06-01");
        insertUser(db, "lisi", "ls456", "女", "15098765432", "助理", "2023-07-05");
        insertUser(db, "wangwu", "ww789", "男", "13911112222", "经理", "2024-07-12");
        insertUser(db, "zhaoliu", "zl000", "女", "18633334444", "实习生", "2023-06-29");
        insertUser(db, "sunqi", "sq111", "男", "15855556666", "顾问", "2024-08-08");
        insertUser(db, "1", "1", "男", "12345678901", "员工", "2024-01-01");
        insertAdmin(db, "admin", "123"); // 插入初始管理员
        insertNews(db, "通知1", R.drawable.news01, "亲爱的员工们，公司将在本周五举办一场关于人工智能最新进展的研讨会，欢迎大家踊跃参加，共同探讨如何将最新的 AI 技术应用到我们的产品开发中，提升我们的产品竞争力。研讨会将在公司的会议室举行，时间是下午 2 点至 4 点。");
        insertNews(db, "通知2", R.drawable.news02, "我们的新软件产品的开发已经进入了关键阶段，为了确保项目的顺利进行，各部门需要紧密合作。开发团队正在进行最后的代码优化，测试团队请做好准备，将在下周开始进行全面的功能和性能测试。同时，我们将引入新的自动化测试工具，以提高测试效率和质量。期待大家齐心协力，共同推动项目的成功！");
//        db.execSQL(insertAttendanceSQL);
//        addAttendance(2, "2023-04-01", null, null, "Late", "On Time");
//        addAttendance(3, "2023-04-01", "09:10", "18:45", "Late", "Overtime");
//        addAttendance(4, "2023-04-01", DEFAULT_TIME_IN, DEFAULT_TIME_OUT, "On Time", "On Time");
//        addAttendance(1, "2023-04-01", "08:45", "18:00", "Early", "On Time");
//        printCreateAttendanceTableLog();
    }

    @Override
    public void onUpgrade(SQLiteDatabase sqLiteDatabase, int i, int i1) {

    }

    private void insertNews(SQLiteDatabase db, String title, int resourceId, String content) {
        ContentValues values = new ContentValues();
        values.put(NEWS_TITLE, title);
        values.put(NEWS_IMAGE_RESOURCE_ID, resourceId);
        values.put(NEWS_CONTENT, content);
        db.insert(TABLE_NEWS, null, values);
    }

    private void insertUser(SQLiteDatabase db, String name, String password, String sex, String tel, String position, String time) {
        ContentValues values = new ContentValues();
        values.put(USER_NAME, name);
        values.put(USER_PASSWORD, password);
        values.put(USER_SEX, sex);
        values.put(USER_TEL, tel);
        values.put(USER_POSITION, position);
        values.put(USER_TIME, time);
        db.insert(TABLE_USER, null, values);
    }

    private void insertAdmin(SQLiteDatabase db, String adminName, String adminPassword) {
        ContentValues values = new ContentValues();
        values.put(ADMIN_NAME, adminName);
        values.put(ADMIN_PASSWORD, adminPassword);
        db.insert(TABLE_ADMIN, null, values);
    }

    // 获取所有用户
    public List<User> getAllUsers() {
        List<User> users = new ArrayList<>();
        try (SQLiteDatabase db = getReadableDatabase();
             Cursor cursor = db.query(TABLE_USER, null, null, null, null, null, USER_NAME + " DESC")) {
            while (cursor.moveToNext()) {
                users.add(createUserFromCursor(cursor));
            }
        }
        return users;
    }

    private User createUserFromCursor(Cursor cursor) {
        return new User(
                cursor.getInt(cursor.getColumnIndex(USER_ID)),
                cursor.getString(cursor.getColumnIndex(USER_NAME)),
                cursor.getString(cursor.getColumnIndex(USER_PASSWORD)),
                cursor.getString(cursor.getColumnIndex(USER_SEX)),
                cursor.getString(cursor.getColumnIndex(USER_TEL)),
                cursor.getString(cursor.getColumnIndex(USER_POSITION)),
                cursor.getString(cursor.getColumnIndex(USER_TIME))
        );
    }

    // 验证用户登录
    // 验证用户登录，返回用户 ID，如果未找到返回 -1
    public int validateUser(String name, String password) {
        try (SQLiteDatabase db = getReadableDatabase();
             Cursor cursor = db.query(TABLE_USER, new String[]{USER_ID}, USER_NAME + "=? AND " + USER_PASSWORD + "=?", new String[]{name, password}, null, null, null)) {
            if (cursor.moveToFirst()) {
                return cursor.getInt(cursor.getColumnIndex(USER_ID));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return -1;
    }

    // 验证管理员登录，返回管理员 ID，如果未找到返回 -1
    public int validateAdmin(String adminName, String adminPassword) {
        try (SQLiteDatabase db = getReadableDatabase();
             Cursor cursor = db.query(TABLE_ADMIN, new String[]{ADMIN_ID}, ADMIN_NAME + "=? AND " + ADMIN_PASSWORD + "=?", new String[]{adminName, adminPassword}, null, null, null)) {
            if (cursor.moveToFirst()) {
                return cursor.getInt(cursor.getColumnIndex(ADMIN_ID));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return -1;
    }

    private AdminUser createAdminFromCursor(Cursor cursor) {
        return new AdminUser(
                cursor.getInt(cursor.getColumnIndex(ADMIN_ID)),
                cursor.getString(cursor.getColumnIndex(ADMIN_NAME)),
                cursor.getString(cursor.getColumnIndex(ADMIN_PASSWORD))
        );
    }

    // 添加新用户
    public long user_add(String name, String password, String sex, String tel, String position, String time) {
        SQLiteDatabase db = getWritableDatabase();
        ContentValues values = new ContentValues();

        try {
            // 检查用户名是否已存在
            Cursor cursor = db.query(TABLE_USER, new String[]{USER_ID},
                    USER_NAME + "=?", new String[]{name}, null, null, null);

            if (cursor.getCount() > 0) {
                cursor.close();
                return -1; // 用户已存在
            }
            cursor.close();

            // 添加新用户数据
            values.put(USER_NAME, name);
            values.put(USER_PASSWORD, password);
            values.put(USER_SEX, sex);
            values.put(USER_TEL, tel);
            values.put(USER_POSITION, position);
            values.put(USER_TIME, time);

            return db.insert(TABLE_USER, null, values);
        } catch (Exception e) {
            Log.e("DatabaseHelper", "Error adding user: " + e.getMessage());
            return -1;
        } finally {
            db.close();
        }
    }

    // 删除用户
    public boolean user_delete(String name) {
        try (SQLiteDatabase db = getWritableDatabase()) {
            // 删除用户的考勤记录
            Cursor userCursor = db.query(TABLE_USER, new String[]{USER_ID},
                    USER_NAME + "=?", new String[]{name}, null, null, null);

            if (userCursor.moveToFirst()) {
                int userId = userCursor.getInt(userCursor.getColumnIndex(USER_ID));
                db.delete(TABLE_ATTENDANCE, ATTENDANCE_USER_ID + "=?",
                        new String[]{String.valueOf(userId)});
            }
            userCursor.close();

            // 删除用户
            int result = db.delete(TABLE_USER, USER_NAME + "=?", new String[]{name});
            return result > 0;
        } catch (Exception e) {
            Log.e("DatabaseHelper", "Error deleting user: " + e.getMessage());
            return false;
        }
    }

    // 更新用户信息
    public boolean user_update(String name, String password, String sex, String tel, String position, String time) {
        SQLiteDatabase db = getWritableDatabase();
        ContentValues values = new ContentValues();

        try {
            // 检查用户是否存在
            Cursor cursor = db.query(TABLE_USER, new String[]{USER_ID},
                    USER_NAME + "=?", new String[]{name}, null, null, null);

            if (cursor.getCount() == 0) {
                cursor.close();
                return false; // 用户不存在
            }
            cursor.close();

            // 更新用户数据
            values.put(USER_PASSWORD, password);
            values.put(USER_SEX, sex);
            values.put(USER_TEL, tel);
            values.put(USER_POSITION, position);
            values.put(USER_TIME, time);

            int result = db.update(TABLE_USER, values, USER_NAME + "=?", new String[]{name});
            return result > 0;
        } catch (Exception e) {
            Log.e("DatabaseHelper", "Error updating user: " + e.getMessage());
            return false;
        } finally {
            db.close();
        }
    }

    // 将用户名和密码加入到用户表中，其他数据为默认值
    public long login_add(String username, String password) {
        SQLiteDatabase db = getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(USER_NAME, username);
        values.put(USER_PASSWORD, password);
        // 以下为默认值，可根据需求修改
        values.put(USER_SEX, "未指定");
        values.put(USER_TEL, "未指定");
        values.put(USER_POSITION, "未指定");
        values.put(USER_TIME, "1970-01-01");

        long result = db.insert(TABLE_USER, null, values);
        db.close();
        if (result == -1) {
            Log.e("DatabaseHelper", "插入用户数据失败");
        } else {
            Log.d("DatabaseHelper", "插入用户数据成功，新用户的 ID: " + result);
        }
        return result;
    }


    public void addAttendance(int userId, String date, String timeIn, String timeOut, String statusIn, String statusOut) {
        SQLiteDatabase db = getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(ATTENDANCE_USER_ID, userId);
        values.put(ATTENDANCE_DATE, date);
        values.put(ATTENDANCE_TIME_IN, (timeIn == null || timeIn.isEmpty())? DEFAULT_TIME_IN : timeIn);
        values.put(ATTENDANCE_TIME_OUT, (timeOut == null || timeOut.isEmpty())? DEFAULT_TIME_OUT : timeOut);
        values.put(ATTENDANCE_STATUS_IN, statusIn);
        values.put(ATTENDANCE_STATUS_OUT, statusOut);

        long result = db.insert(TABLE_ATTENDANCE, null, values);
        db.close();
        if (result == -1) {
            Log.e("DatabaseHelper", "添加考勤记录失败");
        } else {
            Log.d("DatabaseHelper", "添加考勤记录成功，新记录的 ID: " + result);
        }
    }
    public void printCreateAttendanceTableLog() {
        // 获取考勤表的建表语句
        String createAttendanceTableSQL = "CREATE TABLE " + TABLE_ATTENDANCE + "(" +
                ATTENDANCE_ID + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                ATTENDANCE_USER_ID + " INTEGER," +
                ATTENDANCE_DATE + " TEXT," +
                ATTENDANCE_TIME_IN + " TEXT," +
                ATTENDANCE_TIME_OUT + " TEXT," +
                ATTENDANCE_STATUS_IN + " TEXT," +
                ATTENDANCE_STATUS_OUT + " TEXT," +
                "FOREIGN KEY(" + ATTENDANCE_USER_ID + ") REFERENCES " + TABLE_USER + "(" + USER_ID + "));";

        // 打印建表语句
        Log.d("DatabaseHelper", "Creating ATTENDANCE table with SQL: " + createAttendanceTableSQL);
    }

    public Map<String, Integer> getAttendanceStatistics(int userId) {
        SQLiteDatabase db = getReadableDatabase();
        Map<String, Integer> statistics = new HashMap<>();
        // 定义需要统计的考勤状态
        String[] statuses = {"正常", "迟到", "早退", "缺勤"}; // 根据你的实际情况修改
        // 遍历所有考勤状态，统计每个状态的出现次数
        for (String status : statuses) {
            // 构建查询条件
            String selection = ATTENDANCE_USER_ID + " = ? AND (" +
                    ATTENDANCE_STATUS_IN + " = ? OR " + ATTENDANCE_STATUS_OUT + " = ?)";
            String[] selectionArgs = {String.valueOf(userId), status, status};
            // 执行查询
            Cursor cursor = db.query(TABLE_ATTENDANCE, null, selection, selectionArgs, null, null, null);
            // 获取查询结果数量
            int count = cursor.getCount();
            cursor.close();
            // 将统计结果存入 Map
            statistics.put(status, count);
        }
        db.close();
        return statistics;
    }
}