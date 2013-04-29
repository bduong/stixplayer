//arm-linux-gcc player.c -lm -lmad -lasound -o player

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <mad.h>

#include <alsa/asoundlib.h>
#include "player.h"

#define ALSA_PCM_NEW_HW_PARAMS_API

snd_pcm_t *handle;

typedef struct buffer_s {
  unsigned char *buf;
  unsigned long len;
} buffer_t;

//static void mad_decode(unsigned char *, unsigned long);
/*
int main(int argc, char *argv[]){

  init();
   if (argc != 2) {
    fprintf(stderr, "Usage: ./madtest mp3file\n");
    return 1;
  }

  char *filename = argv[1];
  int ret;

  struct stat s;
  ret = stat(filename, &s);
  if (ret < 0) {
    fprintf(stderr, "Could not stat %s\n", filename);
    return 1;
  }

  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
  
  unsigned char *data;
  data = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE|MAP_FILE, fd, 0);
  if ((int)data == -1) {
    perror("mmap");
    close(fd);
    return 1;
  }

  mad_decode(data, s.st_size);

  munmap(data, s.st_size);
  close(fd);

  snd_pcm_drain(handle);
  snd_pcm_close(handle);

  return 0;
}
*/
static inline signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static enum mad_flow mad_input(void *data,
                               struct mad_stream *stream) {
  buffer_t *buffer = data;
  if (!buffer->len)
    return MAD_FLOW_STOP;
  mad_stream_buffer(stream, buffer->buf, buffer->len);
  buffer->len = 0;
  return MAD_FLOW_CONTINUE;
}

static enum mad_flow mad_output(void *data,
                                struct mad_header const *header,
                                struct mad_pcm *pcm) {
  int error;

        char _buf[4];
        unsigned int nchannels, nsamples;
        mad_fixed_t const *left_ch, *right_ch;
        signed int sample;
        int rc;
  
        nchannels = pcm->channels;
        nsamples  = pcm->length;

        left_ch   = pcm->samples[0];
        right_ch  = pcm->samples[1];
  
        while (nsamples--)
        {
                /* output sample(s) in 16-bit signed little-endian PCM */
                sample = scale(*left_ch++);
                _buf[0] = (sample >> 0) & 0xff;
                _buf[1] = (sample >> 8) & 0xff;
                sample = scale(*right_ch++);
                _buf[2] = (sample >> 0) & 0xff;
                _buf[3] = (sample >> 8) & 0xff;

                rc = snd_pcm_writei(handle, _buf, 1);
                if (rc == -EPIPE)
                {
                        /* EPIPE means underrun */
                        fprintf(stderr, "underrun occurred\n");
                        snd_pcm_prepare(handle);
                }
                else if (rc < 0)
                {
                        fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
                }
                else if (rc != (int)1)
                {
                        fprintf(stderr, "short write, write %d frames\n", rc);
                }
        }
        
        return MAD_FLOW_CONTINUE;
}

static enum mad_flow mad_error(void *data,
                               struct mad_stream *stream,
                               struct mad_frame *frame) {
  buffer_t *buffer = data;

  fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - buffer->buf);

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
  
}

void mad_decode(unsigned char *data, unsigned long size) {
  struct mad_decoder decoder;
  buffer_t buffer;

  buffer.buf = data;
  buffer.len = size;

  mad_decoder_init(&decoder, &buffer,
                   mad_input,
                   NULL, /* header */
                   NULL, /* filter */
                   mad_output,
                   mad_error,
                   NULL /* message */
                   );

  mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
  mad_decoder_finish(&decoder);
}

int init()
{
  int err;
  int rate, dir;
  snd_pcm_hw_params_t *hw_params;

  snd_pcm_uframes_t frames;

  rate=44100;
  frames = 1024;
  dir = 0;

  /* Open PCM device for playback. */
  err = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_PLAYBACK, 0);
  if (err < 0) {
    fprintf(stderr,
           "unable to open pcm device: %s\n",
            snd_strerror(err));
    return(1);
  }

  if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0)
  {
    fprintf (stderr,
            "cannot allocate hardware params (%s)\n",
	     snd_strerror (err));
   	     return (1);
  }
				 
  if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0)
  {
    fprintf (stderr,
            "cannot initialize hardware params (%s)\n",
             snd_strerror (err));
    return (1);
  }
	
  if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
    fprintf (stderr,
            "cannot set access type (%s)\n",
             snd_strerror (err));
    return (1);
  }
	
  if ((err = snd_pcm_hw_params_set_format (handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
  {
    fprintf (stderr,
            "cannot set sample format (%s)\n",
             snd_strerror (err));
    return (1);
  }
	
  if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &rate, &dir)) < 0)
  {
    fprintf (stderr,
            "cannot set sample rate (%s)\n",
             snd_strerror (err));
    return (1);
  }	
/*
  if ((err = snd_pcm_hw_params_set_period_size_near(handle, hw_params, &frames, &dir)) < 0)
  {
    fprintf (stderr,
            "cannot set period(%s)\n",
             snd_strerror (err));
    return (1);
  }
*/
  if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, 2)) < 0)
  {
    fprintf (stderr,
            "cannot set channel count (%s)\n",
             snd_strerror (err));
    return (1);
  }
	
  if ((err = snd_pcm_hw_params (handle, hw_params)) < 0)
  {
    fprintf (stderr,
            "cannot set parameters (%s)\n",
             snd_strerror (err));
    return (1);
  }
	
  snd_pcm_hw_params_free (hw_params);
	
  if ((err = snd_pcm_prepare (handle)) < 0)
  {
    fprintf (stderr,
            "cannot prepare audio interface for use (%s)\n",
	     snd_strerror (err));
    return (1);
  }

  return 0;
}

int close_handle()
{
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  return 0;
}