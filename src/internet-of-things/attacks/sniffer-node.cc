#include <ns3/address-utils.h>
#include "sniffer-node.h"
#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"
#include "ns3/ipv4-address.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/sixlowpan-module.h"


NS_LOG_COMPONENT_DEFINE ("SnifferNode");


namespace ns3 {

    TypeId
    SnifferNode::GetTypeId(void) {
        static TypeId tid = TypeId("ns3::SnifferNode")
                .SetParent<Application>()
                .SetGroupName("Applications")
                .AddConstructor<SnifferNode>()
                .AddAttribute("Local",
                              "The Address on which to Bind the rx socket.",
                              AddressValue(),
                              MakeAddressAccessor(&SnifferNode::m_local),
                              MakeAddressChecker());
//                .AddAttribute("Protocol",
//                              "The type id of the protocol to use for the rx socket.",
//                              TypeIdValue(TcpSocketFactory::GetTypeId()),
//                              MakeTypeIdAccessor(&SnifferNode::m_tid),
//                              MakeTypeIdChecker());
//                .AddTraceSource("Rx",
//                                "A packet has been received",
//                                MakeTraceSourceAccessor(&SnifferNode::m_rxTrace),
//                                "ns3::Packet::AddressTracedCallback");
        return tid;
    }

    //конструктор, вызывается при запуске SnifferNode snifferNode
    SnifferNode::SnifferNode() {
        NS_LOG_FUNCTION (this);
    }

    void
    SnifferNode::DoDispose() {
        NS_LOG_FUNCTION (this);
        m_socket = 0;

        // chain up
        Application::DoDispose();
    }

    SnifferNode::~SnifferNode() {
        NS_LOG_FUNCTION (this);
    }

    //--- твои методы

    //по идее должен работать так
    //или забирать множество адресов
    // нужно найти функцию ns3 которая бы по адресу (или по ноде) смогла выводить прочитанные пакеты
    void SnifferNode::AddListeningNode(Address address) {
        m_local = address;
    }

    //но на деле вот так
    void SnifferNode::SetListeningP2P(const PointToPointHelper& pointToPointHelper){
        m_p2p = pointToPointHelper;
    }


    //---
    void
    SnifferNode::StartApplication()    // Called at time specified by Start
    {
        NS_LOG_FUNCTION (this);
        AsciiTraceHelper ascii;
        m_p2p.EnableAsciiAll(ascii.CreateFileStream("p2p.tr"));

    }

    void
    SnifferNode::StopApplication()     // Called at time specified by Stop
    {
        NS_LOG_FUNCTION (this);

    }




}




