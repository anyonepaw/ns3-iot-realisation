//
// Created by ricardo on 28.05.19.
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





using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("EvreiVPERED");


void addMobility(double x_position, double y_position, NodeContainer container);

void addNat(Ptr<Node> gateNode, Ipv4Address gateAddress, Ipv4Address routerAddress);


int
main (int argc, char *argv[])
{

    //добавляем логирование для клиента и сервера, чтобы в терминал выводилась рассылка
    LogComponentEnable("TcpEchoServerApplication", LOG_LEVEL_INFO);
    LogComponentEnable("TcpEchoClientApplication", LOG_LEVEL_INFO);


    /** NODES */

    NodeContainer firstNodeContainer;
    firstNodeContainer.Create(2);
    Ptr<Node> node1 = firstNodeContainer.Get(0);
    Ptr<Node> node2 = firstNodeContainer.Get(1);

    NodeContainer secondNodeContainer;
    secondNodeContainer.Add(node2);
    secondNodeContainer.Create(1);
    Ptr<Node> node3 = secondNodeContainer.Get(1);

    NodeContainer thirdNodeContainer;
    thirdNodeContainer.Add(node3);
    thirdNodeContainer.Create(1);
    Ptr<Node> node4 = thirdNodeContainer.Get(1);

    NodeContainer fourthNodeContainer;
    fourthNodeContainer.Add(node4);
    fourthNodeContainer.Create(1);
    Ptr<Node> node5 = fourthNodeContainer.Get(1);

    NodeContainer fifthNodeContainer;
    fifthNodeContainer.Add(node5);
    fifthNodeContainer.Create(1);
    Ptr<Node> node6 = fifthNodeContainer.Get(1);

    NodeContainer sixthNodeContainer;
    sixthNodeContainer.Add(node6);
    sixthNodeContainer.Create(1);
    Ptr<Node> node7 = sixthNodeContainer.Get(1);

    NodeContainer seventhNodeContainer;
    seventhNodeContainer.Add(node7);
    seventhNodeContainer.Create(1);
    Ptr<Node> node8 = seventhNodeContainer.Get(1);

    NodeContainer eighthNodeContainer;
    eighthNodeContainer.Add(node8);
    eighthNodeContainer.Add(node1);


    /** MOBILITY */ //задаем координаты узлам

    /** NODES*/
    addMobility(30.0, 10.0, node1);
    addMobility(0.0, 25.0, node2);
    addMobility(-10.0, 40.0, node3);
    addMobility(0.0, 55.0, node4);
    addMobility(30.0, 70.0, node5);
    addMobility(60.0, 55.0, node6);
    addMobility(70.0, 40.0, node7);
    addMobility(60.0, 25.0, node8);


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

    PointToPointHelper p2p6;
    p2p3.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p3.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper p2p7;
    p2p4.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p4.SetChannelAttribute("Delay", StringValue("1ms"));

    PointToPointHelper p2p8;
    p2p5.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p5.SetChannelAttribute("Delay", StringValue("1ms"));

    //подстраиваем топологии к узлам

    NetDeviceContainer devices1 = p2p1.Install(firstNodeContainer);
    NetDeviceContainer devices2 = p2p2.Install(secondNodeContainer);
    NetDeviceContainer devices3 = p2p3.Install(thirdNodeContainer);
    NetDeviceContainer devices4 = p2p4.Install(fourthNodeContainer);
    NetDeviceContainer devices5 = p2p5.Install(fifthNodeContainer);
    NetDeviceContainer devices6 = p2p6.Install(sixthNodeContainer);
    NetDeviceContainer devices7 = p2p7.Install(seventhNodeContainer);
    NetDeviceContainer devices8 = p2p8.Install(eighthNodeContainer);

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    InternetStackHelper internet;
    internet.Install(firstNodeContainer);
    internet.Install(secondNodeContainer.Get(1));
    internet.Install(thirdNodeContainer.Get(1));
    internet.Install (fourthNodeContainer.Get (1));
    internet.Install(fifthNodeContainer.Get(1));
    internet.Install(sixthNodeContainer.Get(1));
    internet.Install (seventhNodeContainer.Get (1));


    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов

    Ipv4AddressHelper address1;
    address1.SetBase("10.0.0.1", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase("10.0.0.2", "255.255.255.0");

    Ipv4AddressHelper address3;
    address3.SetBase("10.0.0.3", "255.255.255.0");

    Ipv4AddressHelper address4;
    address4.SetBase ("10.0.0.4", "255.255.255.0");

    Ipv4AddressHelper address5;
    address5.SetBase ("10.0.0.5", "255.255.255.0");

    Ipv4AddressHelper address6;
    address6.SetBase("10.0.0.6", "255.255.255.0");

    Ipv4AddressHelper address7;
    address7.SetBase ("10.0.0.7", "255.255.255.0");

    Ipv4AddressHelper address8;
    address8.SetBase ("10.0.0.8", "255.255.255.0");


    // присваеваем IP адресс нашим устройствам

    Ipv4InterfaceContainer firstInterfaces = address1.Assign(devices1);
    Ipv4InterfaceContainer secondInterfaces = address2.Assign(devices2);
    Ipv4InterfaceContainer thirdInterfaces = address3.Assign(devices3);
    Ipv4InterfaceContainer fourthInterfaces = address4.Assign(devices4);
    Ipv4InterfaceContainer fifthInterfaces = address5.Assign(devices5);
    Ipv4InterfaceContainer sixthInterfaces = address6.Assign(devices6);
    Ipv4InterfaceContainer seventhInterfaces = address7.Assign(devices7);
    Ipv4InterfaceContainer eighthInterfaces = address8.Assign(devices8);











    /* ANIMATION */
    AnimationInterface anim("ZVEZDaDAVIDA.xml");

    //string sourceDir = ICONS_DIR;

   // uint32_t pictureIoT = anim.AddResource(sourceDir + "/iot.png");
    //uint32_t pictureGate = anim.AddResource(sourceDir + "/gate.png");
    //uint32_t pictureRouter = anim.AddResource(sourceDir + "/router.png");
   // uint32_t pictureClient = anim.AddResource(sourceDir + "/client.png");
    //uint32_t pictureSniffer = anim.AddResource(sourceDir + "/attacker.png");

    //anim.UpdateNodeImage(0, pictureRouter);
    //anim.UpdateNodeImage(1, pictureGate);
    //anim.UpdateNodeImage(2, pictureIoT);
   // anim.UpdateNodeImage(3, pictureClient);
    //anim.UpdateNodeImage(4, pictureSniffer);

    //for (uint32_t i = 0; i <= 4; i++) {
    //    anim.UpdateNodeSize(i, 9, 9);
    //}


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