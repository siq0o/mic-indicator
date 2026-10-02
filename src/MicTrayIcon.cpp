#include "MicTrayIcon.h"
#include <QApplication>

MicTrayIcon::MicTrayIcon()
    : m_usersAction("Microphone is not in use"), m_exitAction("Exit"), m_offIcon(QIcon::fromTheme("mic-indicator-off")),
      m_lowVolumeIcon(QIcon::fromTheme("mic-indicator-low")),
      m_highVolumeIcon(QIcon::fromTheme("mic-indicator-high")),
      m_micActive(false), m_volumeHigh(false), m_status(Off) {

  setMicStatus(Off);
  m_trayIcon.show();

  QObject::connect(&m_exitAction, &QAction::triggered, qApp,
                   &QApplication::exit, Qt::DirectConnection);
  m_usersAction.setEnabled(false);
  m_menu.addAction(&m_usersAction);
  m_menu.addSeparator();
  m_menu.addAction(&m_exitAction);
  m_trayIcon.setContextMenu(&m_menu);
}

void MicTrayIcon::setMicActive(bool micActive) {
  m_micActive = micActive;
  if (micActive) {
    setMicStatus(Low);
  } else {
    setMicStatus(Off);
    m_volumeHigh = false;
  }
}

void MicTrayIcon::setVolumeHigh(bool volumeHigh) {
  if (m_volumeHigh == volumeHigh || !m_micActive) {
    return;
  }

  m_volumeHigh = volumeHigh;
  if (volumeHigh) {
    setMicStatus(High);
  } else {
    setMicStatus(Low);
  }
}

void MicTrayIcon::setMicUsers(const QStringList &users) {
  m_users = users;
  setMicStatus(m_status);
}

void MicTrayIcon::setMicStatus(const MicStatus status) {
  m_status = status;

  QString toolTip;
  switch (status) {
  case Off:
    m_trayIcon.setIcon(m_offIcon);
    toolTip = "Microphone is not in use";
    break;
  case Low:
    m_trayIcon.setIcon(m_lowVolumeIcon);
    toolTip = "Microphone is in use, no speech detected";
    break;
  case High:
    m_trayIcon.setIcon(m_highVolumeIcon);
    toolTip = "Microphone is in use, speech detected";
    break;
  }

  if (status != Off && !m_users.isEmpty()) {
    toolTip += "\nUsed by: " + m_users.join(", ");
    m_usersAction.setText("Used by: " + m_users.join(", "));
  } else {
    m_usersAction.setText("Microphone is not in use");
  }
  m_trayIcon.setToolTip(toolTip);
}
