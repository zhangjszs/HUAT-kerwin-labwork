package com.example.company.data.repository

import com.example.company.data.dao.LeaveRequestDao
import com.example.company.data.entity.LeaveRequest
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class LeaveRequestRepository @Inject constructor(private val leaveRequestDao: LeaveRequestDao) {

    suspend fun insert(request: LeaveRequest): Long = leaveRequestDao.insert(request)

    suspend fun getByUserId(userId: Int): List<LeaveRequest> = leaveRequestDao.getByUserId(userId)

    suspend fun getPending(): List<LeaveRequest> = leaveRequestDao.getPending()

    suspend fun getById(id: Int): LeaveRequest? = leaveRequestDao.getById(id)

    suspend fun update(request: LeaveRequest) = leaveRequestDao.update(request)
}
