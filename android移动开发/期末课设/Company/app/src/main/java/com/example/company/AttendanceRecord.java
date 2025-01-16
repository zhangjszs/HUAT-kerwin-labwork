package com.example.company;

public class AttendanceRecord {
    private int id; // 考勤记录ID
    private int userId; // 关联的用户ID
    private String date;
    private String timeIn;
    private String timeOut;
    private String status;

    public AttendanceRecord() {} // 空构造方法，方便Cursor读取数据

    public AttendanceRecord(int id, int userId, String date, String timeIn, String timeOut, String status) {
        this.id = id;
        this.userId = userId;
        this.date = date;
        this.timeIn = timeIn;
        this.timeOut = timeOut;
        this.status = status;
    }

    // Getters and setters
    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getTimeIn() {
        return timeIn;
    }

    public void setTimeIn(String timeIn) {
        this.timeIn = timeIn;
    }

    public String getTimeOut() {
        return timeOut;
    }

    public void setTimeOut(String timeOut) {
        this.timeOut = timeOut;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}