package com.kerwin.ssm.blog.controller.home;

import com.kerwin.ssm.blog.entity.Article;
import com.kerwin.ssm.blog.entity.Notice;
import com.kerwin.ssm.blog.service.ArticleService;
import com.kerwin.ssm.blog.service.NoticeService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;

import java.util.List;

/**
 * @author kerwin
 */
@Controller
public class NoticeController {

    @Autowired
    private NoticeService noticeService;

    @Autowired
    private ArticleService articleService;

    /**
     * 公告详情页显示
     *
     * @param noticeId
     * @return
     */
    @RequestMapping(value = "/notice/{noticeId}")
    public String NoticeDetailView(@PathVariable("noticeId") Integer noticeId,
                                         Model model) {
        //公告内容和信息显示
        Notice notice  = noticeService.getNoticeById(noticeId);
        model.addAttribute("notice",notice);

        //侧边栏显示
        //获得热评文章
        List<Article> mostCommentArticleList = articleService.listArticleByCommentCount(8);
        model.addAttribute("mostCommentArticleList", mostCommentArticleList);
        return "Home/Page/noticeDetail";
    }
}
