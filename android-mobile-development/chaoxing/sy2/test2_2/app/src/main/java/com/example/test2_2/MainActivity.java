package com.example.test2_2;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class MainActivity extends Activity {
    private DBManager mgr;
    private ListView listView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mgr = new DBManager(this);
        listView = findViewById(R.id.listView);

        // 初始加载数据
        query(null);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // 使用的最后一个Activity关闭时应释放DB
        mgr.closeDB();
    }

    public void add(View view) {
        ArrayList<Person> persons = new ArrayList<>();
        Person person1 = new Person("李子航", 19, "Student A");
        Person person2 = new Person("张三", 20, "Student B");
        Person person3 = new Person("李四", 30, "Student C");
        Person person4 = new Person("王五", 40, "Student D");
        Person person5 = new Person("赵六", 50, "Student E");
        persons.add(person1);
        persons.add(person2);
        persons.add(person3);
        persons.add(person4);
        persons.add(person5);
        mgr.add(persons);
        query(null);
    }

    public void update(View view) {
        Person person = new Person();
        person.set_id(4); // 假设要更新ID为4的记录
        person.setName("zhang");
        person.setAge(45);
        mgr.updateAge(person);
        query(null);
    }

    public void delete(View view) {
        mgr.deleteAllPersons();
        query(null);
    }

    public void query(View view) {
        List<Person> persons = mgr.query();
        ArrayList<Map<String, String>> list = new ArrayList<>();
        for (Person person : persons) {
            HashMap<String, String> map = new HashMap<>();
            map.put("name", person.getName());
            map.put("info", person.getAge() + " years old, " + person.getInfo());
            list.add(map);
        }
        SimpleAdapter adapter = new SimpleAdapter(this, list, android.R.layout.simple_list_item_2,
                new String[]{"name", "info"},
                new int[]{android.R.id.text1, android.R.id.text2});
        listView.setAdapter(adapter);
    }
}



