#version 330 core

layout (location = 0) in vec3 aPos;       // ���_���W
layout (location = 1) in vec3 aNormal;    // �@��
layout (location = 2) in vec2 aTexCoords; // uv���W
layout (location = 3) in vec3 aTangent;   // �]�@��

// �|�C���g���C�g
struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 dpecular;
};

out VS_OUT
{
    PointLight Light;
    vec2 TexCoords;
    vec3 FragPos;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
}vx_out;

uniform PointLight in_light;

uniform mat4 model;      // ���f���s��
uniform mat4 view;       // �r���[�s��
uniform mat4 projection; // �v���W�F�N�V�����s��
uniform vec3 viewPos;    // �J�����̍��W

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos,1.0));
    vs_out.TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = normalize(normalMatrix * aTangent);
    T = normalize(T - dot(T,N) * N);
    vec3 B = cross(N,T);

    mat3 TBN = transpose(mat3(T,B,N));

    // ���C�g�̃^���W�F���g��Ԃł̈ʒu
    vs_out.Light = in_light;
    vs_out.TangentLightPos = TBN * in_light.position;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TagngetFragPos = TBN * vs_out.FragPos;

    gl_Position = projection * view * model * vec4(aPos,1.0);
}