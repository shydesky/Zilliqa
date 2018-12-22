/*
 * Copyright (c) 2018 Zilliqa
 * This source code is being disclosed to you solely for the purpose of your
 * participation in testing Zilliqa. You may view, compile and run the code for
 * that purpose and pursuant to the protocols and algorithms that are programmed
 * into, and intended by, the code. You may not do anything else with the code
 * without express permission from Zilliqa Research Pte. Ltd., including
 * modifying or publishing the code (or any part of it), and developing or
 * forming another public or private blockchain network. This source code is
 * provided 'as is' and no warranties are given as to title or non-infringement,
 * merchantability or fitness for purpose and, to the extent permitted by law,
 * all liability for your use of the code is disclaimed. Some programs in this
 * code are governed by the GNU General Public License v3.0 (available at
 * https://www.gnu.org/licenses/gpl-3.0.en.html) ('GPLv3'). The programs that
 * are governed by GPLv3.0 are those programs that are located in the folders
 * src/depends and tests/depends and which include a reference to GPLv3 in their
 * program files.
 */

#include "Peer.h"
#include <arpa/inet.h>
#include "common/Constants.h"
#include "libMessage/Messenger.h"

using namespace std;
using namespace boost::multiprecision;

Peer::Peer() : m_ipAddress(0), m_listenPortHost(0) {}

Peer::Peer(const uint128_t& ip_address, uint32_t listen_port_host)
    : m_ipAddress(ip_address), m_listenPortHost(listen_port_host) {}

Peer::Peer(const bytes& src, unsigned int offset)
    : m_ipAddress(0), m_listenPortHost(0) {
  if (Deserialize(src, offset) != 0) {
    LOG_GENERAL(WARNING, "We failed to init Peer.");
  }
}

bool Peer::operator==(const Peer& r) const {
  return (m_ipAddress == r.m_ipAddress) &&
         (m_listenPortHost == r.m_listenPortHost);
}

bool Peer::operator!=(const Peer& r) const {
  return (m_ipAddress != r.m_ipAddress) ||
         (m_listenPortHost != r.m_listenPortHost);
}

bool Peer::operator<(const Peer& r) const {
  return (m_ipAddress < r.m_ipAddress) ||
         ((m_ipAddress == r.m_ipAddress) &&
          (m_listenPortHost < r.m_listenPortHost));
}

const string Peer::GetPrintableIPAddress() const {
  char str[INET_ADDRSTRLEN];
  struct sockaddr_in serv_addr;
  serv_addr.sin_addr.s_addr = m_ipAddress.convert_to<unsigned long>();
  inet_ntop(AF_INET, &(serv_addr.sin_addr), str, INET_ADDRSTRLEN);
  return string(str);
}

unsigned int Peer::Serialize(bytes& dst, unsigned int offset) const {
  Serializable::SetNumber<uint128_t>(dst, offset, m_ipAddress, UINT128_SIZE);
  Serializable::SetNumber<uint32_t>(dst, offset + UINT128_SIZE,
                                    m_listenPortHost, sizeof(uint32_t));

  return UINT128_SIZE + sizeof(uint32_t);
}

int Peer::Deserialize(const bytes& src, unsigned int offset) {
  try {
    m_ipAddress = Serializable::GetNumber<uint128_t>(src, offset, UINT128_SIZE);
    m_listenPortHost = Serializable::GetNumber<uint32_t>(
        src, offset + UINT128_SIZE, sizeof(uint32_t));
  } catch (const std::exception& e) {
    LOG_GENERAL(WARNING, "Error with Peer::Deserialize." << ' ' << e.what());
    return -1;
  }
  return 0;
}

const uint32_t& Peer::GetListenPortHost() const { return m_listenPortHost; }
const boost::multiprecision::uint128_t& Peer::GetIpAddress() const {
  return m_ipAddress;
}
