package com.darkneees.mobilerobotcv;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class ApplicationRobot extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(ApplicationRobot.class.getResource("MainWindow.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 1024, 768);
        stage.setTitle("Control Robot App");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}