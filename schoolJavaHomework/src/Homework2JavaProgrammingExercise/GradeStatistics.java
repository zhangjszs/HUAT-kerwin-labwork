package Homework2JavaProgrammingExercise;

/**
 * @author kerwinz
 * @changtime 2023/10/10
 * @projectname schoolJavaHomework
 */

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

public class GradeStatistics {
    public static void main(String[] args) {
        System.out.println("==== 成绩统计 ====");
        Scanner scanner = new Scanner(System.in);
        System.out.println("请输入逗号分隔的多个成绩分数（最多一位小数）：");
        String input = scanner.nextLine();
        List<Double> scores = parseScores(input);

        GradeAnalyzer analyzer = new GradeAnalyzer(scores);
        double max = analyzer.getMax();
        double min = analyzer.getMin();
        double average = analyzer.getAverage();

        System.out.println("最高分：" + String.format("%.1f", max));
        System.out.println("最低分：" + String.format("%.1f", min));
        System.out.println("平均分：" + String.format("%.2f", average));
    }

    private static List<Double> parseScores(String input) {
        String[] scoreStrings = input.split(",");
        List<Double> scores = new ArrayList<>();

        for (String scoreString : scoreStrings) {
            try {
                double score = Double.parseDouble(scoreString);
                scores.add(score);
            } catch (NumberFormatException e) {
                // 处理无效输入，如非数字字符串
                System.err.println("无效的输入: " + scoreString);
            }
        }

        return scores;
    }
}

class GradeAnalyzer {
    private List<Double> scores;

    public GradeAnalyzer(List<Double> scores) {
        this.scores = scores;
    }

    public double getMax() {
        return Collections.max(scores);
    }

    public double getMin() {
        return Collections.min(scores);
    }

    public double getAverage() {
        double sum = 0;
        for (Double score : scores) {
            sum += score;
        }
        return sum / scores.size();
    }
}

