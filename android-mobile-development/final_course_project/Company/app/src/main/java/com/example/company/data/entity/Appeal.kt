package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "APPEAL")
data class Appeal(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "user_id") val userId: Int,
    @ColumnInfo(name = "date") val date: String,
    @ColumnInfo(name = "appeal_time") val appealTime: String,
    @ColumnInfo(name = "reason") val reason: String,
    @ColumnInfo(name = "status") val status: String = "PENDING"
)
