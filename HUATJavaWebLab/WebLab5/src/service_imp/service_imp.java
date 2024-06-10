package service_imp;

import dateimp.DateImp;
import service.Service;
import user.User;

import java.util.LinkedList;

public class service_imp implements Service {
    @Override
    public LinkedList<User> s_get_show() {

        return new DateImp().get_show();
    }

    @Override
    public LinkedList<User> s_search(String name) {
        return new DateImp().search(name);
    }

    @Override
    public void del(String num) {
          new DateImp().del(num);
    }

    @Override
    public void adds(String num, String name, String sex) {
        new DateImp().add(num,name,sex);
    }

    @Override
    public void update(String num, String name, String sex, String passwords) {
        new DateImp().update(num,name,sex,passwords);
    }
}
