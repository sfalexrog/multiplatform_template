package me.sfalexrog.multiplatform;

import org.libsdl.app.SDLActivity;

import android.os.Bundle;

public class MainActivity extends SDLActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected String[] getLibraries() {
        return new String[] {"hidapi", "main"};
    }

    @Override
    protected String getMainSharedObject() {
        return "libmain.so";
    }
}
