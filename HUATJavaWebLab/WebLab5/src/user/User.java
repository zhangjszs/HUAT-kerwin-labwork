package user;

public class User {
    String num;
    String name;
    String sex;
    public User(String num,String name,String sex){
        this.num=num;
        this.name=name;
        this.sex=sex;
    }
    public String getNum() {
        return num;
    }

    public void setNum(String num) {
        this.num = num;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

}
