//
// Created by ricardo on 23.05.19.
//
#include <string>
#include <iostream>
#include <fstream>
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/bridge-helper.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("IoT_With_Sniffer");

void AddMobility(double val1, double val2, NodeContainer container);

void
TcPacketsInQueueTrace (uint32_t oldValue, uint32_t newValue)
{
    std::cout << "TcPacketsInQueue " << oldValue << " to " << newValue << std::endl;
}

void
DevicePacketsInQueueTrace (uint32_t oldValue, uint32_t newValue)
{
    std::cout << "DevicePacketsInQueue " << oldValue << " to " << newValue << std::endl;
}

void
SojournTimeTrace (Time sojournTime)
{
    std::cout << "Sojourn time " << sojournTime.ToDouble (Time::MS) << "ms" << std::endl;
}

int
main (int argc, char *argv[])
{
    double simulationTime = 10; //seconds
    std::string transportProt = "Tcp";
    std::string socketType;

    CommandLine cmd;
    cmd.AddValue ("transportProt", "Transport protocol to use: Tcp, Udp", transportProt);
    cmd.Parse (argc, argv);

    if (transportProt.compare ("Tcp") == 0)
    {
        socketType = "ns3::TcpSocketFactory";
    }
    else
    {
        socketType = "ns3::UdpSocketFactory";
    }

    //добавляем логирование для клиента и сервера

    LogComponentEnable ("TcpEchoServerApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("TcpEchoClientApplication", LOG_LEVEL_INFO);

    //первые 2 узла

    NodeContainer firstNodeContainer;
    firstNodeContainer.Create (2);

    //вторые 2 узла

    NodeContainer secondNodeContainer;
    secondNodeContainer.Add ( firstNodeContainer.Get (1) );
    secondNodeContainer.Create (1);

    //именуем узлы

    Names::Add("Client", firstNodeContainer.Get(0));
    Names::Add("Gate", firstNodeContainer.Get(1));
    Names::Add("Server", secondNodeContainer.Get(1));

    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    AddMobility(20.0, 0.0, firstNodeContainer.Get(0));
    AddMobility(20.0, 10.0, firstNodeContainer.Get(1));
    AddMobility(25.0, 15.0, secondNodeContainer.Get(1));

    //строим топологии для двух пар узлов

    PointToPointHelper p2p1;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper p2p2;
    p2p2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    p2p2.SetChannelAttribute ("Delay", StringValue ("2ms"));

    //подстраиваем топологии к узлам

    NetDeviceContainer devices = p2p1.Install (firstNodeContainer);
    NetDeviceContainer devices2 = p2p2.Install (secondNodeContainer);

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    InternetStackHelper internet;
    internet.Install (firstNodeContainer);
    internet.Install (secondNodeContainer.Get (1));

    TrafficControlHelper tch;
    tch.SetRootQueueDisc ("ns3::RedQueueDisc");
    QueueDiscContainer qdiscs = tch.Install (devices);
    QueueDiscContainer qdiscs1 = tch.Install (devices2);

    Ptr<QueueDisc> q = qdiscs.Get (1);
    q->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&TcPacketsInQueueTrace));
    Config::ConnectWithoutContext ("/NodeList/1/$ns3::TrafficControlLayer/RootQueueDiscList/0/SojournTime",
                                   MakeCallback (&SojournTimeTrace));

    Ptr<QueueDisc> q1 = qdiscs1.Get (1);
    q1->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&TcPacketsInQueueTrace));
    Config::ConnectWithoutContext ("/NodeList/1/$ns3::TrafficControlLayer/RootQueueDiscList/0/SojournTime",
                                   MakeCallback (&SojournTimeTrace));

    Ptr<NetDevice> nd = devices.Get (1);
    Ptr<PointToPointNetDevice> ptpnd = DynamicCast<PointToPointNetDevice> (nd);
    Ptr<Queue<Packet> > queue = ptpnd->GetQueue ();
    queue->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&DevicePacketsInQueueTrace));

    Ptr<NetDevice> nd1 = devices2.Get (1);
    Ptr<PointToPointNetDevice> ptpnd1 = DynamicCast<PointToPointNetDevice> (nd1);
    Ptr<Queue<Packet> > queue1 = ptpnd1->GetQueue ();
    queue1->TraceConnectWithoutContext ("PacketsInQueue", MakeCallback (&DevicePacketsInQueueTrace));

    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов

    Ipv4AddressHelper address1;
    address1.SetBase ("192.168.1.0", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase ("203.82.48.0", "255.255.255.0");

    // присваеваем IP адресс нашим устройствам

    Ipv4InterfaceContainer firstInterfaces = address1.Assign (devices);
    Ipv4InterfaceContainer secondInterfaces = address2.Assign (devices2);

    //NAT
//        private address    NAT      public address
// n0 <--------------------> n1 <-----------------------> n2
// 192.168.1.1   192.168.1.2    203.82.48.1  203.82.48.2
//

    Ipv4NatHelper natHelper;
    //нулевой элемент в secondNodeContainer узхлах это НАТ узел
    Ptr<Ipv4Nat> nat = natHelper.Install (secondNodeContainer.Get (0));
    // Configure which of its Ipv4Interfaces are inside and outside interfaces
    // The zeroth Ipv4Interface is reserved for the loopback interface
    // Hence, the interface facing n0 is numbered "1" and the interface
    // facing n2 is numbered "2" (since it was assigned in the secondNodeContainer step above)
    nat->SetInside (1);
    nat->SetOutside (2);

    //Adding the address to be translated to and port pools.

    nat->AddAddressPool (Ipv4Address ("192.168.1.2"), Ipv4Mask ("255.255.255.255"));
    nat->AddPortPool (49153, 49163);

    // Add a rule here to map outbound connections from n0, port 49153, UDP

    Ipv4DynamicNatRule rule (Ipv4Address ("192.168.1.0"), Ipv4Mask ("255.255.255.0"));
    nat->AddDynamicRule (rule);


    Ptr<OutputStreamWrapper> natStream = Create<OutputStreamWrapper> ("nat.rules", std::ios::out);
    nat->PrintTable (natStream);


    // Протокол TCP: клиент-серверное сообщение
    NS_LOG_UNCOND ("TCP trace:");

    TcpEchoClientHelper tcpClient(secondInterfaces.GetAddress (1), 9);
    tcpClient.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    tcpClient.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client1 = tcpClient.Install (firstNodeContainer.Get (0));
    client1.Start(Seconds (2.0));
    client1.Stop (Seconds (simulationTime + 0.1));

    TcpEchoServerHelper tcpServer(9);
    ApplicationContainer server = tcpServer.Install (secondNodeContainer.Get (1));
    server.Start (Seconds (1.0));
    server.Stop (Seconds (simulationTime + 0.1));

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    /* PCAP */
    p2p2.EnablePcapAll ("ipv4-nat", false);
    p2p1.EnablePcapAll ("ipv4-nat", false);

    p2p1.EnablePcapAll("p2p1");
    p2p2.EnablePcapAll("p2p2");


    /* ANIMATION */
    AnimationInterface anim("one-iot.xml");


    /* Simulation */
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    Simulator::Stop (Seconds (simulationTime + 5));
    Simulator::Run ();

    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
    std::cout << std::endl << "*** Flow monitor statistics ***" << std::endl;
    std::cout << "  Tx Packets/Bytes:   " << stats[1].txPackets
              << " / " << stats[1].txBytes << std::endl;
    std::cout << "  Offered Load: " << stats[1].txBytes * 8.0 / (stats[1].timeLastTxPacket.GetSeconds () - stats[1].timeFirstTxPacket.GetSeconds ()) / 1000000 << " Mbps" << std::endl;
    std::cout << "  Rx Packets/Bytes:   " << stats[1].rxPackets
              << " / " << stats[1].rxBytes << std::endl;
    uint32_t packetsDroppedByQueueDisc = 0;
    uint64_t bytesDroppedByQueueDisc = 0;
    if (stats[1].packetsDropped.size () > Ipv4FlowProbe::DROP_QUEUE_DISC)
    {
        packetsDroppedByQueueDisc = stats[1].packetsDropped[Ipv4FlowProbe::DROP_QUEUE_DISC];
        bytesDroppedByQueueDisc = stats[1].bytesDropped[Ipv4FlowProbe::DROP_QUEUE_DISC];
    }
    std::cout << "  Packets/Bytes Dropped by Queue Disc:   " << packetsDroppedByQueueDisc
              << " / " << bytesDroppedByQueueDisc << std::endl;
    uint32_t packetsDroppedByNetDevice = 0;
    uint64_t bytesDroppedByNetDevice = 0;
    if (stats[1].packetsDropped.size () > Ipv4FlowProbe::DROP_QUEUE)
    {
        packetsDroppedByNetDevice = stats[1].packetsDropped[Ipv4FlowProbe::DROP_QUEUE];
        bytesDroppedByNetDevice = stats[1].bytesDropped[Ipv4FlowProbe::DROP_QUEUE];
    }
    std::cout << "  Packets/Bytes Dropped by NetDevice:   " << packetsDroppedByNetDevice
              << " / " << bytesDroppedByNetDevice << std::endl;
    std::cout << "  Throughput: " << stats[1].rxBytes * 8.0 / (stats[1].timeLastRxPacket.GetSeconds () - stats[1].timeFirstRxPacket.GetSeconds ()) / 1000000 << " Mbps" << std::endl;
    std::cout << "  Mean delay:   " << stats[1].delaySum.GetSeconds () / stats[1].rxPackets << std::endl;
    std::cout << "  Mean jitter:   " << stats[1].jitterSum.GetSeconds () / (stats[1].rxPackets - 1) << std::endl;
    auto dscpVec = classifier->GetDscpCounts (1);
    for (auto p : dscpVec)
    {
        std::cout << "  DSCP value:   0x" << std::hex << static_cast<uint32_t> (p.first) << std::dec
                  << "  count:   "<< p.second << std::endl;
    }

    Simulator::Destroy ();

    std::cout << std::endl << "*** Application statistics ***" << std::endl;
    double thr = 0;
    double thr1 = 0;
    uint64_t totalPacketsThr = DynamicCast<PacketSink> (client1.Get (0))->GetTotalRx ();
    uint64_t totalPacketsThr1 = DynamicCast<PacketSink> (server.Get (0))->GetTotalRx ();
    thr = totalPacketsThr * 8 / (simulationTime * 1000000.0); //Mbit/s
    thr1 = totalPacketsThr1 * 8 / (simulationTime * 1000000.0); //Mbit/s
    std::cout << "  Rx Bytes: " << totalPacketsThr << std::endl;
    std::cout << "  Rx Bytes: " << totalPacketsThr1 << std::endl;
    std::cout << "  Average Goodput: " << thr << " Mbit/s" << std::endl;
    std::cout << "  Average Goodput: " << thr1 << " Mbit/s" << std::endl;
    std::cout << std::endl << "*** TC Layer statistics ***" << std::endl;
    std::cout << q->GetStats () << std::endl;
    std::cout << q1->GetStats () << std::endl;
    return 0;
}

void AddMobility(double val1, double val2, NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(val1), "MinY", DoubleValue(val2));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(std::move(container));
}






