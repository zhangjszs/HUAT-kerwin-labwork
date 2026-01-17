package com.example.test2_3;

import android.net.Uri;
import android.os.Bundle;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.database.Cursor;
import android.widget.Toast;

public class MainActivity extends Activity {
    public static final Uri CONTENT_URI = Uri.parse("content://com.example.test2_3.mycontentprovider/users");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        insertRecord("李子航");
        displayRecords();
    }

    private void insertRecord(String userName) {
        ContentValues values = new ContentValues();
        values.put("USER_NAME", userName);
        ContentResolver cr = getContentResolver();
        cr.insert(CONTENT_URI, values);
    }

    private void displayRecords() {
        String[] columns = new String[]{"_id", "USER_NAME"};
        ContentResolver cr = getContentResolver();
        Cursor cur = cr.query(CONTENT_URI, columns, null, null, null);

        if (cur != null) {
            if (cur.moveToFirst()) {
                do {
                    int idIndex = cur.getColumnIndex("_id");
                    int nameIndex = cur.getColumnIndex("USER_NAME");

                    if (idIndex != -1 && nameIndex != -1) {
                        String id = cur.getString(idIndex);
                        String userName = cur.getString(nameIndex);
                        Toast.makeText(this, id + "\n" + userName, Toast.LENGTH_LONG).show();
                    } else {
                        Toast.makeText(this, "Column index not found", Toast.LENGTH_SHORT).show();
                    }
                } while (cur.moveToNext());
            }
            cur.close();
        } else {
            Toast.makeText(this, "No data found", Toast.LENGTH_SHORT).show();
        }
    }
}



