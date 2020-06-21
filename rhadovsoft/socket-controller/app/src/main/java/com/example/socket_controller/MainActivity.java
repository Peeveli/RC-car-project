package com.example.socket_controller;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.style.ForegroundColorSpan;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;

import com.google.android.material.textfield.TextInputEditText;

public class MainActivity extends AppCompatActivity {
    public static String ipaddr = "0.0.0.0"; //generating default ip
    TextInputEditText inputText;
    TextView totalTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //setup welcome screen objects
        totalTextView = (TextView) findViewById(R.id.totalText);
        inputText = (TextInputEditText) findViewById(R.id.ipinput);
        final Button ipbutton = (Button) findViewById(R.id.ipbutton);

        //listening the button
        ipbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ipaddr = inputText.getText().toString();
                totalTextView.setText(ipaddr);
                Intent nextActivity = new Intent(MainActivity.this, Main2Activity.class);
                startActivity(nextActivity);
            }
        });
    }
}
