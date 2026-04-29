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
    private boolean isBold = false;
    private boolean isItalic = false;
    private JTextArea textArea;

    private void createAndShowGUI() {
        JFrame frame = new JFrame("文本显示器");
        frame.setBackground(Color.GRAY);
        JPanel panel = new JPanel();
        JCheckBox boldButton = new JCheckBox("粗体");
        JCheckBox italicButton = new JCheckBox("斜体");
        ButtonGroup fontSizeGroup = new ButtonGroup();
        JRadioButton largeButton = new JRadioButton("大字号");
        JRadioButton mediumButton = new JRadioButton("中字号");
        JRadioButton smallButton = new JRadioButton("小字号");
        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
        int width = 800;
        int height = 600;
        textArea = new JTextArea(7, 20);
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
        frame.setLayout(new BorderLayout());
        panel.setLayout(new GridLayout(5, 1, 5, 5));

        boldButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                int style = font.getStyle();
                int newStyle;

                if (isBold) {
                    newStyle = style & ~Font.BOLD;
                } else {
                    newStyle = style | Font.BOLD;
                }

                Font newFont = new Font(font.getFontName(), newStyle, font.getSize());
                textArea.setFont(newFont);
                isBold = !isBold;
            }
        });

        italicButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                int style = font.getStyle();
                int newStyle;

                if (isItalic) {
                    newStyle = style & ~Font.ITALIC;
                } else {
                    newStyle = style | Font.ITALIC;
                }

                Font newFont = new Font(font.getFontName(), newStyle, font.getSize());
                textArea.setFont(newFont);
                isItalic = !isItalic;
            }
        });

        largeButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                Font newFont = new Font(font.getFontName(), font.getStyle(), 60);
                textArea.setFont(newFont);
            }
        });
        fontSizeGroup.add(largeButton);

        mediumButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Font font = textArea.getFont();
                Font newFont = new Font(font.getFontName(), font.getStyle(), 30);
                textArea.setFont(newFont);
            }
        });
        fontSizeGroup.add(mediumButton);

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

        JScrollPane scrollPane = new JScrollPane(textArea);
        JPanel rightPanel = new JPanel(new BorderLayout());
        rightPanel.add(scrollPane, BorderLayout.WEST);
        frame.add(rightPanel, BorderLayout.EAST);
        frame.add(panel, BorderLayout.WEST);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new TextDisplay().createAndShowGUI());
    }
}
