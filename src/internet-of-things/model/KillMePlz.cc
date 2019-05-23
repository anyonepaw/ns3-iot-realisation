//
// Created by ricardo on 22.04.19.
//

#include <ns3/type-id.h>
#include <ns3/object-base.h>
#include <ns3/application.h>
#include <ns3/core-module.h>
#include <ns3/socket.h>
#include "KillMePlz.h"

#include <algorithm>
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/address.h"

#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"

#include "ns3/boolean.h"
#include "ns3/address-utils.h"



namespace ns3 {



    NS_LOG_COMPONENT_DEFINE ("IotNode");

    NS_OBJECT_ENSURE_REGISTERED (IotNode);

    TypeId
    IotNode::GetTypeId(void) {
        static TypeId tid = TypeId("ns3::IotNode")
                .SetParent<Application>()
                .SetGroupName("Applications")
                .AddConstructor<IotNode>()
                .AddAttribute("Local",
                      "The Address on which to Bind the rx socket.",
                      AddressValue(),
                      MakeAddressAccessor(&IotNode::m_local),
                      MakeAddressChecker())
                .AddAttribute("Protocol",
                              "The type id of the protocol to use for the rx socket.",
                              TypeIdValue(UdpSocketFactory::GetTypeId()),
                              MakeTypeIdAccessor(&IotNode::m_tid),
                              MakeTypeIdChecker())
                .AddTraceSource("Rx",
                                "A packet has been received",
                                MakeTraceSourceAccessor(&IotNode::m_rxTrace),
                                "ns3::Packet::AddressTracedCallback");
        return tid;
    }

    IotNode::IotNode(void): m_commPort(5555), m_secondsPerMin(60) {

        NS_LOG_FUNCTION (this);

        m_socket = 0;
        m_gatewayNodeId = 0;
        m_gatewayAddress = Ipv4Address("::1");
        m_numberOfPeers = m_peersAddresses.size();

    }

    IotNode::~IotNode() {
        NS_LOG_FUNCTION (this);
    }

    Ptr<Socket>
    IotNode::GetListeningSocket(void) const {
        NS_LOG_FUNCTION (this);
        return m_socket;
    }


    void
    IotNode::SetPeersAddresses(const std::vector<Ipv4Address> &peers) {
        NS_LOG_FUNCTION (this);
        m_peersAddresses = peers;
        m_numberOfPeers = m_peersAddresses.size();
    }

    void
    IotNode::AddPeer(Ipv4Address newPeer) {
        NS_LOG_FUNCTION (this);
        m_peersAddresses.push_back(newPeer);
        m_numberOfPeers = m_peersAddresses.size();
    }

    void
    IotNode::SetPeersDownloadSpeeds(const std::map<Ipv4Address, double> &peersDownloadSpeeds) {
        NS_LOG_FUNCTION (this);
        m_peersDownloadSpeeds = peersDownloadSpeeds;
    }


    void
    IotNode::SetPeersUploadSpeeds(const std::map<Ipv4Address, double> &peersUploadSpeeds) {
        NS_LOG_FUNCTION (this);
        m_peersUploadSpeeds = peersUploadSpeeds;
    }

    void
    IotNode::DoDispose(void) {
        NS_LOG_FUNCTION (this);
        m_socket = 0;

        // chain up
        Application::DoDispose();
    }
    //----------------------------------


    void
    IotNode::GetNodeId() {
        NS_LOG_INFO (m_gatewayNodeId);

    }

    // Application Methods
    void
    IotNode::StartApplication()    // Called at time specified by Start
    {
        NS_LOG_FUNCTION (this);

        srand(time(NULL) + GetNode()->GetId());
        NS_LOG_INFO ("Node " << GetNode()->GetId() << ": m_numberOfPeers = " << m_numberOfPeers);

        NS_LOG_INFO ("Node " << GetNode()->GetId() << ": Gateway node = " << m_gatewayNodeId);
        NS_LOG_INFO ("Node " << GetNode()->GetId() << ": My peers are");

        for (auto it = m_peersAddresses.begin(); it != m_peersAddresses.end(); it++)
            NS_LOG_INFO("\t" << *it);

        double currentMax = 0;

        if (!m_socket) {
            m_socket = Socket::CreateSocket(GetNode(), m_tid);
            m_socket->Bind(m_local);
            m_socket->Listen();
            m_socket->ShutdownSend();
            if (addressUtils::IsMulticast(m_local)) {
                Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket>(m_socket);
                if (udpSocket) {
                    // equivalent to setsockopt (MCAST_JOIN_GROUP)
                    udpSocket->MulticastJoinGroup(0, m_local);
                } else {
                    NS_FATAL_ERROR ("Error: joining multicast on a non-UDP socket");
                }
            }
        }

        m_socket->SetRecvCallback(MakeCallback(&IotNode::HandleRead, this));
        m_socket->SetAcceptCallback(
                MakeNullCallback<bool, Ptr<Socket>, const Address &>(),
                MakeCallback(&IotNode::HandleAccept, this));
        m_socket->SetCloseCallbacks(
                MakeCallback(&IotNode::HandlePeerClose, this),
                MakeCallback(&IotNode::HandlePeerError, this));

        NS_LOG_DEBUG ("Node " << GetNode()->GetId() << ": Before creating sockets");
        for (std::vector<Ipv4Address>::const_iterator i = m_peersAddresses.begin(); i != m_peersAddresses.end(); ++i) {
            m_peersSockets[*i] = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
//            m_peersSockets[*i]->Connect (Inet6SocketAddress (*i, m_commPort));
        }
        NS_LOG_DEBUG ("Node " << GetNode()->GetId() << ": After creating sockets");
    }

    void
    IotNode::StopApplication()     // Called at time specified by Stop
    {
        NS_LOG_FUNCTION (this);

        for (std::vector<Ipv4Address>::iterator i = m_peersAddresses.begin();
             i != m_peersAddresses.end(); ++i) //close the outgoing sockets
        {
            m_peersSockets[*i]->Close();
        }


        if (m_socket) {
            m_socket->Close();
            m_socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
        }

        NS_LOG_WARN ("\n\nIoT Sensor Node " << GetNode()->GetId() << ":");

    }

    void
    IotNode::HandleRead(Ptr<Socket> socket) {
        NS_LOG_FUNCTION (this << socket);
        Ptr<Packet> packet;
        Address from;

        while ((packet = socket->RecvFrom(from))) {
            if (packet->GetSize() == 0) { //EOF
                break;
            }

            if (Inet6SocketAddress::IsMatchingType(from)) {
                /**
                 * We may receive more than one packets simultaneously on the socket,
                 * so we have to parse each one of them.
                 */
                std::string delimiter = "#";
                std::string parsedPacket;
                size_t pos = 0;
                char *packetInfo = new char[packet->GetSize() + 1];
                std::ostringstream totalStream;

                packet->CopyData(reinterpret_cast<uint8_t *>(packetInfo), packet->GetSize());
                packetInfo[packet->GetSize()] = '\0'; // ensure that it is null terminated to avoid bugs

                /**
                 * Add the buffered data to complete the packet
                 */
//                totalStream << m_bufferedData[from] << packetInfo;
                std::string totalReceivedData(totalStream.str());
                NS_LOG_INFO("Node " << GetNode()->GetId() << " Total Received Data: " << totalReceivedData);



                /**
                * Buffer the remaining data
                */

//                m_bufferedData[from] = totalReceivedData;
                delete[] packetInfo;
            } else if (Inet6SocketAddress::IsMatchingType(from)) {
                NS_LOG_INFO ("At time " << Simulator::Now().GetSeconds()
                                        << "s iot node " << GetNode()->GetId() << " received "
                                        << packet->GetSize() << " bytes from "
                                        << Inet6SocketAddress::ConvertFrom(from).GetIpv6()
                                        << " port " << Inet6SocketAddress::ConvertFrom(from).GetPort());
            }
            m_rxTrace(packet, from);
        }
    }



    void
    IotNode::HandlePeerClose(Ptr<Socket> socket) {
        NS_LOG_FUNCTION (this << socket);
    }

    void IotNode::HandlePeerError(Ptr<Socket> socket) {
        NS_LOG_FUNCTION (this << socket);
    }

    void
    IotNode::HandleAccept(Ptr<Socket> s, const Address &from) {
        NS_LOG_FUNCTION (this << s << from);
        s->SetRecvCallback(MakeCallback(&IotNode::HandleRead, this));
    }




}