package com.example.company;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.ArrayList;

public class GloginActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "GloginActivity"; // 添加TAG方便日志输出

    private CompanyDatabaseHelper dbHelper; //
    private EditText etUsername; //
    private EditText etPassword; //
    private Button btnLogin; //
    private Button btnBack;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_glogin);

        initViews();
        dbHelper = new CompanyDatabaseHelper(this);
    }

    private void initViews() {
        etUsername = findViewById(R.id.et_loginactivity_username);
        etPassword = findViewById(R.id.et_loginactivity_password);
        btnLogin = findViewById(R.id.bt_loginactivity_login);
        btnBack = findViewById(R.id.b_gloginback);

        btnLogin.setOnClickListener(this);
        btnBack.setOnClickListener(this);
    }

    @SuppressLint("NonConstantResourceId")
    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.b_gloginback:
                startActivity(new Intent(this, LoginActivity.class));
                finish();
                break;
            case R.id.bt_loginactivity_login:
                login();
                break;
        }
    }

    private void login() {
        String username = etUsername.getText().toString().trim();
        String password = etPassword.getText().toString().trim();

        if (TextUtils.isEmpty(username) || TextUtils.isEmpty(password)) {
            Toast.makeText(this, "请输入用户名和密码", Toast.LENGTH_SHORT).show();
            return; // 提前返回，避免后续空指针异常
        }

        int adminId = dbHelper.validateAdmin(username, password);

        if (adminId!= -1) {
            Toast.makeText(this, "登录成功", Toast.LENGTH_SHORT).show();
            startActivity(new Intent(this, GuanliActivity.class));
            finish();
        } else {
            Toast.makeText(this, "用户名或密码不正确", Toast.LENGTH_SHORT).show();
            Log.d(TAG, "Login failed for username: " + username); // 添加日志方便调试
        }
    }
}