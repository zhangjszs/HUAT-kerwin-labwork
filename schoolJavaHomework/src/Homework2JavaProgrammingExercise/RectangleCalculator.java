package Homework2JavaProgrammingExercise;

/*
  @author kerwinz
 * @changtime 2023/10/10
 * @projectname schoolJavaHomework
 */

import java.util.Scanner;

public class RectangleCalculator {
    private double length;
    private double width;

    public RectangleCalculator(double length, double width) {
        this.length = length;
        this.width = width;
    }

    public double getLength() {
        return length;
    }

    public double getWidth() {
        return width;
    }

    public double getArea() {
        return length * width;
    }

    public double getperimeter() {
        return 2 * (length + width);
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("==== 计算矩形面积周长 ====");
        System.out.print("请输入矩形的长度：");
        double length = scanner.nextDouble();
        System.out.print("请输入矩形的宽度：");
        double width = scanner.nextDouble();
        RectangleCalculator rectangleCalculator = new RectangleCalculator(length, width);
        System.out.println("长" + String.format("%.2f", rectangleCalculator.getLength()) + "，宽" + String.format("%.2f", rectangleCalculator.getWidth()) + "的矩形面积为：" + String.format("%.2f", rectangleCalculator.getArea()));
        System.out.println("长" + String.format("%.2f", rectangleCalculator.getLength()) + "，宽" + String.format("%.2f", rectangleCalculator.getWidth()) + "的矩形周长为：" + String.format("%.2f", rectangleCalculator.getperimeter()));
    }
}
