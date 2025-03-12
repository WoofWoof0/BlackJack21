package com.example.myapplication;

import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.LinearLayout;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("myapplication");
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
        // Get dealer and player hands as strings (e.g., "king_of_spades2 ace_of_hearts")
        String dealerHand = getDealerHand();
        String playerHand = getPlayerHand();
        Log.d("CARDS", "Dealer: " + dealerHand); // Add this
        Log.d("CARDS", "Player: " + playerHand); // Add this

        // Split into individual card names
        String[] dealerCards = dealerHand.split(" ");
        String[] playerCards = playerHand.split(" ");

        // Load dealer's cards
        LinearLayout dealerLayout = findViewById(R.id.dealerCardsLayout);
        dealerLayout.removeAllViews(); // Clear old cards
        for (String cardName : dealerCards) {
            ImageView imageView = new ImageView(this);
            LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
                    (int) getResources().getDimension(R.dimen.card_width),
                    (int) getResources().getDimension(R.dimen.card_height)
            );
            params.setMargins(4, 0, 4, 0);
            imageView.setLayoutParams(params);

            int resId = getResources().getIdentifier(cardName, "drawable", getPackageName());
            if (resId != 0) {
                imageView.setImageResource(resId);
            } else {
                Log.e("Missing_Image", "Missing: " + cardName);
                imageView.setImageResource(android.R.drawable.ic_menu_report_image);
            }

            dealerLayout.addView(imageView);
            imageView.setScaleType(ImageView.ScaleType.FIT_XY);
            imageView.setAdjustViewBounds(true);
            imageView.setImageResource(resId);
            dealerLayout.addView(imageView);
        }

        // Load player's cards similarly
        LinearLayout playerLayout = findViewById(R.id.playerCardsLayout);
        playerLayout.removeAllViews();
        for (String cardName : playerCards) {
            ImageView imageView = new ImageView(this);

            LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
                    (int) getResources().getDimension(R.dimen.card_width),
                    (int) getResources().getDimension(R.dimen.card_height)
            );
            params.setMargins(4, 0, 4, 0);
            imageView.setLayoutParams(params);

            int resId = getResources().getIdentifier(cardName, "drawable", getPackageName());
            if (resId != 0) {
                imageView.setImageResource(resId);
            } else {
                Log.e("Missing_Image", "Missing: " + cardName);
                imageView.setImageResource(android.R.drawable.ic_menu_report_image);
            }
            imageView.setScaleType(ImageView.ScaleType.FIT_XY);
            imageView.setAdjustViewBounds(true);
            imageView.setImageResource(resId);
            playerLayout.addView(imageView);
        }
        tvStatus.setText(getGameStatus());
    }
}
