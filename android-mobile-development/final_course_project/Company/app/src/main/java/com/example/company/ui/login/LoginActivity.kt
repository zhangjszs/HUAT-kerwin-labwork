package com.example.company.ui.login

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.ui.admin.GloginActivity
import com.example.company.ui.main.FirstActivity
import com.example.company.ui.register.RegisterActivity
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class LoginActivity : AppCompatActivity() {

    private val viewModel: LoginViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)

        val etUsername = findViewById<EditText>(R.id.et_loginactivity_username)
        val etPassword = findViewById<EditText>(R.id.et_loginactivity_password)

        findViewById<Button>(R.id.bt_loginactivity_login).setOnClickListener {
            viewModel.login(
                etUsername.text.toString().trim(),
                etPassword.text.toString().trim()
            )
        }

        findViewById<Button>(R.id.bt_loginactivity_register).setOnClickListener {
            startActivity(Intent(this, RegisterActivity::class.java))
        }

        findViewById<Button>(R.id.bt_loginactivityGl).setOnClickListener {
            startActivity(Intent(this, GloginActivity::class.java))
        }

        viewModel.loginState.observe(this) { state ->
            when (state) {
                is LoginState.Success -> {
                    Toast.makeText(this, "登录成功", Toast.LENGTH_SHORT).show()
                    startActivity(Intent(this, FirstActivity::class.java).apply {
                        putExtra("user_id", state.user.id)
                        putExtra("username", state.user.name)
                    })
                    finish()
                }
                is LoginState.Error -> Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                else -> {}
            }
        }
    }
}
