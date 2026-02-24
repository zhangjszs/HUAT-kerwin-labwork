package com.example.company.ui.leave

import android.os.Bundle
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.Button
import android.widget.LinearLayout
import android.widget.ListView
import android.widget.TextView
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.data.entity.LeaveRequest
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class LeaveManageActivity : AppCompatActivity() {

    private val viewModel: LeaveManageViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_leave_manage)

        val listView = findViewById<ListView>(R.id.list_pending_leaves)

        viewModel.pendingRequests.observe(this) { requests ->
            listView.adapter = LeaveAdapter(requests)
        }

        viewModel.state.observe(this) { state ->
            when (state) {
                is LeaveManageState.Success -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                    viewModel.resetState()
                }
                is LeaveManageState.Error -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                    viewModel.resetState()
                }
                else -> {}
            }
        }

        viewModel.loadPending()
    }

    private inner class LeaveAdapter(private val items: List<LeaveRequest>) : BaseAdapter() {
        override fun getCount() = items.size
        override fun getItem(position: Int) = items[position]
        override fun getItemId(position: Int) = items[position].id.toLong()

        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
            val request = items[position]
            val layout = LinearLayout(this@LeaveManageActivity).apply {
                orientation = LinearLayout.VERTICAL
                setPadding(24, 24, 24, 24)
                setBackgroundResource(R.drawable.brutalist_card)
            }

            val info = TextView(this@LeaveManageActivity).apply {
                text = "用户ID:${request.userId} [${request.leaveType}]\n${request.startDate} ~ ${request.endDate}\n原因:${request.reason}"
                textSize = 14f
            }
            layout.addView(info)

            val btnRow = LinearLayout(this@LeaveManageActivity).apply {
                orientation = LinearLayout.HORIZONTAL
                setPadding(0, 16, 0, 0)
            }

            val btnApprove = Button(this@LeaveManageActivity).apply {
                text = "APPROVE"
                setOnClickListener { viewModel.reviewRequest(request.id, true) }
                layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f).apply {
                    marginEnd = 8
                }
            }

            val btnReject = Button(this@LeaveManageActivity).apply {
                text = "REJECT"
                setOnClickListener { viewModel.reviewRequest(request.id, false) }
                layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f).apply {
                    marginStart = 8
                }
            }

            btnRow.addView(btnApprove)
            btnRow.addView(btnReject)
            layout.addView(btnRow)

            return layout
        }
    }
}
