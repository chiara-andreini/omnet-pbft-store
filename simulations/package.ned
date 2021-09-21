package byzantinetolerantdatastore.simulations;

@license(LGPL);

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.node.ethernet.Eth1G;

network ReplicatedDataStoreWithClient
{
    parameters:
        int numReplicas;
        int viewCode;
        @display("bgb=600,500;bgi=maps/italy,s;bgg=100,2,grey95;bgu=m");
        // This is the operational mode:
        //	- 0 is for normal operation
        // 	- 1 is for view change
        int mode;
        
    types:
        channel Channel extends ned.DelayChannel {
            delay = 300ms;
        }
        
    submodules:
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            parameters:
                @display("p=50,450;is=s");
        }
        /// Ipv4NetworkConfigurator assigns IP addresses to hosts and routers, and sets up static routing.
        /// It is used when modeling dynamic IP address assignment (e.g. via DHCP) or dynamic routing is not of importance.
        
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=50,350;is=s");
        }

        replica[numReplicas]: Replica {
                parameters:
                @display("r=,,#707070;p=200,200;i=device/server2");
                	
        }
        
        client1: Client {
        	parameters:
                @display("r=,,#707070;p=350,250;i=device/laptop");
        }
        
        client2: Client {
        	parameters:
                @display("r=,,#707070;p=350,250;i=device/laptop");
        }
        
        connections:
			replica[5].g++ <--> Channel <--> replica[6].g++;
            replica[5].g++ <--> Channel <--> replica[4].g++;
            replica[1].g++ <--> Channel <--> replica[4].g++;
            replica[6].g++ <--> Channel <--> replica[8].g++;
            replica[3].g++ <--> Channel <--> replica[7].g++;
            replica[3].g++ <--> Channel <--> replica[0].g++;
            replica[8].g++ <--> Channel <--> replica[0].g++;
            replica[0].g++ <--> Channel <--> replica[9].g++;
            replica[9].g++ <--> Channel <--> replica[2].g++;

            // Customizable initial connection
			client1.g++ <--> Channel <--> replica[8].g++;
			client2.g++ <--> Channel <--> replica[6].g++;
}



network ReplicatedDataStore
{
    parameters:
        int numReplicas;
        int viewCode;
        @display("bgb=600,500;bgi=maps/italy,s;bgg=100,2,grey95;bgu=m");
        // This is the operational mode:
        //	- 0 is for normal operation
        // 	- 1 is for view change
        int mode;
        
    types:
        channel Channel extends ned.DelayChannel {
            delay = 300ms;
        }
        
    submodules:
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            parameters:
                @display("p=50,450;is=s");
        }
        /// Ipv4NetworkConfigurator assigns IP addresses to hosts and routers, and sets up static routing.
        /// It is used when modeling dynamic IP address assignment (e.g. via DHCP) or dynamic routing is not of importance.
        
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=50,350;is=s");
        }

        replica[numReplicas]: Replica {
                parameters:
                @display("r=,,#707070;p=200,200;i=device/server2");   	
        }
        
        connections:
			replica[5].g++ <--> Channel <--> replica[6].g++;
            replica[5].g++ <--> Channel <--> replica[4].g++;
            replica[1].g++ <--> Channel <--> replica[4].g++;
            replica[6].g++ <--> Channel <--> replica[8].g++;
            replica[3].g++ <--> Channel <--> replica[7].g++;
            replica[3].g++ <--> Channel <--> replica[0].g++;
            replica[8].g++ <--> Channel <--> replica[0].g++;
            replica[0].g++ <--> Channel <--> replica[9].g++;
            replica[9].g++ <--> Channel <--> replica[2].g++;
            replica[6].g++ <--> Channel <--> replica[7].g++;
            
}