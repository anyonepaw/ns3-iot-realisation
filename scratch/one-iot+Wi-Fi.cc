//
// Created by IlyaWhitee on 15.05.19.
//

#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/gnuplot.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/ipv4-global-routing-helper.h"


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fstream>

using namespace ns3;
using namespace std;

/*
 *
 * p2p (client/IoT) --------<-------Wifi AP (Gate)  <<<<<   Wifi STA(source) (Router)
 *
*/

std::string delay = "0ms";        //delay;p2p link
std::string datarate = "35Mbps";    //data rate of p2p link
Time stoptime = Seconds(5.0);

NS_LOG_COMPONENT_DEFINE ("Router-Gate-IoT");

void AddMobility(double val1, double val2, NodeContainer container);

int main(int argc, char *argv[]) {
    uint32_t port = 5000;
    double rss = 0;
    std::string phyMode("ErpOfdmRate54Mbps");//wifi channel
    std::string ctlMode("ErpOfdmRate54Mbps");//wifi channel

    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(1360));
//    Config::SetDefault("ns3::WifiMacQueue::MaxPacketNumber", UintegerValue(100));

    // disable fragmentation for frames below 2200 bytes
    Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue("2200"));
    // turn off RTS/CTS for frames below 2200 bytes
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("2200"));
    // Fix non-unicast data rate to be the same as that of unicast
    Config::SetDefault("ns3::WifiRemoteStationManager::NonUnicastMode",
                       StringValue(phyMode));

    /**********************define nodes***************/

    NodeContainer wifinodes;
    wifinodes.Create(2);//create two wifi nodes..one is an AP and other is a STA

    NodeContainer p2pnodes;

    p2pnodes.Add(wifinodes.Get(0));//attach the AP to p2p nodes
    p2pnodes.Create(1);//create p2p client



    //именуем узлы

    Names::Add("Client", p2pnodes.Get(1));
    Names::Add("Gate", wifinodes.Get(1));
    Names::Add("Router", wifinodes.Get(0));

    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    AddMobility(0.0, 0.0, wifinodes.Get(0));
    AddMobility(49.5, 7.5, wifinodes.Get(1));
    AddMobility(99.0, 15.0, p2pnodes.Get(1));

/*****************************Wi-Fi*********************/
    WifiHelper wifi;

    wifi.SetStandard(WIFI_PHY_STANDARD_80211g);

    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
    wifiPhy.Set("RxGain", DoubleValue(0));
    wifiPhy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

    YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");

    wifiChannel.AddPropagationLoss("ns3::FixedRssLossModel", "Rss", DoubleValue(rss));
    wifiPhy.SetChannel(wifiChannel.Create());
    WifiMacHelper wifiMac;
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode", StringValue(phyMode),
                                 "ControlMode", StringValue(phyMode));

    /***********************Install Sta/Ap mode************************************/
    NetDeviceContainer devices;

    Ssid ssid = Ssid("ns-3-ssid");
    wifiMac.SetType("ns3::StaWifiMac",
                    "Ssid", SsidValue(ssid),
                    "ActiveProbing", BooleanValue(false));

    devices.Add(wifi.Install(wifiPhy, wifiMac, wifinodes.Get(1))); //install STA mode on all the wireless nodes

    wifiMac.SetType("ns3::ApWifiMac",
                    "Ssid", SsidValue(ssid));

    NetDeviceContainer devicesap = wifi.Install(wifiPhy, wifiMac, wifinodes.Get(0));
    devices.Add(devicesap.Get(0)); //install AP mode

    /*****Gate---P2P--->IoT****************************/

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue(datarate));
    pointToPoint.SetChannelAttribute("Delay", StringValue(delay));
    pointToPoint.SetDeviceAttribute("Mtu", UintegerValue(1500));

    NetDeviceContainer devices1;
    devices1 = pointToPoint.Install(p2pnodes);


    /************************************************/
    InternetStackHelper stack;
    stack.Install(wifinodes);
    stack.Install(p2pnodes.Get(1));

    Ipv4AddressHelper address;

    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices); //wifi devices addresses

    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces1 = address.Assign(devices1);//p2p devices addresses

    /*********************NAT************************/

    //NAT
//        private address    NAT      public address
// n0 <--------------------> n1 <-----------------------> n2
// 192.168.1.1   192.168.1.2    203.82.48.1  203.82.48.2
//

    Ipv4NatHelper natHelper;
    //нулевой элемент в secondNodeContainer узхлах это НАТ узел
    Ptr<Ipv4Nat> nat = natHelper.Install (wifinodes.Get (1));
    // Configure which of its Ipv4Interfaces are inside and outside interfaces
    // The zeroth Ipv4Interface is reserved for the loopback interface
    // Hence, the interface facing n0 is numbered "1" and the interface
    // facing n2 is numbered "2" (since it was assigned in the secondNodeContainer step above)
    nat->SetInside (1);
    nat->SetOutside (2);

    //Adding the address to be translated to and port pools.

    nat->AddAddressPool (Ipv4Address ("10.1.1.5"), Ipv4Mask ("255.255.255.255"));
    nat->AddPortPool (49153, 49163);

    // Add a rule here to map outbound connections from n0, port 49153, UDP

    Ipv4DynamicNatRule rule (Ipv4Address ("10.1.1.0"), Ipv4Mask ("255.255.255.0"));
    nat->AddDynamicRule (rule);


    Ptr<OutputStreamWrapper> natStream = Create<OutputStreamWrapper> ("nat.rules", std::ios::out);
    nat->PrintTable (natStream);

    /*******************TCP**************************/

    UdpEchoServerHelper echoServer (9);

    ApplicationContainer serverApps = echoServer.Install (p2pnodes.Get (1));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

    UdpEchoClientHelper echoClient (interfaces1.GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApps = echoClient.Install (p2pnodes.Get (1));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

    ApplicationContainer clientApps1 = echoClient.Install (wifinodes.Get (1));
    clientApps1.Start (Seconds (2.0));
    clientApps1.Stop (Seconds (10.0));


    // Протокол TCP: клиент-серверное сообщение
  //  NS_LOG_UNCOND ("TCP trace:");

  //  TcpEchoClientHelper tcpClient(interfaces1.GetAddress (1), 9);
   // tcpClient.SetAttribute ("MaxPackets", UintegerValue (1));
    //tcpClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
   // tcpClient.SetAttribute ("PacketSize", UintegerValue (512));

   // ApplicationContainer client1;
    //PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
   // client1.Add(sink.Install(p2pnodes.Get(1)));
   // client1.Start(Seconds (2.0));
   // client1.Stop (Seconds (10.0));

   // TcpEchoServerHelper tcpServer(9);

    //ApplicationContainer server = tcpServer.Install (p2pnodes.Get (0));
    // server.Start (Seconds (1.0));
   //  server.Stop (Seconds (10.0));



    /******************Animation*************************/

    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("one-iot+Wi-Fi.xml");


    string sourceDir = ICONS_DIR;

    uint32_t resourceId1 = anim.AddResource(sourceDir + "/iot.png");
    uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    uint32_t pictureRouter = anim.AddResource(sourceDir + "/router.png");

    anim.UpdateNodeImage(0, pictureRouter);  // "0" is the Node ID
    anim.UpdateNodeImage(1, pictureGate);
    anim.UpdateNodeImage(2, resourceId1);

    for (uint32_t i = 0; i <= 2; i++) {
        anim.UpdateNodeSize(i, 9, 9);
    }

    /******************PCAP*************************/

    wifiPhy.EnablePcapAll("wifi");
    pointToPoint.EnablePcapAll("p2p");


    /* Simulation */


    Ipv4GlobalRoutingHelper::PopulateRoutingTables();


   // Ipv4GlobalRoutingHelper router;
   // router.Create (wifinodes.Get(0));



    Simulator::Stop(stoptime);
    Simulator::Run();
    Simulator::Destroy ();
    return 0;


}

void AddMobility(double val1, double val2, NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(val1), "MinY", DoubleValue(val2));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(std::move(container));
}