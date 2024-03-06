package com.example.control_app;

import android.os.Bundle;
import android.view.WindowManager;

import io.flutter.embedding.android.FlutterActivity;
import io.flutter.plugin.common.MethodChannel;

public class MainActivity extends FlutterActivity {
    private static final String LIFECYCLE_CHANNEL = "com.example.app/lifecycle";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        sendLifecycleEvent("onDestroy");
    }

    private void sendLifecycleEvent(String eventName) {
        new MethodChannel(getFlutterEngine().getDartExecutor().getBinaryMessenger(), LIFECYCLE_CHANNEL)
                .invokeMethod(eventName, null);
    }
}
