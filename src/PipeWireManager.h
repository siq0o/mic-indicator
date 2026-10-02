#pragma once

#include <QMetaType>
#include <QTimer>
#include <QString>
#include <QStringList>
#include <pipewire/pipewire.h>
#include <vector>

// PipeWire’s `pw_stream` is an opaque/incomplete type; Qt6.4 and lower requires
// this so `pw_stream*` can be used in typed signal/slot connections without a
// sizeof(incomplete) static_assert.
Q_DECLARE_OPAQUE_POINTER(pw_stream *)
Q_DECLARE_METATYPE(pw_stream *)

class PipeWireManager : public QObject {
  Q_OBJECT
public:
  PipeWireManager(QStringList excludedNodes, uint32_t samplingRate,
                  uint32_t channels);
  void stop();
  void run();

signals:
  void onMicUsageChanged(bool micUsed);
  void onMicUsersChanged(QStringList users);
  void onStreamCreated(pw_stream *stream);

private:
  void setupAudioStream();

  void onGlobalEvent(uint32_t id, const spa_dict *props);
  void onGlobalRemoveEvent(uint32_t id);
  void emitUsers();

  pw_main_loop *m_loop;
  pw_context *m_context;
  pw_core *m_core;
  pw_registry *m_registry;
  spa_hook m_registry_listener;
  pw_stream *m_stream;

  QStringList m_excludedNodes;
  uint32_t m_samplingRate;
  uint32_t m_channels;

  struct MicNode {
    uint32_t id;
    QString name;
  };

  // There are usually not many nodes, so a vector is sufficient.
  std::vector<MicNode> m_nodes;
};
