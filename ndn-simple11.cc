#include "ns3/ndnSIM-module.h"
#include "ns3/core-module.h"
#include "ns3/vector.h"
#include "ns3/string.h"
#include "ns3/socket.h"
#include "ns3/double.h"
#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/command-line.h"
#include "ns3/mobility-model.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include <iostream>
#include "ns3/ocb-wifi-mac.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/vector.h"
#include "ns3/string.h"
#include "ns3/socket.h"
#include "ns3/double.h"
#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/wifi-module.h"
#include "ns3/random-variable-stream.h"
#include "ns3/ndnSIM/apps/ndn-producer.hpp"
#include "ns3/ndnSIM/apps/ndn-consumer-cbr.hpp"
#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/helper/ndn-app-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include <ns3/ndnSIM/helper/ndn-global-routing-helper.hpp>
#include "ns3/animation-interface.h"
#include "ns3/ocb-wifi-mac.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/yans-wifi-helper.h"

//#include "ndn-l3-aggregate-tracer.h"

#include <algorithm>
#include <vector>

extern int array007[20][10];

namespace ns3{

/**
 * This scenario simulates a scenario with 6 cars movind and communicating
 * in an ad-hoc way.
 *
 * 5 consumers request data from producer with frequency 1 interest per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-v2v-simple
 *
 * To modify the mobility model, see function installMobility.
 * To modify the wifi model, see function installWifi.
 * To modify the NDN settings, see function installNDN and for consumer and
 * producer settings, see functions installConsumer and installProducer
 * respectively.
 */

NS_LOG_COMPONENT_DEFINE ("V2VSimple");

int a=0;
static const uint32_t numNodes = 11;

void printPosition(Ptr<const MobilityModel> mobility) //DEBUG purpose
{
  Simulator::Schedule(Seconds(1), &printPosition, mobility);
  NS_LOG_INFO("Car "<<  mobility->GetObject<Node>()->GetId() << " is at: " <<mobility->GetPosition());
}


void installMobility(NodeContainer &c, int simulationEnd)
{
//	int ii; int b; int cc;
	//for (ii=0; ii<2000; ii++)
	{
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::WaypointMobilityModel");
  mobility.Install(c);

  bool test = true;


    Ptr<WaypointMobilityModel> wayMobility[numNodes];
    for (uint32_t i = 0; i < numNodes; i++) {
  //  	cc= i* 65;  //b= i* 65;
      wayMobility[i] = c.Get(i)->GetObject<WaypointMobilityModel>();
      Waypoint waypointStart(Seconds(0), Vector3D((i* 65), 0, 0));
      Waypoint waypointEnd(Seconds(simulationEnd), Vector3D((i* 65+10000), 0, 0));

      wayMobility[i]->AddWaypoint(waypointStart);
      wayMobility[i]->AddWaypoint(waypointEnd);
      NS_LOG_INFO("Node " << i << " positions " << waypointStart << " : " << waypointEnd);
    }

//	 cc= cc +10;
    return;
//	}
  }
  /*
  Ptr<WaypointMobilityModel> wayMobility[numNodes];
  for (uint32_t i = 0; i < numNodes; i++) {
    wayMobility[i] = c.Get(i)->GetObject<WaypointMobilityModel>();
    Waypoint waypointStart(Seconds(0), Vector3D(i*10, 0, 0));
    NS_LOG_UNCOND("*Node " << i << " waypointStart ");
    Waypoint waypointMiddle(Seconds(simulationEnd/2), Vector3D(i*10+5, 0, 0));
    NS_LOG_UNCOND("**Node " << i << " waypointMiddle ");
    Waypoint waypointEnd(Seconds(simulationEnd+1), Vector3D(i*50+5, 0, 0));
    NS_LOG_UNCOND("***Node " << i << " waypointEnd ");

    wayMobility[i]->AddWaypoint(waypointStart);
    wayMobility[i]->AddWaypoint(waypointMiddle);
    wayMobility[i]->AddWaypoint(waypointEnd);
    NS_LOG_INFO("Node " << i << " positions " << waypointStart << " : " << waypointMiddle << " " << waypointEnd);
  }*/
}


void installWifi(NodeContainer &c, NetDeviceContainer &devices)
{
  /*
	// Modulation and wifi channel bit rate
  std::string phyMode("OfdmRate24Mbps");

  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue(phyMode));

  WifiHelper wifi;
  wifi.SetStandard(WIFI_PHY_STANDARD_80211a);

  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
  wifiPhy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss("ns3::RangePropagationLossModel",
                                 "MaxRange", DoubleValue(19.0));
  wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel",
                                 "m0", DoubleValue(1.0),
                                 "m1", DoubleValue(1.0),
                                 "m2", DoubleValue(1.0));
  wifiPhy.SetChannel(wifiChannel.Create());

  // Add a non-QoS upper mac
//  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default();
  WifiMacHelper wifiMac;

  // Set it to adhoc mode
  wifiMac.SetType("ns3::AdhocWifiMac");

  // Disable rate control
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode", StringValue(phyMode),
                               "ControlMode", StringValue(phyMode));

  devices = wifi.Install(wifiPhy, wifiMac, c);
  */
	// The below set of helpers will help us to put together the wifi NICs we want
	  std::string phyMode ("OfdmRate6MbpsBW10MHz");
	  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
	  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
	  Ptr<YansWifiChannel> channel = wifiChannel.Create ();
	  wifiPhy.SetChannel (channel);
	  // ns-3 supports generate a pcap trace
	  wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11);
	  NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default ();
	  Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default ();
	  bool verbose = false;

	  if (verbose)
	    {
	      wifi80211p.EnableLogComponents ();      // Turn on all Wifi 802.11p logging
	    }

	  wifi80211p.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
	                                      "DataMode",StringValue (phyMode),
	                                      "ControlMode",StringValue (phyMode));
	  devices = wifi80211p.Install (wifiPhy, wifi80211pMac, c);



}

void installNDN(NodeContainer &c)
{
  ndn::StackHelper ndnHelper;
 // ndnHelper.SetOldContentStore ("ns3::ndn::cs::Lru", "MaxSize", "100"); // default ContentStore parameters    ///New Changed
  ndnHelper.SetDefaultRoutes(true);

  ndnHelper.Install(c);
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/broadcast");

  ///todo add v2v face
}


void installConsumer(NodeContainer &c)
{
  ndn::AppHelper helper("ns3::ndn::ConsumerCbr");
  //helper.SetAttribute("Frequency", DoubleValue (1)); //------------------------------->
  helper.SetAttribute("Frequency", StringValue("3")); // 10 interests a second

//  helper.SetAttribute("Randomize", StringValue("uniform"));
  helper.SetPrefix("/Monet");

  for(uint32_t i=0; i<c.GetN(); i++){
	//  helper.SetAttribute("StartSeq", IntegerValue(i*50));
	ApplicationContainer app13 =  helper.Install(c.Get(i));
	//app13.Start (Seconds (3.0));
  }
}


void installProducer(NodeContainer &c)
{
  ndn::AppHelper producerHelper("CustomApp");
 // producerHelper.SetPrefix1("/malik");

  producerHelper.Install(c.Get(0));
  NS_LOG_INFO("Producer installed on node " << c.Get(0)->GetId());

}


/*
void installProducer1(NodeContainer &c)
{
  ndn::AppHelper producerHelper1("ns3::ndn::Producer");
  producerHelper1.SetPrefix("/v2v");

  producerHelper1.Install(c.Get(0));
  NS_LOG_INFO("Producer1s installed on node " << c.Get(0)->GetId());

}
*/


void installApp5(NodeContainer &c)
{
	ndn::AppHelper consumerHelper1 ("ns3::ndn::MyChanged");
	 consumerHelper1.SetPrefix1("/Monet");
	 ApplicationContainer app1 = consumerHelper1.Install(c.Get(0));
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void installApp51(NodeContainer &c)
{
	ndn::AppHelper consumerHelper13 ("ns3::ndn::MyChanged1");
	 consumerHelper13.SetPrefix1("/Monet1");
	 ApplicationContainer app1 = consumerHelper13.Install(c.Get(0));
	 NS_LOG_INFO("Router installed on node " << c.Get(0)->GetId());
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}


void installApp52(NodeContainer &c)
{
	ndn::AppHelper consumerHelper14 ("ns3::ndn::MyChanged2");
	 consumerHelper14.SetPrefix1("/Monet2");
	 ApplicationContainer app1 = consumerHelper14.Install(c.Get(0));
	 NS_LOG_INFO("Router installed on node " << c.Get(0)->GetId());
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}

void installApp53(NodeContainer &c)
{
	ndn::AppHelper consumerHelper15 ("ns3::ndn::MyChanged3");
	 consumerHelper15.SetPrefix1("/Monet3");
	 ApplicationContainer app1 = consumerHelper15.Install(c.Get(0));
	 NS_LOG_INFO("Router installed on node " << c.Get(0)->GetId());
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}



void installApp54(NodeContainer &c)
{
	ndn::AppHelper consumerHelper16 ("ns3::ndn::MyChanged4");
	 consumerHelper16.SetPrefix1("/Monet4");
	 ApplicationContainer app1 = consumerHelper16.Install(c.Get(0));
	 NS_LOG_INFO("Router installed on node " << c.Get(0)->GetId());
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}

void installApp55(NodeContainer &c)
{
	ndn::AppHelper consumerHelper17 ("ns3::ndn::MyChanged5");
	 consumerHelper17.SetPrefix1("/Monet5");
	 ApplicationContainer app1 = consumerHelper17.Install(c.Get(0));
	 NS_LOG_INFO("Router installed on node " << c.Get(0)->GetId());
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}

void installApp56(NodeContainer &c)
{
	ndn::AppHelper consumerHelper18 ("ns3::ndn::MyChanged6");
	 consumerHelper18.SetPrefix1("/Monet6");
	 ApplicationContainer app1 = consumerHelper18.Install(c.Get(0));
	 NS_LOG_INFO("Router installed on node " << c.Get(0)->GetId());
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}

void installApp57(NodeContainer &c)
{
	ndn::AppHelper consumerHelper19 ("ns3::ndn::MyChanged7");
	 consumerHelper19.SetPrefix1("/Monet7");
	 ApplicationContainer app1 = consumerHelper19.Install(c.Get(0));
	 NS_LOG_INFO("Router installed on node " << c.Get(0)->GetId());
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}

void installApp58(NodeContainer &c)
{
	ndn::AppHelper consumerHelper20 ("ns3::ndn::MyChanged8");
	 consumerHelper20.SetPrefix1("/Monet8");
	 ApplicationContainer app1 = consumerHelper20.Install(c.Get(0));
	 NS_LOG_INFO("Router installed on node " << c.Get(0)->GetId());
	  //ApplicationContainer app2 = consumerHelper1.Install(nodes.Get(1));
	 // app1.Start (Seconds (0.0));
  //NS_LOG_INFO("Applicaticouton installed on node " << c.Get(0)->GetId());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
	 NS_LOG_INFO ("V2VTest Simulator");

//  uint32_t numProducer = 1;
  int simulationEnd = 1000;

  NodeContainer c;
  c.Create(numNodes);

  installMobility(c, simulationEnd);

  NetDeviceContainer netDevices;
  installWifi(c, netDevices);

  installNDN(c);

  //setting application
/*  Ptr<UniformRandomVariable> randomNum = CreateObject<UniformRandomVariable> ();
  uint32_t producerId = randomNum->GetValue(0,numNodes-1);

  NodeContainer producer;
  producer.Add(c.Get(producerId));

  NodeContainer consumers;
  for(uint32_t i=0; i<numNodes; i++){
    if(i!=producerId){
      consumers.Add(c.Get(i));
      NS_LOG_INFO("Consumer " << i);
      // break;//tmp ******************
    }
  }
*/
  for (uint32_t n11=0; n11<20 ;n11++)
      {
      	  for (uint32_t n12=0; n12<10 ;n12++)
          {
      		  array007[n11][n12] = 0;
          }
 //             cout<<endl;
      }

  NodeContainer producer;
  NodeContainer consumer;
 // NodeContainer producer1;
  NodeContainer App5;
  NodeContainer App51; //////////////////////////////////////////////new changed
  NodeContainer App52; //////////////////////////////////////////////new changed
  NodeContainer App53; //////////////////////////////////////////////new changed
  NodeContainer App54; //////////////////////////////////////////////new changed
  NodeContainer App55; //////////////////////////////////////////////new changed
  NodeContainer App56; //////////////////////////////////////////////new changed
  NodeContainer App57; //////////////////////////////////////////////new changed
  NodeContainer App58; //////////////////////////////////////////////new changed


  producer.Add(c.Get(10));
  consumer.Add(c.Get(0));
 // producer1.Add(c.Get(2));
  App5.Add(c.Get(1));
  App51.Add(c.Get(2));////////////////////////////////////////////////////new changed
  App52.Add(c.Get(3));////////////////////////////////////////////////////new changed
  App53.Add(c.Get(4));////////////////////////////////////////////////////new changed
  App54.Add(c.Get(5));////////////////////////////////////////////////////new changed
  App55.Add(c.Get(6));////////////////////////////////////////////////////new changed
  App56.Add(c.Get(7));////////////////////////////////////////////////////new changed
  App57.Add(c.Get(8));////////////////////////////////////////////////////new changed
  App58.Add(c.Get(9));////////////////////////////////////////////////////new changed



  installConsumer(consumer);
  installProducer(producer);
  //installProducer1(producer1);
  installApp5(App5);
  installApp51(App51);////////////////////////////////////////////////////new changed
  installApp52(App52);////////////////////////////////////////////////////new changed
  installApp53(App53);////////////////////////////////////////////////////new changed
  installApp54(App54);////////////////////////////////////////////////////new changed
  installApp55(App55);////////////////////////////////////////////////////new changed
  installApp56(App56);////////////////////////////////////////////////////new changed
  installApp57(App57);////////////////////////////////////////////////////new changed
  installApp58(App58);////////////////////////////////////////////////////new changed





//Printing the position of moving vehicle after every one second
  for(uint32_t i=0; i<c.GetN(); i++){
  Simulator::Schedule(Seconds(1), &printPosition, c.Get(i)->GetObject<WaypointMobilityModel>());
  }

  Simulator::Stop(Seconds(simulationEnd));

  std::string animFile = "v2v-test.xml";
  AnimationInterface anim(animFile);

  //////////////////////////////////////////////TRACING//////////////////////////////////////////////////////////////////////

  //ndn::L3AggregateTracer::InstallAll ("aggregate-trace.txt", Seconds (0.1));
//   ndn::L3RateTracer::InstallAll ("rate-trace.txt", Seconds (0.1));
//   L2RateTracer::InstallAll ("drop-trace.txt", Seconds (0.1));
//   ndn::CsTracer::InstallAll ("cs-trace.txt", Seconds (0.1));
//   ndn::AppDelayTracer::InstallAll ("app-delays-trace.txt");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  Simulator::Run ();
  return 0;
}
} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}

