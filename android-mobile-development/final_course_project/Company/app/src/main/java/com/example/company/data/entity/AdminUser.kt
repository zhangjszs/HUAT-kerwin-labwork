package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "ADMINUSER")
data class AdminUser(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "admin_name") val adminName: String,
    @ColumnInfo(name = "admin_password") val adminPassword: String
)
