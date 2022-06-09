package com.darkneees.mobilerobotcv.controllers;

import com.darkneees.mobilerobotcv.nativeLib.ImageLib;
import javafx.embed.swing.SwingFXUtils;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.image.ImageView;

import java.awt.image.BufferedImage;
import java.io.IOException;

public class MainWindowController {

    @FXML
    private ImageView ImageCameraView;
    @FXML
    private ListView ListPoints;
    @FXML
    private Button DeletePointButton;
    @FXML
    private Button MenuButton;
    @FXML
    private Button ServoLeftButton;
    @FXML
    private Button ServoRightButton;
    @FXML
    private Button RobotUpLeftButton;
    @FXML
    private Button RobotForwardButton;
    @FXML
    private Button RobotRightUpButton;
    @FXML
    private Button RobotLeftButton;
    @FXML
    private Button RobotRightButton;
    @FXML
    private Button RobotLeftBackButton;
    @FXML
    private Button RobotBackButton;
    @FXML
    private Button RobotRightBackButton;

    @FXML
    public void initialize() throws IOException {

        //Client client = new Client(ImageCameraView);
        //client.connect();
        //client.start();

        ImageLib imageLib = new ImageLib();
        byte[] pixels = imageLib.helloNative();

        BufferedImage bufferedImage = new BufferedImage(640, 480, BufferedImage.TYPE_INT_RGB);


        final int pixelLength = 3;
        for (int pixel = 0, row = 0, col = 0; pixel + 2 < pixels.length; pixel += pixelLength) {
            int argb = 0;
            argb += -16777216; // 255 alpha
            argb += ((int) pixels[pixel] & 0xff); // blue
            argb += (((int) pixels[pixel + 1] & 0xff) << 8); // green
            argb += (((int) pixels[pixel + 2] & 0xff) << 16); // red
            bufferedImage.setRGB(col, row, argb);
            col++;
            if (col == 640) {
                col = 0;
                row++;
            }
        }
        ImageCameraView.setImage(SwingFXUtils.toFXImage(bufferedImage, null));
        DeletePointButton.setOnAction(event -> System.out.println("Clicked"));

    }
}
