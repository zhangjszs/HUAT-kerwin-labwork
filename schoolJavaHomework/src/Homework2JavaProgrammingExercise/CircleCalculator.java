package Homework2JavaProgrammingExercise;

/*
  @author kerwinz
 * @changtime 2023/10/10
 * @projectname schoolJavaHomework
 */

import java.util.Scanner;

public class CircleCalculator {
    private double radius;

    public CircleCalculator(double radius) {
        this.radius = radius;
    }

    public double getRadius() {
        return radius;
    }

    public double getArea() {
        return Math.PI * radius * radius;
    }

    public double getPerimeter() {
        return 2 * Math.PI * radius;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("==== 计算园面积周长 ====");
        System.out.print("请输入圆的半径：");
        double radius = scanner.nextDouble();
        CircleCalculator circleCalculator = new CircleCalculator(radius);
        System.out.println("半径为" + String.format("%.2f", circleCalculator.getRadius()) + "的圆的面积为：" + String.format("%.2f", circleCalculator.getArea()));
        System.out.println("半径为" + String.format("%.2f", circleCalculator.getRadius()) + "的圆的周长为：" + String.format("%.2f", circleCalculator.getPerimeter()));

    }

}
