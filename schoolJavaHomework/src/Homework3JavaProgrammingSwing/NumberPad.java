package Homework3JavaProgrammingSwing;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class NumberPad {

    private static void createAndShowGUI() {
        JFrame frame = new JFrame("整数加法器");
        JPanel panel = new JPanel();
        JButton addButton = new JButton("加");
        JButton equalButton = new JButton("等于");
        JButton clearButton = new JButton("清除");
        frame.setLayout(new BorderLayout());
        panel.setLayout(new GridLayout(4, 3, 5, 5));
        frame.setBounds(400, 300, 600, 500);
        // 创建数字按钮
        JButton[] numberButtons = new JButton[10];
        for (int i = 0; i < numberButtons.length; i++) {
            numberButtons[i] = new JButton(String.valueOf(i));
            panel.add(numberButtons[i]);
            numberButtons[i].addActionListener(new NumberButtonActionListener(numberButtons[i], clearButton));
        }
        addButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String content = clearButton.getText();
                content += "+";
                clearButton.setText(content);
            }
        });

        panel.add(addButton);

        equalButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String str = clearButton.getText();
                String[] numbers = str.split("\\+");
                int result = 0;
                for (String number : numbers) {
                    result += Integer.parseInt(number);
                }
                str += "=" + result;
                clearButton.setText(str);
            }
        });

        panel.add(equalButton);

        clearButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                clearButton.setText("清除");
            }
        });

        frame.add(clearButton, BorderLayout.PAGE_START);
        frame.add(panel, BorderLayout.CENTER);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(NumberPad::createAndShowGUI);
    }

    private static class NumberButtonActionListener implements ActionListener {
        private JButton button;
        private JButton clearButton;

        public NumberButtonActionListener(JButton button, JButton clearButton) {
            this.button = button;
            this.clearButton = clearButton;
        }

        @Override
        public void actionPerformed(ActionEvent e) {
            String content = clearButton.getText();
            if (content.equals("清除")) {
                content = "" + button.getText();
            } else {
                content += button.getText();
            }
            clearButton.setText(content);
        }
    }
}