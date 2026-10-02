package dateinterface;
import java.sql.*;
import java.util.LinkedList;

import user.User;

public interface DateInterface {
    public LinkedList<User> get_show();
    public LinkedList<User> search(String name);
    public void del(String num);
    public void add(String num,String name,String sex);
    public void update(String num,String name,String sex,String passwords);

}
