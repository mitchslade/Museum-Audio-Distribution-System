/*
 * Functions.h
 *
 *  Created on: 22 Nov 2014
 *      Author: James Harvey
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <gst/gst.h>
#define LOGON 128
#define LOGOFF 64
#define PLAY 1
#define PAUSE 2
#define REWIND 4
#define REWIND_STOP 5
#define FAST_FORWARD 8
#define FAST_FORWARD_STOP 9
#define SRV_RESPONSE 255
#define FILE_REQ 16
  GstElement *sink;
  GstStateChangeReturn ret;
  GIOChannel *io_stdin;
  GstPad *pad;
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
  char* HostName;
} CustomData;
void send_seek_event (CustomData *data);
void togglePlayPause(CustomData *data);
int buildPipeline (char* FilePath, CustomData *data);
#endif
