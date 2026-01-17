package com.example.homeworkthree;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import androidx.appcompat.app.AppCompatActivity;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 获取水果列表
        List<Fruit> fruitList = Fruit.getDefaultList();

        // 创建适配器
        FruitAdapter adapter = new FruitAdapter(this, fruitList);

        // 设置 ListView 的适配器
        ListView listView = findViewById(R.id.fruitListView);
        listView.setAdapter(adapter);

        // 设置点击事件
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Fruit selectedFruit = fruitList.get(position);

                // 跳转到详情页面
                Intent intent = new Intent(MainActivity.this, FruitDetailActivity.class);
                intent.putExtra("name", selectedFruit.name);
                intent.putExtra("description", selectedFruit.description);
                startActivity(intent);
            }
        });
    }
}