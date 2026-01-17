package com.example.homeworktwo;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class DisplayActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display);

        TextView tvName = findViewById(R.id.tvName);
        TextView tvStudentId = findViewById(R.id.tvStudentId);
        TextView tvMajor = findViewById(R.id.tvMajor);
        TextView tvGender = findViewById(R.id.tvGender);
        TextView tvHobbies = findViewById(R.id.tvHobbies);
        TextView tvPartyMember = findViewById(R.id.tvPartyMember);

        Intent intent = getIntent();
        tvName.setText("姓名: " + intent.getStringExtra("NAME"));
        tvStudentId.setText("学号: " + intent.getStringExtra("STUDENT_ID"));
        tvMajor.setText("专业: " + intent.getStringExtra("MAJOR"));
        tvGender.setText("性别: " + intent.getStringExtra("GENDER"));
        tvHobbies.setText("兴趣爱好: " + intent.getStringExtra("HOBBIES"));
        tvPartyMember.setText("是否党员: " + intent.getStringExtra("PARTY_MEMBER"));

        Button btnBack = findViewById(R.id.btnBack);
        btnBack.setOnClickListener(v -> finish());
    }
}