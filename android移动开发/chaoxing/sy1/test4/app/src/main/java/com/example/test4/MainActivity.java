package com.example.test4;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        Button btn = findViewById(R.id.submit);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 获取用户名
                EditText usertxt = findViewById(R.id.user);
                String userstr = usertxt.getText().toString();

                // 获取年龄
                EditText agetxt = findViewById(R.id.age);
                String agestr = agetxt.getText().toString();

                // 校验输入
                if (userstr.isEmpty() || agestr.isEmpty()) {
                    Toast.makeText(MainActivity.this, "请输入完整信息！", Toast.LENGTH_SHORT).show();
                    return;
                }

                int age = Integer.parseInt(agestr);

                // 创建 Intent 并传递数据
                Intent intent = new Intent(MainActivity.this, OtherActivity.class);
                Bundle bundle = new Bundle();
                bundle.putString("name", userstr);
                bundle.putInt("age", age);
                intent.putExtras(bundle);

                // 启动新Activity
                startActivity(intent);
            }
        });
    }
}
