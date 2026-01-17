package com.example.test2;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private EditText etUsername, etPassword;
    private Button btnLogin, btnCancel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main); // 加载布局

        // 初始化控件
        etUsername = findViewById(R.id.username);
        etPassword = findViewById(R.id.password);
        btnLogin = findViewById(R.id.btn1);
        btnCancel = findViewById(R.id.btn2);

        // 登录按钮点击事件
        btnLogin.setOnClickListener(v -> {
            String username = etUsername.getText().toString().trim();
            String password = etPassword.getText().toString().trim();

            if (!username.isEmpty() && !password.isEmpty()) {
                String message = "用户名：" + username + "\n密码：" + password;
                Toast.makeText(MainActivity.this, message, Toast.LENGTH_LONG).show();
            } else {
                Toast.makeText(MainActivity.this, "请输入用户名和密码！", Toast.LENGTH_SHORT).show();
            }
        });

        // 取消按钮点击事件
        btnCancel.setOnClickListener(v -> {
            etUsername.setText("");
            etPassword.setText("");
            Toast.makeText(MainActivity.this, "已清空输入内容", Toast.LENGTH_SHORT).show();
        });
    }
}
