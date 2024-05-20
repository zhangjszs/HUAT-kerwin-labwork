package org.example.weblab4;

import java.sql.ResultSet;

public interface StudentDao {
//    public Integer addUser(User user);
//    public Integer delById(Integer id);
//    public User findById(Integer id);
//    public Integer updateUser(User user);
//    public List<User> findAll();
    ResultSet queryStudent(String stuno, String password);
}


