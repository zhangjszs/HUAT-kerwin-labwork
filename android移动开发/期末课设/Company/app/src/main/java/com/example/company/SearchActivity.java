package com.example.company;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class SearchActivity extends AppCompatActivity implements View.OnClickListener {

    private CompanyDatabaseHelper mDBOpenHelper;

    private EditText medts;
    private Button mbtok;
    private Button mbtback;
    private TextView mtvid;
    private TextView mtvname;
    private TextView mtvpassword;
    private TextView mtvsex;
    private TextView mtvtel;
    private TextView mtvposition;
    private TextView mtvtime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_search);

        initializeViews();
        mDBOpenHelper = new CompanyDatabaseHelper(this);
    }

    private void initializeViews() {
        mtvid = findViewById(R.id.s_id);
        mtvname = findViewById(R.id.s_name);
        mtvpassword = findViewById(R.id.s_password);
        mtvsex = findViewById(R.id.s_sex);
        mtvtel = findViewById(R.id.s_tel);
        mtvposition = findViewById(R.id.s_position);
        mtvtime = findViewById(R.id.s_time);
        medts = findViewById(R.id.edt_search);
        mbtok = findViewById(R.id.bt_ok);
        mbtback = findViewById(R.id.bt_searchback);

        mbtok.setOnClickListener(this);
        mbtback.setOnClickListener(this);
    }

    private void clearDisplayFields() {
        mtvid.setText("");
        mtvname.setText("");
        mtvpassword.setText("********"); // 密码显示为星号
        mtvsex.setText("");
        mtvtel.setText("");
        mtvposition.setText("");
        mtvtime.setText("");
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.bt_searchback:
                startActivity(new Intent(SearchActivity.this, GuanliActivity.class));
                finish();
                break;

            case R.id.bt_ok:
                String name = medts.getText().toString().trim();

                if (TextUtils.isEmpty(name)) {
                    Toast.makeText(this, "请输入要搜索的用户名", Toast.LENGTH_SHORT).show();
                    return;
                }

                searchUser(name);
                break;
        }
    }

    private void searchUser(String name) {
        try (SQLiteDatabase db = mDBOpenHelper.getReadableDatabase();
             Cursor cursor = db.query("USER",
                     new String[]{"_id", "name", "password", "sex", "tel", "position", "TIME"},
                     "name=?",
                     new String[]{name},
                     null, null, null)) {

            if (cursor.moveToFirst()) {
                // 查询成功，显示用户信息
                mtvid.setText(cursor.getString(cursor.getColumnIndex("_id")));
                mtvname.setText(cursor.getString(cursor.getColumnIndex("name")));
                mtvpassword.setText("********"); // 出于安全考虑，不显示实际密码
                mtvsex.setText(cursor.getString(cursor.getColumnIndex("sex")));
                mtvtel.setText(cursor.getString(cursor.getColumnIndex("tel")));
                mtvposition.setText(cursor.getString(cursor.getColumnIndex("position")));
                mtvtime.setText(cursor.getString(cursor.getColumnIndex("TIME")));

                Toast.makeText(this, "查询成功", Toast.LENGTH_SHORT).show();
            } else {
                // 未找到用户
                clearDisplayFields();
                Toast.makeText(this, "未找到该用户", Toast.LENGTH_SHORT).show();
            }
        } catch (Exception e) {
            clearDisplayFields();
            Toast.makeText(this, "查询出错: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }
}