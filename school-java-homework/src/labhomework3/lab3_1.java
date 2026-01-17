package labhomework3;

/**
 * @author kerwinz
 * @changtime 2023/10/15
 * @projectname schoolJavaHomework
 */
class Person {
    private String name;
    private String sex;
    private int age;

    public String getName() {
        return name;
    }

    public String getSex() {
        return sex;
    }

    public int getAge() {
        return age;
    }

    public Person(String name, String sex, int age) {
        this.name = name;
        this.sex = sex;
        this.age = age;
    }

    @Override
    public String toString() {
        return "Person: [name=" + name + ", sex=" + sex + ", age=" + age + "]";
    }
}

class Teacher extends Person {
    private String pro;
    private String department;

    public Teacher(String name, String sex, int age, String pro, String department) {
        super(name, sex, age);
        this.pro = pro;
        this.department = department;
    }

    public String getPro() {
        return pro;
    }

    public String getDepartment() {
        return department;
    }

    @Override
    public String toString() {
        return "老师的信息为:" + getName() + "," + getSex() + "," + getAge() + "," + getPro() + "," + getDepartment();
    }
}

class Student extends Person {
    private int no;
    private String enter;
    private String major;

    public Student(String name, String sex, int age, int no, String enter, String major) {
        super(name, sex, age);
        this.no = no;
        this.enter = enter;
        this.major = major;
    }

    public int getNo() {
        return no;
    }

    public String getEnter() {
        return enter;
    }

    public String getMajor() {
        return major;
    }

    @Override
    public String toString() {
        return "学生基本信息为:" + getName() + "," + getSex() + "," + getAge() + "," + getNo() + "," + getEnter() + "," + getMajor();
    }
}

public class lab3_1 {
    public static void main(String[] args) {
        Student studentInfo = new Student("张三", "男", 20, 202102088, "2021", "计算机科学与技术");
        Teacher teacher = new Teacher("齐心", "男", 44, "高级实验师", "计算机工程与软件实验中心");

        System.out.println(studentInfo);
        System.out.println(teacher);

    }
}

