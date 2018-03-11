/*
 * Intel License
 */

#ifndef WOOGEEN_BASE_PEERCONNECTIONCHANNEL_H_
#define WOOGEEN_BASE_PEERCONNECTIONCHANNEL_H_

#include <vector>

#include "webrtc/rtc_base/messagehandler.h"
#include "webrtc/rtc_base/sigslot.h"

#include "talk/ics/sdk/base/peerconnectiondependencyfactory.h"
#include "talk/ics/sdk/base/mediaconstraintsimpl.h"
#include "talk/ics/sdk/base/functionalobserver.h"
#include "talk/ics/sdk/include/cpp/ics/base/commontypes.h"

namespace rtc {
class NetworkMonitorInterface;
}

namespace ics {
namespace base {

using webrtc::PeerConnectionInterface;

struct SetSessionDescriptionMessage : public rtc::MessageData {
  explicit SetSessionDescriptionMessage(
      FunctionalSetSessionDescriptionObserver* observer,
      webrtc::SessionDescriptionInterface* desc)
      : observer(observer), description(desc) {}

  virtual ~SetSessionDescriptionMessage() {
    delete description;
  }

  rtc::scoped_refptr<FunctionalSetSessionDescriptionObserver> observer;
  webrtc::SessionDescriptionInterface* description;
};

struct GetStatsMessage : public rtc::MessageData {
  explicit GetStatsMessage(
      FunctionalStatsObserver* observer,
      webrtc::MediaStreamInterface* stream,
      webrtc::PeerConnectionInterface::StatsOutputLevel level)
      : observer(observer), stream(stream), level(level) {}

  rtc::scoped_refptr<FunctionalStatsObserver> observer;
  webrtc::MediaStreamInterface* stream;
  webrtc::PeerConnectionInterface::StatsOutputLevel level;
};

struct GetNativeStatsMessage : public rtc::MessageData {
  explicit GetNativeStatsMessage(
      FunctionalNativeStatsObserver* observer,
      webrtc::MediaStreamInterface* stream,
      webrtc::PeerConnectionInterface::StatsOutputLevel level)
      : observer(observer), stream(stream), level(level) {}
  rtc::scoped_refptr<FunctionalNativeStatsObserver> observer;
  webrtc::MediaStreamInterface* stream;
  webrtc::PeerConnectionInterface::StatsOutputLevel level;
};

struct PeerConnectionChannelConfiguration
    : public webrtc::PeerConnectionInterface::RTCConfiguration {
 public:
  explicit PeerConnectionChannelConfiguration();

  std::vector<VideoEncodingParameters> video;
  std::vector<AudioEncodingParameters> audio;

  /// Indicate whether this PeerConnection is used for sending encoded frame.
  bool encoded_video_frame_;
};

class PeerConnectionChannel : public rtc::MessageHandler,
                              public webrtc::PeerConnectionObserver,
                              public webrtc::DataChannelObserver,
                              public sigslot::has_slots<> {
 public:
  PeerConnectionChannel(PeerConnectionChannelConfiguration configuration);

 protected:
  virtual ~PeerConnectionChannel();
  bool InitializePeerConnection();
  const webrtc::SessionDescriptionInterface* LocalDescription();
  PeerConnectionInterface::SignalingState SignalingState() const;

  // Apply the bitrate settings on all tracks available. Failing to set any of them
  // will result in a false return, with remaining settings applicable still applied.
  // Subclasses can override this to implementation specific bitrate allocation policies.
  bool ApplyBitrateSettings();
  // Subclasses should prepare observers for these functions and post
  // message to PeerConnectionChannel.
  virtual void CreateOffer() = 0;
  virtual void CreateAnswer() = 0;

  // Message looper event
  virtual void OnMessage(rtc::Message* msg);

  // PeerConnectionObserver
  virtual void OnStateChange(StateType state_changed){};
  virtual void OnSignalingChange(
      PeerConnectionInterface::SignalingState new_state);
  virtual void OnAddStream(
      rtc::scoped_refptr<MediaStreamInterface> stream) override;
  virtual void OnRemoveStream(
      rtc::scoped_refptr<MediaStreamInterface> stream) override;
  virtual void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;
  virtual void OnRenegotiationNeeded();
  virtual void OnIceConnectionChange(
      PeerConnectionInterface::IceConnectionState new_state);
  virtual void OnIceGatheringChange(
      PeerConnectionInterface::IceGatheringState new_state);
  virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);

  // DataChannelObserver proxy
  // Data channel events will be bridged to these methods to avoid name
  // conflict.
  virtual void OnDataChannelStateChange(){};
  virtual void OnDataChannelMessage(const webrtc::DataBuffer& buffer){};

  // CreateSessionDescriptionObserver
  virtual void OnCreateSessionDescriptionSuccess(
      webrtc::SessionDescriptionInterface* desc);
  virtual void OnCreateSessionDescriptionFailure(const std::string& error);

  // SetSessionDescriptionObserver
  virtual void OnSetLocalSessionDescriptionSuccess();
  virtual void OnSetLocalSessionDescriptionFailure(const std::string& error);
  virtual void OnSetRemoteSessionDescriptionSuccess();
  virtual void OnSetRemoteSessionDescriptionFailure(const std::string& error);

  // Fired when networks changed. (Only works on iOS)
  virtual void OnNetworksChanged();

  enum MessageType : int {
    kMessageTypeCreateOffer = 101,
    kMessageTypeCreateAnswer,
    kMessageTypeCreateDataChannel,
    kMessageTypeSetLocalDescription,
    kMessageTypeSetRemoteDescription,
    kMessageTypeSetRemoteIceCandidate,
    kMessageTypeAddStream,
    kMessageTypeRemoveStream,
    kMessageTypeClosePeerConnection,
    kMessageTypeGetStats,
    kMessageTypeCreateNetworkMonitor=201,
  };

  Thread* pc_thread_;
  PeerConnectionChannelConfiguration configuration_;
  // Use this data channel to send p2p messages.
  // Use a map if we need more than one data channels for a PeerConnection in
  // the future.
  rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel_;
  MediaConstraintsImpl media_constraints_;

 private:
  // DataChannelObserver
  virtual void OnStateChange() { OnDataChannelStateChange(); }
  virtual void OnMessage(const webrtc::DataBuffer& buffer) {
    OnDataChannelMessage(buffer);
  };

  // |factory_| is got from PeerConnectionDependencyFactory::Get() which is
  // shared among all PeerConnectionChannels.
  rtc::scoped_refptr<PeerConnectionDependencyFactory> factory_;
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
};
}
}

#endif  // ICS_BASE_PEERCONNECTIONCHANNEL_H_
