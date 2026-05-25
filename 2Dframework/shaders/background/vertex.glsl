#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform vec2 texOffset;
uniform float texZoom;

void main() {
  gl_Position = vec4(aPos.xy, 0.0f, 1.0f);
  texCoord = (aTexCoord.xy + texOffset.xy) / texZoom;
}
