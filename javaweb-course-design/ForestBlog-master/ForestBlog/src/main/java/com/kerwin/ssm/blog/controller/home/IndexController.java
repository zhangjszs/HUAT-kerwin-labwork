package com.kerwin.ssm.blog.controller.home;

import com.github.pagehelper.PageInfo;
import com.kerwin.ssm.blog.entity.*;
import com.kerwin.ssm.blog.enums.ArticleStatus;
import com.kerwin.ssm.blog.enums.NoticeStatus;
import com.kerwin.ssm.blog.service.*;
import com.kerwin.ssm.blog.entity.Link;

import com.kerwin.ssm.blog.enums.LinkStatus;

import com.kerwin.ssm.blog.entity.*;
import com.kerwin.ssm.blog.service.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.List;

/**
 * 用户的controller
 *
 * @author kerwin
 * @date 2024/4/24
 */
@Controller
public class IndexController {

    @Autowired
    private ArticleService articleService;

    @Autowired
    private LinkService linkService;

    @Autowired
    private NoticeService noticeService;

    @Autowired
    private TagService tagService;

    @Autowired
    private CommentService commentService;

    /**
     * 首页控制器
     *
     * @param pageIndex 页码，默认1
     * @param pageSize 每页大小，默认10
     * @param model Spring MVC模型
     * @return 首页视图
     */
    @RequestMapping(value = {"/", "/article"})
    public String index(@RequestParam(required = false, defaultValue = "1") Integer pageIndex,
                        @RequestParam(required = false, defaultValue = "10") Integer pageSize, Model model) {
        try {
            HashMap<String, Object> criteria = new HashMap<>(1);
            criteria.put("status", ArticleStatus.PUBLISH.getValue());
            //文章列表
            PageInfo<Article> articleList = articleService.pageArticle(pageIndex, pageSize, criteria);
            model.addAttribute("pageInfo", articleList);

            //公告
            List<Notice> noticeList = noticeService.listNotice(NoticeStatus.NORMAL.getValue());
            model.addAttribute("noticeList", noticeList);
            //友情链接
            List<Link> linkList = linkService.listLink(LinkStatus.NORMAL.getValue());
            model.addAttribute("linkList", linkList);

            //侧边栏显示
            //标签列表显示
            List<Tag> allTagList = tagService.listTag();
            model.addAttribute("allTagList", allTagList);
            //最新评论
            List<Comment> recentCommentList = commentService.listRecentComment(null, 10);
            model.addAttribute("recentCommentList", recentCommentList);
            model.addAttribute("pageUrlPrefix", "/article?pageIndex");
            return "Home/index";
        } catch (Exception e) {
            model.addAttribute("message", "系统错误，请稍后重试");
            return "Home/Error/500";
        }
    }

    /**
     * 搜索控制器
     *
     * @param keywords 搜索关键词
     * @param pageIndex 页码
     * @param pageSize 每页大小
     * @param model Spring MVC模型
     * @return 搜索结果视图
     */
    @RequestMapping(value = "/search")
    public String search(
            @RequestParam("keywords") String keywords,
            @RequestParam(required = false, defaultValue = "1") Integer pageIndex,
            @RequestParam(required = false, defaultValue = "10") Integer pageSize, Model model) {
        try {
            if (keywords == null || keywords.trim().isEmpty()) {
                model.addAttribute("message", "搜索关键词不能为空");
                return "Home/Error/404";
            }

            //文章列表
            HashMap<String, Object> criteria = new HashMap<>(2);
            criteria.put("status", ArticleStatus.PUBLISH.getValue());
            criteria.put("keywords", keywords);
            PageInfo<Article> articlePageInfo = articleService.pageArticle(pageIndex, pageSize, criteria);
            model.addAttribute("pageInfo", articlePageInfo);

            //侧边栏显示
            //标签列表显示
            List<Tag> allTagList = tagService.listTag();
            model.addAttribute("allTagList", allTagList);
            //获得随机文章
            List<Article> randomArticleList = articleService.listRandomArticle(8);
            model.addAttribute("randomArticleList", randomArticleList);
            //获得热评文章
            List<Article> mostCommentArticleList = articleService.listArticleByCommentCount(8);
            model.addAttribute("mostCommentArticleList", mostCommentArticleList);
            //最新评论
            List<Comment> recentCommentList = commentService.listRecentComment(null, 10);
            model.addAttribute("recentCommentList", recentCommentList);
            model.addAttribute("pageUrlPrefix", "/search?keywords=" + keywords + "&pageIndex");
            return "Home/Page/search";
        } catch (Exception e) {
            model.addAttribute("message", "搜索失败，请稍后重试");
            return "Home/Error/500";
        }
    }

    @RequestMapping("/404")
    public String NotFound(@RequestParam(required = false) String message, Model model) {
        model.addAttribute("message", message);
        return "Home/Error/404";
    }


    @RequestMapping("/403")
    public String Page403(@RequestParam(required = false) String message, Model model) {
        model.addAttribute("message", message);
        return "Home/Error/403";
    }

    @RequestMapping("/500")
    public String ServerError(@RequestParam(required = false) String message, Model model) {
        model.addAttribute("message", message);
        return "Home/Error/500";
    }


}




