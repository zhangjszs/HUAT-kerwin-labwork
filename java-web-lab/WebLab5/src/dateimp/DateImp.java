package dateimp;

import dateinterface.DateInterface;
import user.User;
import utils.databasebean;

import java.io.IOException;
import java.sql.*;
import java.util.LinkedList;

public class DateImp implements DateInterface {
    Connection con = null;
    PreparedStatement psmt = null;
    ResultSet rs = null;

    @Override
    public LinkedList<User> get_show() {
        con = databasebean.getconnection();
        User student = null;
        LinkedList<User> students = new LinkedList<>();
        if (con != null) {
            String sql = "select num,name,sex from users";
            try {
                psmt = con.prepareStatement(sql);
                rs = psmt.executeQuery();
                while (rs.next()) {
                    student = new User(rs.getString("num"), rs.getString("name"), rs.getString("sex"));
                    students.add(student);
                }
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
        }
        return students;
    }

    @Override
    public LinkedList<User> search(String name)  {
        con = databasebean.getconnection();
        String sql = "select * from users where name like ?";
        User student = null;
        LinkedList<User> students = new LinkedList<>();
        try {
            psmt = con.prepareStatement(sql);
            psmt.setString(1,"%"+name+"%");
            rs = psmt.executeQuery();
            while(rs.next()){
                student = new User(rs.getString("num"), rs.getString("name"), rs.getString("sex"));
                students.add(student);
            }

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }

        databasebean.close(con,psmt,rs);
        return students;
    }

    @Override
    public void del(String num) {
        con = databasebean.getconnection();
        String sql = "delete from users where num= ?";
        try {
            psmt = con.prepareStatement(sql);
            psmt.setString(1,num);
            psmt.executeUpdate();

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        databasebean.close(con,psmt,rs);
    }

    @Override
    public void add(String num,String name,String sex) {
        con = databasebean.getconnection();
        String sql = "select num from users where num =?";
        boolean flag = true;
        try {
            psmt = con.prepareStatement(sql);
            psmt.setString(1,num);
            rs = psmt.executeQuery();
            while(rs.next()){
                if(rs.getString("num").equals(num)){
                    flag=false;
                    break;
                }
            }
            if(flag){
                psmt = con.prepareStatement("insert into users  values (?,?,?,?)");
                psmt.setString(1,num);
                psmt.setString(2,name);
                psmt.setString(3,sex);
                psmt.setString(4,"1234");
                psmt.executeUpdate();
            }

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        databasebean.close(con,psmt,rs);
    }

    @Override
    public void update(String num, String name, String sex, String passwords) {
        con = databasebean.getconnection();
        String sql = "update users set name=?,sex=?,password=? where num=?";
        try {
            psmt = con.prepareStatement(sql);
            psmt.setString(1,name);
            psmt.setString(2,sex);
            psmt.setString(3,passwords);
            psmt.setString(4,num);
            psmt.executeUpdate();

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        databasebean.close(con,psmt,rs);

    }
}
