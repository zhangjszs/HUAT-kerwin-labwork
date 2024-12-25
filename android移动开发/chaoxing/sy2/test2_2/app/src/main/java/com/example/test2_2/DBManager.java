package com.example.test2_2;

import java.util.ArrayList;
import java.util.List;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

public class DBManager {
    private static final String TAG = "DBManager";
    private DBHelper helper;
    private SQLiteDatabase db;

    public DBManager(Context context) {
        helper = new DBHelper(context);
        db = helper.getWritableDatabase();
    }

    public void add(List<Person> persons) {
        db.beginTransaction(); // 开始事务
        try {
            for (Person person : persons) {
                db.execSQL("INSERT INTO person VALUES(null, ?, ?, ?)", new Object[]{person.getName(), person.getAge(), person.getInfo()});
            }
            db.setTransactionSuccessful(); // 设置事务成功完成
        } finally {
            db.endTransaction(); // 结束事务
        }
    }

    public void updateAge(Person person) {
        ContentValues cv = new ContentValues();
        cv.put("age", person.getAge());
        db.update("person", cv, "_id = ?", new String[]{String.valueOf(person.get_id())});
    }

    public void deleteAllPersons() {
        db.delete("person", null, null);
    }

    public List<Person> query() {
        ArrayList<Person> persons = new ArrayList<>();
        Cursor c = queryTheCursor();
        if (c != null && c.moveToFirst()) {
            do {
                Person person = new Person();
                person.set_id(c.getInt(c.getColumnIndexOrThrow("_id")));
                person.setName(c.getString(c.getColumnIndexOrThrow("name")));
                person.setAge(c.getInt(c.getColumnIndexOrThrow("age")));
                person.setInfo(c.getString(c.getColumnIndexOrThrow("info")));
                persons.add(person);
            } while (c.moveToNext());
            c.close();
        }
        return persons;
    }

    public Cursor queryTheCursor() {
        return db.rawQuery("SELECT * FROM person", null);
    }

    public void closeDB() {
        db.close();
    }
}



