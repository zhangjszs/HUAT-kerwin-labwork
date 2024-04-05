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
    private boolean connected = false;
    JLabel ipLabel = new JLabel();
    JLabel portLabel = new JLabel("端口");
    JTextField portTextField = new JTextField("8000");
    JButton listenButton = new JButton("监听");
    JButton closeButton = new JButton("关闭");
    JPanel panel = new JPanel();
    JTextField inputTextField = new JTextField();
    JTextArea contentTextArea = new JTextArea();
    Thread recvThread = new Thread(new RecvThread());

    public Server() {
        setTitle("服务器端");
        setLayout(new BorderLayout());
        panel.setLayout(new FlowLayout());
        try {
            ipLabel.setText("本机IP信息：" + InetAddress.getLocalHost().toString());
        } catch (HeadlessException e) {
            e.printStackTrace();
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
        // 将窗体位于屏幕的中央
        setLocationRelativeTo(null);
        setVisible(true);
    }

    public void start() {
        try {
            int port = Integer.parseInt(portTextField.getText());
            connected = true;
            ss = new ServerSocket(port);
            s = ss.accept();
            os = new DataOutputStream(s.getOutputStream());
            is = new DataInputStream(s.getInputStream());
            recvThread.start();
        } catch (IOException ignored) {
        } finally {
            try {
                ss.close();
            } catch (IOException ignored) {
            }
        }
    }

    public void disconnect() {
        try {
            if (os != null)
                os.close();
            if (is != null)
                is.close();
            if (s != null)
                s.close();
        } catch (IOException ignored) {
        }
        System.exit(0);
    }

    // 使用内部类创建接收数据线程
    private class RecvThread implements Runnable {
        public void run() {
            try {
                while (connected) {
                    String str = is.readUTF();
                    contentTextArea.setText(contentTextArea.getText() + "client:" + str + '\n');
                }
            } catch (EOFException e) {
                contentTextArea.setText("Client closed!");
//            } catch (IOException e) {
//                e.printStackTrace();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

        }
    }

    public void actionPerformed(ActionEvent e) {
        // 如果按下的是监听按钮
        if (e.getSource() == listenButton) {
            listenButton.setEnabled(false);
            start();
        } else if (e.getSource() == closeButton) {
            disconnect();
        } else if (e.getSource() == inputTextField) {
            String str = inputTextField.getText().trim();
            inputTextField.setText("");
            try {
                os.writeUTF(str);
                os.flush();
                contentTextArea.setText(contentTextArea.getText() + "server:" + str + '\n');
            } catch (IOException e1) {
            }
        }
    }

    public static void main(String[] args) {
        new Server();
    }
}
