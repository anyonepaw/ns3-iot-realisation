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

extern "C" {
#include "MQTTClient.h"
}

#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L


//using namespace ns3;
using namespace std;

//NS_LOG_COMPONENT_DEFINE ("Trying_toBuild_NAT");

//void addMobility(double val1, double val2, NodeContainer container);

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char *payloadptr;
    printf("Message arrived\n");
    printf(" topic: %s\n", topicName);
    printf(" message: ");
    payloadptr = static_cast<char *>(message->payload);
    for (i = 0; i < message->payloadlen; i++) {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf(" cause: %s\n", cause);
}


int
main(int argc, char *argv[]) {


    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);
    do {
        ch = getchar();
    } while (ch != 'Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;














//    pubmsg.payload = PAYLOAD;
//    pubmsg.payloadlen = strlen(PAYLOAD);
//    pubmsg.qos = QOS;
//    pubmsg.retained = 0;
//    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
//    printf("Waiting for up to %d seconds for publication of %s\n"
//           "on topic %s for client with ClientID: %s\n",
//           (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
//    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
//    printf("Message with delivery token %d delivered\n", token);
//    MQTTClient_disconnect(client, 10000);
//    MQTTClient_destroy(&client);
//
//    //добавляем логирование для клиента и сервера
//
//    LogComponentEnable ("TcpEchoServerApplication", LOG_LEVEL_INFO);
//    LogComponentEnable ("TcpEchoClientApplication", LOG_LEVEL_INFO);
//
//    /** NODES */
//
//    //первые 2 узла
//
//    NodeContainer firstNodeContainer;
//    firstNodeContainer.Create (2);
//
//    //вторые 2 узла
//
//    NodeContainer secondNodeContainer;
//    secondNodeContainer.Add ( firstNodeContainer.Get (1) );
//    secondNodeContainer.Create (1);
//
//    //именуем узлы
//
//    Names::Add("Client", firstNodeContainer.Get(0));
//    Names::Add("Gate", firstNodeContainer.Get(1));
//    Names::Add("Server", secondNodeContainer.Get(1));
//
//    /** MOBILITY */ //задаем координаты узлам
//
//    /** NODES */
//    addMobility(20.0, 0.0, firstNodeContainer.Get(0));
//    addMobility(20.0, 10.0, firstNodeContainer.Get(1));
//    addMobility(25.0, 15.0, secondNodeContainer.Get(1));
//
//    //строим топологии для двух пар узлов
//
//    PointToPointHelper p2p1;
//    p2p1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
//    p2p1.SetChannelAttribute("Delay", StringValue("1ms"));
//
//    PointToPointHelper p2p2;
//    p2p2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
//    p2p2.SetChannelAttribute ("Delay", StringValue ("2ms"));
//
//    //подстраиваем топологии к узлам
//
//    NetDeviceContainer devices = p2p1.Install (firstNodeContainer);
//    NetDeviceContainer devices2 = p2p2.Install (secondNodeContainer);
//
//    // создаем и устанавливаем все сетевые карты, учавсвтующие в передаче
//
//    InternetStackHelper internet;
//    internet.Install (firstNodeContainer);
//    internet.Install (secondNodeContainer.Get (1));
//
//    // устанавливаем базовый IP адресс и маску для первых и вторых пар узлов
//
//    Ipv4AddressHelper address1;
//    address1.SetBase ("192.168.1.0", "255.255.255.0");
//
//    Ipv4AddressHelper address2;
//    address2.SetBase ("203.82.48.0", "255.255.255.0");
//
//    // присваеваем IP адресс нашим устройствам
//
//    Ipv4InterfaceContainer firstInterfaces = address1.Assign (devices);
//    Ipv4InterfaceContainer secondInterfaces = address2.Assign (devices2);
//
//
////NAT
////        private address    NAT      public address
//// n0 <--------------------> n1 <-----------------------> n2
//// 192.168.1.1   192.168.1.2    203.82.48.1  203.82.48.2
////
//
//    Ipv4NatHelper natHelper;
//    //нулевой элемент в secondNodeContainer узхлах это НАТ узел
//    Ptr<Ipv4Nat> nat = natHelper.Install (secondNodeContainer.Get (0));
//    // Configure which of its Ipv4Interfaces are inside and outside interfaces
//    // The zeroth Ipv4Interface is reserved for the loopback interface
//    // Hence, the interface facing n0 is numbered "1" and the interface
//    // facing n2 is numbered "2" (since it was assigned in the secondNodeContainer step above)
//    nat->SetInside (1);
//    nat->SetOutside (2);
//
//    //Adding the address to be translated to and port pools.
//
//    nat->AddAddressPool (Ipv4Address ("192.168.1.2"), Ipv4Mask ("255.255.255.255"));
//    nat->AddPortPool (49153, 49163);
//
//    // Add a rule here to map outbound connections from n0, port 49153, UDP
//
//    Ipv4DynamicNatRule rule (Ipv4Address ("192.168.1.0"), Ipv4Mask ("255.255.255.0"));
//    nat->AddDynamicRule (rule);
//
//
//    Ptr<OutputStreamWrapper> natStream = Create<OutputStreamWrapper> ("nat.rules", std::ios::out);
//    nat->PrintTable (natStream);
//
//
//    // Протокол TCP: клиент-серверное сообщение
//    NS_LOG_UNCOND ("TCP trace:");
//
//    TcpEchoClientHelper tcpClient(secondInterfaces.GetAddress (1), 9);
//    tcpClient.SetAttribute ("MaxPackets", UintegerValue (1));
//    tcpClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
//    tcpClient.SetAttribute ("PacketSize", UintegerValue (512));
//
//    ApplicationContainer client1 = tcpClient.Install (firstNodeContainer.Get (0));
//    client1.Start(Seconds (2.0));
//    client1.Stop (Seconds (10.0));
//
//    TcpEchoServerHelper tcpServer(9);
//    ApplicationContainer server = tcpServer.Install (secondNodeContainer.Get (1));
//    server.Start (Seconds (1.0));
//    server.Stop (Seconds (10.0));
//
//
//    /* PCAP */
//    p2p2.EnablePcapAll ("ipv4-nat", false);
//    p2p1.EnablePcapAll ("ipv4-nat", false);
//
//    p2p1.EnablePcapAll("p2p1");
//    p2p2.EnablePcapAll("p2p2");
//
//
//    /* ANIMATION */
//    AnimationInterface anim("one-iot.xml");
//
//
//    /* Simulation */
//    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
//
//    Simulator::Run ();
//    Simulator::Destroy ();
//    return 0;
//
//}
//
//void addMobility(double x_position, double y_position, NodeContainer container) {
//    MobilityHelper mobility;
//    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX",
//                                  DoubleValue(x_position), "MinY", DoubleValue(y_position));
//    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
//    mobility.Install(std::move(container));
//}
}