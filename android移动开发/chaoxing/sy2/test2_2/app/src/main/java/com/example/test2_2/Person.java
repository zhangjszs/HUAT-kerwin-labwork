package com.example.test2_2;

public class Person {
    public int _id;
    public String name;
    public int age;
    public String info;

    public Person() {}

    public Person(String name, int age, String info) {
        this.name = name;
        this.age = age;
        this.info = info;
    }

    public Person(int _id, String name, int age, String info) {
        this._id = _id;
        this.name = name;
        this.age = age;
        this.info = info;
    }

    public int get_id() {
        return _id;
    }

    public void set_id(int _id) {
        this._id = _id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public String getInfo() {
        return info;
    }

    public void setInfo(String info) {
        this.info = info;
    }
}



