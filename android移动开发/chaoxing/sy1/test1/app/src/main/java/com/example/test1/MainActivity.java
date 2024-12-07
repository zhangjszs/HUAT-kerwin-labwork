package com.example.test1;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private EditText etUsername, etPassword;
    private RadioGroup rgGender;
    private CheckBox cbReading, cbTraveling, cbSports;
    private Button btnSubmit;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        // 初始化控件
        etUsername = findViewById(R.id.et_username);
        etPassword = findViewById(R.id.et_password);
        rgGender = findViewById(R.id.rg_gender);
        cbReading = findViewById(R.id.cb_reading);
        cbTraveling = findViewById(R.id.cb_traveling);
        cbSports = findViewById(R.id.cb_sports);
        btnSubmit = findViewById(R.id.btn_submit);

        // 性别按钮监听
        rgGender.setOnCheckedChangeListener((group, checkedId) -> {
            String gender = "";
            if (checkedId == R.id.rb_male) {
                gender = "男";
            } else if (checkedId == R.id.rb_female) {
                gender = "女";
            }
            Toast.makeText(MainActivity.this, "性别：" + gender, Toast.LENGTH_SHORT).show();
        });

        // 复选框监听
        View.OnClickListener checkboxListener = v -> {
            CheckBox checkBox = (CheckBox) v;
            if (checkBox.isChecked()) {
                Toast.makeText(MainActivity.this, "选中：" + checkBox.getText(), Toast.LENGTH_SHORT).show();
            }
        };
        cbReading.setOnClickListener(checkboxListener);
        cbTraveling.setOnClickListener(checkboxListener);
        cbSports.setOnClickListener(checkboxListener);

        // 提交按钮监听
        btnSubmit.setOnClickListener(v -> {
            // 获取用户名和密码
            String username = etUsername.getText().toString();
            String password = etPassword.getText().toString();

            // 获取性别
            int selectedGenderId = rgGender.getCheckedRadioButtonId();
            String gender = selectedGenderId == R.id.rb_male ? "男" : "女";

            // 获取选中的爱好
            StringBuilder hobbies = new StringBuilder();
            if (cbReading.isChecked()) {
                hobbies.append("阅读 ");
            }
            if (cbTraveling.isChecked()) {
                hobbies.append("旅游 ");
            }
            if (cbSports.isChecked()) {
                hobbies.append("运动 ");
            }
            // 拼接并显示结果
            String result = "用户名：" + username + "\n" +
                    "密码：" + password + "\n" +
                    "性别：" + gender + "\n" +
                    "爱好：" + (hobbies.length() > 0 ? hobbies.toString() : "无");
            Toast.makeText(MainActivity.this, result, Toast.LENGTH_LONG).show();
        });
    }
}
