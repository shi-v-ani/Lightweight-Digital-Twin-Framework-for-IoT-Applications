import inet.applications.contract.IApp;

simple TemperatureSensor like IApp
{
    parameters:
        @class(miot_simulation::TemperatureSensor);  // <-- Changed this line
        volatile double sendInterval @unit(s) = default(1s);
        string destAddress;
    gates:
        input socketIn @labels(UdpControlInfo/up);
        output socketOut @labels(UdpControlInfo/down);
}