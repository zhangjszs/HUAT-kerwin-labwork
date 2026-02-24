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

sealed class AppealManageState {
    object Idle : AppealManageState()
    data class Success(val message: String) : AppealManageState()
    data class Error(val message: String) : AppealManageState()
}

@HiltViewModel
class AppealManageViewModel @Inject constructor(
    private val appealRepository: AppealRepository
) : ViewModel() {

    private val _state = MutableLiveData<AppealManageState>(AppealManageState.Idle)
    val state: LiveData<AppealManageState> = _state

    private val _pendingAppeals = MutableLiveData<List<Appeal>>()
    val pendingAppeals: LiveData<List<Appeal>> = _pendingAppeals

    fun loadPending() {
        viewModelScope.launch {
            _pendingAppeals.value = appealRepository.getPending()
        }
    }

    fun reviewAppeal(id: Int, approved: Boolean) {
        viewModelScope.launch {
            val appeal = appealRepository.getById(id)
            if (appeal != null) {
                val updated = appeal.copy(status = if (approved) "APPROVED" else "REJECTED")
                appealRepository.update(updated)
                _state.value = AppealManageState.Success(if (approved) "已批准" else "已驳回")
                loadPending()
            } else {
                _state.value = AppealManageState.Error("申诉记录未找到")
            }
        }
    }

    fun resetState() { _state.value = AppealManageState.Idle }
}
