// Copyright (C) <2018> Intel Corporation
//
// SPDX-License-Identifier: Apache-2.0

#include "talk/owt/sdk/base/win/msdkvideodecoderfactory.h"
#include "talk/owt/sdk/base/win/msdkvideodecoder.h"
#include "talk/owt/sdk/base/win/d3d11va_h264_decoder.h"

namespace owt {
namespace base {

MSDKVideoDecoderFactory::MSDKVideoDecoderFactory(ID3D11Device* d3d11_device_external) {
  supported_codec_types_.clear();
  
  supported_codec_types_.push_back(webrtc::kVideoCodecVP8);

  bool is_h264_hw_supported = true;
  if (is_h264_hw_supported) {
    supported_codec_types_.push_back(webrtc::kVideoCodecH264);
  }
#ifndef DISABLE_H265
// TODO: Add logic to detect plugin by MSDK.
  bool is_h265_hw_supported = true;
  if (is_h265_hw_supported) {
    supported_codec_types_.push_back(webrtc::kVideoCodecH265);
  }
#endif
  external_device_ = d3d11_device_external;
}

MSDKVideoDecoderFactory::~MSDKVideoDecoderFactory() {
}

webrtc::VideoDecoder* MSDKVideoDecoderFactory::CreateVideoDecoder(webrtc::VideoCodecType type) {
  if (supported_codec_types_.empty()) {
    return nullptr;
  }
  RTC_LOG(LS_ERROR) << "VideoDecoderFactory: try to setup decoder.";
  for (std::vector<webrtc::VideoCodecType>::const_iterator it =
    supported_codec_types_.begin(); it != supported_codec_types_.end();
    ++it) {
    if (*it == type && type == webrtc::kVideoCodecVP8) {
      return new owt::base::MSDKVideoDecoder(type, external_device_);
    } else if (*it == type && type == webrtc::kVideoCodecH264) {
      return new owt::base::H264DXVADecoderImpl(external_device_);
#ifndef DISABLE_H265
    } else if (*it == type && type == webrtc::kVideoCodecH265) {
      return new MSDKVideoDecoder(type, external_device_);
#endif
    }
  }
  return nullptr;
}

void MSDKVideoDecoderFactory::DestroyVideoDecoder(webrtc::VideoDecoder* decoder) {
  delete decoder;
}
}  // namespace base
}  // namespace owt
