package com.example.PlantWateringApp.services;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class BackendCommunicationService {

    private static final String BackendEndpoint = "https://api.psam.io/PlantWateringData";

    public void Write(double humidity, double threshold) throws IOException, JSONException {
        URL url = new URL(BackendEndpoint);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setConnectTimeout(5000);
        conn.setRequestProperty("Content-Type", "application/json; charset=UTF-8");
        conn.setDoOutput(true);
        conn.setDoInput(true);
        conn.setRequestMethod("POST");

        JSONObject payload = new JSONObject();
        payload.put("humidity", humidity);
        payload.put("threshold", threshold);

        OutputStream os = conn.getOutputStream();
        os.write(payload.toString().getBytes("UTF-8"));
        os.close();

        conn.disconnect();
    }

    public void Get() {

    }

}
