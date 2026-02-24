package com.example.company.ui.appeal

import android.os.Bundle
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.EditText
import android.widget.ListView
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class AppealActivity : AppCompatActivity() {

    private val viewModel: AppealViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_appeal)

        val userId = intent.getIntExtra("user_id", -1)

        val edtDate = findViewById<EditText>(R.id.edt_appeal_date)
        val edtTime = findViewById<EditText>(R.id.edt_appeal_time)
        val edtReason = findViewById<EditText>(R.id.edt_appeal_reason)
        val btnSubmit = findViewById<Button>(R.id.btn_submit_appeal)
        val listAppeals = findViewById<ListView>(R.id.list_appeals)

        btnSubmit.setOnClickListener {
            viewModel.submitAppeal(
                userId,
                edtDate.text.toString().trim(),
                edtTime.text.toString().trim(),
                edtReason.text.toString().trim()
            )
        }

        viewModel.state.observe(this) { state ->
            when (state) {
                is AppealState.Success -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                    edtDate.setText("")
                    edtTime.setText("")
                    edtReason.setText("")
                    viewModel.resetState()
                }
                is AppealState.Error -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                    viewModel.resetState()
                }
                else -> {}
            }
        }

        viewModel.appeals.observe(this) { appeals ->
            val items = appeals.map { "日期:${it.date} 时间:${it.appealTime} [${it.status}]\n原因:${it.reason}" }
            listAppeals.adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, items)
        }

        viewModel.loadMyAppeals(userId)
    }
}
