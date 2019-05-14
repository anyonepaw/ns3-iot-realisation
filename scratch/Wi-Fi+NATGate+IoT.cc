
//
// Created by IlyaWhitee on 07.05.19.
//



#include <string>
#include <iostream>
#include <fstream>
//#include "ns3/KillMePlz.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/bridge-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("Trying_To_Build_Wi-Fi");

void AddMobility(double val1, double val2, NodeContainer container);

int
main (int argc, char *argv[])
{
    NS_LOG_UNCOND ("Start");

    //добавляем логирование для клиента и сервера
    LogComponentEnable ("TcpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("TcpEchoClientApplication", LOG_LEVEL_ALL);

    /** WI-FI */

    WifiHelper WIFI;
    InternetStackHelper STACK;
    WIFI.SetStandard(WIFI_PHY_STANDARD_80211a);

    /** NODES */

    /** GATES */
    uint32_t numberOfGates = 1;
    NodeContainer gateNodes;
    gateNodes.Create(numberOfGates);

    /** ROUTER */
    NodeContainer routerNode;
    routerNode.Create(1);

    /** IOT */
    NodeContainer vlan1Nodes;
    vlan1Nodes.Create(1);
    NodeContainer gate1;
    gate1.Add(gateNodes.Get(0));

    NodeContainer variant;
    variant.Add(vlan1Nodes.Get(0));
    variant.Add(gateNodes.Get(0));

    NS_LOG_UNCOND ("Router and internet-thing and Gate created");


    //именуем узлы

    Names::Add("Client", vlan1Nodes.Get(0));
    Names::Add("Gate", gate1.Get(0));
    Names::Add("Server/Router", routerNode.Get(0));

    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    AddMobility(20.0, 0.0, gate1.Get(0));
    AddMobility(20.0, 10.0, routerNode.Get(0));
    AddMobility(25.0, 15.0, vlan1Nodes.Get(0));

    /** Point-to-Point*/
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("1ms"));

    NetDeviceContainer devices = p2p.Install (variant);

    STACK.Install(variant);
    Ipv4AddressHelper address2;
    address2.SetBase ("203.82.48.0", "255.255.255.0");

    Ipv4InterfaceContainer firstInterfaces = address2.Assign (devices);


    /** Wi-Fi */
    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiPhy.SetChannel(wifiChannel.Create());

    WifiMacHelper wifimac;
    Ssid wifissid = Ssid("ns-3_ssid");
    wifimac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(wifissid),
                     "ActiveProbing", BooleanValue(false));
    NetDeviceContainer gateDevices = WIFI.Install(wifiPhy, wifimac, gateNodes);

    wifimac.SetType ("ns3::ApWifiMac",
                 "Ssid", SsidValue (wifissid));

    NetDeviceContainer wifiDevices = WIFI.Install(wifiPhy, wifimac, routerNode);



    /** GIVE ADDRESSES */

    STACK.Install(routerNode);

    Ipv4AddressHelper address1;
    address1.SetBase("192.168.1.0", "255.255.255.0");

    Ipv4InterfaceContainer routerApDeviceInterface;
    routerApDeviceInterface = address1.Assign(wifiDevices);



    //        private address    NAT      public address
// n0 <--------------------> n1 <-----------------------> n2
// 192.168.1.1      192.168.1.2 203.82.48.1          203.82.48.2
//
    Ipv4NatHelper natHelper;
    Ptr<Ipv4Nat> nat = natHelper.Install (gateNodes.Get(0));

    nat->SetInside (1);
    nat->SetOutside (2);

    //Adding the address to be translated to and port pools.

    nat->AddAddressPool (Ipv4Address ("192.168.1.5"), Ipv4Mask ("255.255.255.255"));
    nat->AddPortPool (49153, 49163);
    // Add a rule here to map outbound connections from n0, port 49153, UDP
    Ipv4DynamicNatRule rule (Ipv4Address ("192.168.1.0"), Ipv4Mask ("255.255.255.0"));
    nat->AddDynamicRule (rule);

    // Протокол TCP: клиент-серверное сообщение
    NS_LOG_UNCOND ("TCP trace:");

    TcpEchoClientHelper tcpClient(firstInterfaces.GetAddress (0), 9);
    tcpClient.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    tcpClient.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client1 = tcpClient.Install (vlan1Nodes.Get (0));
    client1.Start(Seconds (2.0));
    client1.Stop (Seconds (10.0));


    TcpEchoServerHelper tcpServer(9);
    ApplicationContainer server = tcpServer.Install (gate1.Get (0));
    server.Start (Seconds (1.0));
    server.Stop (Seconds (10.0));


    /* PCAP */
    p2p.EnablePcapAll ("ipv4-nat", false);

    p2p.EnablePcapAll("Wi-Fi+NATGate+IoT");


    /* ANIMATION */
    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("Wi-Fi+NATGATE+IoT.xml");

    string sourceDir = ICONS_DIR;

    uint32_t resourceId1 = anim.AddResource(sourceDir + "/iot.png");
    uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    uint32_t pictureRouter = anim.AddResource(sourceDir + "/router.png");

    anim.UpdateNodeImage(0, resourceId1);  // "0" is the Node ID
    anim.UpdateNodeImage(1, pictureGate);
    anim.UpdateNodeImage(2, pictureRouter);

    for (uint32_t i = 0; i <= 2; i++) {
        anim.UpdateNodeSize(i, 4, 4);
    }



    /* Simulation */
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    Simulator::Stop (Seconds (10.0));

    Simulator::Run ();
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