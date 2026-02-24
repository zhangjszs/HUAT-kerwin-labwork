package com.example.company.ui.history

import android.graphics.Color
import android.graphics.drawable.GradientDrawable
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.Button
import android.widget.GridView
import android.widget.LinearLayout
import android.widget.TextView
import androidx.activity.viewModels
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint
import java.util.Calendar

@AndroidEntryPoint
class AttendanceHistoryActivity : AppCompatActivity() {

    private val viewModel: AttendanceHistoryViewModel by viewModels()
    private var userId: Int = -1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_attendance_history)

        userId = intent.getIntExtra("user_id", -1)

        val tvMonthLabel = findViewById<TextView>(R.id.tv_month_label)
        val btnPrev = findViewById<Button>(R.id.btn_prev_month)
        val btnNext = findViewById<Button>(R.id.btn_next_month)
        val gridCalendar = findViewById<GridView>(R.id.grid_calendar)
        val tvTotal = findViewById<TextView>(R.id.tv_stat_total)
        val tvOnTime = findViewById<TextView>(R.id.tv_stat_ontime)
        val tvLate = findViewById<TextView>(R.id.tv_stat_late)
        val tvAbsent = findViewById<TextView>(R.id.tv_stat_absent)
        val llHeaders = findViewById<LinearLayout>(R.id.ll_weekday_headers)

        setupWeekdayHeaders(llHeaders)

        btnPrev.setOnClickListener { viewModel.previousMonth(userId) }
        btnNext.setOnClickListener { viewModel.nextMonth(userId) }

        viewModel.year.observe(this) { y ->
            val m = viewModel.month.value ?: return@observe
            tvMonthLabel.text = "%d-%02d".format(y, m)
        }
        viewModel.month.observe(this) { m ->
            val y = viewModel.year.value ?: return@observe
            tvMonthLabel.text = "%d-%02d".format(y, m)
        }

        viewModel.stats.observe(this) { stats ->
            tvTotal.text = "工作日\n${stats.totalWorkDays}"
            tvOnTime.text = "正常\n${stats.onTime}"
            tvLate.text = "迟到\n${stats.late}"
            tvAbsent.text = "缺勤\n${stats.absent}"
        }

        viewModel.daySummaries.observe(this) { summaries ->
            val y = viewModel.year.value ?: return@observe
            val m = viewModel.month.value ?: return@observe
            val cal = Calendar.getInstance()
            cal.set(y, m - 1, 1)
            val firstDayOfWeek = cal.get(Calendar.DAY_OF_WEEK)
            // Monday-based offset: Mon=0, Tue=1, ..., Sun=6
            val offset = if (firstDayOfWeek == Calendar.SUNDAY) 6 else firstDayOfWeek - Calendar.MONDAY
            gridCalendar.adapter = CalendarAdapter(summaries, offset)
            gridCalendar.setOnItemClickListener { _, _, position, _ ->
                val idx = position - offset
                if (idx in summaries.indices) {
                    showDayDetail(summaries[idx], y, m)
                }
            }
        }

        val now = Calendar.getInstance()
        viewModel.loadMonth(userId, now.get(Calendar.YEAR), now.get(Calendar.MONTH) + 1)
    }

    private fun setupWeekdayHeaders(container: LinearLayout) {
        val days = arrayOf("Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun")
        days.forEach { day ->
            val tv = TextView(this).apply {
                text = day
                gravity = Gravity.CENTER
                textSize = 12f
                layoutParams = LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f)
            }
            container.addView(tv)
        }
    }

    private fun showDayDetail(summary: DaySummary, year: Int, month: Int) {
        val dateStr = "%d年%02d月%02d日".format(year, month, summary.day)
        val detail = when (summary.status) {
            DayStatus.ON_TIME -> "签到: ${summary.record?.timeIn ?: "-"}\n签退: ${summary.record?.timeOut ?: "-"}\n状态: 正常"
            DayStatus.LATE -> "签到: ${summary.record?.timeIn ?: "-"}\n签退: ${summary.record?.timeOut ?: "-"}\n状态: ${summary.record?.statusIn ?: "迟到"}"
            DayStatus.ABSENT -> "状态: 缺勤"
            DayStatus.LEAVE -> "状态: 请假"
            DayStatus.WEEKEND -> "状态: 周末"
            DayStatus.FUTURE -> "状态: 未到"
        }
        AlertDialog.Builder(this)
            .setTitle(dateStr)
            .setMessage(detail)
            .setPositiveButton("确定", null)
            .show()
    }

    private inner class CalendarAdapter(
        private val summaries: List<DaySummary>,
        private val offset: Int
    ) : BaseAdapter() {

        override fun getCount() = summaries.size + offset
        override fun getItem(position: Int): Any? {
            val idx = position - offset
            return if (idx in summaries.indices) summaries[idx] else null
        }
        override fun getItemId(position: Int) = position.toLong()

        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
            val idx = position - offset
            val tv = TextView(this@AttendanceHistoryActivity).apply {
                gravity = Gravity.CENTER
                textSize = 14f
                setPadding(4, 16, 4, 16)
            }

            if (idx < 0 || idx >= summaries.size) {
                tv.text = ""
                return tv
            }

            val summary = summaries[idx]
            tv.text = summary.day.toString()

            val bgColor = when (summary.status) {
                DayStatus.ON_TIME -> Color.parseColor("#06D6A0")   // green
                DayStatus.LATE -> Color.parseColor("#FFE600")      // yellow
                DayStatus.ABSENT -> Color.parseColor("#EF4444")    // red
                DayStatus.LEAVE -> Color.parseColor("#2563EB")     // blue
                DayStatus.FUTURE -> Color.parseColor("#D1D5DB")    // gray
                DayStatus.WEEKEND -> Color.parseColor("#E5E7EB")   // light gray
            }

            val textColor = when (summary.status) {
                DayStatus.LEAVE, DayStatus.ABSENT -> Color.WHITE
                else -> Color.parseColor("#1A1A1A")
            }

            val drawable = GradientDrawable().apply {
                setColor(bgColor)
                setStroke(2, Color.parseColor("#1A1A1A"))
                cornerRadius = 8f
            }
            tv.background = drawable
            tv.setTextColor(textColor)

            return tv
        }
    }
}
