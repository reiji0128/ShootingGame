﻿#include "Shader.h"
#include "Texture.h"
#include <SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
    :mShaderProgram(0)
    ,mVertexShader(0)
    ,mFragShader(0)
{

}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// Compile vertex and pixel shaders
	if (!CompileShader(vertName,GL_VERTEX_SHADER,mVertexShader) ||
		!CompileShader(fragName,GL_FRAGMENT_SHADER,mFragShader))
	{
		return false;
	}

	// Now create a shader program that
	// links together the vertex/frag shaders
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// Verify that the program linked successfully
	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}

void Shader::Unload()
{
	// Delete the program/shaders
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	// Set this program as the active one
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	//この名前でユニフォームを探す
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//マトリックスデータをユニフォームに送信
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// 行列配列データをシェーダー変数に送る
	glUniformMatrix4fv(loc, count, GL_TRUE, matrices->GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//ベクターデータを送る
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* name, const Vector2& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// ベクトル値をシェーダー変数に送る
	float v[2];
	v[0] = vector.x;
	v[1] = vector.y;
	glUniform2fv(loc, 1, v);
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//floatデータを送る
	glUniform1f(loc, value);
}

void Shader::SetIntUniform(const char* name, int value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniform1i(loc, value);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLenum& outShader)
{
	//オープンファイル
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		//すべてのテキストをstringで読みます
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		//指定されたタイプのシェーダーを作成します
		outShader = glCreateShader(shaderType);
		//ソース文字を設定し、コンパイルを試みます
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("シェーダのコンパイルに失敗しました。 %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("シェーダーファイルが見つかりませんでした。: %s", fileName.c_str());
		return false;
	}

    return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// Query the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	// Query the link status
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}
