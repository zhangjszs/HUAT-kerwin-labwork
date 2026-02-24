package com.example.company.ui.leave

import android.os.Bundle
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.EditText
import android.widget.ListView
import android.widget.Spinner
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class LeaveRequestActivity : AppCompatActivity() {

    private val viewModel: LeaveRequestViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_leave_request)

        val userId = intent.getIntExtra("user_id", -1)

        val spinnerType = findViewById<Spinner>(R.id.spinner_leave_type)
        val edtStart = findViewById<EditText>(R.id.edt_start_date)
        val edtEnd = findViewById<EditText>(R.id.edt_end_date)
        val edtReason = findViewById<EditText>(R.id.edt_leave_reason)
        val btnSubmit = findViewById<Button>(R.id.btn_submit_leave)
        val listRequests = findViewById<ListView>(R.id.list_leave_requests)

        val leaveTypes = arrayOf("年假", "病假", "事假")
        spinnerType.adapter = ArrayAdapter(this, android.R.layout.simple_spinner_dropdown_item, leaveTypes)

        btnSubmit.setOnClickListener {
            viewModel.submitRequest(
                userId,
                spinnerType.selectedItem.toString(),
                edtStart.text.toString().trim(),
                edtEnd.text.toString().trim(),
                edtReason.text.toString().trim()
            )
        }

        viewModel.state.observe(this) { state ->
            when (state) {
                is LeaveRequestState.Success -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                    edtStart.setText("")
                    edtEnd.setText("")
                    edtReason.setText("")
                    viewModel.resetState()
                }
                is LeaveRequestState.Error -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                    viewModel.resetState()
                }
                else -> {}
            }
        }

        viewModel.requests.observe(this) { requests ->
            val items = requests.map {
                "[${it.leaveType}] ${it.startDate} ~ ${it.endDate} [${it.status}]\n原因:${it.reason}"
            }
            listRequests.adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, items)
        }

        viewModel.loadMyRequests(userId)
    }
}
