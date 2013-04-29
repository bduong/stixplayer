package com.stix.player;

import android.widget.TextView;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ReceiverThread extends Thread{

    private TextView songTitle;
    private boolean appRunning;

    public ReceiverThread() {
        super();
    }

    public ReceiverThread(TextView songTitle) {
        this.songTitle = songTitle;
    }


    private void waitForInfo() throws IOException {
        ServerSocket socket = new ServerSocket(PlayerControl.SOCKET_NUMBER);
        Socket clientSocket = socket.accept();
        DataInputStream dataInputStream = new DataInputStream(clientSocket.getInputStream());
        String message = dataInputStream.readUTF();
        clientSocket.close();
        socket.close();
    }

    @Override
    public void run() {
        appRunning = true;
        try {
            waitForInfo();
        } catch (IOException e) {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }
    }

    public void tearDown() {
        appRunning = false;
    }

}
