package Labhomework4;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class ArtFont extends JFrame implements ActionListener, ItemListener {
    JComboBox<String> fontType;
    JComboBox<String> fontSize;
    JComboBox<String> windowStyle;
    JCheckBox boldBx;
    JCheckBox italicBx;
    JButton colorBtn;
    String[] fontNames;
    String[] fontSizes;
    JLabel label;
    JLabel labelName;
    JTextField inputText;
    JTextArea txtArea;
    JPanel northPanel;
    JPanel centerPanel;
    JPanel southPanel;
    Font font;
    int boldStyle, italicStyle;
    int fontSizeStyle;
    String fontNameStyle;
    Color colorStyle = Color.black;
    String[] style = {"默认显示效果", "Windows显示效果", "Unix显示效果"};

    public ArtFont() {
        super("字体设置");
        boldStyle = 0;
        italicStyle = 0;
        fontSizeStyle = 10;
        fontNameStyle = "宋体";
        font = new Font(fontNameStyle, boldStyle + italicStyle, fontSizeStyle);
        northPanel = getNorthPanel();
        centerPanel = getCenterPanel();
        southPanel = getSouthPanel();
        Container container = getContentPane();
        container.setLayout(new BorderLayout());
        container.add(northPanel, BorderLayout.NORTH);
        container.add(centerPanel, BorderLayout.CENTER);
        container.add(southPanel, BorderLayout.SOUTH);
        setSize(500, 300);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private JPanel getNorthPanel() {
        JPanel panel = new JPanel();
        label = new JLabel("请输入要显示的文字:");
        labelName = new JLabel("计算机222 章崇文 202202296");
        inputText = new JTextField(10);
        boldBx = new JCheckBox("粗体");
        italicBx = new JCheckBox("斜体");
        colorBtn = new JButton("颜色");
        panel.add(label);
        panel.add(inputText);
        panel.add(boldBx);
        panel.add(italicBx);
        panel.add(colorBtn);
        panel.add(labelName);
        inputText.addActionListener(this);
        boldBx.addItemListener(this);
        italicBx.addItemListener(this);
        colorBtn.addActionListener(this);
        return panel;
    }

    private JPanel getCenterPanel() {
        JPanel panel = new JPanel();
        txtArea = new JTextArea(6, 20);
        panel.setLayout(new BorderLayout());
        panel.add(new JScrollPane(txtArea), BorderLayout.CENTER);
        return panel;
    }

    private JPanel getSouthPanel() {
        JPanel panel = new JPanel();
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        fontNames = ge.getAvailableFontFamilyNames();
        fontType = new JComboBox<>(fontNames);
        fontSizes = new String[63];
        for (int i = 0; i < fontSizes.length; i++) {
            fontSizes[i] = Integer.toString(i + 10);
        }
        fontSize = new JComboBox<>(fontSizes);
        windowStyle = new JComboBox<>(style);
        panel.add(fontType);
        panel.add(fontSize);
        panel.add(windowStyle);
        fontType.addItemListener(this);
        fontSize.addItemListener(this);
        windowStyle.addItemListener(this);
        return panel;
    }

    @Override
    public void itemStateChanged(ItemEvent e) {
        if (e.getSource() == boldBx) {
            boldStyle = boldBx.isSelected() ? Font.BOLD : Font.PLAIN;
        } else if (e.getSource() == italicBx) {
            italicStyle = italicBx.isSelected() ? Font.ITALIC : Font.PLAIN;
        } else if (e.getSource() == fontType) {
            fontNameStyle = (String) fontType.getSelectedItem();
        } else if (e.getSource() == fontSize) {
            fontSizeStyle = Integer.parseInt((String) fontSize.getSelectedItem());
        } else if (e.getSource() == windowStyle) {
            String s = (String) e.getItem();
            String className = "";
            if (s.equals("Windows显示效果"))
                className = "com.sun.java.swing.plaf.windows.WindowsLookAndFeel";
            else if (s.equals("Unix显示效果"))
                className = "com.sun.java.swing.plaf.motif.MotifLookAndFeel";
            else if (s.equals("默认显示效果"))
                className = UIManager.getCrossPlatformLookAndFeelClassName();
            try {
                UIManager.setLookAndFeel(className);
                SwingUtilities.updateComponentTreeUI(this);
            } catch (Exception de) {
                System.out.println("切换外观失败: " + de.getMessage());
            }
        }
        font = new Font(fontNameStyle, boldStyle + italicStyle, fontSizeStyle);
        txtArea.setFont(font);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == inputText) {
            txtArea.setText(inputText.getText());
        } else if (e.getSource() == colorBtn) {
            colorStyle = JColorChooser.showDialog(this, "选择字体颜色", colorStyle);
            txtArea.setForeground(colorStyle);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            ArtFont frame = new ArtFont();
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        });
    }
}
