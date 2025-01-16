package com.example.company;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.os.Bundle;
import android.text.format.DateFormat;
import android.util.Log;
import android.widget.Button;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.util.Calendar;

public class FirstActivity extends AppCompatActivity {
    private static final String TAG = "FirstActivity";
    private static final String MY_RMB_COST = "MY_RMBCost";
    private static final String TODAY_TIME = "TodayTime";
    private CompanyDatabaseHelper dbHelper;

    private Cursor cursor;
    private TextView tvTime, tvWorkHours; // tvWorkHours 用于显示上下班时间
    private Button btnQiandao;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first);

        // 处理从其他Activity返回时的数据和欢迎信息
        handleIntentAndWelcomeBack();

        // 初始化视图
        initializeViews();

        // 设置按钮的点击事件
        setupLoginButton();
        setupQiandaoButton();
        setupNewsList();
        setupIntroductionButton();


        // 更新签到UI
        SharedPreferences myRmbData = getSharedPreferences(MY_RMB_COST, MODE_PRIVATE);
        String savedDate = myRmbData.getString(TODAY_TIME, "");
        String currentDate = getCurrentDate();
        updateQiandaoUI(currentDate, savedDate);
    }

    // 处理Intent数据和显示欢迎信息
    private void handleIntentAndWelcomeBack() {
        Intent intent = getIntent();
        int userId = intent.getIntExtra("user_id", -1);
        String username = intent.getStringExtra("username");

        if (userId != -1 && username != null && !username.isEmpty()) {
            showToast("欢迎回来, " + username);
        } else {
            showToast("无法获取用户信息");
        }
    }

    // 初始化视图
    private void initializeViews() {
        btnQiandao = findViewById(R.id.bt_qiandao);
        tvTime = findViewById(R.id.tv_qiandaotime);
        tvWorkHours = findViewById(R.id.tv_work_hours);
    }

    // 设置登录按钮的点击事件
    private void setupLoginButton() {
        Button btnLogin = findViewById(R.id.login);
        btnLogin.setOnClickListener(v -> {
            Intent intent = new Intent(FirstActivity.this, LoginActivity.class);
            startActivity(intent);
        });
    }

    // 设置签到按钮的点击事件
    private void setupQiandaoButton() {
        btnQiandao.setOnClickListener(v -> {
            Intent originalIntent = getIntent();
            int userId = originalIntent.getIntExtra("user_id", -1);
            String name = originalIntent.getStringExtra("username");

            if (userId != -1 && name != null) {
                Intent intent = new Intent(FirstActivity.this, QiandaoActivity.class);
                intent.putExtra("user_id", userId);
                intent.putExtra("username", name);
                startActivity(intent);
            } else {
                showToast("用户信息获取失败，无法签到");
            }
        });
    }

    // 获取当前日期
    private String getCurrentDate() {
        Calendar calendar = Calendar.getInstance();
        String format = "yyyy年MM月dd日";
        return DateFormat.format(format, calendar).toString();
    }


    // 更新签到UI
    @SuppressLint("SetTextI18n")
    private void updateQiandaoUI(String currentDate, String savedDate) {
        if (currentDate.equals(savedDate)) {
            tvTime.setText(getString(R.string.date_signed_in, savedDate));
            btnQiandao.setBackgroundResource(R.drawable.yqd); // 已签到背景
            btnQiandao.setText("✔签到");
        } else {
            // 显示未签到的日期和上下班时间
            tvTime.setText(getString(R.string.date_not_signed, currentDate) + "\n上班时间：" + CompanyDatabaseHelper.DEFAULT_TIME_IN + "  下班时间：" + CompanyDatabaseHelper.DEFAULT_TIME_OUT);
            btnQiandao.setBackgroundResource(R.drawable.qd); // 未签到背景
            btnQiandao.setText("签到");
        }
    }

    // 设置新闻列表
    private void setupNewsList() {
        ListView listNews = findViewById(R.id.list_news);
        SQLiteOpenHelper databaseHelper = new CompanyDatabaseHelper(this);

        try {
            SQLiteDatabase db = databaseHelper.getReadableDatabase();
            cursor = db.query("NEWS", new String[]{"_id", "TITLE"}, null, null, null, null, null);

            SimpleCursorAdapter listAdapter = new SimpleCursorAdapter(this,
                    android.R.layout.simple_list_item_1, cursor, new String[]{"TITLE"}, new int[]{android.R.id.text1}, 0);

            listNews.setAdapter(listAdapter);

            listNews.setOnItemClickListener((parent, view, position, id) -> {
                Intent intent = new Intent(FirstActivity.this, NewsActivity.class);
                intent.putExtra(NewsActivity.EXTRA_DRINKID, (int) id);
                startActivity(intent);
            });
        } catch (SQLException e) {
            Log.e(TAG, "数据库错误: " + e.getMessage());
            showToast("数据库不可用");
        }
    }

    // 设置公司介绍按钮
    private void setupIntroductionButton() {
        Button btnIntroduction = findViewById(R.id.introduction);
        btnIntroduction.setOnClickListener(v -> {
            Intent intent = new Intent(FirstActivity.this, IntroductionActivity.class);
            startActivity(intent);
        });
    }

    // 显示Toast消息
    private void showToast(String message) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (cursor != null && !cursor.isClosed()) {
            cursor.close();
        }
    }
}