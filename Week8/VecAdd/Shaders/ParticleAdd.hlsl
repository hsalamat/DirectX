struct Particle
{
    float3 Position;
    float3 Velocity;
    float3 Acceleration;
};
//declare the type as static const, otherwise this would override it from the app via constant buffer.
static const float TimeStep = 1.0f / 1.0f;
ConsumeStructuredBuffer<Particle> gInput : register(u0);
AppendStructuredBuffer<Particle> gOutput : register(u1);
[numthreads(16, 16, 1)]
void CS()
{
// Consume a data element from the input buffer.
    Particle p = gInput.Consume();
    p.Velocity += p.Acceleration * TimeStep;
    p.Position += p.Velocity * TimeStep;
// Append normalized vector to output buffer.
    gOutput.Append(p);
}