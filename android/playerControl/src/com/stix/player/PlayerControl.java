package com.stix.player;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.*;

public class PlayerControl extends Activity {

    private TextView songTitle;
    private TextView artist;
    public static final int RECEIVE_SOCKET_PORT = 5000;
    public static final int TRANSMIT_SOCKET_PORT =5001;
    public static final int PLAYLIST_SOCKET_PORT = 5002;
    public static final String GUMSTIX_IP= "192.168.0.4";
    public Activity thisActivity;


    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        thisActivity = this;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);


        songTitle = (TextView) findViewById(R.id.SongTitle);
        artist = (TextView) findViewById(R.id.artist);

        final ImageButton refresh = (ImageButton) findViewById(R.id.refresh);
        refresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new ReceiverThread(songTitle, artist).start();
            }
        });

        final ImageButton backButton = (ImageButton) findViewById(R.id.back);
        backButton.setOnClickListener(new SendingClickListener("Back"));

        final ImageButton pauseButton = (ImageButton) findViewById(R.id.pause);
        pauseButton.setOnClickListener(new SendingClickListener("Pause"));

        final ImageButton nextButton = (ImageButton) findViewById(R.id.next);
        nextButton.setOnClickListener(new SendingClickListener("Next"));

        final ImageButton stopButton = (ImageButton) findViewById(R.id.stop);
        stopButton.setOnClickListener(new SendingClickListener("Stop"));

        final LinearLayout layout = (LinearLayout) findViewById(R.id.playlistView);
        layout.removeAllViews();


        final Button playlistbutton = (Button) findViewById(R.id.playlist);
        playlistbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new GetPlaylistThread(layout, thisActivity).start();
            }
        });
    }

    public class SendingClickListener implements View.OnClickListener {

        private String message;

        public SendingClickListener(String message) {
            this.message = message;
        }

        @Override
        public void onClick(View view) {
            new TransmitterThread(message).start();
        }
    }
}