#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/packet/Packet.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"

using namespace omnetpp;
using namespace inet;

namespace miot_simulation {

class TemperatureSensor : public ApplicationBase
{
  private:
    UdpSocket socket;
    cMessage *sendMessage = nullptr;
    int destPort = 5000;

  public:
    TemperatureSensor();
    virtual ~TemperatureSensor();

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;
};

Define_Module(TemperatureSensor);

TemperatureSensor::TemperatureSensor()
{
    sendMessage = new cMessage("sendTemperature");
}

TemperatureSensor::~TemperatureSensor()
{
    cancelAndDelete(sendMessage);
}

void TemperatureSensor::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_APPLICATION_LAYER) {
        socket.setOutputGate(gate("socketOut"));
        socket.bind(5000);
    }
}

void TemperatureSensor::handleStartOperation(LifecycleOperation *operation)
{
    scheduleAt(simTime() + par("sendInterval").doubleValue(), sendMessage);
}

void TemperatureSensor::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(sendMessage);
}

void TemperatureSensor::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(sendMessage);
}

void TemperatureSensor::handleMessageWhenUp(cMessage *msg)
{
    if (msg == sendMessage) {
        auto packet = new Packet("TemperatureData");
        L3Address destAddr = L3AddressResolver().resolve(par("destAddress").stringValue());
        socket.sendTo(packet, destAddr, destPort);
        scheduleAt(simTime() + par("sendInterval").doubleValue(), sendMessage);
    }
    else {
        delete msg;
    }
}

}
