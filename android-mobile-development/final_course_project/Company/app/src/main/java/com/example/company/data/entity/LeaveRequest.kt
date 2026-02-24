package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "LEAVE_REQUEST")
data class LeaveRequest(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "user_id") val userId: Int,
    @ColumnInfo(name = "leave_type") val leaveType: String,
    @ColumnInfo(name = "start_date") val startDate: String,
    @ColumnInfo(name = "end_date") val endDate: String,
    @ColumnInfo(name = "reason") val reason: String,
    @ColumnInfo(name = "status") val status: String = "PENDING"
)
