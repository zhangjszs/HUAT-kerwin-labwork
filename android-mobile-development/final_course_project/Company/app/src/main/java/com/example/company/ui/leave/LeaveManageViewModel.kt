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

sealed class LeaveManageState {
    object Idle : LeaveManageState()
    data class Success(val message: String) : LeaveManageState()
    data class Error(val message: String) : LeaveManageState()
}

@HiltViewModel
class LeaveManageViewModel @Inject constructor(
    private val leaveRequestRepository: LeaveRequestRepository
) : ViewModel() {

    private val _state = MutableLiveData<LeaveManageState>(LeaveManageState.Idle)
    val state: LiveData<LeaveManageState> = _state

    private val _pendingRequests = MutableLiveData<List<LeaveRequest>>()
    val pendingRequests: LiveData<List<LeaveRequest>> = _pendingRequests

    fun loadPending() {
        viewModelScope.launch {
            _pendingRequests.value = leaveRequestRepository.getPending()
        }
    }

    fun reviewRequest(id: Int, approved: Boolean) {
        viewModelScope.launch {
            val request = leaveRequestRepository.getById(id)
            if (request != null) {
                val updated = request.copy(status = if (approved) "APPROVED" else "REJECTED")
                leaveRequestRepository.update(updated)
                _state.value = LeaveManageState.Success(if (approved) "已批准" else "已驳回")
                loadPending()
            } else {
                _state.value = LeaveManageState.Error("请假记录未找到")
            }
        }
    }

    fun resetState() { _state.value = LeaveManageState.Idle }
}
