/*
 * Intel License
 */

#include "talk/woogeen/sdk/base/stream.h"
#include "talk/woogeen/sdk/base/peerconnectiondependencyfactory.h"

namespace woogeen {

scoped_refptr<MediaStreamInterface> Stream::MediaStream() {
  CHECK(media_stream_);
  return media_stream_;
}

void Stream::MediaStream(scoped_refptr<MediaStreamInterface> media_stream) {
  CHECK(media_stream);
  media_stream_=media_stream;
}

scoped_refptr<LocalCameraStream> LocalCameraStream::Create(cricket::VideoCapturer* capturer) {
  rtc::RefCountedObject<LocalCameraStream>* local_camera_stream = new rtc::RefCountedObject<LocalCameraStream>(capturer);
  return local_camera_stream;
}

LocalCameraStream::LocalCameraStream(cricket::VideoCapturer* capturer) {
  scoped_refptr<PeerConnectionDependencyFactory> factory = PeerConnectionDependencyFactory::Get();
  media_stream_=factory->CreateLocalMediaStream("mediastream", capturer, nullptr);
}

scoped_refptr<RemoteStream> RemoteStream::Create() {
  rtc::RefCountedObject<RemoteStream>* remote_stream = new rtc::RefCountedObject<RemoteStream>();
  return remote_stream;
}

void RemoteStream::MediaStream(scoped_refptr<MediaStreamInterface> media_stream){
  media_stream_=media_stream;
}

scoped_refptr<MediaStreamInterface> RemoteStream::MediaStream() {
  CHECK(media_stream_);
  return media_stream_;
}

}