package com.example.company.ui.admin

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
class GloginActivity : AppCompatActivity() {

    private val viewModel: AdminLoginViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_glogin)

        val etUsername = findViewById<EditText>(R.id.et_loginactivity_username)
        val etPassword = findViewById<EditText>(R.id.et_loginactivity_password)

        findViewById<Button>(R.id.bt_loginactivity_login).setOnClickListener {
            viewModel.login(etUsername.text.toString().trim(), etPassword.text.toString().trim())
        }

        findViewById<Button>(R.id.b_gloginback).setOnClickListener {
            startActivity(Intent(this, LoginActivity::class.java))
            finish()
        }

        viewModel.state.observe(this) { state ->
            when (state) {
                is AdminLoginState.Success -> {
                    Toast.makeText(this, "登录成功", Toast.LENGTH_SHORT).show()
                    startActivity(Intent(this, GuanliActivity::class.java))
                    finish()
                }
                is AdminLoginState.Error -> Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                else -> {}
            }
        }
    }
}
