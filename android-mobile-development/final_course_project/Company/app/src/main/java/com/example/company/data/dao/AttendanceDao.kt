package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.AttendanceRecord

@Dao
interface AttendanceDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(record: AttendanceRecord): Long

    @Query("SELECT * FROM ATTENDANCE WHERE user_id = :userId AND date = :date LIMIT 1")
    suspend fun getByUserAndDate(userId: Int, date: String): AttendanceRecord?

    @Update
    suspend fun update(record: AttendanceRecord): Int

    @Query("SELECT * FROM ATTENDANCE WHERE user_id = :userId ORDER BY date DESC")
    suspend fun getByUserId(userId: Int): List<AttendanceRecord>

    @Query("DELETE FROM ATTENDANCE WHERE user_id = :userId")
    suspend fun deleteByUserId(userId: Int)

    @Query("SELECT * FROM ATTENDANCE WHERE user_id = :userId AND date IN (:dates)")
    suspend fun getByUserIdAndDateRange(userId: Int, dates: List<String>): List<AttendanceRecord>
}
