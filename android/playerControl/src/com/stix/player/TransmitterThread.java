package com.stix.player;

import java.io.*;
import java.net.Socket;

/**
 * The <code>TransmitterThread</code> sends a message to the server.
 */
public class TransmitterThread extends Thread{


    private String message;

    /**
     * @param message the message to send
     */
    public TransmitterThread(String message) {
        super();
        this.message = message;
    }

    /**
     * Send a message to the server.
     *
     * @throws IOException If we cannot connect.
     */
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
            e.printStackTrace();
        }
    }

}
