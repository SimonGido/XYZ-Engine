#type compute
#version 430
//Size of compute shader local work group - x=32, y=32, z=1(default)
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

const int c_MaxParticles = 10000;

struct ParticleVertex
{
	vec4 color;
	vec4 position;
	vec2 texCoordOffset;
	vec2 size;
	float angle;
	float alignment;
	float alignment2;
	float alignment3;
};

struct ParticleData
{
	vec4 colorBegin;
	vec4 colorEnd;
	vec2 startVelocity;
	vec2 endVelocity;
	float sizeBegin;
	float sizeEnd;
	float rotation;
	float lifeTime;
	float timeAlive;

	float alignment;
	float alignment2;
	float alignment3;
};

layout(std430, binding = 0) buffer
buffer_InPos
{
	ParticleVertex InVertex[];
};

layout(std430, binding = 1) buffer
buffer_InData
{
	ParticleData InData[];
};

uniform float u_Time;
uniform float u_Gravity;
uniform float u_Speed;
uniform float u_NumberRows;
uniform float u_NumberColumns;
uniform vec2 u_Emitter;

float c_NumberOfStages = u_NumberRows * u_NumberColumns;

float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453) * 1000.0;
}


subroutine vec4 colorRedBlue();
subroutine(colorRedBlue) vec4 redColor() 
{
	return vec4(1.0, 0.0, 0.0, 1.0);
}
subroutine(colorRedBlue) vec4 blueColor() {

	return vec4(0.0, 0.0, 1.0, 1.0);
}

vec4 ChangeColorOverLife(ParticleData data)
{
	return mix(data.colorBegin, data.colorEnd, data.timeAlive / data.lifeTime);
}
vec2 ChangeSizeOverLife(ParticleData data)
{
	vec2 begin = vec2(data.sizeBegin, data.sizeBegin);
	vec2 end = vec2(data.sizeEnd, data.sizeEnd);
	return mix(begin, end, data.timeAlive / data.lifeTime);
}

subroutine uniform colorRedBlue sub_BlueSelection;

void main(void)
{
	uint id = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	if (id > c_MaxParticles)
		return;

	ParticleVertex pVertex = InVertex[id];
	ParticleData pData = InData[id];

	pVertex.position.x += (pData.startVelocity.x * u_Speed) * u_Time;
	pVertex.position.y += (pData.startVelocity.y * u_Speed) * u_Time;
	

	pVertex.angle += u_Time * pData.rotation;
	pVertex.size = ChangeSizeOverLife(pData);
	pVertex.color = sub_BlueSelection();
	pVertex.color = ChangeColorOverLife(pData);

	pData.startVelocity.y += u_Gravity * u_Time;
	pData.timeAlive += u_Time;


	float stageCount = u_NumberRows * u_NumberColumns;
	float lifeFactor = pData.timeAlive / pData.lifeTime;
	float stageProgress = lifeFactor * stageCount;

	int index = int(floor(stageProgress));
	int column = int(mod(index, u_NumberColumns));
	int row = int(index / u_NumberRows);
	
	pVertex.texCoordOffset = vec2(float(column) / u_NumberColumns, float(row) / u_NumberRows);
	
	if (pData.timeAlive > pData.lifeTime)
	{
		pData.startVelocity = pData.endVelocity;
		pData.timeAlive = 0;
		pVertex.position.xy = u_Emitter;
	}

	InVertex[id] = pVertex;
	InData[id] = pData;
}