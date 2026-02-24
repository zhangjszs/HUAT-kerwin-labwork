package com.example.company.ui.attendance

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.AttendanceRecord
import com.example.company.data.repository.AttendanceRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale
import javax.inject.Inject

sealed class AttendanceState {
    object Idle : AttendanceState()
    data class SignedIn(val status: String) : AttendanceState()
    data class SignedOut(val status: String) : AttendanceState()
    data class AlreadySignedIn(val record: AttendanceRecord) : AttendanceState()
    data class Error(val message: String) : AttendanceState()
}

@HiltViewModel
class AttendanceViewModel @Inject constructor(
    private val attendanceRepository: AttendanceRepository
) : ViewModel() {

    private val _state = MutableLiveData<AttendanceState>(AttendanceState.Idle)
    val state: LiveData<AttendanceState> = _state

    private val _todayRecord = MutableLiveData<AttendanceRecord?>()
    val todayRecord: LiveData<AttendanceRecord?> = _todayRecord

    fun loadTodayRecord(userId: Int, date: String) {
        viewModelScope.launch {
            val record = attendanceRepository.getTodayRecord(userId, date)
            _todayRecord.value = record
            if (record != null) {
                _state.value = AttendanceState.AlreadySignedIn(record)
            }
        }
    }

    fun signIn(userId: Int, date: String) {
        viewModelScope.launch {
            val existing = attendanceRepository.getTodayRecord(userId, date)
            if (existing != null) {
                _state.value = AttendanceState.Error("今日已签到")
                return@launch
            }
            val timeNow = SimpleDateFormat("HH:mm", Locale.getDefault()).format(Date())
            val status = getSignInStatus(timeNow)
            val record = AttendanceRecord(
                userId = userId, date = date,
                timeIn = timeNow, timeOut = "",
                statusIn = status, statusOut = ""
            )
            attendanceRepository.signIn(record)
            _todayRecord.value = record
            _state.value = AttendanceState.SignedIn(status)
        }
    }

    fun signOut(userId: Int, date: String) {
        viewModelScope.launch {
            val existing = attendanceRepository.getTodayRecord(userId, date)
            if (existing == null) {
                _state.value = AttendanceState.Error("请先签到")
                return@launch
            }
            val timeNow = SimpleDateFormat("HH:mm", Locale.getDefault()).format(Date())
            val status = getSignOutStatus(timeNow)
            val updated = existing.copy(timeOut = timeNow, statusOut = status)
            attendanceRepository.signOut(updated)
            _todayRecord.value = updated
            _state.value = AttendanceState.SignedOut(status)
        }
    }

    private fun getSignInStatus(time: String): String {
        return try {
            val sdf = SimpleDateFormat("HH:mm", Locale.getDefault())
            val signIn = sdf.parse(time) ?: return "异常签到"
            val workStart = sdf.parse("09:00") ?: return "异常签到"
            when {
                signIn.before(workStart) -> "早签到"
                signIn.time == workStart.time -> "正常签到"
                else -> "迟到"
            }
        } catch (e: Exception) {
            "异常签到"
        }
    }

    private fun getSignOutStatus(time: String): String {
        return try {
            val sdf = SimpleDateFormat("HH:mm", Locale.getDefault())
            val signOut = sdf.parse(time)
            val workEnd = sdf.parse("18:00")
            if (signOut != null && workEnd != null && signOut.before(workEnd)) "异常签退" else "正常签退"
        } catch (e: Exception) {
            "异常签退"
        }
    }
}
