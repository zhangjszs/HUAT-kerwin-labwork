package com.example.test2_1;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // 应用自定义布局文件

        final EditText usernameET = (EditText) findViewById(R.id.username); // 获得用户名控件
        final EditText passwordET = (EditText) findViewById(R.id.password); // 获得密码控件
        Button login = (Button) findViewById(R.id.login); // 获得按钮控件

        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String username = usernameET.getText().toString(); // 获得用户名
                String password = passwordET.getText().toString(); // 获得密码

                // 获得私有类型的SharedPreferences
                SharedPreferences sp = getSharedPreferences("mrsoft", MODE_PRIVATE);
                Editor editor = sp.edit(); // 获得Editor对象
                editor.putString("username", username); // 增加用户名
                editor.putString("password", password); // 增加密码
                editor.apply(); // 使用apply代替commit以提高性能

                Intent intent = new Intent(MainActivity.this, OtherActivity.class); // 指定跳转到OtherActivity
                startActivity(intent); // 实现跳转
            }
        });
    }
}