package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.LeaveRequest

@Dao
interface LeaveRequestDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(request: LeaveRequest): Long

    @Query("SELECT * FROM LEAVE_REQUEST WHERE user_id = :userId ORDER BY _id DESC")
    suspend fun getByUserId(userId: Int): List<LeaveRequest>

    @Query("SELECT * FROM LEAVE_REQUEST WHERE status = 'PENDING' ORDER BY _id DESC")
    suspend fun getPending(): List<LeaveRequest>

    @Query("SELECT * FROM LEAVE_REQUEST WHERE _id = :id")
    suspend fun getById(id: Int): LeaveRequest?

    @Update
    suspend fun update(request: LeaveRequest)
}
