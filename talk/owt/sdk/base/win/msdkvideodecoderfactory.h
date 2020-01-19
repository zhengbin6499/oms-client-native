// Copyright (C) <2018> Intel Corporation
//
// SPDX-License-Identifier: Apache-2.0

#ifndef OWT_BASE_WIN_MSDKVIDEODECODERFACTORY_H_
#define OWT_BASE_WIN_MSDKVIDEODECODERFACTORY_H_

#include <d3d11.h>
#include "webrtc/media/engine/webrtcvideodecoderfactory.h"

namespace owt {
namespace base {
// Declaration of MSDK based decoder factory.
class MSDKVideoDecoderFactory
    : public cricket::WebRtcVideoDecoderFactory {
public:
 explicit MSDKVideoDecoderFactory(ID3D11Device* d3d11_device_external);
 virtual ~MSDKVideoDecoderFactory();

 // WebRtcVideoDecoderFactory implementation.
 webrtc::VideoDecoder* CreateVideoDecoder(webrtc::VideoCodecType type) override;

 void DestroyVideoDecoder(webrtc::VideoDecoder* decoder) override;

private:
 std::vector<webrtc::VideoCodecType> supported_codec_types_;
 ID3D11Device* external_device_ = nullptr;
};
}
}  // namespace owt
#endif  // OWT_BASE_WIN_MSDKVIDEODECODERFACTORY_H_
