import inet.physicallayer.wireless.ieee802154.packetlevel.Ieee802154NarrowbandScalarRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

network MiotNetwork
{
    parameters:
        @display("bgb=600,400");
    submodules:
        radioMedium: Ieee802154NarrowbandScalarRadioMedium {
            @display("p=300,100");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=400,50");
        }
        esp32: ESP32 {
            @display("p=100,200");
        }
        gateway: Gateway {
            @display("p=500,200");
        }
}