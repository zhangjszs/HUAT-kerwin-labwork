package com.example.company.ui.admin

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.ui.appeal.AppealManageActivity
import com.example.company.ui.leave.LeaveManageActivity
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class GuanliActivity : AppCompatActivity() {

    private val viewModel: ManageViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_guanli)

        val edtName = findViewById<EditText>(R.id.edt_name)
        val edtPassword = findViewById<EditText>(R.id.edt_password)
        val edtSex = findViewById<EditText>(R.id.edt_sex)
        val edtTel = findViewById<EditText>(R.id.edt_tel)
        val edtPosition = findViewById<EditText>(R.id.edt_position)
        val edtTime = findViewById<EditText>(R.id.edt_time)

        fun clearFields() {
            listOf(edtName, edtPassword, edtSex, edtTel, edtPosition, edtTime).forEach { it.setText("") }
        }

        fun fields() = listOf(
            edtName.text.toString().trim(), edtPassword.text.toString().trim(),
            edtSex.text.toString().trim(), edtTel.text.toString().trim(),
            edtPosition.text.toString().trim(), edtTime.text.toString().trim()
        )

        findViewById<Button>(R.id.bt_add).setOnClickListener {
            val f = fields()
            viewModel.addUser(f[0], f[1], f[2], f[3], f[4], f[5])
        }

        findViewById<Button>(R.id.bt_delete).setOnClickListener {
            AlertDialog.Builder(this)
                .setTitle("删除用户信息").setMessage("确认删除该用户吗？")
                .setPositiveButton("确定") { _, _ -> viewModel.deleteUser(edtName.text.toString().trim()) }
                .setNegativeButton("取消", null).show()
        }

        findViewById<Button>(R.id.bt_update).setOnClickListener {
            val f = fields()
            viewModel.updateUser(f[0], f[1], f[2], f[3], f[4], f[5])
        }

        findViewById<Button>(R.id.bt_search).setOnClickListener {
            startActivity(Intent(this, SearchActivity::class.java))
        }

        findViewById<Button>(R.id.bt_manage_appeals).setOnClickListener {
            startActivity(Intent(this, AppealManageActivity::class.java))
        }

        findViewById<Button>(R.id.bt_manage_leaves).setOnClickListener {
            startActivity(Intent(this, LeaveManageActivity::class.java))
        }

        findViewById<Button>(R.id.bt_guanliback).setOnClickListener {
            startActivity(Intent(this, GloginActivity::class.java))
            finish()
        }

        viewModel.result.observe(this) { result ->
            when (result) {
                is ManageResult.Success -> {
                    Toast.makeText(this, result.message, Toast.LENGTH_SHORT).show()
                    clearFields()
                    viewModel.resetResult()
                }
                is ManageResult.Error -> {
                    Toast.makeText(this, result.message, Toast.LENGTH_SHORT).show()
                    viewModel.resetResult()
                }
                else -> {}
            }
        }
    }
}
