package com.darkneees.mobilerobotcv;

import javafx.application.Platform;
import javafx.embed.swing.SwingFXUtils;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

import static java.awt.image.BufferedImage.TYPE_3BYTE_BGR;

public class Client extends Thread {

    private final String host = "";
    private final int port = 8888;
    private final ImageView view;
    private Socket client;

    private int WIDTH = 640;
    private int HEIGHT = 480;

    private DataInputStream in;
    private DataOutputStream out;

    public Client(ImageView view){
        this.view = view;
    }

    public void connect(){
        try {
            client = new Socket(host, port);
        } catch (IOException e) {
            System.out.println("Неудачная попытка подключения: " + e.getMessage());
        }

        System.out.println("Подключение успешно!");
    }

    public void disconnect(){
        try {
            out.write('b');
            out.flush();
        } catch (IOException e) {
            System.out.println("[disconnect]: Не удалось отправить сообщение серверу: " + e.getMessage());
        }
    }

    @Override
    public void run() {
        while (true) {
            try {
                getImageFromServer();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public BufferedImage getImageFromServer() throws IOException {

        try {

            in = new DataInputStream(client.getInputStream());
            out = new DataOutputStream(client.getOutputStream());

            int sizeImage = in.readInt();

            sizeImage = 921600;

            out.write('A');

            List<byte[]> bytes = new ArrayList<>();
            int i = 0;
            while (i < sizeImage) {
                byte[] buffData = in.readNBytes(4096);
                bytes.add(buffData);
                i += 4096;
            }

            System.out.println("Успешно!");
//
            BufferedImage bufferedImage = new BufferedImage(WIDTH, HEIGHT, TYPE_3BYTE_BGR);
            final int pixelLength = 3;
            int row = 0, col = 0;
            for(byte[] b: bytes) {
                for (int pixel = 0; pixel + 2 < b.length; pixel += pixelLength) {
                    int argb = 0;
                    argb += -16777216; // 255 alpha
                    argb += ((int) b[pixel] & 0xff); // blue
                    argb += (((int) b[pixel + 1] & 0xff) << 8); // green
                    argb += (((int) b[pixel + 2] & 0xff) << 16); // red
                    bufferedImage.setRGB(col, row, argb);
                    col++;
                    if (col == WIDTH) {
                        col = 0;
                        row++;
                    }
                }
            }


            Platform.runLater(new Runnable() {
                @Override
                public void run() {
                    Image image = SwingFXUtils.toFXImage(bufferedImage, null);
                    view.setImage(image);

                }
            });
        } catch (IOException e) {
            System.out.println("Не удалось получить картину: " + e.getMessage());
        }
        return null;
    }
}
