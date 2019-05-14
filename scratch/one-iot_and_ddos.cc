//
// Created by admeen on 13.05.19.
//
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

NS_LOG_COMPONENT_DEFINE ("Trying_toBuild_NAT");

void AddMobility(double val1, double val2, NodeContainer container);

int
main (int argc, char *argv[])
{
    // Set up some default values for the simulation.
    Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (137));
    // ??? try and stick 15kb/s into the data rate
    Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("14kb/s"));
    // Default number of nodes in the star.  Overridable by command line argument.

    RngSeedManager::SetSeed(3);
    RngSeedManager::SetRun(7);

    Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
    uint32_t nSpokes = 8;

    //добавляем логирование для клиента и сервера

    LogComponentEnable ("TcpEchoServerApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("TcpEchoClientApplication", LOG_LEVEL_INFO);

    /** NODES */

    //первые 2 узла

    NodeContainer firstNodeContainer;
    firstNodeContainer.Create (2);

    //вторые 2 узла

    NodeContainer secondNodeContainer;
    secondNodeContainer.Add ( firstNodeContainer.Get (1) );
    secondNodeContainer.Create (1);

    //именуем узлы

    Names::Add("Client", firstNodeContainer.Get(0));
    Names::Add("Gate", firstNodeContainer.Get(1));
    Names::Add("Server", secondNodeContainer.Get(1));

    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    AddMobility(20.0, 0.0, firstNodeContainer.Get(0));
    AddMobility(20.0, 10.0, firstNodeContainer.Get(1));
    AddMobility(25.0, 15.0, secondNodeContainer.Get(1));

    //строим топологии для двух пар узлов

    PointToPointHelper p2p1;
    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p1.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper p2p2;
    p2p2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    p2p2.SetChannelAttribute ("Delay", StringValue ("2ms"));

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    PointToPointStarHelper star (nSpokes, pointToPoint);

    //подстраиваем топологии к узлам

    NetDeviceContainer devices = p2p1.Install (firstNodeContainer);
    NetDeviceContainer devices2 = p2p2.Install (secondNodeContainer);

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    InternetStackHelper internet;
    internet.Install (firstNodeContainer);
    internet.Install (secondNodeContainer.Get (1));
    star.InstallStack (internet);

    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов

    Ipv4AddressHelper address1;
    address1.SetBase ("192.168.1.0", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase ("203.82.48.0", "255.255.255.0");

    star.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"));



    // присваеваем IP адресс нашим устройствам

    Ipv4InterfaceContainer firstInterfaces = address1.Assign (devices);
    Ipv4InterfaceContainer secondInterfaces = address2.Assign (devices2);

    NS_LOG_INFO ("Create applications.");
    // Create a packet sink on the star "hub" to receive packets.
    uint16_t port = 50000;
    Address hubLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
    PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", hubLocalAddress);
    ApplicationContainer hubApp = packetSinkHelper.Install (star.GetHub ());
    hubApp.Start (Seconds (1.0));
    hubApp.Stop (Seconds (100.0));

    // Create OnOff applications to send TCP to the hub, one on each spoke node.
    OnOffHelper onOffHelper ("ns3::TcpSocketFactory", Address ());
    onOffHelper.SetAttribute ("OnTime", StringValue
            ("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute ("OffTime", StringValue
            ("ns3::ConstantRandomVariable[Constant=0]"));

    //ddos
    BulkSendHelper bulkHelper ("ns3::TcpSocketFactory", Address ());
    bulkHelper.SetAttribute ("MaxBytes", UintegerValue (0));

    ApplicationContainer spokeOnOffApps;
    ApplicationContainer spokeBulkApps;

    for (uint32_t i = 0; i < star.SpokeCount (); ++i)
    {
        AddressValue remoteAddress (InetSocketAddress (star.GetHubIpv4Address (i),
                                                       port));
        onOffHelper.SetAttribute ("Remote", remoteAddress);
        bulkHelper.SetAttribute ("Remote", remoteAddress);

        spokeOnOffApps.Add (onOffHelper.Install (star.GetSpokeNode (i)));
        spokeBulkApps.Add (bulkHelper.Install (star.GetSpokeNode (i)));
    }

    spokeOnOffApps.Start (Seconds (1.0));
    spokeOnOffApps.Stop (Seconds (100.0));

    spokeBulkApps.Start (Seconds (34));
    spokeBulkApps.Stop (Seconds (69));


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

    TcpEchoClientHelper tcpClient(secondInterfaces.GetAddress (1), 9);
    tcpClient.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    tcpClient.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client1 = tcpClient.Install (firstNodeContainer.Get (0));
    client1.Start(Seconds (2.0));
    client1.Stop (Seconds (10.0));

    TcpEchoServerHelper tcpServer(9);
    ApplicationContainer server = tcpServer.Install (secondNodeContainer.Get (1));
    server.Start (Seconds (1.0));
    server.Stop (Seconds (10.0));


    /* PCAP */
    p2p2.EnablePcapAll ("ipv4-nat", false);
    p2p1.EnablePcapAll ("ipv4-nat", false);

    pointToPoint.EnablePcapAll ("attack");
    p2p1.EnablePcapAll("p2p1");
    p2p2.EnablePcapAll("p2p2");


    /* ANIMATION */
    AnimationInterface anim("one-iot.xml");


    /* Simulation */
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    Simulator::Run ();
    Simulator::Stop(Seconds(10.0));
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