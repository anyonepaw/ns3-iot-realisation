//
// Created by vadim on 15.04.19.
//

// 10.1.1.0

// n0 -------------- n1

// точка-точка

 // Импорт библиотек

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

// Декларация пространства имён

using namespace ns3;

//Запуск логгирования

NS_LOG_COMPONENT_DEFINE ("onething");


// Объявление главной функции

int
main(int argc, char *argv[]) {


//Логгирование для компонентов echo-клиента и -сервера

LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

// Создание узлов, осуществляющих взаимодействие

NodeContainer nodes;
nodes.Create (3);

// Скорость сети и задержка сети

PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

// Конфигурация сетевой карты

NetDeviceContainer devices;
devices = pointToPoint.Install (nodes);

//Настройка стека протколов

InternetStackHelper stack;
stack.Install (nodes);

//Установка базового IP и маски

static Ipv4AddressHelper ADDRESS;
ADDRESS.SetBase ("10.1.1.0", "255.255.255.0");

//Присовение IP
Ipv4InterfaceContainer interfaces = ADDRESS.Assign (devices);

//Установка порта
UdpEchoServerHelper echoServer (9);

//Интервал генерации трафика
ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
ApplicationContainer serverApps1 = echoServer.Install (nodes.Get (2));
serverApps.Start (Seconds (1.0));
serverApps.Stop (Seconds (10.0));


//Настройка клиентского приложения
UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

//Запуск приложения в интервале от 2-х до 10 секунд
ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
clientApps.Start (Seconds (2.0));
clientApps.Stop (Seconds (10.0));

//Сбор трафика

pointToPoint.EnablePcapAll("onething");

//Запуск моделирования
Simulator::Run ();



//Очистка памяти
Simulator::Destroy ();
return 0;
}