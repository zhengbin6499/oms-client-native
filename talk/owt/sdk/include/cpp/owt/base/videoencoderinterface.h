// Copyright (C) <2018> Intel Corporation
//
// SPDX-License-Identifier: Apache-2.0
#ifndef OWT_BASE_VIDEOENCODERINTERFACE_H_
#define OWT_BASE_VIDEOENCODERINTERFACE_H_
#include <memory>
#include <vector>
#include "owt/base/commontypes.h"
namespace owt {
namespace base {

struct EncodedImageMetaData {
  // The picture id of frame.
  uint16_t picture_id;
  // Indicate if current data is the end of a frame which might be sent slice-by-slice
  bool last_fragment = true;
  // Capture timestamp
  uint64_t capture_timestamp;
  // Start encoding time in ms
  uint64_t encoding_start;
  // End encoding time in ms
  uint64_t encoding_end;
};

/**
  @brief Video encoder interface
  @details Internal webrtc encoder will request from this
   interface when it needs one complete encoded frame.
*/
class VideoEncoderInterface {
  public:
  /**
   @brief Destructor
   */
   virtual ~VideoEncoderInterface() {}
  /**
   @brief Initialize the customized video encoder
   @param resolution Resolution of frame to be encoded.
   @param fps Estimated frame rate expected.
   @param bitrate_kbps bitrate in kbps the caller expect the encoder to
   output at current resolution and frame rate.
   @param video_codec codec type requested.
   @return Return true if successfully inited the encoder context; Return
   false on failing to init the encoder context.
   */
  virtual bool InitEncoderContext(Resolution& resolution,
      uint32_t fps, uint32_t bitrate_kbps, VideoCodec video_codec) = 0;
#ifdef WEBRTC_ANDROID
  virtual uint32_t EncodeOneFrame(bool key_frame, uint8_t** data) = 0;
#else
  /**
   @brief Retrieve byte buffer from encoder that holds one complete frame.
   @details The buffer is provided by caller and EncodedOneFrame implementation should
   copy encoded data to this buffer. After return, the caller owns the buffer and
   VideoEncoderInterface implementation should not assume the buffer valid.
   @param buffer Output buffer that holds the encoded data.
   @param key_frame Indicates whether we're requesting an AU representing an key frame.
   @param meta_data The returned metadata of the encoded frame.
   @return Returns true if the encoder successfully returns one frame; returns false
   if the encoder fails to encode one frame.
   */
  virtual bool EncodeOneFrame(std::vector<uint8_t>& buffer,
                              bool key_frame,
                              EncodedImageMetaData& meta_data) = 0;
#endif
  /**
   @brief Release the resources that current encoder holds.
   @return Return true if successfully released the encoder; return false if
   the release fails.
  */
  virtual bool Release() = 0;
  /**
   @brief Duplicate the VideoEncoderInterface instance.
   @return The newly created VideoEncoderInterface instance.
   */
  virtual VideoEncoderInterface* Copy() = 0;
};
}
}
#endif  // OWT_BASE_VIDEOENCODERINTERFACE_H_
