
/**
 * @author kerwinz
 * @changtime 2023/9/14
 * @projectname schoolJavaHomework
 */
package Homework1JavaProgrammingFundamentals;

import java.util.Scanner;

public class GreetingProgram {
    public static void main(String[] args) {
        // 创建一个Scanner对象，用于获取用户输入
        Scanner scanner = new Scanner(System.in);

        // 提示用户输入名字
        System.out.print("请输入您的名字: \n");

        // 读取用户输入的名字
        String name = scanner.nextLine();

        // 关闭Scanner
        scanner.close();

        // 构建欢迎消息
        String greeting = name + "，您好！，欢迎学习java。";

        // 输出欢迎消息
        System.out.println(greeting);
    }
}
