package labhomework3;

/**
 * @author kerwinz
 * @changtime 2023/10/15
 * @projectname schoolJavaHomework
 */
interface Display {
    void display();
}

class Inform implements Display {
    @Override
    public void display() {
        System.out.println("通知内容");
    }
}

class Car implements Display {
    @Override
    public void display() {
        System.out.println("汽车油量");
    }
}

class Advertise implements Display {
    @Override
    public void display() {
        System.out.println("广告消息");
    }
}

/**
 * @author kerwinz
 */
public class lab3_3 {
    public static void main(String[] args) {
        Display[] arr = { new Inform(), new Car(), new Advertise() };
        for (Display d : arr) {
            d.display();
        }
    }
}

