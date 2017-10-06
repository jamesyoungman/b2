#ifndef HEADER_5658C00A506F464FA4F6A3A3803849A4 // -*- mode:c++ -*-
#define HEADER_5658C00A506F464FA4F6A3A3803849A4

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include <shared/enum_decl.h>
#include "TVOutput.inl"
#include <shared/enum_end.h>

#include <memory>
#include <vector>
#include <beeb/OutputData.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct VideoDataUnit;
union VideoDataHalfUnit;
struct SDL_PixelFormat;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// TVOutput is the analogue of a combination of the video encoding and
// the TV - it looks after converting a stream of video data chunks
// into a graphical display.
//
// The texture is always TV_TEXTURE_WIDTH*TV_TEXTURE_HEIGHT, and its
// stride is TV_OUTPUT_WIDTH*4.

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class TVOutput {
public:
    TVOutput();
    ~TVOutput();

    bool InitTexture(const SDL_PixelFormat *pixel_format);

    // returns number of us consumed.
    void Update(const VideoDataUnit *units,size_t num_units);

    void UpdateOneHalfUnit(const VideoDataHalfUnit *hu,float amt);
    
    // *data_version (optional) is set to texture data version, incremented on
    // each vblank. (Between vblanks, the buffer contains a partially scanned-out frame.)
    const void *GetTextureData(uint64_t *texture_data_version) const;

    // returns pointer to TVOutput's copy of the pixel format.
    const SDL_PixelFormat *GetPixelFormat() const;

    bool IsInVerticalBlank() const;

     double GetGamma() const;
    void SetGamma(double gamma);
protected:
private:
    TVOutputState m_state=TVOutputState_VerticalRetrace;
    uint32_t *m_line=nullptr;
    size_t m_x=0;
    size_t m_y=0;
    int m_state_timer=0;

    uint32_t m_palette[2][64]={};
    uint32_t m_reds[16]={};
    uint32_t m_greens[16]={};
    uint32_t m_blues[16]={};
    SDL_PixelFormat *m_pixel_format=nullptr;
    size_t m_num_fields=0;

    // TV - output texture and its properties
    std::vector<uint32_t> m_texture_data;
    uint64_t m_texture_data_version=1;

#if TRACK_VIDEO_LATENCY
    uint64_t render_latency_ticks;
    size_t num_renders;
#endif

    double m_gamma=2.2;

    void InitPalette(size_t palette,double fa);
    void InitPalette();
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif
