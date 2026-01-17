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
    private boolean connected = false;
    JLabel ipLabel = new JLabel("IP");
    JTextField ipTextField = new JTextField("127.0.0.1");
    JLabel portLabel = new JLabel("端口");
    JTextField portTextField = new JTextField("8000");
    JButton connectButton = new JButton("连接");
    JButton closeButton = new JButton("关闭");
    JPanel panel = new JPanel();
    JTextField inputTextField = new JTextField();
    JTextArea contentTextArea = new JTextArea();
    Thread recvThread = new Thread(new RecvThread());

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
        // 将窗体位于屏幕的中央
        setLocationRelativeTo(null);
        setVisible(true);
    }

    public void connect() {
        try {
            String host, port;
            host = ipTextField.getText();
            port = portTextField.getText();
            s = new Socket(host, Integer.parseInt(port));
            os = new DataOutputStream(s.getOutputStream());
            is = new DataInputStream(s.getInputStream());
            connected = true;
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
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
        } catch (IOException e) {
        }
        System.exit(0);
    }

    // 使用内部类创建接收数据线程
    private class RecvThread implements Runnable {
        public void run() {
            try {
                while (connected) {
                    String str = is.readUTF();
                    contentTextArea.setText(contentTextArea.getText() + "server:" + str + '\n');
                }
            } catch (IOException e) {
            }
        }
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == connectButton) {
            connectButton.setEnabled(false);
            connect();
            recvThread.start();
        } else if (e.getSource() == closeButton) {
            disconnect();
        } else if (e.getSource() == inputTextField) {
            String str = inputTextField.getText().trim();
            inputTextField.setText("");
            try {
                os.writeUTF(str);
                os.flush();
                contentTextArea.setText(contentTextArea.getText() + "client:" + str + '\n');
            } catch (IOException e1) {
            }
        }
    }

    public static void main(String[] args) {
        new Client();
    }
}
