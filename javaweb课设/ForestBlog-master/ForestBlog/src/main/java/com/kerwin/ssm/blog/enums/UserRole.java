package com.kerwin.ssm.blog.enums;

/**
 * 角色枚举
 *
 * @author kerwin
 * @date 2024/5/24
 */

public enum UserRole {

    ADMIN("admin", "管理员"),

    USER("user", "用户");

    private String value;

    private String message;

    UserRole(String value, String message) {
        this.value = value;
        this.message = message;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }
}
