package Labhomework6;

/**
 * @author kerwinz
 * @changtime 2023/11/28
 * @projectname schoolJavaHomework
 */
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;

import javax.swing.*;

public class Server extends JFrame implements ActionListener {
    ServerSocket ss = null;
    Socket s = null;
    private DataInputStream is = null;
    private DataOutputStream os = null;
    private volatile boolean connected = false;
    JLabel ipLabel = new JLabel();
    JLabel portLabel = new JLabel("端口");
    JTextField portTextField = new JTextField("8000");
    JButton listenButton = new JButton("监听");
    JButton closeButton = new JButton("关闭");
    JPanel panel = new JPanel();
    JTextField inputTextField = new JTextField();
    JTextArea contentTextArea = new JTextArea();
    Thread recvThread = null;

    public Server() {
        setTitle("服务器端");
        setLayout(new BorderLayout());
        panel.setLayout(new FlowLayout());
        try {
            ipLabel.setText("本机IP信息：" + InetAddress.getLocalHost().toString());
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        panel.add(ipLabel);
        panel.add(portLabel);
        panel.add(portTextField);
        panel.add(listenButton);
        panel.add(closeButton);
        add(panel, BorderLayout.NORTH);
        add(new JScrollPane(contentTextArea), BorderLayout.CENTER);
        add(inputTextField, BorderLayout.SOUTH);
        listenButton.addActionListener(this);
        closeButton.addActionListener(this);
        inputTextField.addActionListener(this);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                disconnect();
            }
        });
        setSize(500, 300);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    public void start() {
        try {
            int port = Integer.parseInt(portTextField.getText());
            ss = new ServerSocket(port);
            connected = true;
            appendMessage("等待客户端连接...");
            s = ss.accept();
            appendMessage("客户端已连接: " + s.getInetAddress());
            os = new DataOutputStream(s.getOutputStream());
            is = new DataInputStream(s.getInputStream());
            recvThread = new Thread(new RecvThread());
            recvThread.start();
        } catch (IOException e) {
            appendMessage("服务器异常: " + e.getMessage());
            e.printStackTrace();
            SwingUtilities.invokeLater(() -> listenButton.setEnabled(true));
        }
    }

    public void disconnect() {
        connected = false;
        try {
            if (os != null)
                os.close();
            if (is != null)
                is.close();
            if (s != null)
                s.close();
            if (ss != null)
                ss.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.exit(0);
    }

    private void appendMessage(String msg) {
        SwingUtilities.invokeLater(() -> contentTextArea.append(msg + "\n"));
    }

    private class RecvThread implements Runnable {
        public void run() {
            try {
                while (connected) {
                    String str = is.readUTF();
                    appendMessage("client: " + str);
                }
            } catch (EOFException e) {
                appendMessage("Client closed!");
            } catch (IOException e) {
                if (connected) {
                    appendMessage("连接异常: " + e.getMessage());
                }
            }
        }
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == listenButton) {
            listenButton.setEnabled(false);
            start();
        } else if (e.getSource() == closeButton) {
            disconnect();
        } else if (e.getSource() == inputTextField) {
            String str = inputTextField.getText().trim();
            inputTextField.setText("");
            if (os == null || !connected) {
                appendMessage("错误: 尚未有客户端连接");
                return;
            }
            try {
                os.writeUTF(str);
                os.flush();
                appendMessage("server: " + str);
            } catch (IOException e1) {
                appendMessage("发送失败: " + e1.getMessage());
                e1.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(Server::new);
    }
}
