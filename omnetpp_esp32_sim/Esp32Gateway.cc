#include <omnetpp.h>
#include "messages_m.h"

using namespace omnetpp;

// =======================
// ESP32 NODE
// =======================
class Esp32Node : public cSimpleModule
{
  private:
    cMessage *sendEvent;
    double lastSentTemp;
    double threshold = 0.5;   // used later for lightweight twin
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Esp32Node);

void Esp32Node::initialize()
{
    // Each node gets a unique baseline based on its index
    int id = getIndex();
    lastSentTemp = 20.0 + (id * 5.0);

    sendEvent = new cMessage("sendEvent");
    // Stagger start times so they don't all send at exactly the same microsecond
    scheduleAt(simTime() + 1.0 + (id * 0.1), sendEvent);
}

void Esp32Node::handleMessage(cMessage *msg)
{
    if (msg == sendEvent) {

        // Generate synthetic temperature
        double currentTemp = 20 + uniform(-2, 2);

        // Heavy Twin: send every reading
        TempMsg *t = new TempMsg("tempReading");
        t->setTemperature(currentTemp);
        send(t, "out");

        lastSentTemp = currentTemp;

        // Schedule next reading
        scheduleAt(simTime() + 1, sendEvent);
    }
}

// =======================
// GATEWAY NODE
// =======================
class GatewayNode : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(GatewayNode);

void GatewayNode::handleMessage(cMessage *msg)
{
    TempMsg *t = check_and_cast<TempMsg *>(msg);

    int senderIndex = msg->getArrivalGate()->getIndex();

    EV << "Gateway received temperature: "
       << t->getTemperature()
       << " from ESP32 node index: "
       << senderIndex
       << endl;

    delete t;
}
