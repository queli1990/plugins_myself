package com.gochinatv;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import com.gochinatv.Util.Result;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements Discovery.Delegate {

  private JSONArray services;
  private boolean is_scan = false;
  private int volume = 10;

  @Override
  public void discovery_change() {
    services = Discovery.service("MEDIA_RENDERER");
    // services = Discovery.service("DIAL");

    String data = "";

    try {
      for (int i = 0; i < services.length(); i++) {
        JSONObject obj = services.getJSONObject(i);
        data += String.format("##friendly_name: %s, uuid: %s, hostname: %s, port: %d, location: %s, type: %s\n",
                obj.getString("friendly_name"),
                obj.getString("uuid"),
                obj.getString("hostname"),
                obj.getInt("port"),
                obj.getString("location"),
                obj.getString("type")
        );
      }

    } catch (JSONException e) {
      e.printStackTrace();
    }

    ((TextView)findViewById(R.id.text)).setText(data);
  }

  @Override
  public void discovery_error(String err) {

  }

  private ArrayList<String> get_hostname() {

    ArrayList<String> rv = new ArrayList<String>();

    if ( services == null ) {
      return rv;
    }

    try {
      for ( int i = 0; i < services.length(); i++) {
        JSONObject o = services.getJSONObject(i);
        String friendly_name = o.getString("friendly_name");
        String hostname = o.getString("hostname");
        rv.add(hostname);
      }
    } catch (JSONException ex) {
      ex.printStackTrace();
    }

    return rv;
  }
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    Discovery.setDelegate(this);

    findViewById(R.id.scan).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {
        is_scan = !is_scan;
        if ( is_scan ) {
          Discovery.scan("MEDIA_RENDERER");
          // Discovery.scan("DIAL");
          ((Button)findViewById(R.id.scan)).setText("StopScan");
          ((TextView)findViewById(R.id.text)).setText("Scaning...");
        } else {
          Discovery.stop();
          ((Button)findViewById(R.id.scan)).setText("Scan");
        }
      }
    });

    findViewById(R.id.start).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {
      for ( String name: get_hostname() ) {
        MediaRenderer.setAVTransportURI(name,
                "http://huace.cdn.ottcloud.tv/huace/videos/dst/2016/08/14461-ai-de-zhui-zong-01-ji_201608180551/14461-ai-de-zhui-zong-01-ji.m3u8");
      }
      }
    });

    findViewById(R.id.stop).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {
        for ( String name: get_hostname() ) {
          MediaRenderer.stop(name);
        }
      }
    });

    findViewById(R.id.play).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {
        for ( String name: get_hostname() ) {
          MediaRenderer.play(name);
        }
      }
    });

    findViewById(R.id.pause).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {
        for ( String name: get_hostname() ) {
          MediaRenderer.pause(name);
        }
      }
    });

    findViewById(R.id.seek).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {
        for ( String name: get_hostname() ) {
          MediaRenderer.seek(name, 1000);
        }
      }
    });

    findViewById(R.id.set_volume_1).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {
        for ( String name: get_hostname() ) {
          MediaRenderer.setVolume(name, volume--);
        }
      }
    });

    findViewById(R.id.set_volume_2).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {
        for ( String name: get_hostname() ) {
          MediaRenderer.setVolume(name, volume++);
        }
      }
    });

    findViewById(R.id.info).setOnClickListener(new View.OnClickListener() {
      public void onClick(View view) {

        String data = "";

        for ( String name: get_hostname() ) {

          Result res1 = MediaRenderer.getMediaInfo(name);
          Result res2 = MediaRenderer.getPositionInfo(name);
          Result res3 = MediaRenderer.getVolume(name);

          if (res2.code == 0) {
            try {
              data += String.format("##MediaDuration: %d, RelTime: %d \n##TrackURI: %s\n",
                      // es.data.getInt("MediaDuration"),
                      res2.data.getInt("TrackDuration"),
                      res2.data.getInt("RelTime"),
                      res2.data.getString("TrackURI")
              );
            } catch (JSONException e) {
              e.printStackTrace();
            }
          }

          if (res3.code == 0) {
            try {
              data += String.format("##CurrentVolume: %d", res3.data.getInt("CurrentVolume"));
            } catch (JSONException e) {
              e.printStackTrace();
            }
          }
        }

        ((TextView)findViewById(R.id.text2)).setText(data);
      }
    });
  }
}