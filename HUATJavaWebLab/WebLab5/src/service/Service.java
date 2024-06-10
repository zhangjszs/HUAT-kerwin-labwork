package service;

import user.User;

import java.util.LinkedList;

public interface Service {
    public LinkedList<User> s_get_show();
    public LinkedList<User> s_search(String name);
    public void del(String num);
    public void adds(String num,String name,String sex);
    public void update(String num,String name,String sex,String passwords);
}
