//
// Created by ricardo on 15.04.19.
//
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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ILUHAPLUHA");

//объявим главную функцию

int
main (int argc, char *argv[])
{

    //устанавливаем временную шкалу

    Time::SetResolution (Time::NS);
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //делаем узлы

    NodeContainer nodes;
    nodes.Create (3);

    // устанавливаем топологию точка-точка

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    NetDeviceContainer devices;
    devices = pointToPoint.Install (nodes);

    // создаем протоколы сети

    InternetStackHelper stack;
    stack.Install (nodes);

    // устанавливаем базовый IP адресс и маску

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");

    // присваеваем IP адресс нашим устройствам

    Ipv4InterfaceContainer interfaces = address.Assign (devices);

    // настройка серверного приложения. Устанавливаем необходимый порт
    UdpEchoServerHelper echoServer (9);

    // ставим в соответствие наше приложение первому и второму узлам и задаем интервал градации трафика
    ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
    ApplicationContainer serverApps1 = echoServer.Install (nodes.Get (2));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

    // производим настройку IoT устройства

    UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    // запускаем наше приложение в интервале от 2 до 10 секунд

    ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

    /** ANIMATION */

    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("I_TRIED.xml");

    //anim.UpdateNodeDescription (vlan1Nodes.Get(0), "IoT3");

    string sourceDir = ICONS_DIR;

    uint32_t resourceId1 = anim.AddResource(sourceDir + "/iot.png");
    uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    uint32_t pictureRouter = anim.AddResource(sourceDir + "/router.png");

    anim.UpdateNodeImage(0, pictureGate); // "0" is the Node ID
    anim.UpdateNodeImage(1, pictureGate);
    anim.UpdateNodeImage(2, pictureGate);


    for (uint32_t i = 0; i <= 2; i++) {
        anim.UpdateNodeSize(i, 2, 2);
    }

    // создаем pcap файл для дальнейшенго просматривания трафика

    pointToPoint.EnablePcapAll("ILUSHAN");

    // запускаем моделирование; отчищаем все объекты; завершаем главную функцию с возвратом кода 0
    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}



