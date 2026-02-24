package com.example.company.ui.appeal

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
import com.example.company.data.entity.Appeal
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class AppealManageActivity : AppCompatActivity() {

    private val viewModel: AppealManageViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_appeal_manage)

        val listView = findViewById<ListView>(R.id.list_pending_appeals)

        viewModel.pendingAppeals.observe(this) { appeals ->
            listView.adapter = AppealAdapter(appeals)
        }

        viewModel.state.observe(this) { state ->
            when (state) {
                is AppealManageState.Success -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                    viewModel.resetState()
                }
                is AppealManageState.Error -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                    viewModel.resetState()
                }
                else -> {}
            }
        }

        viewModel.loadPending()
    }

    private inner class AppealAdapter(private val items: List<Appeal>) : BaseAdapter() {
        override fun getCount() = items.size
        override fun getItem(position: Int) = items[position]
        override fun getItemId(position: Int) = items[position].id.toLong()

        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
            val appeal = items[position]
            val layout = LinearLayout(this@AppealManageActivity).apply {
                orientation = LinearLayout.VERTICAL
                setPadding(24, 24, 24, 24)
                setBackgroundResource(R.drawable.brutalist_card)
            }

            val info = TextView(this@AppealManageActivity).apply {
                text = "用户ID:${appeal.userId} 日期:${appeal.date}\n时间:${appeal.appealTime}\n原因:${appeal.reason}"
                textSize = 14f
            }
            layout.addView(info)

            val btnRow = LinearLayout(this@AppealManageActivity).apply {
                orientation = LinearLayout.HORIZONTAL
                setPadding(0, 16, 0, 0)
            }

            val btnApprove = Button(this@AppealManageActivity).apply {
                text = "APPROVE"
                setOnClickListener { viewModel.reviewAppeal(appeal.id, true) }
                layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f).apply {
                    marginEnd = 8
                }
            }

            val btnReject = Button(this@AppealManageActivity).apply {
                text = "REJECT"
                setOnClickListener { viewModel.reviewAppeal(appeal.id, false) }
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
