package com.kerwin.ssm.blog.enums;

import lombok.Data;

/**
 * @author kerwin
 * @date 2024/5/24
 */

public enum CategoryStatus {

    NORMAL(1, "正常"),
    HIDDEN(0, "隐藏");

    private Integer value;

    private String message;

    CategoryStatus(Integer value, String message) {
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
