package labhomework3;

/**
 * @author kerwinz
 * @changtime 2023/10/15
 * @projectname schoolJavaHomework
 */

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class StudentInfo {
    private String sno; // 学号
    private String sname; // 姓名
    private String sdept; // 系部

    public String getSno() {
        return sno;
    }

    public void setSno(String sno) {
        this.sno = sno;
    }

    public String getSname() {
        return sname;
    }

    public void setSname(String sname) {
        this.sname = sname;
    }

    public String getSdept() {
        return sdept;
    }

    public void setSdept(String sdept) {
        this.sdept = sdept;
    }
}

interface IStudentDAO {
    void insertStudent(StudentInfo stu);

    void deleteStudent(String sno);

    void updateStudent(StudentInfo stu);

    StudentInfo findStudentBySno(String sno);

    void displayStudentBySno(String sno);

    void displayAllStudent();
}

class StudentDAOImpl implements IStudentDAO {
    static List<StudentInfo> studentInfos = new ArrayList<>();

    @Override
    public void insertStudent(StudentInfo stu) {
        studentInfos.add(stu);
        System.out.println("学生信息已插入");
    }

    @Override
    public void deleteStudent(String sno) {
        StudentInfo stu = findStudentBySno(sno);
        if (stu != null) {
            studentInfos.remove(stu);
            System.out.println("学生信息已删除");
        } else {
            System.out.println("学生信息不存在");
        }
    }

    @Override
    public void updateStudent(StudentInfo stu) {
        // 更新指定学生信息记录
        studentInfos.remove(stu);
        studentInfos.add(stu);
        System.out.println("学生信息已更新");
    }

    @Override
    public StudentInfo findStudentBySno(String sno) {
        for (StudentInfo stu : studentInfos) {
            if (stu.getSno().equals(sno)) {
                System.out.println("学生信息已找到");
                return stu;
            }
        }
        return null;
    }

    @Override
    public void displayStudentBySno(String sno) {
        StudentInfo stu = findStudentBySno(sno);
        if (stu != null) {
            System.out.println("学号: " + stu.getSno() + "\t姓名: " + stu.getSname() + "\t系部: " + stu.getSdept());
        } else {
            System.out.println("学生信息不存在");
        }
    }

    @Override
    public void displayAllStudent() {
        if (!studentInfos.isEmpty()) {
            for (StudentInfo stu : studentInfos) {
                int num = studentInfos.indexOf(stu) + 1;
                System.out.println("所有学生共计" + num + "人的信息如下");
                System.out.println("学号: " + stu.getSno() + "\t姓名: " + stu.getSname() + "\t系部: " + stu.getSdept());
            }
        } else {
            System.out.println("数据库中无学生记录!");
        }
    }
}

/**
 * @author MR
 */
public class MainClass {
    private static final int INSERT = 1;
    private static final int DELETE = 2;
    private static final int UPDATE = 3;
    private static final int DISPLAY_ALL = 4;
    private static final int DISPLAY_BY_SNO = 5;
    private static final int EXIT = 0;

    public static void main(String[] args) {
        int menuChoice;
        Scanner scanner = new Scanner(System.in);
        StudentDAOImpl studentDAOImpl = new StudentDAOImpl();

        do {
            menuChoice = menuSelect(scanner);
            switch (menuChoice) {
                case INSERT:
                    StudentInfo newStu = inputStudentInfo(scanner);
                    studentDAOImpl.insertStudent(newStu);
                    break;
                case DELETE:
                    deleteStudent(studentDAOImpl, scanner);
                    break;
                case UPDATE:
                    updateStudent(studentDAOImpl, scanner);
                    break;
                case DISPLAY_ALL:
                    studentDAOImpl.displayAllStudent();
                    break;
                case DISPLAY_BY_SNO:
                    displayStudentBySno(studentDAOImpl, scanner);
                    break;
                case EXIT:
                    System.out.println("程序已退出!");
                    break;
                default:
                    System.out.println("对不起，您输入的功能编号有错!请重新输入!!!");
                    break;
            }
        } while (menuChoice != EXIT);
        scanner.close();
    }

    private static int menuSelect(Scanner scanner) {
        System.out.print("\n");
        System.out.print("\t\t\t  学生信息管理系统功能菜单		\n");
        System.out.print("\t\t\t       作者：KerwinZhang			\n");
        System.out.print("\t\t\t  =======================		\n");
        System.out.print("\t\t\t  1.插入学生信息记录			\n");
        System.out.print("\t\t\t  2.删除学生信息记录			\n");
        System.out.print("\t\t\t  3.更新学生信息记录			\n");
        System.out.print("\t\t\t  4.显示所有学生信息记录		\n");
        System.out.print("\t\t\t  5.按学号查询指定学生信息		\n");
        System.out.print("\t\t\t  0.结束程序				\n");
        System.out.print("\t\t\t  =======================		\n");
        System.out.print("\t\t\t请输入您的选择：		\n");
        return scanner.nextInt();
    }

    private static StudentInfo inputStudentInfo(Scanner scanner) {
        StudentInfo newStu = new StudentInfo();
        System.out.println("请输入学号：");
        newStu.setSno(scanner.next());
        System.out.println("请输入姓名：");
        newStu.setSname(scanner.next());
        System.out.println("请输入系部：");
        newStu.setSdept(scanner.next());
        return newStu;
    }

    private static void deleteStudent(StudentDAOImpl studentDAO, Scanner scanner) {
        System.out.println("请输入要删除学生信息的学号：");
        studentDAO.deleteStudent(scanner.next());
    }

    private static void updateStudent(StudentDAOImpl studentDAO, Scanner scanner) {
        System.out.println("请输入要更新学生信息的学号：");
        String sno = scanner.next();
        StudentInfo updateStu = studentDAO.findStudentBySno(sno);
        if (updateStu != null) {
            System.out.println("请输入新的学生姓名：");
            updateStu.setSname(scanner.next());
            System.out.println("请输入新的学生系部：");
            updateStu.setSdept(scanner.next());
            studentDAO.updateStudent(updateStu);
        } else {
            System.out.println("要更新的学生信息不存在!");
        }
    }

    private static void displayStudentBySno(StudentDAOImpl studentDAO, Scanner scanner) {
        System.out.println("请输入查询学生信息的学号：");
        studentDAO.displayStudentBySno(scanner.next());
    }
}
