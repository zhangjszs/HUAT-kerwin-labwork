package Homework4tcp;

/**
 * @author kerwinz
 * @changtime 2023/11/24
 * @projectname schoolJavaHomework
 */
import java.io.*;
import java.net.*;
import javax.swing.*;
import java.awt.*;

public class Client {
    // 将DataInputStream和DataOutputStream设置为类的成员变量
    private static DataInputStream in = null;
    private static DataOutputStream out = null;

    public static void main(String[] args) throws Exception {

        JFrame frame = new JFrame("客户端");
        JTextArea textArea = new JTextArea(12, 24);
        textArea.setEditable(false);

        JScrollPane scrollPane = new JScrollPane(textArea);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        textArea.setLineWrap(true);

        JTextField textField = new JTextField();
        JButton button = new JButton("发送");
        button.setSize(10, 10);

        // 创建一个新的JPanel，将文本输入框和发送按钮添加到这个面板中
        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());
        panel.add(textField, BorderLayout.CENTER);
        panel.add(button, BorderLayout.EAST);

        frame.setLayout(new BorderLayout());
        frame.add(scrollPane, BorderLayout.CENTER);
        frame.add(panel, BorderLayout.SOUTH); // 将面板添加到框架的南部
        frame.setSize(400, 300);
        frame.setVisible(true);

        try {
            Socket socket = new Socket("localhost", 7788);
            textArea.append("服务端说: 你好，我是服务端！\n");
            textArea.append("客户端说: 你好，我是客户端！\n");
            in = new DataInputStream(socket.getInputStream());
            out = new DataOutputStream(socket.getOutputStream());
        } catch (IOException e) {
            // 连接失败，你可以在这里处理异常
            System.out.println("无法连接到服务器: " + e.getMessage());
        }

        button.addActionListener(e -> {
            try {
                String message = textField.getText();
                out.writeUTF(message);
                textArea.append("客户端说: " + message + "\n");
                textField.setText("");
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        });

        while (true) {
            try {
                String message = in.readUTF();
                textArea.append("服务器说: " + message + "\n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}


