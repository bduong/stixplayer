package com.stix.player;

import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class ReceiverThread extends Thread{

    private TextView songTitle;
    private TextView artist;

    public ReceiverThread() {
        super();
    }

    public ReceiverThread(TextView songTitle) {
        super();
        this.songTitle = songTitle;
    }

    public ReceiverThread(TextView songTitle, TextView artist) {
        this.songTitle = songTitle;
        this.artist = artist;
    }

    public void waitForInfo() throws IOException {
        //ServerSocket socket = new ServerSocket(PlayerControl.RECEIVE_SOCKET_PORT);
        Socket clientSocket = new Socket(PlayerControl.GUMSTIX_IP, PlayerControl.RECEIVE_SOCKET_PORT);
        BufferedReader dataInputStream = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        final String title = dataInputStream.readLine();
        final String artistName = dataInputStream.readLine();
        clientSocket.close();
        songTitle.post(new Runnable() {
            @Override
            public void run() {
                songTitle.setText(title.trim());
            }
        });
        artist.post(new Runnable() {
            @Override
            public void run() {
                artist.setText(artistName.trim());
            }
        });
        //socket.close();
    }

    @Override
    public void run() {
        try {
            waitForInfo();
        } catch (IOException e) {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }
    }

    public void tearDown() {
    }

}
