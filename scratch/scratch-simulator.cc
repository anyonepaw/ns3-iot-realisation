/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 *  Main class
 *
 *  Documentation lays here:
 *  https://www.nsnam.org/docs/models/html/
 *
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
#include "tangle/attach_to_tangle.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ScratchSimulator");

int
main(int argc, char *argv[]) {
    NS_LOG_UNCOND ("Start");


    /**
     * Создадим три узла, представлющих собой локальную сеть ПК
     * Они будут составлять одну локальную сеть, но также служить
     * шлюзами в локальные подсети интернет-вещей
     * И еще один узел, представляющий из себя роутер
     */
    uint32_t numberOfGates = 3;

    NodeContainer gateNodes;
    gateNodes.Create(numberOfGates);
    NodeContainer routerNode;
    routerNode.Create(1);


    NS_LOG_UNCOND ("Router and internet-things created");


    /**
     * С помощью цикла шлюзы по порядку получают свои имена
     * Имена появятся при визуализации
     */
    for (uint32_t i = 0; i < numberOfGates; ++i) {
        std::ostringstream oss;
        oss << "Gate" << i;
        Names::Add(oss.str(), gateNodes.Get(i));
    }
    Names::Add("Router", routerNode.Get(0));



    /** Расставим шлюзы треугольником */
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(20.0), "MinY", DoubleValue(0.0));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(gateNodes.Get(0));

    MobilityHelper mobility2;
    mobility2.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                   DoubleValue(5.0), "MinY", DoubleValue(20.0));
    mobility2.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility2.Install(gateNodes.Get(1));

    MobilityHelper mobility3;
    mobility3.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                   DoubleValue(35.0), "MinY", DoubleValue(20.0));
    mobility3.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility3.Install(gateNodes.Get(2));



    /** Поставим роутер выше */
    MobilityHelper mobility4;
    mobility4.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                   DoubleValue(60.0), "MinY", DoubleValue(0.0));
    mobility4.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility4.Install(routerNode);



    /** Теперь настроим общий для всех Wi-Fi */
    WifiHelper wifi;
    wifi.SetStandard(WIFI_PHY_STANDARD_80211a);
    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
    YansWifiPhyHelper gatesPhy = YansWifiPhyHelper::Default();
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiPhy.SetChannel(wifiChannel.Create());
    gatesPhy.SetChannel(wifiChannel.Create());


    /** Создадим локальную сеть шлюзов */
    WifiMacHelper gatesMac;
    Ssid gatesSsid = Ssid("Gates");
    gatesMac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(gatesSsid),
                     "ActiveProbing", BooleanValue(false));
    NetDeviceContainer gateDevices = wifi.Install(gatesPhy, gatesMac,
                                                  gateNodes);
    gatesMac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(gatesSsid));
    NetDeviceContainer routerApDevice = wifi.Install(gatesPhy, gatesMac,
                                                     routerNode);

    //for (uint32_t i = 0; i < numberOfGates; i++) {
    //	gateWiFiDevices.push_back(gateDevices.Get(i));
    //}


    /** Раздадим адреса */
    InternetStackHelper stack;
    stack.Install(gateNodes);
    stack.Install(routerNode);

    Ipv4AddressHelper addressIpv4;
    addressIpv4.SetBase("192.168.1.0", "255.255.255.0");
    addressIpv4.Assign(gateDevices);
    Ipv4InterfaceContainer routerApDeviceInterface;
    routerApDeviceInterface = addressIpv4.Assign(routerApDevice);


    // PacketSocketAddress socket;
    // socket.SetSingleDevice (staDevs.Get (0)->GetIfIndex ());
    // socket.SetPhysicalAddress (staDevs.Get (1)->GetAddress ());
    // socket.SetProtocol (1);


    //TODO: настроить шлюзы


    /**
     * Создадим узлы-интернет-вещи (узлы локальной подсети)
     * для каждого шлюза
     */
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
    NetDeviceContainer vlan1Devices = wifi.Install(phy1, vlan1Mac, vlan1Nodes);
    vlan1Mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    NetDeviceContainer vlan1ApDevice = wifi.Install(phy1, vlan1Mac, gate1);

    WifiMacHelper vlan2Mac;
    Ssid ssid2 = Ssid("vlan2-ssid");
    vlan2Mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid2),
                     "ActiveProbing", BooleanValue(false));
    NetDeviceContainer vlan2Devices = wifi.Install(phy2, vlan2Mac, vlan2Nodes);
    vlan2Mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid2));
    NetDeviceContainer vlan2ApDevice = wifi.Install(phy2, vlan2Mac, gate2);

    WifiMacHelper vlan3Mac;
    Ssid ssid3 = Ssid("vlan3-ssid");
    vlan3Mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid3),
                     "ActiveProbing", BooleanValue(false));
    NetDeviceContainer vlan3Devices = wifi.Install(phy3, vlan3Mac, vlan3Nodes);
    vlan3Mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid3));
    NetDeviceContainer vlan3ApDevice = wifi.Install(phy3, vlan3Mac, gate3);


    /** Mobility Helper for vlans*/
    MobilityHelper vlan1Node0;
    vlan1Node0.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                    DoubleValue(0.0), "MinY", DoubleValue(-10.0));
    vlan1Node0.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    vlan1Node0.Install(vlan1Nodes.Get(0));

    MobilityHelper vlan1Node1;
    vlan1Node1.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                    DoubleValue(0.0), "MinY", DoubleValue(-20.0));
    vlan1Node1.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    vlan1Node1.Install(vlan1Nodes.Get(1));

    MobilityHelper vlan2Node0;
    vlan2Node0.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                    DoubleValue(-10.0), "MinY", DoubleValue(25.0));
    vlan2Node0.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    vlan2Node0.Install(vlan2Nodes.Get(0));

    MobilityHelper vlan2Node1;
    vlan2Node1.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                    DoubleValue(0.0), "MinY", DoubleValue(30.0));
    vlan2Node1.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    vlan2Node1.Install(vlan2Nodes.Get(1));

    MobilityHelper vlan3Node0;
    vlan3Node0.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                    DoubleValue(60.0), "MinY", DoubleValue(30.0));
    vlan3Node0.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    vlan3Node0.Install(vlan3Nodes.Get(0));


    // BridgeHelper bridge;
    // bridge.Install (gateNodes.Get(0), vlan1ApDevice);


    /** Install stack */
    stack.Install(vlan1Nodes);
    stack.Install(vlan2Nodes);
    stack.Install(vlan3Nodes);

    addressIpv4.SetBase("10.0.0.0", "255.255.255.0");
    addressIpv4.Assign(vlan1Devices);
    Ipv4InterfaceContainer vlan1ApDeviceInterface;
    vlan1ApDeviceInterface = addressIpv4.Assign(vlan1ApDevice);

    addressIpv4.SetBase("10.1.1.0", "255.255.255.0");
    addressIpv4.Assign(vlan2Devices);
    Ipv4InterfaceContainer vlan2ApDeviceInterface;
    vlan2ApDeviceInterface = addressIpv4.Assign(vlan2ApDevice);

    addressIpv4.SetBase("10.2.2.0", "255.255.255.0");
    addressIpv4.Assign(vlan3Devices);
    Ipv4InterfaceContainer vlan3ApDeviceInterface;
    vlan3ApDeviceInterface = addressIpv4.Assign(vlan3ApDevice);

    //tapBridge.SetAttribute("DeviceName", StringValue("tap-right"));
    //tapBridge.Install(vlan1Nodes.Get(0), vlan1Devices.Get(0));



    //TODO:
    // у нас фиговая рассылка пакетов


    /** Запускаем рассылку TCP-пакетов */

    std::string protocol = "ns3::TcpSocketFactory";
    //vlan1ApDeviceInterface.GetAddress(0) - на самом деле здесь хранится только
    //один интерфейс, но NS3 позволяет достать переменную (в данном случае - адрес)
    //указав "номер"
    Address dest = InetSocketAddress(vlan1ApDeviceInterface.GetAddress(0),
                                     1025);
    OnOffHelper onoff = OnOffHelper(protocol, dest);
    onoff.SetAttribute("OnTime",
                       StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute("OffTime",
                       StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    //Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(137));
    Config::SetDefault("ns3::OnOffApplication::DataRate",
                       StringValue("100kb/s"));

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
    dest = InetSocketAddress(routerApDeviceInterface.GetAddress(0), 1025);
    ApplicationContainer gateApp = onoff.Install(gateNodes.Get(0));
    gateApp.Start(Seconds(1.1));
    gateApp.Stop(Seconds(30.0));
    //gateApp = onoff.Install(gateNodes.Get(1));
    //gateApp.Start(Seconds(1.1));
    //gateApp.Stop(Seconds(30.0));
    //gateApp = onoff.Install(gateNodes.Get(2));
    //gateApp.Start(Seconds(1.1));
    //gateApp.Stop(Seconds(30.0));

    // Create a packet sink to receive these packets
    //PacketSinkHelper sink("ns3::TcpSocketFactory",
    //		Address(InetSocketAddress(Ipv4Address::GetAny(), 1025)));
    //ApplicationContainer apps = sink.Install(gateNodes.Get(0));
    //apps.Start(Seconds(1.1));
    //apps.Stop(Seconds(10.0));

    // Create router nodes, initialize routing database and set up the routing
    // tables in the nodes
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();




    /** ANIMATION */

    AnimationInterface anim("iot.xml");

    NS_LOG_UNCOND ("Create animation");


    //anim.UpdateNodeDescription (vlan1Nodes.Get(0), "IoT3");
    uint32_t resourceId1;


    // TODO:
    // нужно прописать код к картинкам таким образом, чтобы к ним не было полного пути
    // ввести переменную окружения
    // ${ROOT_PATH}/icons/router.png


    // resourceId1 = anim.AddResource ("/Users/anyonepaw/CLionProjects/BlockchainIoT/icons/iot.png");
    //uint32_t pictureGate = anim.AddResource ("/Users/anyonepaw/CLionProjects/BlockchainIoT/icons/gate.png");
    //uint32_t pictureRouter = anim.AddResource ("/Users/anyonepaw/CLionProjects/BlockchainIoT/icons/router.png");


    /** Подключение картинок */

//  anim.UpdateNodeImage(4, resourceId1);  // "0" is the Node ID
//  anim.UpdateNodeImage(5, resourceId1);
//
//  anim.UpdateNodeImage(6, resourceId1);
//  anim.UpdateNodeImage(7, resourceId1);
//
//  anim.UpdateNodeImage(3, pictureRouter);
//  anim.UpdateNodeImage(8, resourceId1);
//
//
//  anim.UpdateNodeImage(0, pictureGate);
//  anim.UpdateNodeImage(1, pictureGate);
//  anim.UpdateNodeImage(2, pictureGate);


    anim.UpdateNodeSize(0, 9, 9);
    anim.UpdateNodeSize(1, 9, 9);
    anim.UpdateNodeSize(2, 9, 9);
    anim.UpdateNodeSize(4, 9, 9);
    anim.UpdateNodeSize(5, 9, 9);
    anim.UpdateNodeSize(6, 9, 9);
    anim.UpdateNodeSize(7, 9, 9);
    anim.UpdateNodeSize(3, 9, 9);
    anim.UpdateNodeSize(8, 9, 9);

    // anim.UpdateNodeCounter (89, 7, 3.4);

/*
 * With the above statement, AnimationInterface records the meta-data
 * of each packet in the xml trace file. Metadata can be used by NetAnim to provide
 * better statistics and filter, along with providing some brief information about
 * the packet such as
 * TCP sequence number or source & destination IP address during packet animation.
 */
    // anim.EnablePacketMetadata (true);

    // Simulator::Schedule (Seconds (0.1), modify);



    Simulator::Stop(Seconds(30.0));
    Simulator::Run();
    Simulator::Destroy();
}
