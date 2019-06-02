#include <utility>

#include <utility>

#include <utility>

#include <utility>

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


const InternetStackHelper STACK;
Ipv4AddressHelper ADDRESS_IPV4;
const uint32_t NUMBER_OF_IOT_NETWORKS = 7;

void addP2P(NodeContainer node1, NodeContainer node2);

void addMobility(double x_position, double y_position, NodeContainer container);

void packageSend(string nodeNameOfSender, Ipv4Address ReceiverIpv4Address, uint16_t port,
                 double start, double end);

void packageReceive(string nodeNameOfReceiver, Ipv4Address SenderIpv4Address, uint16_t port, double start, double end);

int
main(int argc, char *argv[]) {


    NodeContainer gateNodes;
    gateNodes.Create(NUMBER_OF_IOT_NETWORKS);

    NodeContainer clientNode;
    clientNode.Create(1);
    Ptr<Node> client = clientNode.Get(0);

    /** IOT */
    NodeContainer vlan0Nodes;
    vlan0Nodes.Create(3);

    NodeContainer vlan1Nodes;
    vlan1Nodes.Create(2);

    NodeContainer vlan2Nodes;
    vlan2Nodes.Create(2);

    NodeContainer vlan3Nodes;
    vlan3Nodes.Create(3);

    NodeContainer vlan4Nodes;
    vlan4Nodes.Create(2);

    NodeContainer vlan5Nodes;
    vlan5Nodes.Create(2);

    NodeContainer vlan6Nodes;
    vlan6Nodes.Create(3);

//    NodeContainer attackerNode;
//    attackerNode.Create(1);
//    Ptr<Node> attacker = attackerNode.Get(0);


    NS_LOG_UNCOND ("Router and internet-things created");
    /** Give names*/
    for (uint32_t i = 0; i < NUMBER_OF_IOT_NETWORKS; ++i) {
        std::ostringstream oss;
        oss << "Gate" << i;
        Names::Add(oss.str(), gateNodes.Get(i));
    }

//    for (uint32_t i = 0; i < vlan1Nodes.GetN(); ++i) {
//        std::ostringstream oss;
//        oss << "Vlan1IoT" << i;
//        Names::Add(oss.str(), vlan1Nodes.Get(i));
//    }

    Names::Add("Client", client);



    /** MOBILITY */

    /** GATES */
    addMobility(20.0, 0.0, gateNodes.Get(0));
    addMobility(0.0, 20.0, gateNodes.Get(1));
    addMobility(0.0, 40.0, gateNodes.Get(2));
    addMobility(20.0, 60.0, gateNodes.Get(3));
    addMobility(40.0, 60.0, gateNodes.Get(4));
    addMobility(60.0, 40.0, gateNodes.Get(5));
    addMobility(60.0, 20.0, gateNodes.Get(6));

    addMobility(40.0, 0.0, client);


    // addMobility(26.0, 0.0, routerNodes.Get(0));

    /** VLANS: IOT*/
    addMobility(-10.0, -5.0, vlan0Nodes.Get(0));
    addMobility(-10.0, -20.0, vlan0Nodes.Get(1));
    addMobility(3.0, -25.0, vlan0Nodes.Get(2));

    addMobility(-20.0, 15.0, vlan1Nodes.Get(0));
    addMobility(-20.0, 25.0, vlan1Nodes.Get(1));

    addMobility(-20.0, 35.0, vlan2Nodes.Get(0));
    addMobility(-20.0, 45.0, vlan2Nodes.Get(1));

    addMobility(10.0, 80.0, vlan3Nodes.Get(0));
    addMobility(20.0, 80.0, vlan3Nodes.Get(1));
    addMobility(30.0, 80.0, vlan3Nodes.Get(2));

    addMobility(35.0, 80.0, vlan4Nodes.Get(0));
    addMobility(45.0, 80.0, vlan4Nodes.Get(1));

    addMobility(80.0, 35.0, vlan5Nodes.Get(0));
    addMobility(80.0, 45.0, vlan5Nodes.Get(1));

    addMobility(80.0, 5.0, vlan6Nodes.Get(0));
    addMobility(80.0, 15.0, vlan6Nodes.Get(1));
    addMobility(80.0, 25.0, vlan6Nodes.Get(2));

//    addMobility(80.0, 80.0, attacker);


    addP2P(gateNodes.Get(0),gateNodes.Get(1));
    addP2P(gateNodes.Get(0),gateNodes.Get(2));
    addP2P(gateNodes.Get(0),gateNodes.Get(3));
    addP2P(gateNodes.Get(0),gateNodes.Get(4));
    addP2P(gateNodes.Get(0),gateNodes.Get(5));
    addP2P(gateNodes.Get(0),gateNodes.Get(6));
    addP2P(gateNodes.Get(0),client);

    addP2P(gateNodes.Get(0),vlan0Nodes.Get(0));
    addP2P(gateNodes.Get(0),vlan0Nodes.Get(1));
    addP2P(gateNodes.Get(0),vlan0Nodes.Get(2));

    addP2P(gateNodes.Get(1),gateNodes.Get(0));
    addP2P(gateNodes.Get(1),gateNodes.Get(2));
    addP2P(gateNodes.Get(1),gateNodes.Get(3));
    addP2P(gateNodes.Get(1),gateNodes.Get(4));
    addP2P(gateNodes.Get(1),gateNodes.Get(5));
    addP2P(gateNodes.Get(1),gateNodes.Get(6));
    addP2P(gateNodes.Get(1),client);

    addP2P(gateNodes.Get(1),vlan1Nodes.Get(0));
    addP2P(gateNodes.Get(1),vlan1Nodes.Get(1));

    addP2P(gateNodes.Get(2),gateNodes.Get(0));
    addP2P(gateNodes.Get(2),gateNodes.Get(1));
    addP2P(gateNodes.Get(2),gateNodes.Get(3));
    addP2P(gateNodes.Get(2),gateNodes.Get(4));
    addP2P(gateNodes.Get(2),gateNodes.Get(5));
    addP2P(gateNodes.Get(2),gateNodes.Get(6));
    addP2P(gateNodes.Get(2),client);

    addP2P(gateNodes.Get(2),vlan2Nodes.Get(0));
    addP2P(gateNodes.Get(2),vlan2Nodes.Get(1));

    addP2P(gateNodes.Get(3),gateNodes.Get(0));
    addP2P(gateNodes.Get(3),gateNodes.Get(1));
    addP2P(gateNodes.Get(3),gateNodes.Get(2));
    addP2P(gateNodes.Get(3),gateNodes.Get(4));
    addP2P(gateNodes.Get(3),gateNodes.Get(5));
    addP2P(gateNodes.Get(3),gateNodes.Get(6));
    addP2P(gateNodes.Get(3),client);

    addP2P(gateNodes.Get(3),vlan3Nodes.Get(0));
    addP2P(gateNodes.Get(3),vlan3Nodes.Get(1));
    addP2P(gateNodes.Get(3),vlan3Nodes.Get(2));

    addP2P(gateNodes.Get(4),gateNodes.Get(0));
    addP2P(gateNodes.Get(4),gateNodes.Get(1));
    addP2P(gateNodes.Get(4),gateNodes.Get(2));
    addP2P(gateNodes.Get(4),gateNodes.Get(3));
    addP2P(gateNodes.Get(4),gateNodes.Get(5));
    addP2P(gateNodes.Get(4),gateNodes.Get(6));
    addP2P(gateNodes.Get(4),client);

    addP2P(gateNodes.Get(4),vlan4Nodes.Get(0));
    addP2P(gateNodes.Get(4),vlan4Nodes.Get(1));


    addP2P(gateNodes.Get(5),gateNodes.Get(0));
    addP2P(gateNodes.Get(5),gateNodes.Get(1));
    addP2P(gateNodes.Get(5),gateNodes.Get(2));
    addP2P(gateNodes.Get(5),gateNodes.Get(3));
    addP2P(gateNodes.Get(5),gateNodes.Get(4));
    addP2P(gateNodes.Get(5),gateNodes.Get(6));
    addP2P(gateNodes.Get(5),client);

    addP2P(gateNodes.Get(5),vlan5Nodes.Get(0));
    addP2P(gateNodes.Get(5),vlan5Nodes.Get(1));


    addP2P(gateNodes.Get(6),gateNodes.Get(0));
    addP2P(gateNodes.Get(6),gateNodes.Get(1));
    addP2P(gateNodes.Get(6),gateNodes.Get(2));
    addP2P(gateNodes.Get(6),gateNodes.Get(3));
    addP2P(gateNodes.Get(6),gateNodes.Get(4));
    addP2P(gateNodes.Get(6),gateNodes.Get(5));
    addP2P(gateNodes.Get(6),client);

    addP2P(gateNodes.Get(6),vlan6Nodes.Get(0));
    addP2P(gateNodes.Get(6),vlan6Nodes.Get(1));
    addP2P(gateNodes.Get(6),vlan6Nodes.Get(2));

    for (int j = 0; j < NUMBER_OF_IOT_NETWORKS; ++j) {
        addP2P(client,gateNodes.Get(j));
    }

//    addP2P(attacker,gateNodes.Get(0));
//    addP2P(attacker,gateNodes.Get(1));
//    addP2P(attacker,gateNodes.Get(2));
//    addP2P(attacker,gateNodes.Get(3));
//    addP2P(attacker,gateNodes.Get(4));
//    addP2P(attacker,gateNodes.Get(5));
//    addP2P(attacker,gateNodes.Get(6));
//    addP2P(attacker,client);

    InternetStackHelper internet;
    internet.Install(gateNodes.Get(0));
    internet.Install(gateNodes.Get(1));
    internet.Install(gateNodes.Get(2));
    internet.Install(gateNodes.Get(3));
    internet.Install(gateNodes.Get(4));
    internet.Install(gateNodes.Get(5));
    internet.Install(gateNodes.Get(6));
    internet.Install(client);

    Ipv4AddressHelper address1;
    address1.SetBase("234.134.67.68", "255.255.255.0");

//
//    for (int j = 0; j < vlan1IoTsInterfaces.GetN(); ++j) {
//        packageSend("Gate1", vlan1IoTsInterfaces.GetAddress(j), 1025, 0, 15);
//        packageReceive("Gate1", vlan1IoTsInterfaces.GetAddress(j), 1025, 4, 23);
//    }
//
//    for (int k = 0; k < vlan2IoTsInterfaces.GetN(); ++k) {
//        packageSend("Gate2", vlan2IoTsInterfaces.GetAddress(k), 1025, 0, 15);
//        packageReceive("Gate2", vlan2IoTsInterfaces.GetAddress(k), 1025, 3, 25);
//    }
//
//    for (int l = 0; l < vlan3IoTsInterfaces.GetN(); ++l) {
//        packageSend("Gate3", vlan2IoTsInterfaces.GetAddress(l), 1025, 0, 15);
//        packageReceive("Gate3", vlan1IoTsInterfaces.GetAddress(l), 1025, 4, 27);
//    }


    Ipv4GlobalRoutingHelper::PopulateRoutingTables();



    /** ANIMATION */

    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("multiple-iot.xml");
    //anim.EnableIpv4RouteTracking("multiple-iot-ipv4-track", Seconds(0), Seconds(15), Seconds(0));
    anim.EnablePacketMetadata(true);


    //anim.UpdateNodeDescription (vlan1Nodes.Get(0), "IoT3");

    string sourceDir = ICONS_DIR;

    uint32_t pictureIoT = anim.AddResource(sourceDir + "/iot.png");
    uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    uint32_t pictureClient = anim.AddResource(sourceDir + "/client.png");
    uint32_t pictureAttacker = anim.AddResource(sourceDir + "/attacker.png");

    //anim.EnableIpv4RouteTracking()

    anim.UpdateNodeImage(0, pictureGate); // "0" is the Node ID (from Mobility)
    anim.UpdateNodeImage(1, pictureGate);
    anim.UpdateNodeImage(2, pictureGate);
    anim.UpdateNodeImage(3, pictureGate);
    anim.UpdateNodeImage(4, pictureGate);
    anim.UpdateNodeImage(5, pictureGate);
    anim.UpdateNodeImage(6, pictureGate);
    anim.UpdateNodeImage(7, pictureClient);

    anim.UpdateNodeImage(8, pictureIoT);
    anim.UpdateNodeImage(9, pictureIoT);
    anim.UpdateNodeImage(10, pictureIoT);
    anim.UpdateNodeImage(11, pictureIoT);
    anim.UpdateNodeImage(12, pictureIoT);
    anim.UpdateNodeImage(13, pictureIoT);
    anim.UpdateNodeImage(14, pictureIoT);
    anim.UpdateNodeImage(15, pictureIoT);
    anim.UpdateNodeImage(16, pictureIoT);
    anim.UpdateNodeImage(17, pictureIoT);
    anim.UpdateNodeImage(18, pictureIoT);
    anim.UpdateNodeImage(19, pictureIoT);
    anim.UpdateNodeImage(20, pictureIoT);
    anim.UpdateNodeImage(21, pictureIoT);
    anim.UpdateNodeImage(22, pictureIoT);
    anim.UpdateNodeImage(23, pictureIoT);
    anim.UpdateNodeImage(24, pictureIoT);

//    anim.UpdateNodeImage(25, pictureAttacker);



    for (uint32_t i = 0; i <= 24; i++) {
        anim.UpdateNodeSize(i, 9, 9);
    }

    // anim.UpdateNodeCounter (89, 7, 3.4);

    // Simulator::Schedule (Seconds (0.1), modify);


    Simulator::Run();
    Simulator::Stop(Seconds(10.0));
    Simulator::Destroy();

}

void addP2P(NodeContainer node1, NodeContainer node2) {
    NodeContainer nodeContainer;
    nodeContainer.Add(std::move(node1));
    nodeContainer.Add(std::move(node2));

    PointToPointHelper p2p1;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("1ms"));

    NetDeviceContainer devices1 = p2p1.Install(nodeContainer);
}


void addMobility(double x_position, double y_position, NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(x_position), "MinY", DoubleValue(y_position));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(std::move(container));
}

void packageSend(string nodeNameOfSender, Ipv4Address ReceiverIpv4Address, uint16_t port,
                 double start, double end) {

    OnOffHelper onoff = OnOffHelper("ns3::TcpSocketFactory", InetSocketAddress(ReceiverIpv4Address, port));
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    //Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(137));
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue("100kb/s"));

    ApplicationContainer appSend = onoff.Install(std::move(nodeNameOfSender));
    appSend.Start(Seconds(start));
    appSend.Stop(Seconds(end));
}

void packageReceive(string nodeNameOfReceiver, Ipv4Address SenderIpv4Address, uint16_t port, double start, double end) {

    PacketSinkHelper sink("ns3::TcpSocketFactory",
                          Address(InetSocketAddress(SenderIpv4Address, port)));

    ApplicationContainer appReceive = sink.Install(std::move(nodeNameOfReceiver));
    appReceive.Start(Seconds(start));
    appReceive.Stop(Seconds(end));
}

void installStack(NodeContainer container, string ipv4, string Mask){
//    STACK.Install(container);
//    ADDRESS_IPV4.SetBase(ipv4, Mask);

}




