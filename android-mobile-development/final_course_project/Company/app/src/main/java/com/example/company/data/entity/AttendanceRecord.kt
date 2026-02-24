package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.ForeignKey
import androidx.room.PrimaryKey

@Entity(
    tableName = "ATTENDANCE",
    foreignKeys = [ForeignKey(
        entity = User::class,
        parentColumns = ["_id"],
        childColumns = ["user_id"],
        onDelete = ForeignKey.CASCADE
    )]
)
data class AttendanceRecord(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "user_id") val userId: Int,
    @ColumnInfo(name = "date") val date: String,
    @ColumnInfo(name = "time_in") val timeIn: String = "09:00:00",
    @ColumnInfo(name = "time_out") val timeOut: String = "18:00:00",
    @ColumnInfo(name = "status_in") val statusIn: String = "",
    @ColumnInfo(name = "status_out") val statusOut: String = ""
)
