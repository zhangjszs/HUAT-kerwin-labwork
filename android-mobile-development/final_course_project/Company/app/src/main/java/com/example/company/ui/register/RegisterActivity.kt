package com.example.company.ui.register

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.ui.login.LoginActivity
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class RegisterActivity : AppCompatActivity() {

    private val viewModel: RegisterViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_register)

        val etUsername = findViewById<EditText>(R.id.et_registeractivity_username)
        val etPassword1 = findViewById<EditText>(R.id.et_registeractivity_password1)
        val etPassword2 = findViewById<EditText>(R.id.et_registeractivity_password2)

        findViewById<Button>(R.id.bt_registeractivity_register).setOnClickListener {
            viewModel.register(
                etUsername.text.toString().trim(),
                etPassword1.text.toString().trim(),
                etPassword2.text.toString().trim()
            )
        }

        findViewById<Button>(R.id.bt_registeractivity_back).setOnClickListener {
            startActivity(Intent(this, LoginActivity::class.java))
            finish()
        }

        viewModel.registerState.observe(this) { state ->
            when (state) {
                is RegisterState.Success -> {
                    Toast.makeText(this, "验证通过，注册成功", Toast.LENGTH_SHORT).show()
                    startActivity(Intent(this, LoginActivity::class.java))
                    finish()
                }
                is RegisterState.Error -> Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                else -> {}
            }
        }
    }
}
