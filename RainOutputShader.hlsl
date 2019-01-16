//Globals
cbuffer cbPerFrame
{
    float4x4 gViewProj; //64
    float3 gEyePosW;    //12
    float pack;         //4
    float3 gEmitPosW;   //12
    float gTimeStep;    //4
    float3 gEmitDirW;   //12
    float gGameTime;    //4
    
    
    
};

cbuffer cbFixed
{
    //constant acceleration used for particles
    float3 gAccelW = { -1.0f, -9.8f, 0.0f };
};

//array of textures for particles
Texture2DArray gTexArray;

//Random texture used to generate random numbers in shaders.
Texture1D gRandomTex;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

DepthStencilState DisableDepth
{
    DepthEnable = False;
    DepthWriteMask = ZERO;
};

DepthStencilState NoDepthWrites
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

//Helper FUNC
float3 RandUnitVec3(float offset)
{
    float u = (gGameTime + offset);
    float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;

    return normalize(v);
}

float3 RandVec3(float offset)
{
    float u = (gGameTime + offset);
    float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;

    return v;
}

//STREAM OUT TECH

#define PT_EMITTER 0
#define PT_FLARE 1


struct Particle
{
    float3 InitPosW : POSITION;
    float3 InitVelW : VELOCITY;
    float2 SizeW : SIZE;
    float Age : AGE;
    uint Type : TYPE;
};

Particle StreamOutVS(Particle vin)
{
    return vin;
}

[maxvertexcount(6)]
void StreamOutGS(point Particle gin[1], inout PointStream<Particle> ptStream)
{

    gin[0].Age += gTimeStep;

    if (gin[0].Type == PT_EMITTER)
    {
        //time to emit a new particle
        if (gin[0].Age > 0.002f)
        {
            for (int i = 0; i < 5; i++)
            {
                //spread  raindops
                float3 vRandom = 35.0f * RandVec3((float) i / 5.0f);
                vRandom.y = 20.0f;

                Particle p;
                p.InitPosW = gEmitPosW.xyz + vRandom;
                p.InitVelW = float3(0.0f, 0.0f, 0.f);
                p.SizeW = float2(1.0f, 1.0f);
                p.Age = 0.0f;
                p.Type = PT_FLARE;

                ptStream.Append(p);
            }

            //reset emit time
            gin[0].Age = 0.0f;
        }

        //always keep emitters
        ptStream.Append(gin[0]);
    }
    else
    {
        //specify conditions to keep particle; maybe subject to change
        if (gin[0].Age <= 3.0f)
            ptStream.Append(gin[0]);
    }

}

//Draw TECH

struct VertexOut
{
    float3 PosW : POSITION;
    uint Type : TYPE;
};

VertexOut DrawVS(Particle vin)
{
    VertexOut vout;

    float t = vin.Age;

    vout.PosW = 0.5f * t * t * gAccelW + t * vin.InitVelW + vin.InitPosW;
    vout.Type = vin.Type;

    return vout;
}

struct GeoOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

//The draw GS just expands points into lines
[maxvertexcount(2)]
void DrawGS(point VertexOut gin[1], inout LineStream<GeoOut> lineStream)
{
    //do not draw emitter particles
    if (gin[0].Type != PT_EMITTER)
    {
        //slant line in acceleration direction
        float3 p0 = gin[0].PosW;
        float3 p1 = gin[0].PosW + 0.07f * gAccelW;

        GeoOut v0;
        v0.PosH = mul(float4(p0, 1.0f), gViewProj);
        v0.Tex = float2(0.0f, 0.0f);
        lineStream.Append(v0);

        GeoOut v1;
        v1.PosH = mul(float4(p1, 1.0f), gViewProj);
        v1.Tex = float2(0.0f, 0.0f);
        lineStream.Append(v1);

   }
}
float4 DrawPS(GeoOut pin) : SV_TARGET
{
    return gTexArray.Sample(samLinear, float3(pin.Tex, 0));
}

