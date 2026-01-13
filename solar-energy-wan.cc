/*
 * ============================================================================
 * WIDE AREA NETWORK - Solar Energy for Community Electrification
 * ============================================================================
 * 
 * Project: Reliable Electricity Access Using Solar Energy Systems
 *          Connected via WAN for Education and Healthcare
 * 
 * Author: NGATCHA FOTSO CALEX
 * Matricule: ICTU20241105
 * Institution: ICT University Yaounde
 * Course: Wide Area Network
 * 
 * NS-3 Version: 3.29
 * 
 * ============================================================================
 * PROBLEM: Lack of Reliable Electricity Access
 * Many communities worldwide face unreliable electricity access,
 * severely limiting education and healthcare services.
 * 
 * SOLUTION: Solar Energy Systems connected via WAN
 * - Solar-powered schools and clinics
 * - WAN network for monitoring and management
 * - Remote communities connected to central grid
 * ============================================================================
 * 
 * ARCHITECTURE:
 * - Central Grid Station (main power management)
 * - WAN Network (connects remote communities)
 * - Solar-Powered Schools (education facilities)
 * - Solar-Powered Clinics (healthcare facilities)
 * - Community Solar Micro-grids
 * - Monitoring System (energy management)
 * ============================================================================
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SolarEnergyWAN");

int main(int argc, char* argv[])
{
    // ========================================================================
    // SIMULATION PARAMETERS
    // ========================================================================
    
    uint32_t nSchools = 5;              // Solar-powered schools
    uint32_t nClinics = 3;              // Solar-powered health clinics
    uint32_t nMicrogrids = 4;           // Community solar micro-grids
    double simulationTime = 30.0;       // Simulation duration (seconds)
    bool verbose = true;

    CommandLine cmd;
    cmd.AddValue("schools", "Number of solar schools", nSchools);
    cmd.AddValue("clinics", "Number of solar clinics", nClinics);
    cmd.AddValue("microgrids", "Number of community microgrids", nMicrogrids);
    cmd.AddValue("time", "Simulation time", simulationTime);
    cmd.AddValue("verbose", "Enable logging", verbose);
    cmd.Parse(argc, argv);

    if (verbose)
    {
        LogComponentEnable("SolarEnergyWAN", LOG_LEVEL_INFO);
    }

    // Display simulation header
    std::cout << "\n";
    std::cout << "================================================================\n";
    std::cout << "   SOLAR ENERGY WAN - Community Electrification System\n";
    std::cout << "================================================================\n";
    std::cout << "Project: Reliable Electricity via Solar + WAN\n";
    std::cout << "Author: NGATCHA FOTSO CALEX\n";
    std::cout << "Matricule: ICTU20241105\n";
    std::cout << "ICT University Yaounde - NS-3.29\n";
    std::cout << "================================================================\n";
    std::cout << "\nConfiguration:\n";
    std::cout << "  Solar-Powered Schools:    " << nSchools << "\n";
    std::cout << "  Solar-Powered Clinics:    " << nClinics << "\n";
    std::cout << "  Community Micro-grids:    " << nMicrogrids << "\n";
    std::cout << "  Simulation Time:          " << simulationTime << " seconds\n";
    std::cout << "================================================================\n\n";

    // ========================================================================
    // CREATE NETWORK NODES
    // ========================================================================
    
    NS_LOG_INFO("Creating network infrastructure...");

    // Central Grid Station (main power management center)
    NodeContainer centralStation;
    centralStation.Create(1);

    // Monitoring Center (energy monitoring and analytics)
    NodeContainer monitoringCenter;
    monitoringCenter.Create(1);

    // WAN Core Routers (backbone network)
    NodeContainer wanRouters;
    wanRouters.Create(3);

    // Solar-Powered Schools
    NodeContainer solarSchools;
    solarSchools.Create(nSchools);

    // Solar-Powered Health Clinics
    NodeContainer solarClinics;
    solarClinics.Create(nClinics);

    // Community Solar Micro-grids
    NodeContainer microgrids;
    microgrids.Create(nMicrogrids);

    NS_LOG_INFO("Network nodes created successfully");

    // ========================================================================
    // INSTALL INTERNET STACK
    // ========================================================================
    
    NS_LOG_INFO("Installing Internet protocol stack...");

    InternetStackHelper stack;
    stack.Install(centralStation);
    stack.Install(monitoringCenter);
    stack.Install(wanRouters);
    stack.Install(solarSchools);
    stack.Install(solarClinics);
    stack.Install(microgrids);

    // ========================================================================
    // CONFIGURE POINT-TO-POINT LINKS
    // ========================================================================
    
    NS_LOG_INFO("Configuring WAN links...");

    // High-capacity WAN backbone
    PointToPointHelper p2pWAN;
    p2pWAN.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
    p2pWAN.SetChannelAttribute("Delay", StringValue("10ms"));

    // Medium-capacity links to remote sites
    PointToPointHelper p2pRemote;
    p2pRemote.SetDeviceAttribute("DataRate", StringValue("50Mbps"));
    p2pRemote.SetChannelAttribute("Delay", StringValue("20ms")); // Remote locations

    // Low-capacity links for micro-grids
    PointToPointHelper p2pMicrogrid;
    p2pMicrogrid.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    p2pMicrogrid.SetChannelAttribute("Delay", StringValue("5ms"));

    // Central Station to WAN Router 0
    NetDeviceContainer devCentralWAN0 = p2pWAN.Install(centralStation.Get(0), wanRouters.Get(0));

    // Monitoring Center to WAN Router 0
    NetDeviceContainer devMonitorWAN0 = p2pWAN.Install(monitoringCenter.Get(0), wanRouters.Get(0));

    // WAN backbone mesh topology
    NetDeviceContainer devWAN01 = p2pWAN.Install(wanRouters.Get(0), wanRouters.Get(1));
    NetDeviceContainer devWAN12 = p2pWAN.Install(wanRouters.Get(1), wanRouters.Get(2));
    NetDeviceContainer devWAN20 = p2pWAN.Install(wanRouters.Get(2), wanRouters.Get(0));

    // Connect solar schools to WAN Router 1 (Education Network)
    NetDeviceContainer* schoolDevices = new NetDeviceContainer[nSchools];
    for (uint32_t i = 0; i < nSchools; ++i)
    {
        schoolDevices[i] = p2pRemote.Install(solarSchools.Get(i), wanRouters.Get(1));
    }

    // Connect solar clinics to WAN Router 2 (Healthcare Network)
    NetDeviceContainer* clinicDevices = new NetDeviceContainer[nClinics];
    for (uint32_t i = 0; i < nClinics; ++i)
    {
        clinicDevices[i] = p2pRemote.Install(solarClinics.Get(i), wanRouters.Get(2));
    }

    // Connect community micro-grids to WAN Router 0
    NetDeviceContainer* microgridDevices = new NetDeviceContainer[nMicrogrids];
    for (uint32_t i = 0; i < nMicrogrids; ++i)
    {
        microgridDevices[i] = p2pMicrogrid.Install(microgrids.Get(i), wanRouters.Get(0));
    }

    // ========================================================================
    // CONFIGURE MOBILITY
    // ========================================================================
    
    NS_LOG_INFO("Setting up node positions...");

    MobilityHelper mobility;

    // All nodes are fixed (solar installations are stationary)
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(centralStation);
    mobility.Install(monitoringCenter);
    mobility.Install(wanRouters);
    mobility.Install(solarSchools);
    mobility.Install(solarClinics);
    mobility.Install(microgrids);

    // ========================================================================
    // ASSIGN IP ADDRESSES
    // ========================================================================
    
    NS_LOG_INFO("Assigning IP addresses...");

    Ipv4AddressHelper address;

    // Central Station - WAN Router 0: 10.1.1.0/24
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer ifCentralWAN = address.Assign(devCentralWAN0);

    // Monitoring Center - WAN Router 0: 10.1.2.0/24
    address.SetBase("10.1.2.0", "255.255.255.0");
    address.Assign(devMonitorWAN0);

    // WAN backbone: 10.2.x.0/24
    address.SetBase("10.2.1.0", "255.255.255.0");
    address.Assign(devWAN01);
    address.SetBase("10.2.2.0", "255.255.255.0");
    address.Assign(devWAN12);
    address.SetBase("10.2.3.0", "255.255.255.0");
    address.Assign(devWAN20);

    // Solar Schools Network: 172.16.x.0/24 (Education Network)
    for (uint32_t i = 0; i < nSchools; ++i)
    {
        std::ostringstream subnet;
        subnet << "172.16." << (i + 1) << ".0";
        address.SetBase(subnet.str().c_str(), "255.255.255.0");
        address.Assign(schoolDevices[i]);
    }

    // Solar Clinics Network: 172.17.x.0/24 (Healthcare Network)
    for (uint32_t i = 0; i < nClinics; ++i)
    {
        std::ostringstream subnet;
        subnet << "172.17." << (i + 1) << ".0";
        address.SetBase(subnet.str().c_str(), "255.255.255.0");
        address.Assign(clinicDevices[i]);
    }

    // Community Micro-grids: 192.168.x.0/24
    for (uint32_t i = 0; i < nMicrogrids; ++i)
    {
        std::ostringstream subnet;
        subnet << "192.168." << (i + 1) << ".0";
        address.SetBase(subnet.str().c_str(), "255.255.255.0");
        address.Assign(microgridDevices[i]);
    }

    // Enable global routing
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    NS_LOG_INFO("IP addressing and routing configured");

    // ========================================================================
    // CONFIGURE APPLICATIONS
    // ========================================================================
    
    NS_LOG_INFO("Installing monitoring applications...");

    uint16_t port = 9;

    // Central Station Server (receives energy data and management commands)
    UdpEchoServerHelper centralServer(port);
    ApplicationContainer serverApps = centralServer.Install(centralStation.Get(0));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(simulationTime));

    Address centralAddress(InetSocketAddress(ifCentralWAN.GetAddress(0), port));

    // Solar Schools send energy usage data and receive power management
    for (uint32_t i = 0; i < nSchools; ++i)
    {
        UdpEchoClientHelper schoolClient(ifCentralWAN.GetAddress(0), port);
        schoolClient.SetAttribute("MaxPackets", UintegerValue(100));
        schoolClient.SetAttribute("Interval", TimeValue(Seconds(0.5)));
        schoolClient.SetAttribute("PacketSize", UintegerValue(256)); // Small data packets

        ApplicationContainer schoolApp = schoolClient.Install(solarSchools.Get(i));
        schoolApp.Start(Seconds(2.0 + i * 0.3));
        schoolApp.Stop(Seconds(simulationTime));
    }

    // Solar Clinics send critical health facility data
    for (uint32_t i = 0; i < nClinics; ++i)
    {
        UdpEchoClientHelper clinicClient(ifCentralWAN.GetAddress(0), port);
        clinicClient.SetAttribute("MaxPackets", UintegerValue(150));
        clinicClient.SetAttribute("Interval", TimeValue(Seconds(0.3))); // More frequent
        clinicClient.SetAttribute("PacketSize", UintegerValue(512)); // Larger packets

        ApplicationContainer clinicApp = clinicClient.Install(solarClinics.Get(i));
        clinicApp.Start(Seconds(1.5 + i * 0.2));
        clinicApp.Stop(Seconds(simulationTime));
    }

    // Community Micro-grids send energy production/consumption data
    for (uint32_t i = 0; i < nMicrogrids; ++i)
    {
        UdpEchoClientHelper microgridClient(ifCentralWAN.GetAddress(0), port);
        microgridClient.SetAttribute("MaxPackets", UintegerValue(80));
        microgridClient.SetAttribute("Interval", TimeValue(Seconds(0.8)));
        microgridClient.SetAttribute("PacketSize", UintegerValue(128));

        ApplicationContainer microgridApp = microgridClient.Install(microgrids.Get(i));
        microgridApp.Start(Seconds(3.0 + i * 0.4));
        microgridApp.Stop(Seconds(simulationTime));
    }

    NS_LOG_INFO("Applications configured successfully");

    // ========================================================================
    // FLOW MONITOR
    // ========================================================================
    
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // ========================================================================
    // NETANIM CONFIGURATION
    // ========================================================================
    
    NS_LOG_INFO("Configuring NetAnim visualization...");

    AnimationInterface anim("solar-energy-wan.xml");

    // Central Grid Station
    anim.UpdateNodeDescription(centralStation.Get(0), "Central-Grid");
    anim.UpdateNodeColor(centralStation.Get(0), 255, 215, 0); // Gold
    anim.UpdateNodeSize(centralStation.Get(0)->GetId(), 15, 15);

    // Monitoring Center
    anim.UpdateNodeDescription(monitoringCenter.Get(0), "Monitor-Center");
    anim.UpdateNodeColor(monitoringCenter.Get(0), 0, 0, 255); // Blue
    anim.UpdateNodeSize(monitoringCenter.Get(0)->GetId(), 12, 12);

    // WAN Routers
    for (uint32_t i = 0; i < wanRouters.GetN(); ++i)
    {
        std::ostringstream desc;
        desc << "WAN-Router-" << i;
        anim.UpdateNodeDescription(wanRouters.Get(i), desc.str());
        anim.UpdateNodeColor(wanRouters.Get(i), 0, 255, 0); // Green
        anim.UpdateNodeSize(wanRouters.Get(i)->GetId(), 10, 10);
    }

    // Solar Schools (Education facilities)
    for (uint32_t i = 0; i < solarSchools.GetN(); ++i)
    {
        std::ostringstream desc;
        desc << "School-" << (i + 1);
        anim.UpdateNodeDescription(solarSchools.Get(i), desc.str());
        anim.UpdateNodeColor(solarSchools.Get(i), 255, 165, 0); // Orange
    }

    // Solar Clinics (Healthcare facilities)
    for (uint32_t i = 0; i < solarClinics.GetN(); ++i)
    {
        std::ostringstream desc;
        desc << "Clinic-" << (i + 1);
        anim.UpdateNodeDescription(solarClinics.Get(i), desc.str());
        anim.UpdateNodeColor(solarClinics.Get(i), 255, 0, 0); // Red
    }

    // Community Micro-grids
    for (uint32_t i = 0; i < microgrids.GetN(); ++i)
    {
        std::ostringstream desc;
        desc << "Microgrid-" << (i + 1);
        anim.UpdateNodeDescription(microgrids.Get(i), desc.str());
        anim.UpdateNodeColor(microgrids.Get(i), 173, 216, 230); // Light Blue
    }

    // ========================================================================
    // RUN SIMULATION
    // ========================================================================
    
    std::cout << "\nStarting solar energy network simulation...\n\n";

    Simulator::Stop(Seconds(simulationTime));
    Simulator::Run();

    // ========================================================================
    // STATISTICS AND RESULTS
    // ========================================================================
    
    std::cout << "\n================================================================\n";
    std::cout << "              SOLAR ENERGY WAN - RESULTS\n";
    std::cout << "================================================================\n\n";

    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

    uint64_t totalTx = 0, totalRx = 0;
    double totalThroughput = 0.0;
    double totalDelay = 0.0;
    uint32_t flowCount = 0;

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();
         i != stats.end(); ++i)
    {
        totalTx += i->second.txPackets;
        totalRx += i->second.rxPackets;
        totalThroughput += i->second.rxBytes * 8.0 / simulationTime / 1000.0;
        
        if (i->second.rxPackets > 0)
        {
            totalDelay += i->second.delaySum.GetSeconds() / i->second.rxPackets;
            flowCount++;
        }
    }

    std::cout << "Network Performance Metrics:\n";
    std::cout << "  Packets Transmitted:      " << totalTx << "\n";
    std::cout << "  Packets Received:         " << totalRx << "\n";
    std::cout << "  Packets Lost:             " << (totalTx - totalRx) << "\n";
    
    if (totalTx > 0)
    {
        double lossRate = ((totalTx - totalRx) * 100.0 / totalTx);
        std::cout << "  Packet Loss Rate:         " << lossRate << " %\n";
        
        if (lossRate < 3.0)
            std::cout << "  Status: EXCELLENT - Network is highly reliable\n";
        else if (lossRate < 7.0)
            std::cout << "  Status: GOOD - Network performance acceptable\n";
        else
            std::cout << "  Status: NEEDS IMPROVEMENT - Consider upgrading links\n";
    }
    
    std::cout << "  Network Throughput:       " << totalThroughput << " kbps\n";
    
    if (flowCount > 0)
    {
        double avgDelay = (totalDelay / flowCount) * 1000;
        std::cout << "  Average Latency:          " << avgDelay << " ms\n";
        
        if (avgDelay < 50.0)
            std::cout << "  Latency Status: EXCELLENT - Real-time monitoring possible\n";
        else if (avgDelay < 100.0)
            std::cout << "  Latency Status: GOOD - Suitable for most applications\n";
        else
            std::cout << "  Latency Status: ACCEPTABLE - May need optimization\n";
    }

    std::cout << "\n================================================================\n";
    std::cout << "System Components Summary:\n";
    std::cout << "  Solar Schools Connected:     " << nSchools << "\n";
    std::cout << "  Health Clinics Connected:    " << nClinics << "\n";
    std::cout << "  Community Micro-grids:       " << nMicrogrids << "\n";
    std::cout << "  Total Renewable Sites:       " << (nSchools + nClinics + nMicrogrids) << "\n";

    std::cout << "\n================================================================\n";
    std::cout << "Impact on Community:\n";
    std::cout << "  - Enhanced Learning: Schools powered for extended hours\n";
    std::cout << "  - Healthcare Access: Clinics operational 24/7\n";
    std::cout << "  - Economic Growth: Job creation and business development\n";
    std::cout << "  - Environmental: Clean renewable energy, zero emissions\n";
    std::cout << "  - Sustainability: Long-term economically viable solution\n";

    std::cout << "\n================================================================\n";
    std::cout << "Generated Files:\n";
    std::cout << "  Animation: solar-energy-wan.xml (Open with NetAnim)\n";
    std::cout << "================================================================\n";
    
    std::cout << "\nSimulation completed successfully!\n";
    std::cout << "Project: Solar Energy + WAN for Community Electrification\n";
    std::cout << "Author: NGATCHA FOTSO CALEX (ICTU20241105)\n";
    std::cout << "ICT University Yaounde\n";
    std::cout << "================================================================\n\n";

    Simulator::Destroy();
    
    delete[] schoolDevices;
    delete[] clinicDevices;
    delete[] microgridDevices;
    
    return 0;
}
