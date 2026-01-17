package com.kerwin.ssm.blog.dto;

import lombok.Data;

import java.util.List;

/**
 * @author kerwin
 * @date 2024/4/24
 */
@Data
public class ArticleParam {

    private Integer articleId;

    private String articleTitle;

    private String articleContent;

    private Integer articleParentCategoryId;

    private Integer articleChildCategoryId;

    private Integer articleOrder;

    private Integer articleStatus;

    private String articleThumbnail;

    private List<Integer> articleTagIds;

}
