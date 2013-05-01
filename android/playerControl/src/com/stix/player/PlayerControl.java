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
    public static final String GUMSTIX_IP= "192.168.0.4";


    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        songTitle = (TextView) findViewById(R.id.SongTitle);
        artist = (TextView) findViewById(R.id.artist);

        final Button backButton = (Button) findViewById(R.id.back);
        backButton.setOnClickListener(new SendingClickListener("Back"));

        final Button pauseButton = (Button) findViewById(R.id.pause);
        pauseButton.setOnClickListener(new SendingClickListener("Pause"));

        final Button nextButton = (Button) findViewById(R.id.next);
        nextButton.setOnClickListener(new SendingClickListener("Next"));

        final Button stopButton = (Button) findViewById(R.id.stop);
        stopButton.setOnClickListener(new SendingClickListener("Stop"));

        final LinearLayout layout = (LinearLayout) findViewById(R.id.playlistView);


        final Button playlistbutton = (Button) findViewById(R.id.playlist);
        playlistbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new ReceiverThread(songTitle, artist ).start();
//                new GetPlaylistThread(layout, getParent()).start();
            }
        });

        Button hello = new Button(this);
        hello.setText("Hello");
        layout.addView(hello);

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