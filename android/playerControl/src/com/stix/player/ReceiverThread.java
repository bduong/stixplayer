package com.stix.player;

import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

/**
 * The <code>ReceiverThread</code> gets the information of the currently playing song and
 * populates the UI with the information.
 */
public class ReceiverThread extends Thread{

    private TextView songTitle;
    private TextView artist;

    /**
     * @param songTitle the text box to fill with the song title
     * @param artist the text box to fill with the song artist
     */
    public ReceiverThread(TextView songTitle, TextView artist) {
        this.songTitle = songTitle;
        this.artist = artist;
    }

    /**
     * Gets the information for the currently playing song.
     *
     * @throws IOException If we cannot connect.
     */
    public void getInfo() throws IOException {
        Socket clientSocket = new Socket(PlayerControl.GUMSTIX_IP, PlayerControl.RECEIVE_SOCKET_PORT);
        BufferedReader dataInputStream = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        final String title = dataInputStream.readLine();
        final String artistName = dataInputStream.readLine();
        clientSocket.close();
        songTitle.post(new Runnable() {
            @Override
            public void run() {
                String setTitle = title.trim();
                songTitle.setText(setTitle.isEmpty() ? "No Title" : setTitle);
            }
        });
        artist.post(new Runnable() {
            @Override
            public void run() {
                String setArtist = artistName.trim();
                artist.setText(artistName.isEmpty() ? "No Artist" : setArtist);
            }
        });
    }

    @Override
    public void run() {
        try {
            getInfo();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
