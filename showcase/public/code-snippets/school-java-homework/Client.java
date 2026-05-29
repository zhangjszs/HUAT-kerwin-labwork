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

public class Client extends JFrame implements ActionListener {
    Socket s = null;
    DataOutputStream os = null;
    DataInputStream is = null;
    private volatile boolean connected = false;
    JLabel ipLabel = new JLabel("IP");
    JTextField ipTextField = new JTextField("127.0.0.1");
    JLabel portLabel = new JLabel("端口");
    JTextField portTextField = new JTextField("8000");
    JButton connectButton = new JButton("连接");
    JButton closeButton = new JButton("关闭");
    JPanel panel = new JPanel();
    JTextField inputTextField = new JTextField();
    JTextArea contentTextArea = new JTextArea();
    Thread recvThread = null;

    public Client() {
        setTitle("客户端");
        setLocation(400, 300);
        setSize(500, 500);
        setLayout(new BorderLayout());
        panel.setLayout(new FlowLayout());
        panel.add(ipLabel);
        panel.add(ipTextField);
        panel.add(portLabel);
        panel.add(portTextField);
        panel.add(connectButton);
        panel.add(closeButton);
        add(panel, BorderLayout.NORTH);
        add(new JScrollPane(contentTextArea), BorderLayout.CENTER);
        add(inputTextField, BorderLayout.SOUTH);
        connectButton.addActionListener(this);
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

    public void connect() {
        try {
            String host = ipTextField.getText();
            int port = Integer.parseInt(portTextField.getText());
            s = new Socket(host, port);
            os = new DataOutputStream(s.getOutputStream());
            is = new DataInputStream(s.getInputStream());
            connected = true;
            recvThread = new Thread(new RecvThread());
            recvThread.start();
        } catch (UnknownHostException e) {
            appendMessage("错误: 无法解析主机地址");
            e.printStackTrace();
            SwingUtilities.invokeLater(() -> connectButton.setEnabled(true));
        } catch (IOException e) {
            appendMessage("错误: 连接服务器失败");
            e.printStackTrace();
            SwingUtilities.invokeLater(() -> connectButton.setEnabled(true));
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
                    appendMessage("server: " + str);
                }
            } catch (IOException e) {
                if (connected) {
                    appendMessage("与服务器断开连接");
                }
            }
        }
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == connectButton) {
            connectButton.setEnabled(false);
            connect();
        } else if (e.getSource() == closeButton) {
            disconnect();
        } else if (e.getSource() == inputTextField) {
            String str = inputTextField.getText().trim();
            inputTextField.setText("");
            if (os == null || !connected) {
                appendMessage("错误: 尚未连接到服务器");
                return;
            }
            try {
                os.writeUTF(str);
                os.flush();
                appendMessage("client: " + str);
            } catch (IOException e1) {
                appendMessage("发送失败: " + e1.getMessage());
                e1.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(Client::new);
    }
}
