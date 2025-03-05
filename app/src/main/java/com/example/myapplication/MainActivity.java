package com.example.myapplication;

import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("blackjack");
    }

    private native void nativeInit();
    private native void nativeHit();
    private native void nativeStand();
    private native String getDealerHand();
    private native String getPlayerHand();
    private native String getGameStatus();

    private TextView tvDealer,tvPlayer, tvStatus;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvDealer = findViewById(R.id.tvDealer);
        tvPlayer = findViewById(R.id.tvPlayer);
        tvStatus = findViewById(R.id.tvStatus);
        Button btnHit = findViewById(R.id.btnHit);
        Button btnStand = findViewById(R.id.btnStand);

        btnHit.setOnClickListener(v -> {
            nativeHit();
            updateUI();
        });

        btnStand.setOnClickListener(v -> {
            nativeStand();
            updateUI();
        });

        nativeInit();
        updateUI();
    }

    private void updateUI() {
        //Update the TextView with the data fetched from C++
        tvDealer.setText("Dealer: " + getDealerHand());
        tvPlayer.setText("Player: " + getPlayerHand());
        tvStatus.setText(getGameStatus());
    }
}
