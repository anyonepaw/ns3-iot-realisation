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
#include <string>
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/csma-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ILUHAPLUHA");

void AddMobility(double val1, double val2, NodeContainer container);

//объявим главную функцию

int
main (int argc, char *argv[])
{

/** NODES */

/** GATES */
    uint32_t numberOfGates = 1;
    NodeContainer gateNode;
    gateNode.Create(numberOfGates);

    NodeContainer p2pNodes;
    p2pNodes.Create (2);
    NodeContainer gateNodes;
    gateNodes.Add(gateNode.Get(0));

    Names::Add("Gate", gateNode.Get(0));

/** MOBILITY */

/** GATE */
    AddMobility(20.0, 0.0, gateNodes.Get(0));

/** NODES*/
    AddMobility(0.0, -10.0, p2pNodes.Get(1));
    AddMobility(0.0, -20.0, p2pNodes.Get(2));

// устанавливаем топологию точка-точка

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

// создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install (p2pNodes);

    CsmaHelper csma;
    csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
    csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

    NetDeviceContainer csmaDevices;
    csmaDevices = csma.Install (gateNode);

// создаем протоколы сети

    InternetStackHelper stack;
    stack.Install (p2pNodes.Get(0));
    stack.Install (gateNode);

// устанавливаем базовый IP адресс и маску

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign (p2pDevices);

// присваеваем IP адресс нашим устройствам

    address.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign (csmaDevices);

// настройка серверного приложения. Устанавливаем необходимый порт
    UdpEchoServerHelper echoServer (9);

// ставим в соответствие наше приложение первому и второму узлам и задаем интервал градации трафика
    ApplicationContainer serverApps = echoServer.Install (p2pNodes.Get (1));
    ApplicationContainer serverApps1 = echoServer.Install (p2pNodes.Get (2));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

// производим настройку IoT устройства

    UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (0), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (0));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

// запускаем наше приложение в интервале от 2 до 10 секунд

    ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

/** ANIMATION */

    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("NodesandGate.xml");

// создаем pcap файл для дальнейшего просматривания трафика

    pointToPoint.EnablePcapAll("ILUSHAN");

// запускаем моделирование; отчищаем все объекты; завершаем главную функцию с возвратом кода 0
    Simulator::Stop(Seconds(30.0));
    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}

void AddMobility(double val1, double val2,

    NodeContainer container) {
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
                                  DoubleValue(val1), "MinY", DoubleValue(val2));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(container);
}