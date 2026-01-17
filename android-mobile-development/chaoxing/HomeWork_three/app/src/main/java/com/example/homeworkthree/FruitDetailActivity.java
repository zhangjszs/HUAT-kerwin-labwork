package com.example.homeworkthree;

import android.os.Bundle;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

public class FruitDetailActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fruit_detail);

        // 获取传递的数据
        String name = getIntent().getStringExtra("name");
        String description = getIntent().getStringExtra("description");

        // 显示数据
        TextView detailFruitName = findViewById(R.id.detailFruitName);
        TextView detailFruitDescription = findViewById(R.id.detailFruitDescription);

        detailFruitName.setText(name);
        detailFruitDescription.setText(description);
    }
}