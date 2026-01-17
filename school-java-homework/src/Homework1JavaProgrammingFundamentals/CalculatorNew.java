/**
 * @author kerwinz
 * @changtime 2023/9/14
 * @projectname schoolJavaHomework
 */
package Homework1JavaProgrammingFundamentals;

import java.util.Scanner;

public class CalculatorNew {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("==== 字符版计算器2 ====");
        // 提示用户输入第一个操作数
        System.out.print("请输入第一个操作数: x = ");
        double num1 = scanner.nextDouble();
        // 提示用户输入第二个操作数
        System.out.print("请输入第二个操作数: y = ");
        double num2 = scanner.nextDouble();
        System.out.println("运算结果如下：");
        // 计算加法
        double additionResult = num1 + num2;
        System.out.println(num1 + " + " + num2 + " = " + additionResult);
        // 计算减法
        double subtractionResult = num1 - num2;
        System.out.println(num1 + " - " + num2 + " = " + subtractionResult);
        // 计算乘法
        double multiplicationResult = num1 * num2;
        System.out.println(num1 + " * " + num2 + " = " + multiplicationResult);
        // 检查除数是否为零
        if (num2 != 0) {
            // 计算除法
            double divisionResult = num1 / num2;
            System.out.println(num1 + " / " + num2 + " = " + divisionResult);
        } else {
            System.out.println("除数不能为零！");
        }
        scanner.close();
    }
}

