/*26-02-2015 Multiple Streaming Testing Server*/
/*By Lee Jackson - Developed from the GStreamer Online Tutorials*/
/*Includes Addition of Emergency Alarm Option*/

#include <string.h>
#include <gst/gst.h>

#define DEVICELIMIT 9999
#define HOSTIP1 "192.168.1.93"
#define HOSTIP2 "192.168.1.94"
  
typedef struct _CustomData {
  GstElement *pipeline;
  GstElement *audio_sink;
  GstElement *file_src;
  GMainLoop *loop;
  
  gboolean playing;  /* Playing or Paused */
  gdouble rate;      /* Current playback rate (can be negative) */
} CustomData;

CustomData data[DEVICELIMIT];
int track = 0;

/*Event to build pipeline and start playing*/
int buildPipeline (char* FilePath, char* HostName, int DeviceID)
{
  int ret;
  
  if (data[DeviceID].pipeline != NULL)
  {
    gst_element_set_state (data[DeviceID].pipeline, GST_STATE_NULL);
    gst_object_unref (data[DeviceID].audio_sink);
    gst_object_unref(data[DeviceID].file_src);
    gst_object_unref (data[DeviceID].pipeline);
    
  }

  
  data[DeviceID].pipeline = gst_parse_launch ("filesrc name=file-src ! decodebin ! audioconvert ! audio/x-raw-int,channels=1,depth=16,width=16,rate=44100 ! rtpL16pay ! udpsink name=udp-sink ", NULL);

  /*Get udp sink and file source elements*/
  data[DeviceID].audio_sink = gst_bin_get_by_name (GST_BIN(data[DeviceID].pipeline), "udp-sink");
  data[DeviceID].file_src = gst_bin_get_by_name (GST_BIN(data[DeviceID].pipeline), "file-src");
  
  /*Configure UDP Sink*/
  g_object_set (G_OBJECT (data[DeviceID].audio_sink), "host", HostName, NULL);
  g_object_set (G_OBJECT (data[DeviceID].audio_sink), "port", 5000, NULL);

  /*Set File Source*/
  g_object_set (G_OBJECT (data[DeviceID].file_src), "location", FilePath, NULL);

  /* Start playing */
  ret = gst_element_set_state (data[DeviceID].pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (data[DeviceID].pipeline);
    return -1;
  }
  data[DeviceID].playing = TRUE;
  data[DeviceID].rate = 1.0;

}

void togglePlayPause(char* Host, int DeviceID)
{
    data[DeviceID].playing = !data[DeviceID].playing;
    gst_element_set_state (data[DeviceID].pipeline, data[DeviceID].playing ? GST_STATE_PLAYING : GST_STATE_PAUSED);
    g_print ("Setting state to %s\n", data[DeviceID].playing ? "PLAYING" : "PAUSE");
}

/* Send seek event to change rate */
static void send_seek_event (CustomData *data) {
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
  
/* Process keyboard input */
static gboolean handle_keyboard (GIOChannel *source, GIOCondition cond, CustomData data[]) {
  gchar *str = NULL;
  int ret;
    
  if (g_io_channel_read_line (source, &str, NULL, NULL, NULL) != G_IO_STATUS_NORMAL) {
    return TRUE;
  }
  
  switch (g_ascii_tolower (str[0])) {
  case 'p':
    togglePlayPause(HOSTIP1, 1);
    break;
  case 'q':
	togglePlayPause(HOSTIP2, 2);
	break;
  case 'u':
    if (g_ascii_isupper (str[0])) {
      data[1].rate *= 1.5;
    } else {
      data[1].rate /= 1.5;
    }
    send_seek_event (&data[1]);
    break;
  case 's':
    if (g_ascii_isupper (str[0])) {
      data[2].rate *= 1.5;
    } else {
      data[2].rate /= 1.5;
    }
    send_seek_event (&data[2]);
    break;
  case 'd':
    data->rate *= -1.0;
    send_seek_event (&data[1]);
    break;
  case 'n':
	switch(track){
	case 0:
		ret = buildPipeline("/home/lee/EmbeddedProject/TestSong3.ogg", HOSTIP1, 1);
		track++;
		break;
	case 1:
		ret = buildPipeline("/home/lee/EmbeddedProject/TestSong.ogg", HOSTIP1, 1);
		track = 0;
		break;
	}
	break;
	case 'f;':
		ret = buildPipeline("/home/lee/EmbeddedProject/Alarm.ogg", HOSTIP1, 1);
		ret = buildPipeline("/home/lee/EmbeddedProject/Alarm.ogg", HOSTIP2, 2);
		break;
  default:
    break;
  }
  
  g_free (str);
  
  return TRUE;
}
  
int main(int argc, char *argv[]) {
  GstElement *sink;
  GstStateChangeReturn ret;
  GIOChannel *io_stdin;
  GstPad *pad;
  int i;
  
  /* Initialize GStreamer */
  gst_init (&argc, &argv);
  
  /* Initialize our data structure */
  memset (&data, 0, sizeof (data));
  
  /* Print usage map */
  g_print (
    "USAGE: Choose one of the following options, then press enter:\n"
    " 'P' to toggle between PAUSE and PLAY(Device 1)\n"
    " 'Q' to toggle between PAUSE and PLAY (Device 2)\n"
    " 'U' to increase playback speed, 'u' to decrease playback speed (Device 1)\n"
    " 'S' to increase playback speed, 's' to decrease playback speed (Device 2)\n"
    " 'D' to toggle playback direction\n"
    " 'N' to move to next frame (in the current direction, better in PAUSE)\n"
    );
  
  /* Call the Pipeline Build Function for both devices*/
  ret = buildPipeline("/home/lee/EmbeddedProject/TestSong.ogg", HOSTIP1, 1);
  
  ret = buildPipeline("/home/lee/EmbeddedProject/TestSong2.ogg", HOSTIP2, 2);
  
  /* Add a keyboard watch so we get notified of keystrokes */
#ifdef _WIN32
  io_stdin = g_io_channel_win32_new_fd (fileno (stdin));
#else
  io_stdin = g_io_channel_unix_new (fileno (stdin));
#endif
  g_io_add_watch (io_stdin, G_IO_IN, (GIOFunc)handle_keyboard, &data);
  
  /* Create a GLib Main Loop and set it to run */
  /*Loop for all Devices*/
  for(i=0; i<=DEVICELIMIT; i++)
  {
	data[i].loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (data[i].loop);
  }
  
  /* Free resources */
  /*Loop for all devices*/
  for(i=0; i<=DEVICELIMIT; i++)
  {
	g_main_loop_unref (data[i].loop);
	g_io_channel_unref (io_stdin);
	gst_element_set_state (data[i].pipeline, GST_STATE_NULL);
	if (data[i].audio_sink != NULL)
		gst_object_unref (data[i].audio_sink);
	gst_object_unref (data[i].pipeline);
  }
  return 0;
}
