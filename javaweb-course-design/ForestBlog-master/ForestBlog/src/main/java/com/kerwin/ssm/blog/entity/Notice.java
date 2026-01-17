package com.kerwin.ssm.blog.entity;

import lombok.Data;

import java.io.Serializable;
import java.util.Date;

/**
 * @author kerwin
 */
@Data
public class Notice implements Serializable{

    private static final long serialVersionUID = -4901560494243593100L;
    private Integer noticeId;

    private String noticeTitle;

    private String noticeContent;

    private Date noticeCreateTime;

    private Date noticeUpdateTime;

    private Integer noticeStatus;

    private Integer noticeOrder;
}