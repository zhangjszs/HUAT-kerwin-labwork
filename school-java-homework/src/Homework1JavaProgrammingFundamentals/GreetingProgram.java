/**
 * @author kerwinz
 * @changtime 2023/9/14
 * @projectname schoolJavaHomework
 */
package Homework1JavaProgrammingFundamentals;

import java.util.Scanner;

public class GreetingProgram {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("请输入您的名字: \n");
        String name = scanner.nextLine();
        scanner.close();
        String greeting = name + "，您好！，欢迎学习java。";
        System.out.println(greeting);
    }
}
