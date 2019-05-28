#include <utility>

//
// Created by IlyaWhitee on 18.04.19.
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





using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("Sniffer_Attackr");


void addMobility(double x_position, double y_position, NodeContainer container);

void addNat(Ptr<Node> gateNode, Ipv4Address gateAddress, Ipv4Address routerAddress);


int
main (int argc, char *argv[])
{

    //добавляем логирование для клиента и сервера, чтобы в терминал выводилась рассылка
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

    NodeContainer AttackNodeContainer;
    AttackNodeContainer.Add ( gate );
    AttackNodeContainer.Create (1);
    Ptr<Node> sniffer = AttackNodeContainer.Get(1);

    NodeContainer AttackNodeContainer1;
    AttackNodeContainer1.Add ( client );
    AttackNodeContainer1.Add (sniffer);


    //именуем узлы

    Names::Add("Client", client);
    Names::Add("Router", router);
    Names::Add("Gate", gate);
    Names::Add("IoT", iot);
    Names::Add("Sniffer", sniffer);

    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    addMobility(0.0, 6.0, client);
    addMobility(30.0, 10.0, router);
    addMobility(48.0, 12.0, gate);
    addMobility(60.0, 15.0, iot);
    addMobility(30.0, 25.0, sniffer);


    //строим топологии для двух пар узлов


    PointToPointHelper p2p1;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper p2p2;
    p2p2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    p2p2.SetChannelAttribute ("Delay", StringValue ("2ms"));

    PointToPointHelper p2p3;
    p2p3.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p3.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper p2p4;
    p2p4.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p4.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper p2p5;
    p2p5.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p5.SetChannelAttribute("Delay", StringValue("1ms"));



    //подстраиваем топологии к узлам

    NetDeviceContainer devices = p2p1.Install(firstNodeContainer);
    NetDeviceContainer devices2 = p2p2.Install(secondNodeContainer);
    NetDeviceContainer devices3 = p2p3.Install(thirdNodeContainer);
    NetDeviceContainer devices4 = p2p4.Install (AttackNodeContainer);
    NetDeviceContainer devices5 = p2p5.Install (AttackNodeContainer1);

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    InternetStackHelper internet;
    internet.Install(firstNodeContainer);
    internet.Install(secondNodeContainer.Get(1));
    internet.Install(thirdNodeContainer.Get(1));
    internet.Install (AttackNodeContainer.Get (1));



    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов

    Ipv4AddressHelper address1;
    address1.SetBase("192.168.1.1", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase("10.0.0.1", "255.255.255.0");

    Ipv4AddressHelper address3;
    address3.SetBase("203.82.48.1", "255.255.255.0");

    Ipv4AddressHelper address4;
    address4.SetBase ("203.82.49.1", "255.255.255.0");

    Ipv4AddressHelper address5;
    address5.SetBase ("203.82.50.1", "255.255.255.0");

    // присваеваем IP адресс нашим устройствам

    Ipv4InterfaceContainer firstInterfaces = address1.Assign(devices);
    Ipv4InterfaceContainer secondInterfaces = address2.Assign(devices2);
    Ipv4InterfaceContainer thirdInterfaces = address3.Assign(devices3);
    Ipv4InterfaceContainer attackInterfaces = address4.Assign (devices4);
    Ipv4InterfaceContainer attackInterfaces1 = address5.Assign (devices5);

    Ipv4Address firstAddressOfRouter = firstInterfaces.GetAddress(0);
    Ipv4Address secondAddressOfRouter = thirdInterfaces.GetAddress(0);
    Ipv4Address firstAddressOfGate = firstInterfaces.GetAddress(1);
    Ipv4Address secondAddressOfGate = secondInterfaces.GetAddress(0);
    Ipv4Address addressOfIot = secondInterfaces.GetAddress(1);
    Ipv4Address addressOfClient = thirdInterfaces.GetAddress(1);
    Ipv4Address firstAddressOfSniffer = attackInterfaces.GetAddress(0);
    Ipv4Address secondAddressOfSniffer = attackInterfaces1.GetAddress(0);


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

    NS_LOG_UNCOND ("First Address of Sniffer:");
    NS_LOG_UNCOND (firstAddressOfSniffer); //

    NS_LOG_UNCOND ("Second Address of Sniffer:");
    NS_LOG_UNCOND (secondAddressOfSniffer); //



    addNat(gate, firstAddressOfGate, secondAddressOfRouter);
    addNat(router, firstAddressOfRouter, addressOfClient);
    addNat(sniffer, firstAddressOfSniffer, addressOfClient);

    // Протокол TCP: клиент-серверное сообщение
    NS_LOG_UNCOND ("TCP trace:");


    TcpEchoClientHelper tcpClient(secondInterfaces.GetAddress (1), 9);
    tcpClient.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    tcpClient.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client2 = tcpClient.Install (client);
    client2.Start(Seconds (2.0));
    client2.Stop (Seconds (12.0));

    TcpEchoClientHelper tcpClient1(attackInterfaces.GetAddress (1), 9);
    tcpClient1.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient1.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    tcpClient1.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client1 = tcpClient1.Install (client);
    client1.Start(Seconds (2.0));
    client1.Stop (Seconds (10.0));

    TcpEchoClientHelper tcpClient3(attackInterfaces.GetAddress (0), 9);
    tcpClient3.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient3.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    tcpClient3.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client3 = tcpClient3.Install (sniffer);
    client3.Start(Seconds (2.0018));
    client3.Stop (Seconds (10.0));

    TcpEchoServerHelper tcpServer(9);
    ApplicationContainer server = tcpServer.Install (iot);
    server.Start (Seconds (1.0));
    server.Stop (Seconds (10.0));





    AsciiTraceHelper ascii;
    p2p2.EnableAsciiAll (ascii.CreateFileStream ("p2p2.tr"));
    p2p1.EnableAsciiAll (ascii.CreateFileStream ("p2p1.tr"));


    /* PCAP */
    //  p2p2.EnablePcapAll ("ipv4-nat", false);
    // p2p1.EnablePcapAll ("ipv4-nat", false);

    // p2p1.EnablePcapAll("p2p1");
    // p2p2.EnablePcapAll("p2p2");


    /* ANIMATION */
    AnimationInterface anim("IoT_Sniffer.xml");

    string sourceDir = ICONS_DIR;

    uint32_t pictureIoT = anim.AddResource(sourceDir + "/iot.png");
    uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    uint32_t pictureRouter = anim.AddResource(sourceDir + "/router.png");
    uint32_t pictureClient = anim.AddResource(sourceDir + "/client.png");
    uint32_t pictureSniffer = anim.AddResource(sourceDir + "/attacker.png");

    anim.UpdateNodeImage(0, pictureRouter);
    anim.UpdateNodeImage(1, pictureGate);
    anim.UpdateNodeImage(2, pictureIoT);
    anim.UpdateNodeImage(3, pictureClient);
    anim.UpdateNodeImage(4, pictureSniffer);

    for (uint32_t i = 0; i <= 4; i++) {
        anim.UpdateNodeSize(i, 9, 9);
    }


    /* Simulation */
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;

}

void addMobility(double x_position, double y_position, NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(x_position), "MinY", DoubleValue(y_position));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(std::move(container));
}

void addNat(Ptr<Node> gateNode, Ipv4Address gateAddress, Ipv4Address routerAddress) {

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
    nat->AddPortPool(49153, 49163);

    //Add a rule here to map outbound connections from n0, port 49153
    Ipv4DynamicNatRule rule(routerAddress, Ipv4Mask("255.255.255.0"));
    nat->AddDynamicRule(rule);



    Ptr<OutputStreamWrapper> natStream = Create<OutputStreamWrapper>("nat.rules", std::ios::out);
    nat->PrintTable(natStream);
}

