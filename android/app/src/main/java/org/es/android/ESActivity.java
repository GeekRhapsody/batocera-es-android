package org.es.android;

import android.os.Bundle;
import android.view.View;

import org.libsdl.app.SDLActivity;

/**
 * EmulationStation main Activity.
 * Extends SDL2's SDLActivity which manages the OpenGL surface, input events,
 * and lifecycle. The C++ main() runs in a background thread via SDL_main.
 */
public class ESActivity extends SDLActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Keep screen on while the app is active
        getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected String getMainSharedObject() {
        return "libmain.so";
    }

    @Override
    protected String[] getLibraries() {
        // Order matters: dependencies before dependents
        return new String[]{
            "SDL2",
            "SDL2_mixer",
            "main"
        };
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            // Full immersive mode: hide system bars
            getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
            );
        }
    }
}
