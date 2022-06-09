package com.darkneees.mobilerobotcv.nativeLib;

public class ImageLib {

    static {
        System.loadLibrary("ImageLib");
    }

    public native byte[] helloNative();
}
