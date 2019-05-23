#include <utility>

/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  MAIN CLASS
 *  MTUCI RULES
 *
 * <-------- FOR ALL BUILD FILES LOOK HERE (IN BUILD/BIN)
 *  NetAnim: target for .xml is here /build/bin/iot.xml
 *
 *
 *
 *
 *
 */

#include "ns3/core-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-phy.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wifi-phy-standard.h"
#include "ns3/regular-wifi-mac.h"
#include "ns3/bridge-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/ipv4-nat.h"
#include "ns3/ipv4-nat-helper.h"
#include <iostream>



using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("ScratchSimulator");


void AddMobility(double x_position, double y_position, NodeContainer container);

int
main(int argc, char *argv[]) {
    NS_LOG_UNCOND ("Start");


    /** WI-FI */

    WifiHelper WIFI;
    InternetStackHelper STACK;
    Ipv4AddressHelper ADDRESS_IPV4;
    WIFI.SetStandard(WIFI_PHY_STANDARD_80211a);

    /** NODES */

    /** GATES */
    uint32_t numberOfGates = 3;
    NodeContainer gateNodes;
    gateNodes.Create(numberOfGates);


    /** ROUTER */
    NodeContainer routerNode;
    routerNode.Create(1);


    /** IOT */
    NodeContainer vlan1Nodes;
    vlan1Nodes.Create(2);
    NodeContainer gate1;
    gate1.Add(gateNodes.Get(0));

    NodeContainer vlan2Nodes;
    vlan2Nodes.Create(2);
    NodeContainer gate2;
    gate2.Add(gateNodes.Get(1));

    NodeContainer vlan3Nodes;
    vlan3Nodes.Create(1);
    NodeContainer gate3;
    gate3.Add(gateNodes.Get(2));


    NS_LOG_UNCOND ("Router and internet-things created");
    /** Give names*/
    for (uint32_t i = 0; i < numberOfGates; ++i) {
        std::ostringstream oss;
        oss << "Gate" << i;
        Names::Add(oss.str(), gateNodes.Get(i));
    }
    Names::Add("Router", routerNode.Get(0));



    /** MOBILITY */

    /** GATES */
    AddMobility(20.0, 0.0, gateNodes.Get(0));
    AddMobility(5.0, 20.0, gateNodes.Get(1));
    AddMobility(35.0, 20.0, gateNodes.Get(2));

    /** ROUTER */
    AddMobility(60.0, 0.0, routerNode);

    /** VLANS: IOT*/
    AddMobility(0.0, -10.0, vlan1Nodes.Get(0));
    AddMobility(0.0, -20.0, vlan1Nodes.Get(1));
    AddMobility(-10.0, 25.0, vlan2Nodes.Get(0));
    AddMobility(0.0, 30.0, vlan2Nodes.Get(1));
    AddMobility(60.0, 30.0, vlan3Nodes.Get(0));




    /** Wi-Fi */
    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
    YansWifiPhyHelper gatesPhy = YansWifiPhyHelper::Default();
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiPhy.SetChannel(wifiChannel.Create());
    gatesPhy.SetChannel(wifiChannel.Create());


    /** GATES */
    WifiMacHelper gatesMac;
    Ssid gatesSsid = Ssid("Gates");
    gatesMac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(gatesSsid),
                     "ActiveProbing", BooleanValue(false));
    NetDeviceContainer gateDevices = WIFI.Install(gatesPhy, gatesMac, gateNodes);
    gatesMac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(gatesSsid));
    NetDeviceContainer routerApDevice = WIFI.Install(gatesPhy, gatesMac,
                                                     routerNode);



    /** GIVE ADDRESSES */

    STACK.Install(gateNodes);
    STACK.Install(routerNode);

    ADDRESS_IPV4.SetBase("192.168.1.0", "255.255.255.0");
    ADDRESS_IPV4.Assign(gateDevices);
    Ipv4InterfaceContainer routerApDeviceInterface;
    routerApDeviceInterface = ADDRESS_IPV4.Assign(routerApDevice);



    // PacketSocketAddress socket;
    // socket.SetSingleDevice (staDevs.Get (0)->GetIfIndex ());
    // socket.SetPhysicalAddress (staDevs.Get (1)->GetAddress ());
    // socket.SetProtocol (1);


    /**
     * Настроим Wi-Fi для каждой локальной подсети
     */
    YansWifiPhyHelper phy1 = YansWifiPhyHelper::Default();
    YansWifiPhyHelper phy2 = YansWifiPhyHelper::Default();
    YansWifiPhyHelper phy3 = YansWifiPhyHelper::Default();
    phy1.SetChannel(wifiChannel.Create());
    phy2.SetChannel(wifiChannel.Create());
    phy3.SetChannel(wifiChannel.Create());


    /**
     * Настраиваем уровень приложения для всех
     * Присваиваем vlan-узлам StaWifiMac - адреса non-AP station (STA)
     * то есть настраиваем их на прием пакетов от Access Point (AP),
     * которым как раз являются шлюзы (gate1, gate2, gate3), но сами они
     * не являются точками доступа
     */
    WifiMacHelper vlan1Mac;
    Ssid ssid = Ssid("vlan1-ssid");
    vlan1Mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid),
                     "ActiveProbing", BooleanValue(false));
    NetDeviceContainer vlan1Devices = WIFI.Install(phy1, vlan1Mac, vlan1Nodes);
    vlan1Mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    NetDeviceContainer vlan1ApDevice = WIFI.Install(phy1, vlan1Mac, gate1);

    WifiMacHelper vlan2Mac;
    Ssid ssid2 = Ssid("vlan2-ssid");
    vlan2Mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid2),
                     "ActiveProbing", BooleanValue(false));
    NetDeviceContainer vlan2Devices = WIFI.Install(phy2, vlan2Mac, vlan2Nodes);
    vlan2Mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid2));
    NetDeviceContainer vlan2ApDevice = WIFI.Install(phy2, vlan2Mac, gate2);

    WifiMacHelper vlan3Mac;
    Ssid ssid3 = Ssid("vlan3-ssid");
    vlan3Mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid3),
                     "ActiveProbing", BooleanValue(false));
    NetDeviceContainer vlan3Devices = WIFI.Install(phy3, vlan3Mac, vlan3Nodes);
    vlan3Mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid3));
    NetDeviceContainer vlan3ApDevice = WIFI.Install(phy3, vlan3Mac, gate3);


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


    /** Install stack */
    STACK.Install(vlan1Nodes);
    STACK.Install(vlan2Nodes);
    STACK.Install(vlan3Nodes);

    ADDRESS_IPV4.SetBase("10.0.0.0", "255.255.255.0");
    ADDRESS_IPV4.Assign(vlan1Devices);
    Ipv4InterfaceContainer vlan1ApDeviceInterface;
    vlan1ApDeviceInterface = ADDRESS_IPV4.Assign(vlan1ApDevice);


    ADDRESS_IPV4.SetBase("10.1.1.0", "255.255.255.0");
    ADDRESS_IPV4.Assign(vlan2Devices);
    Ipv4InterfaceContainer vlan2ApDeviceInterface;
    vlan2ApDeviceInterface = ADDRESS_IPV4.Assign(vlan2ApDevice);


    ADDRESS_IPV4.SetBase("10.2.2.0", "255.255.255.0");
    ADDRESS_IPV4.Assign(vlan3Devices);
    Ipv4InterfaceContainer vlan3ApDeviceInterface;
    vlan3ApDeviceInterface = ADDRESS_IPV4.Assign(vlan3ApDevice);



    /** TCP APPLICATION*/

    /** Запускаем рассылку TCP-пакетов */

    /**
     * vlan1ApDeviceInterface.GetAddress(0) - на самом деле здесь хранится только
     * один интерфейс, но NS3 позволяет достать переменную (в данном случае - адрес)
     * указав "номер"
     */

    std::string protocol = "ns3::TcpSocketFactory";
    Address dest = InetSocketAddress(vlan1ApDeviceInterface.GetAddress(0), 1025);
    OnOffHelper onoff = OnOffHelper(protocol, dest);
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    //Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(137));
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue("100kb/s"));

    /**VLAN-1*/
    ApplicationContainer app = onoff.Install(vlan1Nodes.Get(0));
    app.Start(Seconds(1));
    app.Stop(Seconds(30.0));
    app = onoff.Install(vlan1Nodes.Get(1));
    app.Start(Seconds(1));
    app.Stop(Seconds(30.0));

    /**VLAN-2*/
    dest = InetSocketAddress(vlan2ApDeviceInterface.GetAddress(0), 1025);
    app = onoff.Install(vlan2Nodes.Get(0));
    app.Start(Seconds(1));
    app.Stop(Seconds(30.0));
    app = onoff.Install(vlan2Nodes.Get(1));
    app.Start(Seconds(1));
    app.Stop(Seconds(30.0));

    /**VLAN-3*/
    dest = InetSocketAddress(vlan3ApDeviceInterface.GetAddress(0), 1025);
    app = onoff.Install(vlan3Nodes.Get(0));
    app.Start(Seconds(1));
    app.Stop(Seconds(30.0));



    /** ROUTER AND GATES APPLICATION */
//    dest = InetSocketAddress(routerApDeviceInterface.GetAddress(0), 1025);
    ApplicationContainer gateApp = onoff.Install(gateNodes.Get(0));
    gateApp.Start(Seconds(1.1));
    gateApp.Stop(Seconds(30.0));
    gateApp = onoff.Install(gateNodes.Get(1));
    gateApp.Start(Seconds(1.1));
    gateApp.Stop(Seconds(30.0));
    gateApp = onoff.Install(gateNodes.Get(2));
    gateApp.Start(Seconds(1.1));
    gateApp.Stop(Seconds(30.0));

    // Create a packet sink to receive these packets
    PacketSinkHelper sink("ns3::TcpSocketFactory",
                          Address(InetSocketAddress(Ipv4Address::GetAny(), 1025)));
    ApplicationContainer apps = sink.Install(gateNodes.Get(0));
    apps.Start(Seconds(1.1));
    apps.Stop(Seconds(10.0));

    // Create router nodes, initialize routing database and set up the routing
    // tables in the nodes
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();


//    PcapHelperForDevice pcapHelperForDevice;
//    pcapHelperForDevice.EnablePcap("gate0", gateDevices.Get(0), true);



    /** ANIMATION */

    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("iot.xml");

    //anim.UpdateNodeDescription (vlan1Nodes.Get(0), "IoT3");

    string sourceDir = ICONS_DIR;

    uint32_t resourceId1 = anim.AddResource(sourceDir + "/iot.png");
    uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    uint32_t pictureRouter = anim.AddResource(sourceDir + "/router.png");

    anim.UpdateNodeImage(4, resourceId1);  // "0" is the Node ID
    anim.UpdateNodeImage(5, resourceId1);
    anim.UpdateNodeImage(6, resourceId1);
    anim.UpdateNodeImage(7, resourceId1);
    anim.UpdateNodeImage(3, pictureRouter);
    anim.UpdateNodeImage(8, resourceId1);
    anim.UpdateNodeImage(0, pictureGate);
    anim.UpdateNodeImage(1, pictureGate);
    anim.UpdateNodeImage(2, pictureGate);


    for (uint32_t i = 0; i <= 8; i++) {
        anim.UpdateNodeSize(i, 9, 9);
    }

    // anim.UpdateNodeCounter (89, 7, 3.4);

/**
 * With the above statement, AnimationInterface records the meta-data
 * of each packet in the xml trace file. Metadata can be used by NetAnim to provide
 * better statistics and filter, along with providing some brief information about
 * the packet such as
 * TCP sequence number or source & destination IP address during packet animation.
 */
    // anim.EnablePacketMetadata (true);

    // Simulator::Schedule (Seconds (0.1), modify);



    Simulator::Run();
    Simulator::Stop(Seconds(30.0));
    NS_LOG_UNCOND ("У МЕНЯ ВСЕ.");
    Simulator::Destroy();

}

void AddMobility(double x_position, double y_position, NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(x_position), "MinY", DoubleValue(y_position));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(std::move(container));
}




