<%--
  Created by IntelliJ IDEA.
  User: 17408
  Date: 2024/4/13
  Time: 20:52
  To change this template use File | Settings | File Templates.
--%>
<%@page import="java.awt.*" %>
<%@page import="java.util.Random" %>
<%@page import="javax.imageio.ImageIO" %>
<%@ page import="java.awt.image.BufferedImage" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
<head>
    <title>Title</title>
</head>
<body>
<%
    response.setHeader("Pragma","N-cache");
    response.setHeader("Cache-Control","no-cache");
    response.addDateHeader("Expires",0);
    int width=70;
    int height=20;
    BufferedImage image = new BufferedImage(width,height,BufferedImage.TYPE_INT_BGR);
    Graphics g = image.getGraphics();
    g.setColor(new Color(200,200,200));
    g.fillRect(0,0,width,height);
    Random rnd = new Random();
    int randNum = rnd.nextInt(8999)+1000;
    String ranStr = String.valueOf(randNum);
    session.setAttribute("ranStr",ranStr);
    g.setColor(new Color(rnd.nextInt(255),rnd.nextInt(255),rnd.nextInt(255)));
    g.setFont(new Font("",Font.PLAIN,20));
    g.drawString(ranStr,8,17);
    for(int i=0;i<100;i++){
        g.setColor(new Color(rnd.nextInt(255),rnd.nextInt(255),rnd.nextInt(255)));
        int x=rnd.nextInt(width);
        int y=rnd.nextInt(height);
        g.drawOval(x,y,1,1);
    }
    ImageIO.write(image,"JPEG",response.getOutputStream());
    out.clear();
    out = pageContext.pushBody();
%>
</body>
</html>
