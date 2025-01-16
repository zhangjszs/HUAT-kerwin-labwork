package com.example.company;

import androidx.appcompat.app.AppCompatActivity;

import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Objects;

public class NewsActivity extends AppCompatActivity {

    public static final String EXTRA_DRINKID = "newsId";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_news);

        int newsId = Objects.requireNonNull(getIntent().getExtras()).getInt(EXTRA_DRINKID);

        SQLiteOpenHelper starbuzzDatabaseHelper = new CompanyDatabaseHelper(this);
        //获得数据库引用
        try (SQLiteDatabase db = starbuzzDatabaseHelper.getReadableDatabase()) {
            Cursor cursor = db.query("NEWS",
                    new String[]{"TITLE", "IMAGE_RESOURCE_ID", "CONTENT"},
                    "_id=?",
                    new String[]{Integer.toString(newsId)},
                    null,
                    null,
                    null);
            if (cursor.moveToFirst()) {

                String titleText = cursor.getString(0);
                int photoId = cursor.getInt(1);
                String contentText = cursor.getString(2);
                TextView name = findViewById(R.id.title);
                name.setText(titleText);

                ImageView imagenews = findViewById(R.id.image_news);
                imagenews.setImageResource(photoId);
                imagenews.setContentDescription(titleText);

                TextView content = findViewById(R.id.content);
                content.setText(contentText);
            }
            cursor.close();
        } catch (SQLException e) {
            Log.e("sqlite", Objects.requireNonNull(e.getMessage()));
            Toast toast = Toast.makeText(this, "Database unavailable", Toast.LENGTH_SHORT);
            toast.show();
        }
    }

}

