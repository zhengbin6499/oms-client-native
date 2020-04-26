// Copyright (C) <2018> Intel Corporation
//
// SPDX-License-Identifier: Apache-2.0
#include "owt/base/globalconfiguration.h"
namespace owt {
namespace base {
#if defined(WEBRTC_WIN)
// Enable hardware acceleration by default is on.
bool GlobalConfiguration::hardware_acceleration_enabled_ = true;
ID3D11Device* GlobalConfiguration::d3d11_decoding_device_ = nullptr;
#endif
int GlobalConfiguration::link_mtu_ = 0; // not set;
int GlobalConfiguration::min_port_ = 0; // not set;
int GlobalConfiguration::max_port_ = 0; // not set;
bool GlobalConfiguration::low_latency_streaming_enabled_ = false;
bool GlobalConfiguration::log_latency_to_file_enabled_ = false;
bool GlobalConfiguration::encoded_frame_ = false;

int GlobalConfiguration::delay_based_bwe_weight_ = 0;
std::unique_ptr<AudioFrameGeneratorInterface>
    GlobalConfiguration::audio_frame_generator_ = nullptr;
#if defined(WEBRTC_WIN) || defined(WEBRTC_LINUX)
std::unique_ptr<VideoDecoderInterface>
    GlobalConfiguration::video_decoder_ = nullptr;
#endif
#if defined(WEBRTC_IOS)
AudioProcessingSettings GlobalConfiguration::audio_processing_settings_ = {
    true, true, true, false};
#else
AudioProcessingSettings GlobalConfiguration::audio_processing_settings_ = {
    true, true, true, true};
#endif
}
}
