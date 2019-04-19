//
// Created by IlyaWhitee on 18.04.19.
//

//        private address    NAT      public address
// n0 <--------------------> n1 <-----------------------> n2
// 192.168.1.1   192.168.1.2    203.82.48.1  203.82.48.2
//
// Desired topology:
//       n0
//192.168.1.1
//       |
//       ====<----> n1 <-----> n3
//        192.168.1.2  203.82.48.1   203.82.48.2


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

NS_LOG_COMPONENT_DEFINE ("Trying_toBuild_NAT");

void AddMobility(double val1, double val2, NodeContainer container);

int
main (int argc, char *argv[])
{

    //добавляем логирование для клиента и сервера
    
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

    /** NODES */

    //первые 2 узла

    NodeContainer nodes;
    nodes.Create (2);

    //вторые 2 узла

    NodeContainer second;
    second.Add ( nodes.Get (1) );
    second.Create (1);

    //именуем узлы

    Names::Add("Client", nodes.Get(0));
    Names::Add("Gate", nodes.Get(1));
    Names::Add("Server", second.Get(1));

    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    AddMobility(20.0, 0.0, nodes.Get(0));
    AddMobility(20.0, 10.0, nodes.Get(1));
    AddMobility(25.0, 15.0, second.Get(1));

    //строим топологии для двух пар узлов

    PointToPointHelper p2p1;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

    //подстраиваем топологии к узлам

    NetDeviceContainer devices = p2p1.Install (nodes);
    NetDeviceContainer devices2 = pointToPoint.Install (second);

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    InternetStackHelper internet;
    internet.Install (nodes);
    internet.Install (second.Get (1));

    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов

    Ipv4AddressHelper address1;
    address1.SetBase ("192.168.1.0", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase ("203.82.48.0", "255.255.255.0");

    // присваеваем IP адресс нашим устройствам

    Ipv4InterfaceContainer firstInterfaces = address1.Assign (devices);
    Ipv4InterfaceContainer secondInterfaces = address2.Assign (devices2);


    /**NAT*/

    Ipv4NatHelper natHelper;
    //нулевой элемент в second узхлах это НАТ узел
    Ptr<Ipv4Nat> nat = natHelper.Install (second.Get (0));
    // Configure which of its Ipv4Interfaces are inside and outside interfaces
    // The zeroth Ipv4Interface is reserved for the loopback interface
    // Hence, the interface facing n0 is numbered "1" and the interface
    // facing n2 is numbered "2" (since it was assigned in the second step above)
    nat->SetInside (1);
    nat->SetOutside (2);

    //Adding the address to be translated to and port pools.

    nat->AddAddressPool (Ipv4Address ("192.168.1.2"), Ipv4Mask ("255.255.255.255"));
    nat->AddPortPool (49153, 49163);

    // Add a rule here to map outbound connections from n0, port 49153, UDP

    Ipv4DynamicNatRule rule (Ipv4Address ("192.168.1.0"), Ipv4Mask ("255.255.255.0"));
    nat->AddDynamicRule (rule);

    // настройка серверного приложения. Устанавливаем необходимый порт

    UdpEchoServerHelper echoServer (9);

    // This application corresponds to the first rule
    // ставим в соответствие наше приложение первому и второму узлам и задаем интервал градации трафика
    ApplicationContainer serverApps = echoServer.Install (second.Get (1));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));


    // производим настройку IoT устройства

    UdpEchoClientHelper echoClient (secondInterfaces.GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (512));

    // запускаем наше приложение в интервале от 2 до 10 секунд

    ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

    // запускаем наше приложение в интервале от 2 до 10 секунд

    clientApps = echoClient.Install (nodes.Get (1));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

    // Prepare to run the simulation
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    pointToPoint.EnablePcapAll ("ipv4-nat", false);
    p2p1.EnablePcapAll ("ipv4-nat", false);

    /** TCP APPLICATION*/

    /** Запускаем рассылку TCP-пакетов */

    std::string protocol = "ns3::TcpSocketFactory";
    Address dest = InetSocketAddress(secondInterfaces.GetAddress(1), 9);
    OnOffHelper onoff = OnOffHelper(protocol, dest);
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    //Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(137));
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue("100kb/s"));

    ApplicationContainer app = onoff.Install(nodes.Get(0));
    app.Start(Seconds(1));
    app.Stop(Seconds(30.0));
    app = onoff.Install(nodes.Get(1));
    app.Start(Seconds(1));
    app.Stop(Seconds(30.0));



    /** ANIMATION */

    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("NAT .xml");

    // создаем pcap файл для дальнейшего просматривания трафика

    p2p1.EnablePcapAll("p2p1");
    pointToPoint.EnablePcapAll("p2p2");


    Simulator::Run ();
    // Now print them out
    Ptr<OutputStreamWrapper> natStream = Create<OutputStreamWrapper> ("nat.rules", std::ios::out);
    nat->PrintTable (natStream);

    Simulator::Destroy ();
    return 0;

}

void AddMobility(double val1, double val2, NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(val1), "MinY", DoubleValue(val2));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(container);
}