/*26-02-2015 Multiple Streaming Testing Server*/
/*By Lee Jackson - Developed from the GStreamer Online Tutorials*/
/*Includes Addition of Emergency Alarm Option*/

#include <string.h>
#include <gst/gst.h>
#include <Functions.h>//definitions for functions



int track = 0;

/*Event to build pipeline and start playing*/
int buildPipeline (char* FilePath, CustomData *data)
{
  int ret;
  
  if (data->pipeline != NULL)
  {
    gst_element_set_state (data->pipeline, GST_STATE_NULL);
    gst_object_unref (data->audio_sink);
    gst_object_unref(data->file_src);
    gst_object_unref (data->pipeline);
    
  }

  
  data->pipeline = gst_parse_launch ("filesrc name=file-src ! decodebin ! audioconvert ! audio/x-raw-int,channels=1,rate=44100 ! rtpL16pay ! udpsink name=udp-sink ", NULL);

  /*Get udp sink and file source elements*/
  data->audio_sink = gst_bin_get_by_name (GST_BIN(data->pipeline), "udp-sink");
  data->file_src = gst_bin_get_by_name (GST_BIN(data->pipeline), "file-src");
  
  /*Configure UDP Sink*/
  g_object_set (G_OBJECT (data->audio_sink), "host", data->HostName, NULL);
  g_object_set (G_OBJECT (data->audio_sink), "port", 5000, NULL);

  /*Set File Source*/
  g_object_set (G_OBJECT (data->file_src), "location", FilePath, NULL);

  /* Start playing */
  ret = gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (data->pipeline);
    return -1;
  }
  data->playing = TRUE;
  data->rate = 1.0;
return 0;
}

void togglePlayPause(CustomData *data)
{
    data->playing = !data->playing;
    gst_element_set_state (data->pipeline, data->playing ? GST_STATE_PLAYING : GST_STATE_PAUSED);
    g_print ("Setting state to %s\n", data->playing ? "PLAYING" : "PAUSE");
}

/* Send seek event to change rate */
void send_seek_event (CustomData *data) {
  gint64 position;
  GstFormat format = GST_FORMAT_TIME;
  GstEvent *seek_event;
  
  /* Obtain the current position, needed for the seek event */
  if (!gst_element_query_position (data->pipeline, &format, &position)) {
    g_printerr ("Unable to retrieve current position.\n");
    return;
  }
  
  /* Create the seek event */
  if (data->rate > 0) {
    seek_event = gst_event_new_seek (data->rate, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE,
        GST_SEEK_TYPE_SET, position, GST_SEEK_TYPE_SET, -1);
  } else {
    seek_event = gst_event_new_seek (data->rate, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE,
        GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_SET, position);
  }
  
  if (data->audio_sink == NULL) {
    /* If we have not done so, obtain the sink through which we will send the seek events */
    data->audio_sink = gst_bin_get_by_name (GST_BIN(data->pipeline), "udp-sink");
  }
  
  /* Send the event */
  gst_element_send_event (data->audio_sink, seek_event);
  
  g_print ("Current rate: %g\n", data->rate);
}
