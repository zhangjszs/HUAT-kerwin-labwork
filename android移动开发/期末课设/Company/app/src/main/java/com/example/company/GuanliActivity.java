package com.example.company;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class GuanliActivity extends AppCompatActivity implements View.OnClickListener {

    private Cursor cursor;
    private CompanyDatabaseHelper mDBOpenHelper;

    // Buttons
    private Button mbtadd;
    private Button mbtdelete;
    private Button mbtupdate;
    private Button mbtsearch;
    private Button mbtback;

    // EditText fields
    private EditText medtname;
    private EditText medtpassword; // 新增密码输入框
    private EditText medtsex;
    private EditText medttel;
    private EditText medtposition;
    private EditText medttime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_guanli);
        initView();
        mDBOpenHelper = new CompanyDatabaseHelper(this);
    }

    private void initView() {
        // Initialize buttons
        mbtadd = findViewById(R.id.bt_add);
        mbtdelete = findViewById(R.id.bt_delete);
        mbtupdate = findViewById(R.id.bt_update);
        mbtsearch = findViewById(R.id.bt_search);
        mbtback = findViewById(R.id.bt_guanliback);

        // Initialize EditText fields
        medtname = findViewById(R.id.edt_name);
        medtpassword = findViewById(R.id.edt_password); // 新增密码输入框绑定
        medtsex = findViewById(R.id.edt_sex);
        medttel = findViewById(R.id.edt_tel);
        medtposition = findViewById(R.id.edt_position);
        medttime = findViewById(R.id.edt_time);

        // Set click listeners
        mbtadd.setOnClickListener(this);
        mbtdelete.setOnClickListener(this);
        mbtupdate.setOnClickListener(this);
        mbtsearch.setOnClickListener(this);
        mbtback.setOnClickListener(this);
    }

    private void clearInputFields() {
        medtname.setText("");
        medtpassword.setText("");
        medtsex.setText("");
        medttel.setText("");
        medtposition.setText("");
        medttime.setText("");
    }

    @SuppressLint("NonConstantResourceId")
    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.bt_add:
                String name = medtname.getText().toString().trim();
                String password = medtpassword.getText().toString().trim();
                String sex = medtsex.getText().toString().trim();
                String tel = medttel.getText().toString().trim();
                String position = medtposition.getText().toString().trim();
                String time = medttime.getText().toString().trim();

                // 添加数据验证
                if (name.isEmpty() || password.isEmpty()) {
                    Toast.makeText(this, "用户名和密码不能为空", Toast.LENGTH_SHORT).show();
                    return;
                }

                long result = mDBOpenHelper.user_add(name, password, sex, tel, position, time);
                if (result != -1) {
                    Toast.makeText(this, "添加成功", Toast.LENGTH_SHORT).show();
                    clearInputFields();
                } else {
                    Toast.makeText(this, "添加失败，用户可能已存在", Toast.LENGTH_SHORT).show();
                }
                break;

            case R.id.bt_delete:
                AlertDialog alertDialog = new AlertDialog.Builder(this)
                        .setTitle("删除用户信息")
                        .setMessage("确认删除该用户吗？")
                        .setPositiveButton("确定", (dialogInterface, i) -> {
                            String deleteName = medtname.getText().toString().trim();
                            if (deleteName.isEmpty()) {
                                Toast.makeText(GuanliActivity.this, "请输入要删除的用户名", Toast.LENGTH_SHORT).show();
                                return;
                            }
                            if (mDBOpenHelper.user_delete(deleteName)) {
                                Toast.makeText(GuanliActivity.this, "删除成功", Toast.LENGTH_SHORT).show();
                                clearInputFields();
                            } else {
                                Toast.makeText(GuanliActivity.this, "删除失败，用户不存在", Toast.LENGTH_SHORT).show();
                            }
                        })
                        .setNegativeButton("取消", null)
                        .create();
                alertDialog.show();
                break;

            case R.id.bt_update:
                String updateName = medtname.getText().toString().trim();
                String updatePassword = medtpassword.getText().toString().trim();
                String updateSex = medtsex.getText().toString().trim();
                String updateTel = medttel.getText().toString().trim();
                String updatePosition = medtposition.getText().toString().trim();
                String updateTime = medttime.getText().toString().trim();

                if (updateName.isEmpty()) {
                    Toast.makeText(this, "请输入要更新的用户名", Toast.LENGTH_SHORT).show();
                    return;
                }

                if (mDBOpenHelper.user_update(updateName, updatePassword, updateSex,
                        updateTel, updatePosition, updateTime)) {
                    Toast.makeText(this, "更新成功", Toast.LENGTH_SHORT).show();
                    clearInputFields();
                } else {
                    Toast.makeText(this, "更新失败，用户不存在", Toast.LENGTH_SHORT).show();
                }
                break;

            case R.id.bt_search:
                startActivity(new Intent(GuanliActivity.this, SearchActivity.class));
                finish();
                break;

            case R.id.bt_guanliback:
                startActivity(new Intent(GuanliActivity.this, GloginActivity.class));
                finish();
                break;
        }
    }
}