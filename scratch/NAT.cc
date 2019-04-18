//
// Created by IlyaWhite on 18.04.19.
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

NS_LOG_COMPONENT_DEFINE ("Trying_toBuild_NAT");

void AddMobility(double val1, double val2, NodeContainer container);

int
main (int argc, char *argv[])
{

   
    
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

    /** NODES */
    
    NodeContainer nodes;
    nodes.Create (2);

    NodeContainer second;
    second.Add ( nodes.Get (1) );
    second.Create (1);

    Names::Add("Client", nodes.Get(0));
    Names::Add("Gate", nodes.Get(1));
    Names::Add("Server", second.Get(1));

    /** MOBILITY */

    /** NODES*/
    AddMobility(20.0, 0.0, nodes.Get(0));
    AddMobility(20.0, 10.0, nodes.Get(1));
    AddMobility(25.0, 15.0, second.Get(1));

    CsmaHelper csma;
    csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
    csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

    NetDeviceContainer devices = csma.Install (nodes);
    NetDeviceContainer devices2 = pointToPoint.Install (second);

    InternetStackHelper internet;
    internet.Install (nodes);
    internet.Install (second.Get (1));

    Ipv4AddressHelper address1;
    address1.SetBase ("192.168.1.0", "255.255.255.0");

    Ipv4AddressHelper address2;
    address2.SetBase ("203.82.48.0", "255.255.255.0");

    Ipv4InterfaceContainer firstInterfaces = address1.Assign (devices);
    Ipv4InterfaceContainer secondInterfaces = address2.Assign (devices2);

    Ipv4NatHelper natHelper;
    // The zeroth element of the second node container is the NAT node
    Ptr<Ipv4Nat> nat = natHelper.Install (second.Get (0));
    nat->SetInside (1);
    nat->SetOutside (2);

    nat->AddAddressPool (Ipv4Address ("192.168.1.2"), Ipv4Mask ("255.255.255.255"));
    nat->AddPortPool (49153, 49163);

    Ipv4DynamicNatRule rule (Ipv4Address ("192.168.1.0"), Ipv4Mask ("255.255.255.0"));
    nat->AddDynamicRule (rule);

    UdpEchoServerHelper echoServer (9);

    // This application corresponds to the first rule
    ApplicationContainer serverApps = echoServer.Install (second.Get (1));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

    UdpEchoClientHelper echoClient (secondInterfaces.GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (512));

    ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

    clientApps = echoClient.Install (nodes.Get (1));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

    // Prepare to run the simulation
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    pointToPoint.EnablePcapAll ("ipv4-nat", false);
    csma.EnablePcapAll ("ipv4-nat", false);

    /** ANIMATION */

    NS_LOG_UNCOND ("Create animation");
    AnimationInterface anim("NAT .xml");



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