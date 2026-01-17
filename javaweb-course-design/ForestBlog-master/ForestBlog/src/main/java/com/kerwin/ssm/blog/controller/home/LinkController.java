package com.kerwin.ssm.blog.controller.home;

import com.kerwin.ssm.blog.entity.Article;
import com.kerwin.ssm.blog.entity.Link;
import com.kerwin.ssm.blog.service.ArticleService;
import com.kerwin.ssm.blog.service.LinkService;
import com.kerwin.ssm.blog.enums.LinkStatus;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.Date;
import java.util.List;

/**
 *
 * @author kerwin
 * @date 2024/5/5
 */
@Controller
public class LinkController {
	@Autowired
	private LinkService linkService;

	@Autowired
	private ArticleService articleService;

	@RequestMapping("/applyLink")
	public String applyLinkView(Model model)  {
		//侧边栏显示
		//获得热评文章
		List<Article> mostCommentArticleList = articleService.listArticleByCommentCount(8);
		model.addAttribute("mostCommentArticleList", mostCommentArticleList);
		return "Home/Page/applyLink";
	}


	@RequestMapping(value = "/applyLinkSubmit",method = {RequestMethod.POST}, produces = {"text/plain;charset=UTF-8"})
	@ResponseBody
	public void applyLinkSubmit(Link link)  {
		link.setLinkStatus(LinkStatus.HIDDEN.getValue());
		link.setLinkCreateTime(new Date());
		link.setLinkUpdateTime(new Date());
		linkService.insertLink(link);
	}
}
