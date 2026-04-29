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

public class MoveButton extends JFrame implements Runnable, ActionListener {
    Thread first, second, third;
    JLabel label;
    JButton redButton, greenButton, blueButton, startButton;
    int distance = 10;
    private boolean started = false;

    MoveButton() {
        first = new Thread(this);
        second = new Thread(this);
        third = new Thread(this);

        redButton = new JButton();
        greenButton = new JButton();
        blueButton = new JButton();
        redButton.setBackground(Color.red);
        greenButton.setBackground(Color.green);
        blueButton.setBackground(Color.blue);

        startButton = new JButton("start");
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
        startButton.setBounds(10, 100, 70, 30);
        add(label);
        label.setBounds(50, 100, 300, 30);

        setTitle("线程接力");
        setBounds(0, 0, 400, 200);
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    public void actionPerformed(ActionEvent e) {
        if (started) {
            return;
        }
        started = true;
        try {
            first.start();
            second.start();
            third.start();
        } catch (Exception exp) {
            exp.printStackTrace();
        }
    }

    public void run() {
        while (true) {
            if (Thread.currentThread() == first) {
                moveComponent(redButton);
                try {
                    Thread.sleep(20);
                } catch (Exception exp) {
                    exp.printStackTrace();
                }
            }
            if (Thread.currentThread() == second) {
                moveComponent(greenButton);
                try {
                    Thread.sleep(10);
                } catch (Exception exp) {
                    exp.printStackTrace();
                }
            }
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
        SwingUtilities.invokeLater(() -> new MoveButton().setLocationRelativeTo(null));
    }
}
