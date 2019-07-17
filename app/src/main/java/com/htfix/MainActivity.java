package com.htfix;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.htfix.hook.BaseInputConnectionReplace;
import com.htfix.hook.Hook;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
    }

    private void initView() {
        findViewById(R.id.hook).setOnClickListener(this);
        findViewById(R.id.start_activity).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.hook) {
            Hook.hook(getPackageName());
            BaseInputConnectionReplace.hookBaseInputConnectionReplace();
        } else if (v.getId() == R.id.start_activity) {
            startActivity();
        }
    }

    private void startActivity() {
        Intent intent = new Intent(this, InputConnectionActivity.class);
        this.startActivity(intent);
    }
}
