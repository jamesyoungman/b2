#ifndef HEADER_B9662BEB223148FC8148B4BF707D4B3D// -*- mode:c++ -*-
#define HEADER_B9662BEB223148FC8148B4BF707D4B3D

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Global constants/defines/etc.

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if BUILD_TYPE_Debug

#define BBCMICRO_TRACE 1

#elif BUILD_TYPE_RelWithDebInfo

#define BBCMICRO_TRACE 1

#elif BUILD_TYPE_Final

#define BBCMICRO_TRACE 0

#else
#error unexpected build type
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Number of disc drives.
#define NUM_DRIVES (2)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define TRACK_VIDEO_LATENCY 0

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if BUILD_TYPE_Final

#else

#define VIDEO_TRACK_METADATA 1

#define BBCMICRO_DEBUGGER 1

#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define BBCMICRO_ENABLE_DISC_DRIVE_SOUND 1

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if BUILD_TYPE_Final

// The 1770 logging stuff is always stripped out in this mode.

#else

// if true, dump sector contents on completion of the corresponding
// type of command
#define WD1770_DUMP_WRITTEN_SECTOR 0
#define WD1770_DUMP_READ_SECTOR 0

// if true, log each byte read to/written from the data register
// during the corresponding type of command
#define WD1770_VERBOSE_READ_SECTOR 0
#define WD1770_VERBOSE_WRITE_SECTOR 0
#define WD1770_VERBOSE_WRITES 0

// if true, log every state change
#define WD1770_VERBOSE_STATE_CHANGES 0

#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Will probably dither over this at least once more...
//
// There's probably a better place for this.
static constexpr char ADDRESS_PREFIX_SEPARATOR='`';

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// It's safe to cast the TV_TEXTURE_xxx values to int/unsigned.

// 736 accommodates Boffin - though the play area part is 720, so
// presumably that's a safe area. (I suspect it didn't work on all
// TVs... on my TV, with RGB SCART input, the Boffin screen isn't
// centred. There's a gap on the right, and a missing bit on the left.
// Looks like it's 2 x 6845 columns too far to the left. With UHF
// input though it is roughly in the right place.)
//
// MOS CRTC values produce a centred image on a TV texture of width
// 720. Sadly that means there are 2 columns missing at the edge of
// the Boffin screen... so the TV texture is currently 736 wide, and
// modes are a bit off centre. Doesn't look like there are any
// settings that will work for everything, at least not without some
// hacks.
//
// (None of the modes are centred on my real M128 - though they're off
// centre differently from in the emulator, so obviously something's
// still wrong.)

static const int TV_TEXTURE_WIDTH=736;

static const int TV_TEXTURE_HEIGHT=288*2;

static_assert(TV_TEXTURE_WIDTH%8==0,"");

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif
