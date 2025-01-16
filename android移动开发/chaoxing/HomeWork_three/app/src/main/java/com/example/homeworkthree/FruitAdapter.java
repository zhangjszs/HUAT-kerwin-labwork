package com.example.homeworkthree;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import java.util.List;

public class FruitAdapter extends ArrayAdapter<Fruit> {

    public FruitAdapter(Context context, List<Fruit> fruits) {
        super(context, 0, fruits);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        Fruit fruit = getItem(position);

        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.item_fruit, parent, false);
        }

        // 绑定数据到视图
        TextView fruitName = convertView.findViewById(R.id.fruitName);
        TextView fruitDescription = convertView.findViewById(R.id.fruitDescription);

        fruitName.setText(fruit.name);
        fruitDescription.setText(fruit.description);

        return convertView;
    }
}