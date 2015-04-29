/*26-03-2015 Multiple Streaming Testing Server – Broken into separate functions*/
/*For use with corresponding server.c file*/
/*By Lee Jackson - Developed from the GStreamer Online Tutorials*/
/*Includes Addition of Emergency Alarm Option*/

#include <string.h>
#include <gst/gst.h>
#include <Functions.h>//!definitions for functions



int track = 0;//<!Track ID - unused


int buildPipeline(char* FilePath, CustomData *data)//!Function to build pipeline and start playing
{
	int ret;

	if (data->pipeline != NULL)//!If old pipeline
	exists, set to null and remove references
	{
		gst_element_set_state(data->pipeline, GST_STATE_NULL);
		gst_object_unref(data->audio_sink);
		gst_object_unref(data->file_src);
		gst_object_unref(data->pipeline);

	}

	data->pipeline = gst_parse_launch("filesrc name=file-src ! decodebin ! audioconvert ! audio/x-raw-int,channels=1,rate=44100 ! rtpL16pay ! udpsink name=udp-sink ", NULL);//! Launch new pipeline with unconfigured UDP sink and file source


	data->audio_sink = gst_bin_get_by_name(GST_BIN(data->pipeline), "udp-sink");//!Get udp sink and file source elements
	data->file_src = gst_bin_get_by_name(GST_BIN(data->pipeline), "file-src");

	g_object_set(G_OBJECT(data->audio_sink), "host", data->HostName, NULL);//!Configure UDP Sink
	g_object_set(G_OBJECT(data->audio_sink), "port", 5000, NULL);

	g_object_set(G_OBJECT(data->file_src), "location", FilePath, NULL);//!Set File Source

	ret = gst_element_set_state(data->pipeline, GST_STATE_PLAYING);//!Set the pipeline to the playing state
	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr("Unable to set the pipeline to the playing state.\n");//!Return error message if unsuccessful
		gst_object_unref(data->pipeline);
		return -1;
	}
	data->playing = TRUE;//!Set state and data rate variables
	data->rate = 1.0;
	return 0;
}

void togglePlayPause(CustomData *data)//!Function to toggle a pipeline between the paused and playing state
{
	data->playing = !data->playing;//!Toggles state variable
	gst_element_set_state(data->pipeline, data->playing ? GST_STATE_PLAYING : GST_STATE_PAUSED);//!Passes new state to pipeline
	g_print("Setting state to %s\n", data->playing ? "PLAYING" : "PAUSE");//!Prints console message for debug
}


void send_seek_event(CustomData *data) {//!Function to send a seek event to change data rate
	gint64 position;//<!track position
	GstFormat format = GST_FORMAT_TIME;//<!time format
	GstEvent *seek_event;//<!seek event


	if (!gst_element_query_position(data->pipeline, &format, &position)) {//! Obtain the current position, needed for the seek event
		g_printerr("Unable to retrieve current position.\n");
		return;
	}


	if (data->rate > 0) {//!Create the seek event suitable for forward playback
		seek_event = gst_event_new_seek(data->rate, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE,
			GST_SEEK_TYPE_SET, position, GST_SEEK_TYPE_SET, -1);
	}
	else {//!Else create the seek event suitable for reverse playback
		seek_event = gst_event_new_seek(data->rate, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE,
			GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_SET, position);
	}

	if (data->audio_sink == NULL) {
		//!If we have not done so, obtain the sink through which we will send the seek events
		data->audio_sink = gst_bin_get_by_name(GST_BIN(data->pipeline), "udp-sink");
	}

	//!Send the event
	gst_element_send_event(data->audio_sink, seek_event);

	g_print("Current rate: %g\n", data->rate);//!Print console message for debug
}
