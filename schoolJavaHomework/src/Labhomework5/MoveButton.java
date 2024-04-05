package Labhomework5;

/**
 * @author kerwinz
 * @changtime 2023/11/21
 * @projectname schoolJavaHomework
 */
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.*;

public class MoveButton extends Frame implements Runnable, ActionListener {
    // 用Thread类声明first,second,third三个线程对象
    Thread first, second, third;
    JLabel label;
    Button redButton, greenButton, blueButton, startButton;
    int distance = 10;

    MoveButton() {
        //分别创建first,second,third三个线程，用当前窗口做为该线程的目标对象
        first = new Thread(this);
        second = new Thread(this);
        third = new Thread(this);

        redButton = new Button();
        greenButton = new Button();
        blueButton = new Button();
        redButton.setBackground(Color.red);
        greenButton.setBackground(Color.green);
        blueButton.setBackground(Color.blue);

        startButton = new Button("start");
        startButton.addActionListener(this);
        label = new JLabel("计算机222 章崇文 202202296");
        setLayout(null);
        add(redButton);
        redButton.setBounds(10, 60, 15, 15);
        add(greenButton);
        greenButton.setBounds(100, 60, 15, 15);
        add(blueButton);
        blueButton.setBounds(200, 60, 15, 15);
        add(startButton);
        startButton.setBounds(10, 100, 30, 30);
        add(label);
        label.setBounds(50, 100, 300, 30);

        setTitle("线程接力");
        setBounds(0, 0, 400, 200);
        setVisible(true);
        validate();
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });
    }

    public void actionPerformed(ActionEvent e) {
        try {
            // 分别启动三个线程
            first.start();
            second.start();
            third.start();
        } catch (Exception exp) {
            exp.printStackTrace();
        }
    }

    public void run() {
        while (true) {
            // 判断当前占有CPU资源的线程是否是first
            if (Thread.currentThread() == first) {
                moveComponent(redButton);
                try {
                    Thread.sleep(20);
                } catch (Exception exp) {
                    exp.printStackTrace();
                }
            }
            // 判断当前占有CPU资源的线程是否是second
            if (Thread.currentThread() == second) {
                moveComponent(greenButton);
                try {
                    Thread.sleep(10);
                } catch (Exception exp) {
                    exp.printStackTrace();
                }
            }
            // 判断当前占有CPU资源的线程是否是third
            if (Thread.currentThread() == third) {
                moveComponent(blueButton);
                try {
                    Thread.sleep(20);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public synchronized void moveComponent(Component b) {
        if (Thread.currentThread() == first) {
            while (distance > 100 && distance <= 300)
                try {
                    wait();
                } catch (Exception exp) {
                    exp.printStackTrace();
                }
            distance = distance + 1;
            b.setLocation(distance, 60);
            if (distance >= 100) {
                b.setLocation(10, 60);
                notifyAll();
            }
        }
        if (Thread.currentThread() == second) {
            while (distance <= 100 || distance > 200)
                try {
                    wait();
                } catch (Exception exp) {
                    exp.printStackTrace();
                }
            distance = distance + 1;
            b.setLocation(distance, 60);
            if (distance > 200) {
                b.setLocation(100, 60);
                notifyAll();
            }
        }
        if (Thread.currentThread() == third) {
            while (distance <= 200 || distance > 300)
                try {
                    wait();
                } catch (Exception exp) {
                    exp.printStackTrace();
                }
            distance = distance + 1;
            b.setLocation(distance, 60);
            if (distance > 300) {
                distance = 10;
                b.setLocation(200, 60);
                notifyAll();
            }
        }
    }

    public static void main(String[] args) {
        new MoveButton().setLocationRelativeTo(null);
    }
}

