package com.example.test3;

import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        // 初始化 ListView
        ListView listView = findViewById(R.id.listView1);

        // 列表项数据
        String[] strs = {"计算机", "英语", "数学", "语文", "物理"};

        // 使用 ArrayAdapter 绑定数据
        ArrayAdapter<String> adapter = new ArrayAdapter<>(
                this,
                android.R.layout.simple_list_item_1, // 使用内置布局样式
                strs
        );
        listView.setAdapter(adapter);

        // 设置点击事件监听器
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                // 获取当前点击项的内容
                String selectedItem = strs[position];
                // 显示 Toast
                Toast.makeText(MainActivity.this, "你选择了: " + selectedItem, Toast.LENGTH_SHORT).show();
            }
        });
    }
}
