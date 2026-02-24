package com.example.company.data.entity

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "NEWS")
data class News(
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "_id") val id: Int = 0,
    @ColumnInfo(name = "TITLE") val title: String,
    @ColumnInfo(name = "IMAGE_RESOURCE_ID") val imageResourceId: Int,
    @ColumnInfo(name = "CONTENT") val content: String
)
