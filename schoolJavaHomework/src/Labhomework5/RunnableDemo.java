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
    private JTextArea textArea; // 文本域组件
    JLabel label;
    Button startBtn, pauseBtn, resumeBtn;
    Panel panel;
    Thread thread;
    boolean move = false;

    // 动画显示的文本字符串
    private final String introduction = "现在大家已经对计算机很熟悉了，如今计算机的操作"
            + "系统可以同时执行多个任务，在听歌的同时能够打字、下载文件，在聊天窗口打"
            + "字的时候，对方同时还能通过视频看到你；听到你。这一切都是使用多任务实现"
            + "的，Java语言使用多线程实现一个程序中的多个任务同时运行。程序员可以在程"
            + "序中执行多个线程，每一个线程完成一个功能，并与其他线程并发执行，这种机"
            + "制被称为多线程。";

    public static void main(String args[]) {
        new RunnableDemo().setLocationRelativeTo(null); // 创建本类实例对象
    }

    public RunnableDemo() {
        setTitle("线程的控制");
        label = new JLabel("多线程简介：计算机222 章崇文 202202296"); // 标签组件
        getContentPane().add(label, BorderLayout.NORTH);// 添加标签到窗体
        textArea = new JTextArea("\t"); // 初始化文本域组件
        textArea.setBorder(new BevelBorder(BevelBorder.LOWERED));// 设置边框
        textArea.setLineWrap(true); // 设置自动折行
        getContentPane().add(textArea, BorderLayout.CENTER);// 添加文本域组件到文本框
        startBtn = new Button("开始");
        pauseBtn = new Button("暂停");
        resumeBtn = new Button("恢复");

        // 设置按钮的最小大小和使用系统默认字体
        Dimension buttonSize = new Dimension(60, 30);
        startBtn.setMinimumSize(buttonSize);
        pauseBtn.setMinimumSize(buttonSize);
        resumeBtn.setMinimumSize(buttonSize);

        startBtn.addActionListener(this);
        pauseBtn.addActionListener(this);
        resumeBtn.addActionListener(this);

        add(label);
        panel = new Panel();
        panel.add(startBtn);
        panel.add(pauseBtn);
        panel.add(resumeBtn);
        getContentPane().add(panel, BorderLayout.SOUTH);

        setBounds(0, 0, 383, 225); // 设置窗体大小位置
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true); // 显示窗体
    }

    /**Runnable接口方法，是线程的执行方法*/
    @Override
    public void run() {
        textArea.setText("\t");
        String[] intros = introduction.split(""); // 将字符串分割为数组
        for (String ch : intros) { // ForEach遍历字符串数组
            while (!move) {
                try {
                    synchronized (this) {
                        wait();
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            textArea.append(ch); // 添加一个字符到文本域
            try {
                Thread.sleep(100); // 线程休眠0.1秒
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        startBtn.setEnabled(true);
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
