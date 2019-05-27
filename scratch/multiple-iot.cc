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
const uint32_t NUMBER_OF_IOT_NETWORKS = 8;


void addMobility(double x_position, double y_position, NodeContainer container);

void packageSend(string nodeNameOfSender, Ipv4Address ReceiverIpv4Address, uint16_t port,
                 double start, double end);

void packageReceive(string nodeNameOfReceiver, Ipv4Address SenderIpv4Address, uint16_t port, double start, double end);

int
main(int argc, char *argv[]) {


    NodeContainer gateNodes;
    gateNodes.Create(NUMBER_OF_IOT_NETWORKS);

//    NodeContainer routerNodes;
//    routerNodes.Create(NUMBER_OF_IOT_NETWORKS);


    NodeContainer clientNode;
    clientNode.Create(1);
    Ptr<Node> client = clientNode.Get(0);

    /** IOT */
    NodeContainer vlan1Nodes;
    vlan1Nodes.Create(2);

    NodeContainer vlan2Nodes;
    vlan2Nodes.Create(2);

    NodeContainer vlan3Nodes;
    vlan3Nodes.Create(1);


    NS_LOG_UNCOND ("Router and internet-things created");
    /** Give names*/
    for (uint32_t i = 0; i < NUMBER_OF_IOT_NETWORKS; ++i) {
        std::ostringstream oss;
        oss << "Gate" << i;
        Names::Add(oss.str(), gateNodes.Get(i));
    }

    for (uint32_t i = 0; i < vlan1Nodes.GetN(); ++i) {
        std::ostringstream oss;
        oss << "Vlan1IoT" << i;
        Names::Add(oss.str(), vlan1Nodes.Get(i));
    }

    Names::Add("Client", client);



    /** MOBILITY */

    /** GATES */
    addMobility(20.0, 0.0, gateNodes.Get(0));
    addMobility(5.0, 20.0, gateNodes.Get(1));
    addMobility(35.0, 20.0, gateNodes.Get(2));


    addMobility(60.0, 0.0, client);

    // addMobility(26.0, 0.0, routerNodes.Get(0));

    /** VLANS: IOT*/
    addMobility(0.0, -10.0, vlan1Nodes.Get(0));
    addMobility(0.0, -20.0, vlan1Nodes.Get(1));
    addMobility(-10.0, 25.0, vlan2Nodes.Get(0));
    addMobility(0.0, 30.0, vlan2Nodes.Get(1));
    addMobility(60.0, 30.0, vlan3Nodes.Get(0));




//        private address    NAT      public address
// n0 <--------------------> n1 <-----------------------> n2
// 192.168.1.1      192.168.1.2 203.82.48.1          203.82.48.2
//
//    Ipv4NatHelper natHelper;
//    Ptr<Ipv4Nat> nat = natHelper.Install(gateNodes.Get(0));
//
//    nat->SetInside(1);
//    nat->SetOutside(2);
//
//    //Adding the address to be translated to and port pools.
//
//    nat->AddAddressPool(Ipv4Address("192.168.1.5"), Ipv4Mask("255.255.255.255"));
//    nat->AddPortPool(49153, 49163);
//    // Add a rule here to map outbound connections from n0, port 49153, UDP
//    Ipv4DynamicNatRule rule(Ipv4Address("192.168.1.0"), Ipv4Mask("255.255.255.0"));
//    nat->AddDynamicRule(rule);

    PointToPointHelper p2p1;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("1ms"));




    for (int j = 0; j < vlan1IoTsInterfaces.GetN(); ++j) {
        packageSend("Gate1", vlan1IoTsInterfaces.GetAddress(j), 1025, 0, 15);
        packageReceive("Gate1", vlan1IoTsInterfaces.GetAddress(j), 1025, 4, 23);
    }

    for (int k = 0; k < vlan2IoTsInterfaces.GetN(); ++k) {
        packageSend("Gate2", vlan2IoTsInterfaces.GetAddress(k), 1025, 0, 15);
        packageReceive("Gate2", vlan2IoTsInterfaces.GetAddress(k), 1025, 3, 25);
    }

    for (int l = 0; l < vlan3IoTsInterfaces.GetN(); ++l) {
        packageSend("Gate3", vlan2IoTsInterfaces.GetAddress(l), 1025, 0, 15);
        packageReceive("Gate3", vlan1IoTsInterfaces.GetAddress(l), 1025, 4, 27);
    }


    Ipv4GlobalRoutingHelper::PopulateRoutingTables();



    /** ANIMATION */

    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("multiple-iot.xml");
    //anim.EnableIpv4RouteTracking("multiple-iot-ipv4-track", Seconds(0), Seconds(15), Seconds(0));
    anim.EnablePacketMetadata(true);


    //anim.UpdateNodeDescription (vlan1Nodes.Get(0), "IoT3");

    string sourceDir = ICONS_DIR;

    uint32_t resourceId1 = anim.AddResource(sourceDir + "/iot.png");
    uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    uint32_t pictureRouter = anim.AddResource(sourceDir + "/router.png");
    uint32_t pictureClient = anim.AddResource(sourceDir + "/client.png");

    anim.EnableIpv4RouteTracking()

    anim.UpdateNodeImage(0, pictureGate);
    anim.UpdateNodeImage(1, pictureGate);
    anim.UpdateNodeImage(2, pictureGate);
    anim.UpdateNodeImage(4, resourceId1);  // "0" is the Node ID
    anim.UpdateNodeImage(5, resourceId1);
    anim.UpdateNodeImage(6, resourceId1);
    anim.UpdateNodeImage(7, resourceId1);
    anim.UpdateNodeImage(3, pictureClient);
    anim.UpdateNodeImage(8, resourceId1);


    for (uint32_t i = 0; i <= 8; i++) {
        anim.UpdateNodeSize(i, 9, 9);
    }

    // anim.UpdateNodeCounter (89, 7, 3.4);



    // Simulator::Schedule (Seconds (0.1), modify);


    Simulator::Run();
    Simulator::Stop(Seconds(10.0));
    Simulator::Destroy();

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
    STACK.Install(container);
    ADDRESS_IPV4.SetBase(ipv4, Mask);

}




