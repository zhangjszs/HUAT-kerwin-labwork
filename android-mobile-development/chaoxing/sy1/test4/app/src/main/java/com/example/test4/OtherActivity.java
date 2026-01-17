package com.example.test4;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class OtherActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.other);

        // 获取传递过来的数据
        Bundle bundle = getIntent().getExtras();
        String username = bundle.getString("name");
        int userage = bundle.getInt("age");

        // 显示数据
        TextView info = findViewById(R.id.info);
        info.setText("用户为：" + username + "\n年龄为：" + userage);
    }
}
