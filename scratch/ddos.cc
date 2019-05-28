#include <utility>
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
#include "ns3/point-to-point-layout-module.h"
#include "ns3/random-variable-stream.h"
#include "limits.h"


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("DDoS attack on standard architecture");

void addMobility(double x_position, double y_position, NodeContainer container);

void addNat(Ptr<Node> gateNode, Ptr<Node> trustNode, Ipv4Address gateAddress, Ipv4Address routerAddress);

void tcpExchange(NodeContainer server, uint16_t serverPort,
                 Address serverAddress, NodeContainer client, uint16_t clientPort, double serverStart,
                 double serverStop,
                 double clientStart, double clientStop);

//static uint32_t
//HookRegistered(Hooks_t hook, Ptr<Packet> packet, Ptr<NetDevice> in,
//               Ptr<NetDevice> out, ContinueCallback& ccb)
//{
//    const char* hooknames[] = {"NF_INET_PRE_ROUTING","NF_INET_LOCAL_IN","NF_INET_FORWARD","NF_INET_LOCAL_OUT","NF_INET_POST_ROUTING","NF_INET_NUMHOOKS"};
//
//    if(out == 0 && in == 0)
//        std::cout<<"-1"<<std::endl;
//    else
//    if(out == 0 && in!= 0)
//        std::cout<<"********On Node "<<in->GetNode()->GetId()<<" "<<hooknames[hook]<<" hit***********"<<std::endl;
//    else
//    if(out!=0 && in == 0)
//        std::cout<<"********On Node "<<out->GetNode()->GetId()<<" "<<hooknames[hook]<<" hit***********"<<std::endl;
//    return 0;
//
//}

int
main(int argc, char *argv[]) {
    // Set up some default values for the simulation.
    Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(600));
    // ??? try and stick 15kb/s into the data rate
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue("14kb/s"));
    // Default number of nodes in the star.  Overridable by command line argument.

    RngSeedManager::SetSeed(3);
    RngSeedManager::SetRun(7);

    Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable>();
    uint32_t nSpokes = 7;

    //добавляем логирование для клиента и сервера

    LogComponentEnable("TcpEchoServerApplication", LOG_LEVEL_INFO);
    LogComponentEnable("TcpEchoClientApplication", LOG_LEVEL_INFO);

    /** NODES */

    //первые 2 узла

    NodeContainer firstNodeContainer;
    firstNodeContainer.Create(2);
    Ptr<Node> router = firstNodeContainer.Get(0);
    Ptr<Node> gate = firstNodeContainer.Get(1);

    NodeContainer secondNodeContainer;
    secondNodeContainer.Add(gate);
    secondNodeContainer.Create(1);
    Ptr<Node> iot = secondNodeContainer.Get(1);

    NodeContainer thirdNodeContainer;
    thirdNodeContainer.Add(router);
    thirdNodeContainer.Create(1);
    Ptr<Node> client = thirdNodeContainer.Get(1);
    // Ptr<Node> cloud = thirdNodeContainer.Get(2);


    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    addMobility(0.0, 6.0, client);
    addMobility(30.0, 10.0, router);
    addMobility(48.0, 12.0, gate);
    addMobility(60.0, 15.0, iot);

    //строим топологии для двух пар узлов

    PointToPointHelper p2p1;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper p2p2;
    p2p2.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p2.SetChannelAttribute("Delay", StringValue("2ms"));

    PointToPointHelper p2p3;
    p2p3.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p3.SetChannelAttribute("Delay", StringValue("2ms"));

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    PointToPointStarHelper star(firstNodeContainer.Get(1), nSpokes, pointToPoint);

    //подстраиваем топологии к узлам

    NetDeviceContainer devices = p2p1.Install(firstNodeContainer);
    NetDeviceContainer devices2 = p2p2.Install(secondNodeContainer);
    NetDeviceContainer devices3 = p2p3.Install(thirdNodeContainer);

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    InternetStackHelper internet;
    internet.Install(firstNodeContainer);
    internet.Install(secondNodeContainer.Get(1));
    internet.Install(thirdNodeContainer.Get(1));
    star.InstallStack(internet);

    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов

    Ipv4AddressHelper address1;
    address1.SetBase("192.168.1.1", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase("10.0.0.1", "255.255.255.0");

    Ipv4AddressHelper address3;
    address3.SetBase("203.82.48.1", "255.255.255.0");

    star.AssignIpv4Addresses(Ipv4AddressHelper("10.1.1.0", "255.255.255.0"));



    // присваеваем IP адресс нашим устройствам

    Ipv4InterfaceContainer firstInterfaces = address1.Assign(devices);
    Ipv4InterfaceContainer secondInterfaces = address2.Assign(devices2);
    Ipv4InterfaceContainer thirdInterfaces = address3.Assign(devices3);

    Ipv4Address firstAddressOfRouter = firstInterfaces.GetAddress(0);
    Ipv4Address secondAddressOfRouter = thirdInterfaces.GetAddress(0);
    Ipv4Address firstAddressOfGate = firstInterfaces.GetAddress(1);
    Ipv4Address secondAddressOfGate = secondInterfaces.GetAddress(0);
    Ipv4Address addressOfIot = secondInterfaces.GetAddress(1);
    Ipv4Address addressOfClient = thirdInterfaces.GetAddress(1);

    NS_LOG_UNCOND ("Client:");
    NS_LOG_UNCOND (addressOfClient); //192.168.1.1

    NS_LOG_UNCOND ("First Address of Router:");
    NS_LOG_UNCOND (firstAddressOfRouter); //192.168.1.1

    NS_LOG_UNCOND ("Second Address of Router:");
    NS_LOG_UNCOND (secondAddressOfRouter); //203.82.48.1

    NS_LOG_UNCOND ("First Address of Gate:");
    NS_LOG_UNCOND (firstAddressOfGate); //192.168.1.2

    NS_LOG_UNCOND ("Second Address of Gate:");
    NS_LOG_UNCOND (secondAddressOfGate); //10.0.0.1

    NS_LOG_UNCOND ("IoT:");
    NS_LOG_UNCOND (addressOfIot); //10.0.0.2

    addNat(gate, router, firstAddressOfGate, secondAddressOfRouter);
    addNat(router, client, firstAddressOfRouter, addressOfClient);


    tcpExchange(client, 9, addressOfClient, iot, 9, 1.0, 15.0, 2.0, 15.0);
    //tcpExchange(iot, 9, addressOfIot, client, 9, 2.5, 15.0, 2.5, 15.0);

/**      STAR        */

    NS_LOG_INFO ("Create applications.");
    // Create a packet sink on the star "hub" to receive packets.
    uint16_t port = 50000;
    Address hubLocalAddress(InetSocketAddress(Ipv4Address::GetAny(), port));
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", hubLocalAddress);
    ApplicationContainer hubApp = packetSinkHelper.Install(star.GetHub());
    hubApp.Start(Seconds(1.8));
    hubApp.Stop(Seconds(15.0));


    OnOffHelper onOffHelper("ns3::UdpSocketFactory", Address());
    onOffHelper.SetAttribute("OnTime", StringValue
            ("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute("OffTime", StringValue
            ("ns3::ConstantRandomVariable[Constant=0]"));

    //ddos
    BulkSendHelper bulkHelper("ns3::TcpSocketFactory", Address());
    bulkHelper.SetAttribute("MaxBytes", UintegerValue(10000000));

    BulkSendApplication bulkSendApplication;
    bulkSendApplication.SetNode(gate);

    ApplicationContainer spokeOnOffApps;
    ApplicationContainer spokeBulkApps;

    for (uint32_t i = 0; i < star.SpokeCount(); ++i) {
        AddressValue remoteAddress(InetSocketAddress(star.GetHubIpv4Address(i),
                                                     port));
        onOffHelper.SetAttribute("Remote", remoteAddress);
        bulkHelper.SetAttribute("Remote", remoteAddress);


        spokeOnOffApps.Add(onOffHelper.Install(star.GetSpokeNode(i)));
//        spokeBulkApps.Add (bulkHelper.Install (star.GetSpokeNode (i)));
    }

    spokeOnOffApps.Start(Seconds(1.8));
    spokeOnOffApps.Stop(Seconds(10.0));

//    spokeBulkApps.Start (Seconds (2.0));
//    spokeBulkApps.Stop (Seconds (10.0));

    bulkSendApplication.SetMaxBytes(10000);

    bulkSendApplication.SetStartTime(Seconds(1.8));
    bulkSendApplication.SetStopTime(Seconds(10.0));


    // Протокол TCP: клиент-серверное сообщение
    NS_LOG_UNCOND ("TCP trace:");



    /* PCAP */
//    pointToPoint.EnablePcapAll ("attack");


    /* ANIMATION */
    AnimationInterface anim("ddos.xml");

    string sourceDir = ICONS_DIR;

    uint32_t pictureIoT = anim.AddResource(sourceDir + "/iot.png");
    uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    uint32_t pictureRouter = anim.AddResource(sourceDir + "/router.png");
    uint32_t pictureClient = anim.AddResource(sourceDir + "/client.png");

    anim.UpdateNodeImage(0, pictureRouter);
    anim.UpdateNodeImage(1, pictureGate);
    anim.UpdateNodeImage(2, pictureIoT);
    anim.UpdateNodeImage(3, pictureClient);

    for (uint32_t i = 0; i <= 3; i++) {
        anim.UpdateNodeSize(i, 9, 9);
    }

    /* Simulation */
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Simulator::Run();
    Simulator::Stop(Seconds(10.0));
    Simulator::Destroy();
    return 0;

}

void addMobility(double x_position, double y_position, NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(x_position), "MinY", DoubleValue(y_position));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(std::move(container));
}

void addNat(Ptr<Node> gateNode, Ptr<Node> trustNode, Ipv4Address gateAddress, Ipv4Address routerAddress) {

    Ipv4NatHelper natHelper;
    Ptr<Ipv4Nat> nat = natHelper.Install(gateNode);
    // Configure which of its Ipv4Interfaces are inside and outside interfaces
    // The zeroth Ipv4Interface is reserved for the loopback interface
    // Hence, the interface facing n0 is numbered "1" and the interface
    // facing n2 is numbered "2" (since it was assigned in the secondNodeContainer step above)
    nat->SetInside(1);
    nat->SetOutside(2);

    //Adding the address to be translated to and port pools.
    nat->AddAddressPool(gateAddress, Ipv4Mask("255.255.255.0"));
    //  Ptr<Ipv4L3Protocol> ipv4L3 = DynamicCast <Ipv4L3Protocol>(trustNode->GetObject<Ipv4> ());
    //   Ptr <Ipv4Netfilter> netfilter = ipv4L3->GetNetfilter ();

    //  NetfilterHookCallback nodehook1 = MakeCallback (&HookRegistered);
    // Ipv4NetfilterHook nfh = Ipv4NetfilterHook (1,  NF_IP_PRI_FIRST, nodehook1);


    nat->AddPortPool(49153, 49163);

    //Add a rule here to map outbound connections from n0, port 49153
    Ipv4DynamicNatRule rule(routerAddress, Ipv4Mask("255.255.255.0"));
    nat->AddDynamicRule(rule);


    Ptr<OutputStreamWrapper> natStream = Create<OutputStreamWrapper>("nat.rules", std::ios::out);
    nat->PrintTable(natStream);
}

void tcpExchange(NodeContainer server, uint16_t serverPort,
                 Address serverAddress, NodeContainer client, uint16_t clientPort, double serverStart,
                 double serverStop,
                 double clientStart, double clientStop) {

    TcpEchoClientHelper tcpClient(serverAddress, clientPort);
    tcpClient.SetAttribute("MaxPackets", UintegerValue(1));
    tcpClient.SetAttribute("Interval", TimeValue(Seconds(3.0)));
    tcpClient.SetAttribute("PacketSize", UintegerValue(512));

    ApplicationContainer client1 = tcpClient.Install(std::move(client));
    client1.Start(Seconds(clientStart));
    client1.Stop(Seconds(clientStop));

    TcpEchoServerHelper tcpServer(serverPort);
    ApplicationContainer serverApp = tcpServer.Install(std::move(server));
    serverApp.Start(Seconds(serverStart));
    serverApp.Stop(Seconds(serverStop));
}