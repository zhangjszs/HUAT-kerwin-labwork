package com.kerwin.ssm.blog.enums;

/**
 * @author kerwin
 * @date 2024/5/24
 */

public enum Role {

    OWNER(1, "博主"),
    VISITOR(0, "其他用户");

    private Integer value;

    private String message;

    Role(Integer value, String message) {
        this.value = value;
        this.message = message;
    }

    public Integer getValue() {
        return value;
    }

    public void setValue(Integer value) {
        this.value = value;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }
}
