//
// Created by anyonepaw on 2019-04-19.
//

#include "sniffer-node.h"

namespace ns3;

Sniffer::Sniffer(void) : m_commPort(5555), m_secondsPerMin(60) {
    NS_LOG_FUNCTION (this);
    m_socket = 0;
    m_gatewayNodeId = 0;
    m_gatewayAddress = Ipv4Address("::1");
    m_numberOfPeers = m_peersAddresses.size();

}


Sniffer::~Sniffer() {
    NS_LOG_FUNCTION (this);
}