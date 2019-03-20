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
#include "ns3/bitcoin-topology-helper.h"
#include "ns3/bitcoin.h"
#include "ns3/bitcoin-node-helper.h"
#include "ns3/bitcoin-topology-helper.h"
#include "ns3/bitcoin-miner-helper.h"
#include "ns3/mpi-interface.h"
#include "time.h"
#include <iostream>
#include <string>


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("ScratchSimulator");


void AddMobility(double val1, double val2, NodeContainer container);

void PrintBitcoinRegionStats(uint32_t *bitcoinNodesRegions, uint32_t totalNodes);


int
main(int argc, char *argv[]) {
    NS_LOG_UNCOND ("Start");


    /** WI-FI */


    WifiHelper WIFI;
    InternetStackHelper STACK;
    Ipv4AddressHelper ADDRESS_IPV4;
    WIFI.SetStandard(WIFI_PHY_STANDARD_80211a);

    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiPhy.SetChannel(wifiChannel.Create());



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




//    /** Wi-Fi */


//    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
//    YansWifiPhyHelper gatesPhy = YansWifiPhyHelper::Default();
//    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
//    wifiPhy.SetChannel(wifiChannel.Create());
//    gatesPhy.SetChannel(wifiChannel.Create());
//
//
//    /** GATES */
//    WifiMacHelper gatesMac;
//    Ssid gatesSsid = Ssid("Gates");
//    gatesMac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(gatesSsid),
//                     "ActiveProbing", BooleanValue(false));
//    NetDeviceContainer gateDevices = wifi.Install(gatesPhy, gatesMac, gateNodes);
//    gatesMac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(gatesSsid));
//    NetDeviceContainer routerApDevice = wifi.Install(gatesPhy, gatesMac,
//                                                     routerNode);



//    /** GIVE ADDRESSES */

    STACK.Install(gateNodes);
//    stack.Install(routerNode);
//

//    addressIpv4.SetBase("192.168.1.0", "255.255.255.0");
//    addressIpv4.Assign(gateDevices);
//    Ipv4InterfaceContainer routerApDeviceInterface;
//    routerApDeviceInterface = addressIpv4.Assign(routerApDevice);




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



    /** BLOCKCHAIN VARIABLES */
    bool nullmsg = false;
    bool testScalability = false;
    bool unsolicited = false;
    bool relayNetwork = false;
    bool unsolicitedRelayNetwork = false;
    bool sendheaders = false;
    bool blockTorrent = false;
    bool spv = false;
    long blockSize = -1;
    int invTimeoutMins = -1;
    int chunkSize = -1;

//    double tStart = get_wall_time(), tStartSimulation, tFinish;
    const int secsPerMin = 60;
    const uint16_t bitcoinPort = 8333;
    const double realAverageBlockGenIntervalMinutes = 10; //minutes
    int targetNumberOfBlocks = 100;
    double averageBlockGenIntervalSeconds = 10 * secsPerMin; //seconds
    double fixedHashRate = 0.5;


    double *minersHash;
    enum BitcoinRegion *minersRegions;
    double bitcoinMinersHash[] = {0.289, 0.196, 0.159, 0.133, 0.066, 0.054,
                                  0.029, 0.016, 0.012, 0.012, 0.012, 0.009,
                                  0.005, 0.005, 0.002, 0.002};
    enum BitcoinRegion bitcoinMinersRegions[] = {EUROPE};

    double averageBlockGenIntervalMinutes = averageBlockGenIntervalSeconds / secsPerMin;
    int start = 0;
    double stop = 15;


    /** CREATE BLOCKCHAIN */

    Blockchain blockchain;
    NS_LOG_UNCOND ("BLOCKCHAIN WAS CREATED. HELLO WORLD");
    cout << "Количество блоков: " << blockchain.GetTotalBlocks() << endl;

    //TODO:
    Block block;
    blockchain.GetCurrentTopBlock();


    /** CREATE MINERS*/
    uint32_t noMiners = gateNodes.GetN();

    //создаем вектора
    Ipv4InterfaceContainer ipv4InterfaceContainer;
    std::map<uint32_t, std::vector<Ipv4Address>> nodesConnections;
    std::map<uint32_t, std::map<Ipv4Address, double>> peersDownloadSpeeds;
    std::map<uint32_t, std::map<Ipv4Address, double>> peersUploadSpeeds;
    std::map<uint32_t, nodeInternetSpeeds> nodesInternetSpeeds;
    std::vector<uint32_t> miners;
    int nodesInSystemId0 = 0;

    Time::SetResolution(Time::NS);



    //why it is determined by miners number?
    minersHash = new double[noMiners];
    minersRegions = new enum BitcoinRegion[noMiners];


    //??
    for (int i = 0; i < noMiners / 16; i++) {
        for (int j = 0; j < 16; j++) {
            minersHash[i * 16 + j] = bitcoinMinersHash[j] * 16 / noMiners;
            minersRegions[i * 16 + j] = bitcoinMinersRegions[j];
        }
    }

    averageBlockGenIntervalSeconds = averageBlockGenIntervalMinutes * secsPerMin;
    stop = targetNumberOfBlocks * averageBlockGenIntervalMinutes; //seconds
    averageBlockGenIntervalMinutes = averageBlockGenIntervalSeconds / secsPerMin;


    LogComponentEnable("BitcoinNode", LOG_LEVEL_INFO);
    LogComponentEnable("BitcoinMiner", LOG_LEVEL_INFO);
    LogComponentEnable("Ipv4AddressGenerator", LOG_LEVEL_FUNCTION);
    LogComponentEnable("OnOffApplication", LOG_LEVEL_DEBUG);
    LogComponentEnable("OnOffApplication", LOG_LEVEL_WARN);


    uint32_t systemId = 0;
    uint32_t systemCount = 0;
    uint32_t totalNoNodes = 4;
    enum Cryptocurrency cryptocurrency = BITCOIN;
    int minConnectionsPerNode = -1;
    int maxConnectionsPerNode = -1;
    double latencyParetoShapeDivider = 5; //?

    nodeStatistics *stats = new nodeStatistics[totalNoNodes];

    BitcoinTopologyHelper bitcoinTopologyHelper (systemCount, totalNoNodes, noMiners, minersRegions,
                                                 cryptocurrency, minConnectionsPerNode,
                                                 maxConnectionsPerNode, latencyParetoShapeDivider, systemId);

    // Install stack on Grid
    bitcoinTopologyHelper.InstallStack (STACK);

    // Assign Addresses to Grid
    bitcoinTopologyHelper.AssignIpv4Addresses (Ipv4AddressHelperCustom ("1.0.0.0", "255.255.255.0", false));
    ipv4InterfaceContainer = bitcoinTopologyHelper.GetIpv4InterfaceContainer();
    nodesConnections = bitcoinTopologyHelper.GetNodesConnectionsIps();
    miners = bitcoinTopologyHelper.GetMiners();
    peersDownloadSpeeds = bitcoinTopologyHelper.GetPeersDownloadSpeeds();
    peersUploadSpeeds = bitcoinTopologyHelper.GetPeersUploadSpeeds();
    nodesInternetSpeeds = bitcoinTopologyHelper.GetNodesInternetSpeeds();

    PrintBitcoinRegionStats(bitcoinTopologyHelper.GetBitcoinNodesRegions(), totalNoNodes);




    NS_LOG_UNCOND ("INSTALL BITCOIN NODES");

    BitcoinNodeHelper bitcoinNodeHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), bitcoinPort),
                                        nodesConnections[0], peersDownloadSpeeds[0], peersUploadSpeeds[0],
                                        nodesInternetSpeeds[0], stats);
    ApplicationContainer bitcoinNodes;


    Ptr<Node> targetNode1 = gateNodes.Get(0);
    bitcoinNodes.Add(bitcoinNodeHelper.Install(targetNode1));
    Ptr<Node> targetNode2 = gateNodes.Get(1);
    bitcoinNodes.Add(bitcoinNodeHelper.Install(targetNode2));
    Ptr<Node> targetNode3 = gateNodes.Get(2);
    bitcoinNodes.Add(bitcoinNodeHelper.Install(targetNode3));

    bitcoinNodes.Start(Seconds(start));
    bitcoinNodes.Stop(Seconds(stop));


////    NS_LOG_UNCOND ("CREATE MINERS");
//    BitcoinMinerHelper bitcoinMinerHelper("ns3::TcpSocketFactory",
//                                          InetSocketAddress(Ipv4Address::GetAny(), bitcoinPort),
//                                          nodesConnections[miners[0]], noMiners, peersDownloadSpeeds[0],
//                                          peersUploadSpeeds[0],
//                                          nodesInternetSpeeds[0], stats, minersHash[0], averageBlockGenIntervalSeconds);
//    ApplicationContainer bitcoinMiners;
////    bitcoinMinerHelper.SetAttribute("FixedBlockIntervalGeneration", DoubleValue(averageBlockGenIntervalSeconds));
//
//    NS_LOG_UNCOND ("DONE - CREATE MINERS");

//    for(auto miner: miners) {
//
//    bitcoinMinerHelper.SetPeersAddresses (nodesConnections[miner]);
//    bitcoinMinerHelper.SetPeersDownloadSpeeds (peersDownloadSpeeds[miner]);
//    bitcoinMinerHelper.SetPeersUploadSpeeds (peersUploadSpeeds[miner]);
//    bitcoinMinerHelper.SetNodeInternetSpeeds (nodesInternetSpeeds[miner]);
//    bitcoinMinerHelper.SetNodeStats (&stats[miner]);
//
//    }
//        bitcoinMiners.Add(bitcoinMinerHelper.Install (targetNode1));
//        bitcoinMiners.Add(bitcoinMinerHelper.Install (targetNode2));
//        bitcoinMiners.Add(bitcoinMinerHelper.Install (targetNode3));
//        std::cout << "SystemId " << systemId << ": Miner " << miner << " with hash power = " << minersHash[count]
//	            << " and systemId = " << targetNode->GetSystemId() << " was installed in node "
//                << targetNode->GetId () << std::endl;

//    bitcoinMiners.Start(Seconds(start));
//    bitcoinMiners.Stop(Minutes(stop));


    // Set up the actual simulation
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
//    tStartSimulation = get_wall_time();
//    if (systemId == 0)
//        std::cout << "Setup time = " << tStartSimulation - tStart << "s\n";
//    Simulator::Stop (Minutes (stop + 0.1));
//    Simulator::Run ();
//    Simulator::Destroy ();
















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




    Simulator::Stop(Seconds(30.0));
    Simulator::Run();
    NS_LOG_UNCOND ("У МЕНЯ ВСЕ.");
    Simulator::Destroy();
}


void AddMobility(double val1, double val2, NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(val1), "MinY", DoubleValue(val2));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(container);
}


void PrintBitcoinRegionStats(uint32_t *bitcoinNodesRegions, uint32_t totalNodes) {
    uint32_t regions[7] = {0, 0, 0, 0, 0, 0, 0};

    for (uint32_t i = 0; i < totalNodes; i++)
        regions[bitcoinNodesRegions[i]]++;

    std::cout << "Nodes distribution: \n";
    for (uint32_t i = 0; i < 7; i++) {
        std::cout << getBitcoinRegion(getBitcoinEnum(i)) << ": " << regions[i] * 100.0 / totalNodes << "%\n";
    }
}

