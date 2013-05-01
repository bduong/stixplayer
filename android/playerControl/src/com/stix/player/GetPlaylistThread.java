package com.stix.player;

import android.app.Activity;
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
                    Button button = new Button(activity);
                    button.setText(message);
                    button.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            new TransmitterThread("Play " + String.valueOf(index)).start();
                        }
                    });
                    linearLayout.addView(button);
                }
            });
        }
//        text.post(new Runnable() {
//            @Override
//            public void run() {
//                text.setText(message);
//            }
//        });
        clientSocket.close();
        //socket.close();
    }
}
