#include "ns3/network-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/dce-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/constant-position-mobility-model.h"

using namespace ns3;
void setPos (Ptr<Node> n, int x, int y, int z)
{
  Ptr<ConstantPositionMobilityModel> loc = CreateObject<ConstantPositionMobilityModel> ();
  n->AggregateObject (loc);
  Vector locVec2 (x, y, z);
  loc->SetPosition (locVec2);
}

int main (int argc, char *argv[])
{
  uint32_t nRtrs = 2;
  CommandLine cmd;
  cmd.AddValue ("nRtrs", "Number of routers. Default 2", nRtrs);
  cmd.Parse (argc, argv);

  NodeContainer nodes, routers;
  nodes.Create (2);
  routers.Create (2);

  DceManagerHelper dceManager;
  dceManager.SetTaskManagerAttribute ("FiberManagerType",
                                      StringValue ("UcontextFiberManager"));

  dceManager.SetNetworkStack ("ns3::LinuxSocketFdFactory",
                              "Library", StringValue ("liblinux.so"));
  LinuxStackHelper stack;
  stack.Install (nodes);
  stack.Install (routers);

  dceManager.Install (nodes);
  dceManager.Install (routers);

  PointToPointHelper pointToPoint;
  NetDeviceContainer devices1, devices2;
  Ipv4AddressHelper address1, address2;
  std::ostringstream cmd_oss;
  address1.SetBase ("10.1.0.0", "255.255.255.0");
  address2.SetBase ("10.2.0.0", "255.255.255.0");

      // Left link
      pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
      pointToPoint.SetChannelAttribute ("Delay", StringValue ("10ms"));
      devices1 = pointToPoint.Install (nodes.Get (0), routers.Get (0));
      // Assign ip addresses
      Ipv4InterfaceContainer if1 = address1.Assign (devices1);
      address1.NewNetwork ();
      // setup ip routes
      cmd_oss.str ("");
      cmd_oss << "rule add from " << if1.GetAddress (0, 0) << " table 1";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.1." << 0 << ".0/24 dev sim" << 0 << " scope link table 1";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add default via " << if1.GetAddress (1, 0) << " dev sim" << 0 << " table 1";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.1.0.0/24 via " << if1.GetAddress (1, 0) << " dev sim0";
      LinuxStackHelper::RunIp (routers.Get (0), Seconds (0.2), cmd_oss.str ().c_str ());
      
      pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
      pointToPoint.SetChannelAttribute ("Delay", StringValue ("10ms"));
      devices1 = pointToPoint.Install (nodes.Get (0), routers.Get (1));
      // Assign ip addresses
      if1 = address1.Assign (devices1);
      address1.NewNetwork ();
      // setup ip routes
      cmd_oss.str ("");
      cmd_oss << "rule add from " << if1.GetAddress (0, 0) << " table 2";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.1." << 1 << ".0/24 dev sim" << 1 << " scope link table 2";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add default via " << if1.GetAddress (1, 0) << " dev sim" << 1 << " table 2";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.1.1.0/24 via " << if1.GetAddress (1, 0) << " dev sim0";
      LinuxStackHelper::RunIp (routers.Get (0), Seconds (0.2), cmd_oss.str ().c_str ());

      // Right link
      pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
      pointToPoint.SetChannelAttribute ("Delay", StringValue ("10ms"));
      devices2 = pointToPoint.Install (nodes.Get (1), routers.Get (0));
      // Assign ip addresses
      Ipv4InterfaceContainer if2 = address2.Assign (devices2);
      address2.NewNetwork ();
      // setup ip routes
      cmd_oss.str ("");
      cmd_oss << "rule add from " << if2.GetAddress (0, 0) << " table " << (1);
      LinuxStackHelper::RunIp (nodes.Get (1), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.2." << 0 << ".0/24 dev sim" << 0 << " scope link table " << (1);
      LinuxStackHelper::RunIp (nodes.Get (1), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add default via " << if2.GetAddress (1, 0) << " dev sim" << 0 << " table " << (1);
      LinuxStackHelper::RunIp (nodes.Get (1), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.2.0.0/24 via " << if2.GetAddress (1, 0) << " dev sim1";
      LinuxStackHelper::RunIp (routers.Get (0), Seconds (0.2), cmd_oss.str ().c_str ());
      
      pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
      pointToPoint.SetChannelAttribute ("Delay", StringValue ("10ms"));
      devices2 = pointToPoint.Install (nodes.Get (1), routers.Get (1));
      // Assign ip addresses
      if2 = address2.Assign (devices2);
      address2.NewNetwork ();
      // setup ip routes
      cmd_oss.str ("");
      cmd_oss << "rule add from " << if2.GetAddress (0, 0) << " table " << (2);
      LinuxStackHelper::RunIp (nodes.Get (1), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.2." << 1 << ".0/24 dev sim" << 1 << " scope link table " << (2);
      LinuxStackHelper::RunIp (nodes.Get (1), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add default via " << if2.GetAddress (1, 0) << " dev sim" << 1 << " table " << (2);
      LinuxStackHelper::RunIp (nodes.Get (1), Seconds (0.1), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.2.1.0/24 via " << if2.GetAddress (1, 0) << " dev sim1";
      LinuxStackHelper::RunIp (routers.Get (1), Seconds (0.2), cmd_oss.str ().c_str ());

      setPos (routers.Get (0), 50, 20, 0);
      setPos (routers.Get (1), 50, 40, 0);
    

  // default route
  LinuxStackHelper::RunIp (nodes.Get (0), Seconds (0.1), "route add default via 10.1.0.2 dev sim0");
  LinuxStackHelper::RunIp (nodes.Get (1), Seconds (0.1), "route add default via 10.2.0.2 dev sim0");
  LinuxStackHelper::RunIp (nodes.Get (0), Seconds (0.1), "rule show");

  // debug
  stack.SysctlSet (nodes, ".net.mptcp.mptcp_debug", "1");

  DceApplicationHelper dce;
  ApplicationContainer apps;

  dce.SetStackSize (1 << 20);

  // Launch iperf client on node 0
  dce.SetBinary ("iperf");
  dce.ResetArguments ();
  dce.ResetEnvironment ();
  dce.AddArgument ("-c");
  dce.AddArgument ("10.2.0.1");
  dce.AddArgument ("-i");
  dce.AddArgument ("1");
  dce.AddArgument ("--time");
  dce.AddArgument ("100");

  apps = dce.Install (nodes.Get (0));
  apps.Start (Seconds (5.0));
  apps.Stop (Seconds (200));

  // Launch iperf server on node 1
  dce.SetBinary ("iperf");
  dce.ResetArguments ();
  dce.ResetEnvironment ();
  dce.AddArgument ("-s");
  dce.AddArgument ("-P");
  dce.AddArgument ("1");
  apps = dce.Install (nodes.Get (1));

  pointToPoint.EnablePcapAll ("boogy", false);

  apps.Start (Seconds (4));

  setPos (nodes.Get (0), 0, 20 * (nRtrs - 1) / 2, 0);
  setPos (nodes.Get (1), 100, 20 * (nRtrs - 1) / 2, 0);
  
  LinuxStackHelper::RunIp (nodes.Get (0), Seconds (11), "link set down dev sim1");
  LinuxStackHelper::RunIp (routers.Get (1), Seconds (11), "link set down dev sim0");
  
  LinuxStackHelper::RunIp (nodes.Get (0), Seconds (13), "link set up dev sim1");
  LinuxStackHelper::RunIp (routers.Get (1), Seconds (13), "link set up dev sim0");
  LinuxStackHelper::RunIp (nodes.Get (0), Seconds (13.5), "addr list");
  LinuxStackHelper::RunIp (nodes.Get (0), Seconds (13.5), "route show table all");
  LinuxStackHelper::RunIp (routers.Get (1), Seconds (13.5), "addr list");
  LinuxStackHelper::RunIp (routers.Get (1), Seconds (13.5), "route show table all");
  
  if1 = address1.Assign (devices1);
      address1.NewNetwork ();
      // setup ip routes
      cmd_oss.str ("");
      cmd_oss << "rule add from " << if1.GetAddress (0, 0) << " table 2";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (13.5), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.1." << 2 << ".0/24 dev sim" << 1 << " scope link table 2";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (13.5), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add default via " << if1.GetAddress (1, 0) << " dev sim" << 1 << " table 2";
      LinuxStackHelper::RunIp (nodes.Get (0), Seconds (13.5), cmd_oss.str ().c_str ());
      cmd_oss.str ("");
      cmd_oss << "route add 10.1.2.0/24 via " << if1.GetAddress (1, 0) << " dev sim0";
      LinuxStackHelper::RunIp (routers.Get (0), Seconds (13.5), cmd_oss.str ().c_str ());
  
  LinuxStackHelper::RunIp (nodes.Get (0), Seconds (14), "route add default via 10.1.0.2 dev sim0");
  

  Simulator::Stop (Seconds (200.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
