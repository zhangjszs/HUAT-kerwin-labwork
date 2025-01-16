package com.example.company;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.format.DateFormat;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

public class QiandaoActivity extends AppCompatActivity {

    private static final String MY_RMB_COST = "MY_RMBCost";
    private static final String TODAY_TIME = "TodayTime";
    private static final String SIGN_IN_STATUS = "SignInStatus"; // 签到状态
    private static final String SIGN_OUT_STATUS = "SignOutStatus"; // 签退状态
    private TextView tvQiandaoTime;
    private Button btnQiandaoConfirm;
    private Button btnQiantuiConfirm; // 签退按钮
    private String currentDate;
    private SharedPreferences myRmbData;

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_qiandao);

        tvQiandaoTime = findViewById(R.id.tv_qiandao_time);
        btnQiandaoConfirm = findViewById(R.id.btn_qiandao_confirm);
        btnQiantuiConfirm = findViewById(R.id.btn_qiantui_confirm); // 初始化签退按钮

        Intent intent = getIntent();
        int userId = intent.getIntExtra("user_id", -1);
        String username = intent.getStringExtra("username");

        if (userId != -1 && username != null) {
            Toast.makeText(this, "欢迎 " + username + " 签到！", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "无法获取用户信息！", Toast.LENGTH_SHORT).show();
        }

        currentDate = getCurrentDate();
        tvQiandaoTime.setText("今天日期：" + currentDate);

        myRmbData = getSharedPreferences(MY_RMB_COST, MODE_PRIVATE);
        String savedDate = myRmbData.getString(TODAY_TIME, "");
        String signInStatus = myRmbData.getString(SIGN_IN_STATUS,"");
        String signOutStatus = myRmbData.getString(SIGN_OUT_STATUS,"");

        if (currentDate.equals(savedDate)) {
            btnQiandaoConfirm.setText("今日已签到");
            btnQiandaoConfirm.setEnabled(false);
            if (signOutStatus.equals("已签退")) {
                btnQiantuiConfirm.setText("今日已签退");
                btnQiantuiConfirm.setEnabled(false);
            }
        }

        btnQiandaoConfirm.setOnClickListener(v -> {
            if (!currentDate.equals(savedDate)) {
                String signInTime = getCurrentTime();
                String status = getSignInStatus(signInTime);
                myRmbData.edit().putString(TODAY_TIME, currentDate).putString(SIGN_IN_STATUS,status).apply();
                Toast.makeText(this, "签到成功！状态：" + status, Toast.LENGTH_SHORT).show();
                btnQiandaoConfirm.setText("今日已签到");
                btnQiandaoConfirm.setEnabled(false);
            } else {
                Toast.makeText(this, "今日已签到！", Toast.LENGTH_SHORT).show();
            }
        });

        btnQiantuiConfirm.setOnClickListener(v -> {
            String signOutTime = getCurrentTime();
            String status = getSignOutStatus(signOutTime);
            myRmbData.edit().putString(SIGN_OUT_STATUS,status).apply();
            Toast.makeText(this, "签退成功！状态：" + status, Toast.LENGTH_SHORT).show();
            btnQiantuiConfirm.setText("今日已签退");
            btnQiantuiConfirm.setEnabled(false);
        });
    }

    private String getCurrentDate() {
        Calendar calendar = Calendar.getInstance();
        String format = "yyyy年MM月dd日";
        return DateFormat.format(format, calendar).toString();
    }

    private String getCurrentTime() {
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm", Locale.getDefault());
        return sdf.format(new Date());
    }

    private String getSignInStatus(String signInTime) {
        try {
            SimpleDateFormat sdf = new SimpleDateFormat("HH:mm", Locale.getDefault());
            Date signInDate = sdf.parse(signInTime);
            Date workStartTime = sdf.parse("09:00");

            if (signInDate != null && workStartTime != null) {
                if (signInDate.before(workStartTime)) {
                    return "早签到";
                } else if (signInDate.equals(workStartTime) || signInDate.after(workStartTime)) {
                    return "正常签到";
                }
            }
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return "异常签到"; // 解析失败也认为是异常
    }
    private String getSignOutStatus(String signOutTime){
        try {
            SimpleDateFormat sdf = new SimpleDateFormat("HH:mm", Locale.getDefault());
            Date signOutDate = sdf.parse(signOutTime);
            Date workEndTime = sdf.parse("18:00");

            if (signOutDate != null && workEndTime != null) {
                if (signOutDate.before(workEndTime)) {
                    return "异常签退";
                } else if (signOutDate.equals(workEndTime) || signOutDate.after(workEndTime)) {
                    return "正常签退";
                }
            }
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return "异常签退"; // 解析失败也认为是异常
    }
}