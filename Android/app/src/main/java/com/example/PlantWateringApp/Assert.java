package com.example.PlantWateringApp;

public class Assert {
    private Assert() {}
    public static void check(boolean condition) {
        if (!condition) {
            throw new AssertionError();
        }
    }
}
