package labhomework3;

/**
 * @author kerwinz
 * @changtime 2023/10/17
 * @projectname schoolJavaHomework
 */

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class MainClass1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        StudentDAOImpl studentDAOImpl = new StudentDAOImpl();

        System.out.println("请依次输入学号、姓名、系部（以空格分隔）：");
        Student stu = new Student();
        stu.setSno(scanner.next());
        stu.setSname(scanner.next());
        stu.setSdept(scanner.next());

        studentDAOImpl.insertStudent(stu);

        System.out.println("1. 插入学生信息如下：");
        studentDAOImpl.displayAllStudent();

        stu.setSname("李四");
        stu.setSdept("计算机系");
        studentDAOImpl.updateStudent(stu);
        System.out.println("2. 更新后学生信息如下：");
        System.out.println(studentDAOImpl.findStudentBySno(stu.getSno()));

        System.out.println("3. 删除当前学号学生信息：" + stu.getSno());
        studentDAOImpl.deleteStudent(stu.getSno());
        System.out.println("学生信息已删除!");

        System.out.println("4. 显示所有学生信息：");
        studentDAOImpl.displayAllStudent();

        scanner.close();
    }

    public interface IStudentDAO {
        void insertStudent(Student stu);
        void deleteStudent(String sno);
        void updateStudent(Student stu);
        Student findStudentBySno(String sno);
        void displayStudentBySno(String sno);
        void displayAllStudent();
    }

    public static class StudentDAOImpl implements IStudentDAO {
        private final List<Student> students = new ArrayList<>();

        @Override
        public void insertStudent(Student stu) {
            students.add(stu);
        }

        @Override
        public void deleteStudent(String sno) {
            Student stu = findStudentBySno(sno);
            if (stu != null) {
                students.remove(stu);
            } else {
                System.out.println("学生信息不存在，无法删除");
            }
        }

        @Override
        public void updateStudent(Student stu) {
            Student existing = findStudentBySno(stu.getSno());
            if (existing != null) {
                existing.setSname(stu.getSname());
                existing.setSdept(stu.getSdept());
            } else {
                System.out.println("学生信息不存在，无法更新");
            }
        }

        @Override
        public Student findStudentBySno(String sno) {
            for (Student stu : students) {
                if (stu.getSno().equals(sno)) {
                    return stu;
                }
            }
            return null;
        }

        @Override
        public void displayStudentBySno(String sno) {
            Student stu = findStudentBySno(sno);
            if (stu != null) {
                System.out.println(stu);
            } else {
                System.out.println("学生信息不存在");
            }
        }

        @Override
        public void displayAllStudent() {
            if (students.isEmpty()) {
                System.out.println("暂无学生记录");
                return;
            }
            for (Student stu : students) {
                displayStudentBySno(stu.getSno());
            }
        }
    }

    public static class Student {
        private String sno;
        private String sname;
        private String sdept;

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

        @Override
        public String toString() {
            return "学号: " + sno + ", 姓名: " + sname + ", 系部: " + sdept;
        }
    }
}
