#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/gnuplot.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fstream>
using namespace ns3;

/*
 * 
 * p2p (client) --------<-------Wifi AP     <<<<<      Wifi STA(source)
 * 
*/
std::string delay="0ms";		//delay;p2p link
std::string datarate="35Mbps";	//data rate of p2p link
Time stoptime=Seconds(5.0);


NS_LOG_COMPONENT_DEFINE ("WifiAPServer");

int main (int argc, char *argv[])
{
    uint32_t port=5000;
    double rss = 0;
    std::string phyMode ("ErpOfdmRate54Mbps");//wifi channel
    std::string ctlMode ("ErpOfdmRate54Mbps");//wifi channel


    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue (1360));
    Config::SetDefault("ns3::WifiMacQueue::MaxPacketNumber",UintegerValue(100));

    // disable fragmentation for frames below 2200 bytes
    Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
    // turn off RTS/CTS for frames below 2200 bytes
    Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
    // Fix non-unicast data rate to be the same as that of unicast
    Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                        StringValue (phyMode));

/**********************define nodes***************/
    NodeContainer wifinodes;
    wifinodes.Create (2);//create two wifi nodes..one is an AP and other is a STA

    NodeContainer p2pnodes;

    p2pnodes.Add(wifinodes.Get(0));//attach the AP to p2p nodes
    p2pnodes.Create (1);//create p2p client

/**************************************************/
    WifiHelper wifi;

    wifi.SetStandard (WIFI_PHY_STANDARD_80211g);

    YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
    wifiPhy.Set ("RxGain", DoubleValue (0) );
    wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

    YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");

    wifiChannel.AddPropagationLoss ("ns3::FixedRssLossModel","Rss",DoubleValue (rss));
    wifiPhy.SetChannel (wifiChannel.Create ());
    NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                  "DataMode",StringValue (phyMode),
                                  "ControlMode",StringValue (phyMode));

    /***********************************************************/
    NetDeviceContainer devices;

    Ssid ssid = Ssid ("ns-3-ssid");
    wifiMac.SetType ("ns3::StaWifiMac",
                     "Ssid", SsidValue (ssid),
                     "ActiveProbing", BooleanValue (false));

    devices.Add(wifi.Install (wifiPhy, wifiMac, wifinodes.Get(1))); //install STA mode on all the wireless nodes

    wifiMac.SetType ("ns3::ApWifiMac",
                     "Ssid", SsidValue (ssid));

    NetDeviceContainer devicesap = wifi.Install (wifiPhy, wifiMac, wifinodes.Get(0));
    devices.Add(devicesap.Get(0)); //install AP mode

/*******************************************************/
    MobilityHelper  mobility;
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (wifinodes);

/*******************************************/

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue (datarate));
    pointToPoint.SetChannelAttribute ("Delay", StringValue (delay));
    pointToPoint.SetDeviceAttribute ("Mtu", UintegerValue (1500));

    NetDeviceContainer devices1;
    devices1 = pointToPoint.Install (p2pnodes);


/************************************************/
    InternetStackHelper stack;
    stack.Install (wifinodes);
    stack.Install(p2pnodes.Get(1));

    Ipv4AddressHelper address;

    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign (devices); //wifi devices addresses

    address.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces1 = address.Assign (devices1);//p2p devices addresses

/*****************************************************RECEIVERs*/

    ApplicationContainer sinkApps;

    PacketSinkHelper sink ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
    sinkApps.Add (sink.Install (p2pnodes.Get (1)));
    sinkApps.Start (Seconds(0));
    sinkApps.Stop (stoptime);

/************************SENDER**********************************/

    ApplicationContainer sourceApps;
    BulkSendHelper source ("ns3::TcpSocketFactory",(InetSocketAddress (interfaces1.GetAddress (1), port)));
    source.SetAttribute ("MaxBytes", UintegerValue (0)); //total number of packets to be sent. 0 is for unlimited
    sourceApps.Add(source.Install (wifinodes.Get (1)));
/*		
		OnOffHelper onOffHelper ("ns3::UdpSocketFactory",(InetSocketAddress (InetSocketAddress (interfaces1.GetAddress (1), port))));
    onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1000]"));
    onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
//onOffHelper.SetAttribute("AccessClass", UintegerValue (UintegerValue(AC_BE)));
    onOffHelper.SetAttribute ("DataRate",StringValue ("54Mbps"));
   onOffHelper.SetAttribute ("PacketSize",UintegerValue(1500));
sourceApps.Add(onOffHelper.Install (wifinodes.Get (1)));
 */

    sourceApps.Start (Seconds(0));
    sourceApps.Stop (stoptime);


    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();




    AnimationInterface anim ("tcp.xml");
    anim.SetConstantPosition (wifinodes.Get(0), 1, 2);

    wifiPhy.EnablePcapAll ("wifi");
    pointToPoint.EnablePcapAll ("p2p");

    Simulator::Stop (stoptime);
    Simulator::Run ();




    /******************************************************/
    Simulator::Destroy ();
    return 0;

}