package com.example.company.ui.leave

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.LeaveRequest
import com.example.company.data.repository.LeaveRequestRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class LeaveRequestState {
    object Idle : LeaveRequestState()
    data class Success(val message: String) : LeaveRequestState()
    data class Error(val message: String) : LeaveRequestState()
}

@HiltViewModel
class LeaveRequestViewModel @Inject constructor(
    private val leaveRequestRepository: LeaveRequestRepository
) : ViewModel() {

    private val _state = MutableLiveData<LeaveRequestState>(LeaveRequestState.Idle)
    val state: LiveData<LeaveRequestState> = _state

    private val _requests = MutableLiveData<List<LeaveRequest>>()
    val requests: LiveData<List<LeaveRequest>> = _requests

    fun submitRequest(userId: Int, leaveType: String, startDate: String, endDate: String, reason: String) {
        if (startDate.isBlank() || endDate.isBlank() || reason.isBlank()) {
            _state.value = LeaveRequestState.Error("请填写所有字段")
            return
        }
        viewModelScope.launch {
            val request = LeaveRequest(
                userId = userId, leaveType = leaveType,
                startDate = startDate, endDate = endDate, reason = reason
            )
            leaveRequestRepository.insert(request)
            _state.value = LeaveRequestState.Success("请假申请提交成功")
            loadMyRequests(userId)
        }
    }

    fun loadMyRequests(userId: Int) {
        viewModelScope.launch {
            _requests.value = leaveRequestRepository.getByUserId(userId)
        }
    }

    fun resetState() { _state.value = LeaveRequestState.Idle }
}
