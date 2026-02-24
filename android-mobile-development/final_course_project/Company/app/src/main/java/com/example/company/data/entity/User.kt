package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "USER")
data class User(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "name") val name: String,
    @ColumnInfo(name = "password") val password: String,
    @ColumnInfo(name = "sex") val sex: String = "未指定",
    @ColumnInfo(name = "tel") val tel: String = "未指定",
    @ColumnInfo(name = "position") val position: String = "未指定",
    @ColumnInfo(name = "TIME") val time: String = "1970-01-01"
)
