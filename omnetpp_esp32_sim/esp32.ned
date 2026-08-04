simple Esp32Node
{
    gates:
        output out;
}

simple GatewayNode
{
    gates:
        input in[];
}

network Esp32GatewayNet
{
    submodules:
        esp32[4]: Esp32Node;
        gateway: GatewayNode;
    connections:
        for i=0..3 {
            esp32[i].out --> gateway.in++;
        }
}