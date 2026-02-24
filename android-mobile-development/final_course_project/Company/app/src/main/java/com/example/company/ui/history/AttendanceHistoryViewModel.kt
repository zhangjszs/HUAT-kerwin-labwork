package com.example.company.ui.history

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.AttendanceRecord
import com.example.company.data.repository.AttendanceRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import java.util.Calendar
import javax.inject.Inject

data class DaySummary(
    val day: Int,
    val status: DayStatus,
    val record: AttendanceRecord? = null
)

enum class DayStatus {
    ON_TIME, LATE, ABSENT, LEAVE, FUTURE, WEEKEND
}

data class MonthStats(
    val totalWorkDays: Int,
    val onTime: Int,
    val late: Int,
    val absent: Int
)

@HiltViewModel
class AttendanceHistoryViewModel @Inject constructor(
    private val attendanceRepository: AttendanceRepository
) : ViewModel() {

    private val _year = MutableLiveData<Int>()
    val year: LiveData<Int> = _year

    private val _month = MutableLiveData<Int>()
    val month: LiveData<Int> = _month

    private val _daySummaries = MutableLiveData<List<DaySummary>>()
    val daySummaries: LiveData<List<DaySummary>> = _daySummaries

    private val _stats = MutableLiveData<MonthStats>()
    val stats: LiveData<MonthStats> = _stats

    fun loadMonth(userId: Int, year: Int, month: Int) {
        _year.value = year
        _month.value = month
        viewModelScope.launch {
            val cal = Calendar.getInstance()
            val today = Calendar.getInstance()
            cal.set(year, month - 1, 1)
            val daysInMonth = cal.getActualMaximum(Calendar.DAY_OF_MONTH)

            val dates = (1..daysInMonth).map { day ->
                "%d年%02d月%02d日".format(year, month, day)
            }

            val records = attendanceRepository.getByUserIdAndDateRange(userId, dates)
            val recordMap = records.associateBy { it.date }

            var onTime = 0
            var late = 0
            var absent = 0
            var totalWorkDays = 0

            val summaries = (1..daysInMonth).map { day ->
                cal.set(year, month - 1, day)
                val dayOfWeek = cal.get(Calendar.DAY_OF_WEEK)
                val isWeekend = dayOfWeek == Calendar.SATURDAY || dayOfWeek == Calendar.SUNDAY
                val isFuture = cal.after(today)
                val dateStr = "%d年%02d月%02d日".format(year, month, day)
                val record = recordMap[dateStr]

                val status = when {
                    isFuture -> DayStatus.FUTURE
                    isWeekend -> DayStatus.WEEKEND
                    record != null -> {
                        totalWorkDays++
                        if (record.statusIn == "迟到") {
                            late++
                            DayStatus.LATE
                        } else {
                            onTime++
                            DayStatus.ON_TIME
                        }
                    }
                    else -> {
                        if (!isFuture && !isWeekend) {
                            totalWorkDays++
                            absent++
                        }
                        DayStatus.ABSENT
                    }
                }
                DaySummary(day, status, record)
            }

            _daySummaries.value = summaries
            _stats.value = MonthStats(totalWorkDays, onTime, late, absent)
        }
    }

    fun previousMonth(userId: Int) {
        val y = _year.value ?: return
        val m = _month.value ?: return
        if (m == 1) loadMonth(userId, y - 1, 12)
        else loadMonth(userId, y, m - 1)
    }

    fun nextMonth(userId: Int) {
        val y = _year.value ?: return
        val m = _month.value ?: return
        if (m == 12) loadMonth(userId, y + 1, 1)
        else loadMonth(userId, y, m + 1)
    }
}
