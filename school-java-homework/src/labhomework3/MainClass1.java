package labhomework3;

/**
 * @author kerwinz
 * @changtime 2023/10/17
 * @projectname schoolJavaHomework
 */

import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;

public class MainClass1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        StudentDAOImpl studentDAOImpl = new StudentDAOImpl();
        //1. 插入学生信息
        Student stu = new Student();
        stu.setSno(scanner.next());
        stu.setSname(scanner.next());
        stu.setSdept(scanner.next());
        studentDAOImpl.insertStudent(stu);
        //2. 显示插入学生信息
        System.out.println("1. 插入学生信息如下：");
        studentDAOImpl.displayAllStudent();
        //3. 更新学生信息
        stu.setSname("李四");
        stu.setSdept("计算机系");
        studentDAOImpl.updateStudent(stu);
        System.out.println("2. 更新后学生信息如下：");
        System.out.println(studentDAOImpl.findStudentBySno(stu.getSno()));
        //4. 删除指定学生信息
        System.out.println("3. 删除当前学号学生信息：" + stu.getSno());
        studentDAOImpl.deleteStudent(stu.getSno());
        System.out.println("学生信息已删除!");
        //2. 显示插入学生信息
        System.out.println("5. 显示所有学生信息：");
        studentDAOImpl.displayAllStudent();

        scanner.close();
    }

    public interface IStudentDAO {
        // 插入学生信息记录
        public void insertStudent(Student stu);
        // 删除指定学号学生信息记录
        public void deleteStudent(String sno);
        // 更新指定学生信息记录
        public void updateStudent(Student stu);
        // 按学号查询指定学生信息记录
        public Student findStudentBySno(String sno);
        // 显示指定学号学生信息记录
        public void displayStudentBySno(String sno);
        // 显示所有学生信息记录
        public void displayAllStudent();

    }

    public static class StudentDAOImpl implements IStudentDAO {
        static List<Student> Students = new ArrayList<Student>();

        @Override
        public void insertStudent(Student stu) {
            // TODO Auto-generated method stub
            Students.add(stu);

        }

        @Override
        public void deleteStudent(String sno) {
            // TODO Auto-generated method stub
            Student stu = findStudentBySno(sno);
            Students.remove(stu);
        }

        @Override
        public void updateStudent(Student stu) {
            // TODO Auto-generated method stub
            Students.remove(stu);
            Students.add(stu);
        }

        @Override
        public Student findStudentBySno(String sno) {
            // TODO Auto-generated method stub
            for (Student stu : Students) {
                if (stu.getSno().equals(sno)) {
                    System.out.println("学号:" + stu.getSno() + "姓名:" + stu.getSname() + "系部:" + stu.getSdept());
                    return stu;
                }
            }
            return null;
        }

        @Override
        public void displayStudentBySno(String sno) {
            // TODO Auto-generated method stub
            Student stu = findStudentBySno(sno);
            //System.out.println("学号:" + stu.getSno() + "姓名:" + stu.getSname()	+"系部:" +stu.getSdept());
        }

        @Override
        public void displayAllStudent() {
            // TODO Auto-generated method stub
            for (Student stu : Students) {
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

        public String toString() {
            return "学号: " + sno + ", 姓名: " + sname + ", 系部: " + sdept;
        }
    }
}
