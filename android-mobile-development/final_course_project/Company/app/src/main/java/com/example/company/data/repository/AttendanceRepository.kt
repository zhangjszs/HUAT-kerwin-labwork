package com.example.company.data.repository

import com.example.company.data.dao.AttendanceDao
import com.example.company.data.entity.AttendanceRecord
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class AttendanceRepository @Inject constructor(private val attendanceDao: AttendanceDao) {

    suspend fun signIn(record: AttendanceRecord): Long =
        attendanceDao.insert(record)

    suspend fun signOut(record: AttendanceRecord): Int =
        attendanceDao.update(record)

    suspend fun getTodayRecord(userId: Int, date: String): AttendanceRecord? =
        attendanceDao.getByUserAndDate(userId, date)

    suspend fun getByUserId(userId: Int): List<AttendanceRecord> =
        attendanceDao.getByUserId(userId)

    suspend fun deleteByUserId(userId: Int) =
        attendanceDao.deleteByUserId(userId)

    suspend fun getByUserIdAndDateRange(userId: Int, dates: List<String>): List<AttendanceRecord> =
        attendanceDao.getByUserIdAndDateRange(userId, dates)
}
