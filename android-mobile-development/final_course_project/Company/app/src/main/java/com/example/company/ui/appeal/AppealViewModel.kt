package com.example.company.ui.appeal

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.Appeal
import com.example.company.data.repository.AppealRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class AppealState {
    object Idle : AppealState()
    data class Success(val message: String) : AppealState()
    data class Error(val message: String) : AppealState()
}

@HiltViewModel
class AppealViewModel @Inject constructor(
    private val appealRepository: AppealRepository
) : ViewModel() {

    private val _state = MutableLiveData<AppealState>(AppealState.Idle)
    val state: LiveData<AppealState> = _state

    private val _appeals = MutableLiveData<List<Appeal>>()
    val appeals: LiveData<List<Appeal>> = _appeals

    fun submitAppeal(userId: Int, date: String, appealTime: String, reason: String) {
        if (date.isBlank() || appealTime.isBlank() || reason.isBlank()) {
            _state.value = AppealState.Error("请填写所有字段")
            return
        }
        viewModelScope.launch {
            val appeal = Appeal(userId = userId, date = date, appealTime = appealTime, reason = reason)
            appealRepository.insert(appeal)
            _state.value = AppealState.Success("申诉提交成功")
            loadMyAppeals(userId)
        }
    }

    fun loadMyAppeals(userId: Int) {
        viewModelScope.launch {
            _appeals.value = appealRepository.getByUserId(userId)
        }
    }

    fun resetState() { _state.value = AppealState.Idle }
}
