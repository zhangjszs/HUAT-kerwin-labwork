package com.example.company.ui.admin

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class SearchActivity : AppCompatActivity() {

    private val viewModel: SearchViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_search)

        val tvId = findViewById<TextView>(R.id.s_id)
        val tvName = findViewById<TextView>(R.id.s_name)
        val tvPassword = findViewById<TextView>(R.id.s_password)
        val tvSex = findViewById<TextView>(R.id.s_sex)
        val tvTel = findViewById<TextView>(R.id.s_tel)
        val tvPosition = findViewById<TextView>(R.id.s_position)
        val tvTime = findViewById<TextView>(R.id.s_time)
        val edtSearch = findViewById<EditText>(R.id.edt_search)

        findViewById<Button>(R.id.bt_ok).setOnClickListener {
            viewModel.search(edtSearch.text.toString().trim())
        }

        findViewById<Button>(R.id.bt_searchback).setOnClickListener {
            startActivity(Intent(this, GuanliActivity::class.java))
            finish()
        }

        viewModel.searchResult.observe(this) { user ->
            if (user != null) {
                tvId.text = user.id.toString()
                tvName.text = user.name
                tvPassword.text = "********"
                tvSex.text = user.sex
                tvTel.text = user.tel
                tvPosition.text = user.position
                tvTime.text = user.time
                Toast.makeText(this, "查询成功", Toast.LENGTH_SHORT).show()
            }
        }

        viewModel.error.observe(this) { error ->
            if (error != null) {
                listOf(tvId, tvName, tvSex, tvTel, tvPosition, tvTime).forEach { it.text = "" }
                tvPassword.text = "********"
                Toast.makeText(this, error, Toast.LENGTH_SHORT).show()
            }
        }
    }
}
