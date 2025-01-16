package com.example.company;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;

public class LoginActivity extends AppCompatActivity implements View.OnClickListener {
    private CompanyDatabaseHelper mDBOpenHelper;

    private EditText mEtLoginactivityUsername;
    private EditText mEtLoginactivityPassword;
    private Button mBtLonginactivittRegister;
    private Button mBtLoginactivityLogin;
    private Button mBtLoginactivityGl;
    private String rname;
    private String rpassword;


    /**
     * 创建 Activity 时先来重写 onCreate() 方法
     * 保存实例状态
     * super.onCreate(savedInstanceState);
     * 设置视图内容的配置文件
     * setContentView(R.layout.activity_login);
     * 上面这行代码真正实现了把视图层 View 也就是 layout 的内容放到 Activity 中进行显示
     * 初始化视图中的控件对象 initView()
     * 实例化 DBOpenHelper，待会进行登录验证的时候要用来进行数据查询
     * mDBOpenHelper = new DBOpenHelper(this);
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        initView();
        rpassword();

        mDBOpenHelper = new CompanyDatabaseHelper(this);
    }

    //记住密码
    private void rpassword() {
        if (rname != null && rpassword != null) {
            mEtLoginactivityUsername.setText(rname);
            mEtLoginactivityPassword.setText(rpassword);
        }
    }

    /**
     * onCreae()中大的布局已经摆放好了，接下来就该把layout里的东西
     * 声明、实例化对象然后有行为的赋予其行为
     * 这样就可以把视图层View也就是layout 与 控制层 Java 结合起来了
     */
    private void initView() {
        // 初始化控件
        mBtLoginactivityLogin = findViewById(R.id.bt_loginactivity_login);
        mBtLoginactivityGl = findViewById(R.id.bt_loginactivityGl);
        mBtLonginactivittRegister = findViewById(R.id.bt_loginactivity_register);
        mEtLoginactivityUsername = findViewById(R.id.et_loginactivity_username);
        mEtLoginactivityPassword = findViewById(R.id.et_loginactivity_password);


        // 设置点击事件监听器
        mBtLoginactivityLogin.setOnClickListener(this);
        mBtLonginactivittRegister.setOnClickListener(this);
        mBtLoginactivityGl.setOnClickListener(this);
    }


    @SuppressLint("NonConstantResourceId")
    public void onClick(View view) {
        switch (view.getId()) {
            // 跳转到注册界面
            case R.id.bt_loginactivity_register:
                startActivity(new Intent(this, RegisterActivity.class));
                break;

            case R.id.bt_loginactivityGl:
                startActivity(new Intent(this, GloginActivity.class));
                break;

            case R.id.bt_loginactivity_login:
                // 获取输入的用户名和密码
                String name = mEtLoginactivityUsername.getText().toString().trim();
                String password = mEtLoginactivityPassword.getText().toString().trim();

                // 校验用户名和密码是否为空
                if (TextUtils.isEmpty(name)) {
                    Toast.makeText(this, "请输入用户名", Toast.LENGTH_SHORT).show();
                    return;
                }

                if (TextUtils.isEmpty(password)) {
                    Toast.makeText(this, "请输入密码", Toast.LENGTH_SHORT).show();
                    return;
                }

                // 从数据库校验用户名和密码
                int userId = mDBOpenHelper.validateUser(name, password);

                if (userId!=-1) {
                    Toast.makeText(this, "登录成功", Toast.LENGTH_SHORT).show();
                    Intent intent = new Intent(this, FirstActivity.class);
                    intent.putExtra("user_id", userId); // 传递用户ID
                    intent.putExtra("username", name); // 传递用户名
                    startActivity(intent);
                    mDBOpenHelper.addAttendance(1, "2023-04-01", "08:55", "17:30", "On Time", "Early");
                    finish(); // 销毁登录界面
                } else {
                    Toast.makeText(this, "用户名或密码不正确，请重新输入", Toast.LENGTH_SHORT).show();
                }
                break;

            default:
                break;
        }
    }

}
