package com.example.homeworktwo;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private EditText etName, etStudentId, etMajor;
    private RadioGroup rgGender, rgPartyMember;
    private CheckBox cbBasketball, cbFootball, cbGymnastics, cbBadminton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 初始化视图
        etName = findViewById(R.id.etName);
        etStudentId = findViewById(R.id.etStudentId);
        etMajor = findViewById(R.id.etMajor);
        rgGender = findViewById(R.id.rgGender);
        rgPartyMember = findViewById(R.id.rgPartyMember);
        cbBasketball = findViewById(R.id.cbBasketball);
        cbFootball = findViewById(R.id.cbFootball);
        cbGymnastics = findViewById(R.id.cbGymnastics);
        cbBadminton = findViewById(R.id.cbBadminton);

        // 提交按钮点击事件
        Button btnSubmit = findViewById(R.id.btnSubmit);
        btnSubmit.setOnClickListener(v -> {
            String name = etName.getText().toString();
            String studentId = etStudentId.getText().toString();
            String major = etMajor.getText().toString();

            if (name.isEmpty() || studentId.isEmpty() || major.isEmpty()) {
                Toast.makeText(MainActivity.this, "姓名、学号和专业不能为空", Toast.LENGTH_SHORT).show();
            } else {
                Intent intent = new Intent(MainActivity.this, DisplayActivity.class);
                intent.putExtra("NAME", name);
                intent.putExtra("STUDENT_ID", studentId);
                intent.putExtra("MAJOR", major);
                intent.putExtra("GENDER", rgGender.getCheckedRadioButtonId() == R.id.rbMale ? "男" : "女");
                intent.putExtra("HOBBIES", getHobbies());
                intent.putExtra("PARTY_MEMBER", rgPartyMember.getCheckedRadioButtonId() == R.id.rbYes ? "是" : "否");
                startActivity(intent);
            }
        });
    }

    private String getHobbies() {
        StringBuilder hobbies = new StringBuilder();
        if (cbBasketball.isChecked()) hobbies.append("篮球 ");
        if (cbFootball.isChecked()) hobbies.append("足球 ");
        if (cbGymnastics.isChecked()) hobbies.append("体操 ");
        if (cbBadminton.isChecked()) hobbies.append("羽毛球 ");
        return hobbies.toString().trim();
    }
}