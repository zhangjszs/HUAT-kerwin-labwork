/**
 * @author kerwinz
 * @changtime 2023/9/14
 * @projectname schoolJavaHomework
 */
package Homework1JavaProgrammingFundamentals;

import java.util.Scanner;

public class Calculator {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("==== 字符版计算器1 ====");
        // 提示用户输入第一个操作数
        System.out.print("请输入第一个操作数 (x) =  ");
        double x = scanner.nextDouble();
        // 提示用户输入第二个操作数
        System.out.print("请输入第二个操作数 (y) =  ");
        double y = scanner.nextDouble();
        System.out.println("运算结果如下：");
        // 计算加法
        double additionResult = x + y;
        System.out.println("x + y = " + additionResult);
        // 计算减法
        double subtractionResult = x - y;
        System.out.println("x - y = " + subtractionResult);
        // 计算乘法
        double multiplicationResult = x * y;
        System.out.println("x * y = " + multiplicationResult);
        // 检查除数是否为零
        if (y != 0) {
            // 计算除法
            double divisionResult = x / y;
            System.out.println("x / y = " + divisionResult);
        } else {
            System.out.println("除数不能为零！");
        }
        scanner.close();
    }
}
