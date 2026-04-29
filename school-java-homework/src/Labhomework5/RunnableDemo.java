package Labhomework5;

/**
 * @author kerwinz
 * @changtime 2023/11/21
 * @projectname schoolJavaHomework
 */
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;
import javax.swing.border.BevelBorder;

public class RunnableDemo extends JFrame implements Runnable, ActionListener {
    private JTextArea textArea;
    JLabel label;
    JButton startBtn, pauseBtn, resumeBtn;
    JPanel panel;
    Thread thread;
    boolean move = false;

    private final String introduction = "现在大家已经对计算机很熟悉了，如今计算机的操作"
            + "系统可以同时执行多个任务，在听歌的同时能够打字、下载文件，在聊天窗口打"
            + "字的时候，对方同时还能通过视频看到你；听到你。这一切都是使用多任务实现"
            + "的，Java语言使用多线程实现一个程序中的多个任务同时运行。程序员可以在程"
            + "序中执行多个线程，每一个线程完成一个功能，并与其他线程并发执行，这种机"
            + "制被称为多线程。";

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new RunnableDemo().setLocationRelativeTo(null));
    }

    public RunnableDemo() {
        setTitle("线程的控制");
        label = new JLabel("多线程简介：计算机222 章崇文 202202296");
        getContentPane().add(label, BorderLayout.NORTH);
        textArea = new JTextArea("\t");
        textArea.setBorder(new BevelBorder(BevelBorder.LOWERED));
        textArea.setLineWrap(true);
        getContentPane().add(textArea, BorderLayout.CENTER);
        startBtn = new JButton("开始");
        pauseBtn = new JButton("暂停");
        resumeBtn = new JButton("恢复");

        Dimension buttonSize = new Dimension(60, 30);
        startBtn.setMinimumSize(buttonSize);
        pauseBtn.setMinimumSize(buttonSize);
        resumeBtn.setMinimumSize(buttonSize);

        startBtn.addActionListener(this);
        pauseBtn.addActionListener(this);
        resumeBtn.addActionListener(this);

        add(label);
        panel = new JPanel();
        panel.add(startBtn);
        panel.add(pauseBtn);
        panel.add(resumeBtn);
        getContentPane().add(panel, BorderLayout.SOUTH);

        setBounds(0, 0, 383, 225);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
    }

    @Override
    public void run() {
        SwingUtilities.invokeLater(() -> textArea.setText("\t"));
        String[] intros = introduction.split("");
        for (String ch : intros) {
            while (!move) {
                try {
                    synchronized (this) {
                        wait();
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            SwingUtilities.invokeLater(() -> textArea.append(ch));
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        SwingUtilities.invokeLater(() -> startBtn.setEnabled(true));
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == startBtn) {
            startBtn.setEnabled(false);
            move = true;
            thread = new Thread(this);
            thread.start();
        } else if (e.getSource() == pauseBtn) {
            move = false;
        } else if (e.getSource() == resumeBtn) {
            move = true;
            synchronized (this) {
                notify();
            }
        }
    }
}
