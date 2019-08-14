package com.htfix;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.widget.TextView;

import com.htfix.hook.ATest;
import com.htfix.hook.BaseInputConnectionReplace;
import com.htfix.hook.HookMethodInfo;
import com.htfix.hook.HookTest;

import lab.galaxy.yahfa.HookMain;

public class MainActivity extends Activity implements View.OnClickListener {

    private TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
    }

    private void initView() {
        findViewById(R.id.hook).setOnClickListener(this);
        findViewById(R.id.hook_cany).setOnClickListener(this);
        findViewById(R.id.start_activity).setOnClickListener(this);
        textView = findViewById(R.id.textview);
        setText();
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.hook) {
            BaseInputConnectionReplace.hookBaseInputConnectionReplace();
        } else if (v.getId() == R.id.start_activity) {
//            startActivity();
            HookTest.hootCanyTest();
            setText();
        } else if (v.getId() == R.id.hook_cany) {
//            hookCany();
            hookTest();
            setText();
        }
    }


    private void setText() {
        textView.setText(ATest.getText());
    }


    private void startActivity() {
        Intent intent = new Intent(this, InputConnectionActivity.class);
        this.startActivity(intent);
    }

    private void hookCany() {
        try {
            ClassLoader classLoader = getClassLoader();
            ClassLoader originClassLoader = BaseInputConnection.class.getClassLoader();
            HookMain.doHookDefault(HookMethodInfo.class.getName(), classLoader, originClassLoader);
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void hookTest() {
        HookTest.hootTest();
    }

}
