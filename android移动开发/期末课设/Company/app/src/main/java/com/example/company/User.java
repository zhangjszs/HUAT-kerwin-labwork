package com.example.company;

import androidx.annotation.NonNull;

/**
 * Created by kerwinzhang 2024/12/24
 */

public class User {
    private int id; // 用户ID，数据库自增
    private String name;
    private String password;
    private String sex;
    private String tel;
    private String position;
    private String time;

    // 构造方法
    public User(int id, String name, String password, String sex, String tel, String position, String time) {
        this.id = id;
        this.name = name;
        this.password = password;
        this.sex = sex;
        this.tel = tel;
        this.position = position;
        this.time = time;
    }

    public User(String name, String password) { // 简化构造方法，用于登录验证等
        this.name = name;
        this.password = password;
    }

    // Getters and setters
    public int getId() { return id; }
    public void setId(int id) { this.id = id; }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

    public String getTel() {
        return tel;
    }

    public void setTel(String tel) {
        this.tel = tel;
    }

    public String getPosition() {
        return position;
    }

    public void setPosition(String position) {
        this.position = position;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }
}

