#include <utility>

#include <utility>

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

NS_LOG_COMPONENT_DEFINE ("Simple IoT Architecture");

//          GlobalNetwork                   GlobalNetwork                LocalNetwork1               LocalNetwork2
//          public address                  public address       NAT    private address    NAT      private address
//   n4 <-------------------------> n3 <-----------------------> n0 <--------------------> n1 <-----------------------> n2
//  client                        cloud                       router                     gate                          iot
// 312.122.128.23      312.122.128.21  203.82.48.2   203.82.48.1  192.168.1.1       192.168.1.2  10.0.0.1           10.0.0.2
//
//                p2p4                           p2p3                        p2p2                         p2p1


//          GlobalNetwork                    LocalNetwork1               LocalNetwork2
//          public address          NAT    private address    NAT       private address
//   n4 <-------------------------> n0 <--------------------> n1 <-----------------------> n2
//  client                        router                     gate                         iot
// 203.82.48.2           203.82.48.1  192.168.1.1   192.168.1.2  10.0.0.1              10.0.0.2
//
//               p2p3                           p2p2                         p2p1


void addMobility(double x_position, double y_position, NodeContainer container);

void addNat(Ptr<Node> gateNode, Ipv4Address gateAddress, Ipv4Address routerAddress);

void tcpExchange(NodeContainer server, uint16_t serverPort,
                 Address serverAddress, NodeContainer client,  uint16_t clientPort, double serverStart, double serverStop,
                 double clientStart, double clientStop);


int
main(int argc, char *argv[]) {

    //добавляем логирование для клиента и сервера, чтобы в терминал выводилась рассылка
    LogComponentEnable("TcpEchoServerApplication", LOG_LEVEL_INFO);
    LogComponentEnable("TcpEchoClientApplication", LOG_LEVEL_INFO);


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

    //подстраиваем топологии к узлам

    NetDeviceContainer devices = p2p1.Install(firstNodeContainer);
    NetDeviceContainer devices2 = p2p2.Install(secondNodeContainer);
    NetDeviceContainer devices3 = p2p3.Install(thirdNodeContainer);

    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче

    InternetStackHelper internet;
    internet.Install(firstNodeContainer);
    internet.Install(secondNodeContainer.Get(1));
    internet.Install(thirdNodeContainer.Get(1));

    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов

    Ipv4AddressHelper address1;
    address1.SetBase("192.168.1.1", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase("10.0.0.1", "255.255.255.0");

    Ipv4AddressHelper address3;
    address3.SetBase("203.82.48.1", "255.255.255.0");

    // присваеваем IP адреса нашим устройствам

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


    addNat(gate, firstAddressOfGate, secondAddressOfRouter);
    addNat(router, firstAddressOfRouter, addressOfClient);


    // Протокол TCP: клиент-серверное сообщение
    NS_LOG_UNCOND ("TCP trace:");

    tcpExchange(client, 9, addressOfClient, iot,
                9, 1.0, 10.0, 2.0, 10.0);
    tcpExchange(iot, 9, addressOfIot, client,
                9, 3.0, 15.0, 4.0, 15.0);


    /* PCAP */
//    p2p2.EnablePcapAll ("ipv4-nat");
//    p2p1.EnablePcapAll ("ipv4-nat");
//
//    p2p1.EnablePcapAll("p2p1");
//    p2p2.EnablePcapAll("p2p2");


    /* ANIMATION */
    AnimationInterface anim("one-iot1.xml");

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

void tcpExchange(NodeContainer server, uint16_t serverPort,
                 Address serverAddress, NodeContainer client,  uint16_t clientPort, double serverStart, double serverStop,
                 double clientStart, double clientStop){

    TcpEchoClientHelper tcpClient(serverAddress, clientPort);
    tcpClient.SetAttribute ("MaxPackets", UintegerValue (1));
    tcpClient.SetAttribute ("Interval", TimeValue (Seconds (3.0)));
    tcpClient.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer client1 = tcpClient.Install (std::move(client));
    client1.Start(Seconds (clientStart));
    client1.Stop (Seconds (clientStop));

    TcpEchoServerHelper tcpServer(serverPort);
    ApplicationContainer serverApp = tcpServer.Install (std::move(server));
    serverApp.Start (Seconds (serverStart));
    serverApp.Stop (Seconds (serverStop));
}