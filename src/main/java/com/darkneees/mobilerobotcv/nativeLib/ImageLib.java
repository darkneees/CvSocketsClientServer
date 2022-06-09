package com.darkneees.mobilerobotcv.nativeLib;

import com.darkneees.mobilerobotcv.controllers.MainWindowController;
import javafx.application.Platform;

public class ImageLib {

    private MainWindowController mainWindowController;

    static {
        System.loadLibrary("ImageLib");
    }

    public ImageLib(MainWindowController mainWindowController) {
        this.mainWindowController = mainWindowController;
    }

    public native byte[] helloNative();

    public void setImageCameraView(byte[] pixels){

        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                mainWindowController.setImageCameraView(pixels);
            }
        });

    }
}
