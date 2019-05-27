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

NS_LOG_COMPONENT_DEFINE ("one-iot_with_sniffer");


void AddMobility(double val1, double val2, NodeContainer container);

int
main (int argc, char *argv[])
{

    //добавляем логирование для клиента и сервера

    LogComponentEnable ("TcpEchoServerApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("TcpEchoClientApplication", LOG_LEVEL_INFO);

    /** NODES */

    //первые 2 узла

    NodeContainer Client_Router;
    Client_Router.Create(2);

    NodeContainer firstNodeContainer;
    firstNodeContainer.Add (Client_Router.Get(1));
    firstNodeContainer.Create(1);

    //вторые 2 узла

    NodeContainer secondNodeContainer;
    secondNodeContainer.Add ( firstNodeContainer.Get (1) );
    secondNodeContainer.Create (1);

    NodeContainer AttackNodeContainer;
    AttackNodeContainer.Add ( firstNodeContainer.Get (1) );
    AttackNodeContainer.Create (1);

    NodeContainer AttackNodeContainer1;
    AttackNodeContainer1.Add ( Client_Router.Get (0) );
    AttackNodeContainer1.Add (AttackNodeContainer.Get(1));


    //именуем узлы

    Names::Add("Server", Client_Router.Get(0));
    Names::Add("Router", Client_Router.Get(1));
    Names::Add("Gate", firstNodeContainer.Get(1));
    Names::Add("IoT", secondNodeContainer.Get(1));
    Names::Add("Attack_router", AttackNodeContainer.Get(1));

    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    AddMobility(20.0, - 5.0, Client_Router.Get(0));
    AddMobility(20.0, 0, Client_Router.Get(1));
    AddMobility(20.0, 10.0, firstNodeContainer.Get(1));
    AddMobility(25.0, 15.0, secondNodeContainer.Get(1));
    AddMobility(15.0, 0, AttackNodeContainer.Get(1));

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

    NetDeviceContainer devices = p2p1.Install (Client_Router);
    NetDeviceContainer devices2 = p2p2.Install (firstNodeContainer);
    NetDeviceContainer devices3 = p2p3.Install (secondNodeContainer);
    NetDeviceContainer devices4 = p2p4.Install (AttackNodeContainer);
    NetDeviceContainer devices5 = p2p5.Install (AttackNodeContainer1);

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    InternetStackHelper internet;
    internet.Install (Client_Router);
    internet.Install (firstNodeContainer.Get (1));
    internet.Install (secondNodeContainer.Get(1));
    internet.Install (AttackNodeContainer.Get (1));



    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов

    Ipv4AddressHelper address1;
    address1.SetBase ("192.168.1.0", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase ("192.168.2.0", "255.255.255.0");

    Ipv4AddressHelper address3;
    address3.SetBase ("192.168.3.0", "255.255.255.0");

    Ipv4AddressHelper address4;
    address4.SetBase ("203.82.48.0", "255.255.255.0");

    Ipv4AddressHelper address5;
    address5.SetBase ("203.82.49.0", "255.255.255.0");

    // присваеваем IP адресс нашим устройствам

    Ipv4InterfaceContainer firstInterfaces = address1.Assign (devices);
    Ipv4InterfaceContainer secondInterfaces = address2.Assign (devices2);
    Ipv4InterfaceContainer thirdInterfaces = address3.Assign (devices3);
    Ipv4InterfaceContainer attackInterfaces = address4.Assign (devices4);
    Ipv4InterfaceContainer attackInterfaces1 = address5.Assign (devices5);



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


    TcpEchoClientHelper tcpClient(attackInterfaces.GetAddress (0), 9);
    tcpClient.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    tcpClient.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client = tcpClient.Install (Client_Router.Get (0));
    client.Start(Seconds (2.0));
    client.Stop (Seconds (12.0));


    TcpEchoClientHelper tcpClient1(thirdInterfaces.GetAddress (1), 9);
    tcpClient1.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient1.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    tcpClient1.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client1 = tcpClient1.Install (Client_Router.Get (0));
    client1.Start(Seconds (2.0));
    client1.Stop (Seconds (10.0));

    TcpEchoServerHelper tcpServer(9);
    ApplicationContainer server = tcpServer.Install (secondNodeContainer.Get (1));
    server.Start (Seconds (1.0));
    server.Stop (Seconds (10.0));



    //AsciiTraceHelper ascii;
    // p2p2.EnableAsciiAll (ascii.CreateFileStream ("p2p2.tr"));
    //p2p1.EnableAsciiAll (ascii.CreateFileStream ("p2p1.tr"));


    /* PCAP */
    //  p2p2.EnablePcapAll ("ipv4-nat", false);
    // p2p1.EnablePcapAll ("ipv4-nat", false);

    // p2p1.EnablePcapAll("p2p1");
    // p2p2.EnablePcapAll("p2p2");


    /* ANIMATION */
    AnimationInterface anim("one-iot.xml");


    /* Simulation */
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

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