package okcoin.oklink.liblink;

/**
 * Created by Administrator on 2014/12/26.
 */
public class MyClass {

    static {
        System.loadLibrary("bitlink");
        System.loadLibrary("link");
    }

    public native String getStringFromNative();

}
