package com.bdxh.hanlibrary;

import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Created by hanlijin on 2017/8/23 10:46
 * Mail：hanlijin@xihebeidou.com
 */
public class HttpRequest {
    private static final String TAG = "HttpRequest";
    private static String urlPath;
    private static final String loginPath = "/xhapi/app/v1/login";
    private static final String heartBeatPath = "/xhapi/app/v1/token/userOnlineHeartBeat";
    private static final String bingPath = "/xhapi/app/v1/token/bindingCustomerTerminal";
    private static final String getOnlineListPaht = "/xhapi/app/v1/token/getOnlineTerminalUserList";

    private final Handler sentHandler;
    private final Context mContext;

    public HttpRequest(Context context) {
        super();
        mContext = context;
        urlPath = "http://192.168.1.101:11000";
        HandlerThread postThread = new HandlerThread("heartRequestThread");
        postThread.start();
        sentHandler = new Handler(postThread.getLooper());
    }

    public void startTokenHandler() {
        sentHandler.post(heartRunnable);
    }

    public void reStartTokenHandler() {
        sentHandler.removeCallbacks(heartRunnable);
        startTokenHandler();
    }

    private final Runnable heartRunnable = new Runnable() {
        @Override
        public void run() {
            postHeartRequest();
            sentHandler.postDelayed(heartRunnable, 60 * 100);
        }
    };

    public boolean postLoginRequest(String name, String passwd, String phoneId) {
        try {
            URL url = new URL(urlPath + loginPath);
            JSONObject obj = new JSONObject();
            obj.put("username", name);
            obj.put("password", passwd);
            obj.put("tel_imei", phoneId);

            HttpURLConnection httpConn = (HttpURLConnection) url.openConnection();
            httpConn.setDoOutput(true);// 使用 URL 连接进行输出
            httpConn.setDoInput(true);// 使用 URL 连接进行输入
            httpConn.setUseCaches(false);// 忽略缓存
            httpConn.setInstanceFollowRedirects(true);
            httpConn.setRequestMethod("POST");// 设置URL请求方法
            httpConn.setRequestProperty("Content-Type", "application/json");
            httpConn.setRequestProperty("Connection", "Keep-Alive");
            httpConn.setRequestProperty("Charset", "utf-8");
            httpConn.connect();
            //POST请求
            DataOutputStream out = new DataOutputStream(httpConn.getOutputStream());
            out.writeBytes(obj.toString());
            out.flush();
            out.close();
            int responseCode = httpConn.getResponseCode();
            Log.d(TAG, "code = " + responseCode);
            if (HttpURLConnection.HTTP_OK == responseCode) {// 连接成功
                InputStream inStream = httpConn.getInputStream();
                String resString = new String(readInputStream(inStream));
                Log.d(TAG, resString);
            } else {
                Log.e(TAG, "code = " + responseCode);
                Toast.makeText(mContext, "登陆请求失败，设备未授权！", Toast.LENGTH_SHORT).show();
            }
        } catch (Exception ex) {
            ex.printStackTrace();
            Log.e(TAG, ex.toString());
            Toast.makeText(mContext, "服务器连接失败，请稍后重试", Toast.LENGTH_SHORT).show();
        }
        return true;
    }

    public static void postHeartRequest() {
        try {
            URL url = new URL("http://192.168.1.101:11000" + heartBeatPath + "?token=ZXlKaGJHY2lPaUpJVXpJMU5pSXNJblI1Y0NJNklrcFhWQ0o5LmV5SjFjMlZ5Ym1GdFpTSTZJblJsYzNRaUxDSjFjMlZ5YVdRaU9pSXhNREF3TURFaWZRLm5DZU9LRTVRMWpERjBsYWo1TXNnS1lUejRUcWI5bHNFQXVjb0FkcXRpQ0U=");
            JSONObject obj = new JSONObject();
            obj.put("user_id", "100003");
            obj.put("heartBeatTime", 123);
            obj.put("tel_imei", "862989035326751");
            obj.put("compellation", "韩立津");
            obj.put("terminal_id", "123456456");
            HttpURLConnection httpConn = (HttpURLConnection) url.openConnection();
            httpConn.setDoOutput(true);// 使用 URL 连接进行输出
            httpConn.setDoInput(true);// 使用 URL 连接进行输入
            httpConn.setUseCaches(false);// 忽略缓存
            httpConn.setInstanceFollowRedirects(true);
            httpConn.setRequestMethod("POST");// 设置URL请求方法
            httpConn.setRequestProperty("Content-Type", "application/json");
            httpConn.setRequestProperty("Connection", "Keep-Alive");
            httpConn.setRequestProperty("Charset", "utf-8");
            httpConn.connect();
            //POST请求
            DataOutputStream out = new DataOutputStream(httpConn.getOutputStream());
            out.writeBytes(obj.toString());
            out.flush();
            out.close();
            int responseCode = httpConn.getResponseCode();
            if (HttpURLConnection.HTTP_OK == responseCode) {// 连接成功
                InputStream inStream = httpConn.getInputStream();
                String resString = new String(readInputStream(inStream));
                System.out.println(resString);
            } else {
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private static byte[] readInputStream(InputStream inStream) throws Exception {
        ByteArrayOutputStream outStream = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        int len;
        while ((len = inStream.read(buffer)) != -1) {
            outStream.write(buffer, 0, len);
        }
        byte[] data = outStream.toByteArray();
        outStream.close();
        inStream.close();
        return data;
    }

}
