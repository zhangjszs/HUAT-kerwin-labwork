package org.example.weblab3;


public class Book {
    private String name;
    private double price;
    private int quantity;

    public Book(String name, double price) {
        this.name = name;
        this.price = price;
        this.quantity = 1;
    }

    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }

    public int getQuantity() {
        return quantity;
    }

    public void incrementQuantity() {
        this.quantity++;
    }
}
