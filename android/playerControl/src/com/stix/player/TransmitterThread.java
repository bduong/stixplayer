package com.stix.player;

import java.io.*;
import java.net.Socket;

public class TransmitterThread extends Thread{


    private String message;

    public TransmitterThread(String message) {
        super();
        this.message = message;
    }

    public void sendMessage() throws IOException {
        Socket clientSocket = new Socket(PlayerControl.GUMSTIX_IP, PlayerControl.TRANSMIT_SOCKET_PORT);
        BufferedWriter dataInputStream = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
        dataInputStream.write(message + '\0');
        dataInputStream.flush();
        dataInputStream.close();
        clientSocket.close();
    }

    @Override
    public void run() {
        try {
            sendMessage();
        } catch (IOException e) {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }
    }

}
