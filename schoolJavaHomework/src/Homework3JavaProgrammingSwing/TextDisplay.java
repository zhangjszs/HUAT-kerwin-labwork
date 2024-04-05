package Homework3JavaProgrammingSwing;

/**
 * @author kerwinz
 * @changtime 2023/10/28
 * @projectname schoolJavaHomework
 */

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class TextDisplay {

    private static void createAndShowGUI() {
        JFrame frame = new JFrame("文本显示器");
        frame.setBackground(Color.GRAY);
        JPanel panel = new JPanel();
        JPanel textPanel = new JPanel();
        JCheckBox boldButton = new JCheckBox("粗体");
        JCheckBox italicButton = new JCheckBox("斜体");
        ButtonGroup fontSizeGroup = new ButtonGroup();
        JRadioButton largeButton = new JRadioButton("大字号");
        JRadioButton mediumButton = new JRadioButton("中字号");
        JRadioButton smallButton = new JRadioButton("小字号");
        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
        Dimension size = frame.getSize();
        int width = 800;
        int height = 600;
        JTextArea textArea = new JTextArea(7, 20);
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        String[] fontNames = ge.getAvailableFontFamilyNames();
        String songTi = null;
        for (String fontName : fontNames) {
            if (fontName.contains("宋体")) {
                songTi = fontName;
                break;
            }
        }
        Font songTiFont = new Font(songTi, Font.PLAIN, 20);
        textArea.setFont(songTiFont);
        frame.setBounds(400, 300, width, height);
        JScrollPane scrollPane = new JScrollPane(textArea);
        frame.setLayout(new BorderLayout());
        panel.setLayout(new GridLayout(5, 1, 5, 5));

        final boolean[] isBold = {false};  // 初始字体样式为非粗体
        boldButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                int style = font.getStyle();  // 获取原字体的样式
                int newStyle;

                if (isBold[0]) {
                    // 切换回正常字体
                    newStyle = style & ~Font.BOLD;  // 将粗体位清零
                } else {
                    // 切换为粗体字体
                    newStyle = style | Font.BOLD;  // 将粗体位置为1
                }

                Font newFont = new Font(font.getFontName(), newStyle, font.getSize());
                textArea.setFont(newFont);
                isBold[0] = !isBold[0];  // 切换字体样式标志
            }
        });

        final boolean[] isItalic = {false};  // 初始字体样式为非斜体

        italicButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                int style = font.getStyle();  // 获取原字体的样式
                int newStyle;

                if (isItalic[0]) {
                    // 切换回正常字体
                    newStyle = style & ~Font.ITALIC;  // 将斜体位清零
                } else {
                    // 切换为斜体字体
                    newStyle = style | Font.ITALIC;  // 将斜体位置为1
                }

                Font newFont = new Font(font.getFontName(), newStyle, font.getSize());
                textArea.setFont(newFont);
                isItalic[0] = !isItalic[0];  // 切换字体样式标志
            }
        });

// 点击增大字体按钮
        largeButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                Font newFont = new Font(font.getFontName(), font.getStyle(), 60);
                textArea.setFont(newFont);
            }
        });
        fontSizeGroup.add(largeButton);

// 点击恢复默认字体按钮
        mediumButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                Font newFont = new Font(font.getFontName(), font.getStyle(), 30);
                textArea.setFont(newFont);
            }
        });
        fontSizeGroup.add(mediumButton);

// 点击减小字体按钮
        smallButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                Font newFont = new Font(font.getFontName(), font.getStyle(), 10);
                textArea.setFont(newFont);
            }
        });
        fontSizeGroup.add(smallButton);

        panel.add(boldButton);
        panel.add(italicButton);
        panel.add(largeButton);
        panel.add(mediumButton);
        panel.add(smallButton);

        textPanel.add(scrollPane);
        JPanel rightPanel = new JPanel(new BorderLayout());
        rightPanel.add(scrollPane, BorderLayout.WEST);
        frame.add(rightPanel, BorderLayout.EAST);
        frame.add(panel, BorderLayout.WEST);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(TextDisplay::createAndShowGUI);
    }
}