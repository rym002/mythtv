#ifndef MYTHV4L2M2MCONTEXT_H
#define MYTHV4L2M2MCONTEXT_H

// MythTV
#include "mythcodeccontext.h"

class MythDRMPRIMEInterop;

class MythV4L2M2MContext : public MythCodecContext
{
  public:
    MythV4L2M2MContext(DecoderBase *Parent, MythCodecID CodecID);
   ~MythV4L2M2MContext() override;
    static MythCodecID GetSupportedCodec (AVCodecContext **Context,
                                          AVCodec       **Codec,
                                          const QString  &Decoder,
                                          AVStream       *Stream,
                                          uint            StreamType);
    void        InitVideoCodec           (AVCodecContext *Context, bool SelectedStream, bool &DirectRendering) override;
    bool        RetrieveFrame            (AVCodecContext *Context, VideoFrame *Frame, AVFrame *AvFrame) override;
    void        SetDecoderOptions        (AVCodecContext* Context, AVCodec* Codec) override;
    int         HwDecoderInit            (AVCodecContext *Context) override;
    static bool GetBuffer                (AVCodecContext *Context, VideoFrame *Frame, AVFrame *AvFrame, int);
    bool        GetDRMBuffer             (AVCodecContext *Context, VideoFrame *Frame, AVFrame *AvFrame, int);
    static bool HaveV4L2Codecs           (AVCodecID Codec = AV_CODEC_ID_NONE);
    static enum AVPixelFormat GetFormat  (AVCodecContext*, const AVPixelFormat *PixFmt);

  private:
    MythDRMPRIMEInterop *m_interop { nullptr };
};

#endif // MYTHV4L2M2MCONTEXT_H