package com.example.company;

public class AdminUser {
    private int id; // 管理员ID
    private String adminName;
    private String adminPassword;

    public AdminUser(int id, String adminName, String adminPassword) {
        this.id = id;
        this.adminName = adminName;
        this.adminPassword = adminPassword;
    }

    public AdminUser(String adminName, String adminPassword) {
        this.adminName = adminName;
        this.adminPassword = adminPassword;
    }
    // Getters and setters
    public int getId() { return id; }
    public void setId(int id) { this.id = id; }

    public String getAdminName() {
        return adminName;
    }

    public void setAdminName(String adminName) {
        this.adminName = adminName;
    }

    public String getAdminPassword() {
        return adminPassword;
    }

    public void setAdminPassword(String adminPassword) {
        this.adminPassword = adminPassword;
    }
}