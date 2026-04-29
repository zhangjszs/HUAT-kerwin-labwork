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
        JTextField displayField = new JTextField("0");
        displayField.setEditable(false);
        displayField.setHorizontalAlignment(SwingConstants.RIGHT);

        frame.setLayout(new BorderLayout());
        panel.setLayout(new GridLayout(4, 3, 5, 5));
        frame.setBounds(400, 300, 600, 500);

        JButton[] numberButtons = new JButton[10];
        for (int i = 0; i < numberButtons.length; i++) {
            numberButtons[i] = new JButton(String.valueOf(i));
            panel.add(numberButtons[i]);
            numberButtons[i].addActionListener(new NumberButtonActionListener(numberButtons[i], displayField));
        }

        addButton.addActionListener(e -> {
            String content = displayField.getText();
            if (!content.endsWith("+")) {
                displayField.setText(content + "+");
            }
        });

        panel.add(addButton);

        equalButton.addActionListener(e -> {
            String str = displayField.getText();
            String[] numbers = str.split("\\+");
            int result = 0;
            for (String number : numbers) {
                if (!number.isEmpty()) {
                    result += Integer.parseInt(number);
                }
            }
            displayField.setText(str + "=" + result);
        });

        panel.add(equalButton);

        clearButton.addActionListener(e -> displayField.setText("0"));

        frame.add(displayField, BorderLayout.PAGE_START);
        frame.add(panel, BorderLayout.CENTER);
        frame.add(clearButton, BorderLayout.PAGE_END);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(NumberPad::createAndShowGUI);
    }

    private static class NumberButtonActionListener implements ActionListener {
        private JButton button;
        private JTextField displayField;

        public NumberButtonActionListener(JButton button, JTextField displayField) {
            this.button = button;
            this.displayField = displayField;
        }

        @Override
        public void actionPerformed(ActionEvent e) {
            String content = displayField.getText();
            if (content.equals("0") || content.contains("=")) {
                displayField.setText(button.getText());
            } else {
                displayField.setText(content + button.getText());
            }
        }
    }
}
