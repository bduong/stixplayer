package com.stix.player;

import android.app.Activity;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;



public class GetPlaylistThread extends Thread{

    private LinearLayout linearLayout;
    private Activity activity;

    public GetPlaylistThread(LinearLayout linearLayout, Activity activity) {
        this.linearLayout = linearLayout;
        this.activity = activity;
    }

    @Override
    public void run() {
        try {
            fetchPlayList();
        } catch (IOException e) {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }

    }

    public void fetchPlayList() throws IOException {
        Socket clientSocket = new Socket(PlayerControl.GUMSTIX_IP, PlayerControl.PLAYLIST_SOCKET_PORT);
        BufferedReader dataInputStream = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        long stopTime = System.currentTimeMillis() + 30000;

        linearLayout.post(new Runnable() {
            @Override
            public void run() {
                linearLayout.removeAllViews();
            }
        });
        int count = 0;
        while (System.currentTimeMillis() < stopTime) {
            final String message = dataInputStream.readLine();
            if ("done".equals(message)) break;
            final int index = count++;
            linearLayout.post(new Runnable() {
                @Override
                public void run() {
                    GradientDrawable gd = new GradientDrawable(
                            GradientDrawable.Orientation.TOP_BOTTOM,
                            new int[] {0xFF449def,0xFF2f6699});
                    gd.setCornerRadius(4f);
                    Button button = new Button(activity);
                    button.setText(message);
                    button.setBackgroundDrawable(gd);
                    button.setMinimumHeight(50);
                    button.setPadding(10,10,10,10);
                    button.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            new TransmitterThread("Play " + String.valueOf(index)).start();
                        }
                    });

                    linearLayout.addView(button);

                    View view = new View(activity);
                    view.setMinimumHeight(10);
                    linearLayout.addView(view);
                }
            });
        }
        clientSocket.close();
    }
}
