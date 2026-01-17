package com.example.company;

import androidx.annotation.NonNull;

public class News {
    private final String name;
    private final String content;
    private final int imageResourceId;

    public News(String name, String content, int imageResourceId) {
        this.name = name;
        this.content = content;
        this.imageResourceId = imageResourceId;
    }

    public static final News[] news = {
            new News("news one","aaaaaaaaaaaaaaaaaa",R.drawable.news01),
            new News("news two","bbbbbbbbbbbbbbbbbb",R.drawable.news02)
    };
    @NonNull
    public String toString()
    {
        return name;
    }

    public String getName() {
        return name;
    }

    public String getContent() {
        return content;
    }

    public int getImageResourceId() {
        return imageResourceId;
    }
}


